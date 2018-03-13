#include "dr_context_manager.h"
#include "scriptdictionary.h"

namespace driderSDK {

ContextManager::ContextManager() {

}

ContextManager::~ContextManager() {

}

//void
//ContextManager::setGetTimeCallback(TIMEFUNC_t func) {
//  m_getTimeFunc = func;
//}

void
ContextManager::registerThreadSupport(asIScriptEngine *engine) {
  Int32 result;

  // Must set the get time callback function for this to work
  if(m_getTimeFunc == 0) {
    return;
  } 
  // Register the sleep function
  result = engine->RegisterGlobalFunction("void sleep(uint)",
                                          asFUNCTION(ScriptSleep), 
                                          asCALL_CDECL);

  // TODO: Add support for spawning new threads, waiting for signals, etc
}

void
ContextManager::registerCoRoutineSupport(asIScriptEngine *engine) {
  Int32 result;
  if(engine->GetTypeInfoByDecl("dictionary") == 0) {
    return;
  }

  result = engine->RegisterGlobalFunction("void yield()",
                                          asFUNCTION(ScriptYield), 
                                          asCALL_CDECL);
  result = engine->RegisterFuncdef("void coroutine(dictionary@)");
  result = engine->RegisterGlobalFunction("void createCoRoutine(coroutine @+, dictionary @+)", 
                                          asFUNCTION(ScriptCreateCoRoutine), 
                                          asCALL_CDECL);
  

}

void
ContextManager::nextCoRoutine() {
  m_threads[m_currentThread]->currentCoRoutine++;
  if (m_threads[m_currentThread]->currentCoRoutine >= 
      m_threads[m_currentThread]->coRoutines.size()) {
    m_threads[m_currentThread]->currentCoRoutine = 0;
  }
}

asIScriptContext *
ContextManager::addContext(asIScriptEngine *engine,
                           asIScriptFunction *func,
                           bool keepCtxAfterExecution) {

  // Use RequestContext instead of CreateContext so we can take 
  // advantage of possible context pooling configured with the engine
  asIScriptContext *ctx = engine->RequestContext();
  if (ctx == 0)
    return 0;

  // Prepare it to execute the function
  int r = ctx->Prepare(func);
  if (r < 0)
  {
    engine->ReturnContext(ctx);
    return 0;
  }

  // Set the context manager as user data with the context so it
  // can be retrieved by the functions registered with the engine
  ctx->SetUserData(this, CONTEXT_MGR);

  // Add the context to the list for execution
  SContextInfo *info = 0;
  if (m_freeThreads.size() > 0)
  {
    info = *m_freeThreads.rbegin();
    m_freeThreads.pop_back();
  }
  else
  {
    info = new SContextInfo;
  }

  info->coRoutines.push_back(ctx);
  info->currentCoRoutine = 0;
  info->sleepUntil = 0;
  info->keepCtxAfterExecution = keepCtxAfterExecution ? ctx : 0;
  m_threads.push_back(info);

  return ctx;

}

void
ContextManager::doneWithContext(asIScriptContext *ctx) {
  ctx->GetEngine()->ReturnContext(ctx);
}

asIScriptContext *
ContextManager::addContextForCoRoutine(asIScriptContext *currCtx,
                                       asIScriptFunction *func) {
  asIScriptEngine *engine = currCtx->GetEngine();
  asIScriptContext *coctx = engine->RequestContext();
  if (coctx == 0)
  {
    return 0;
  }

  // Prepare the context
  int r = coctx->Prepare(func);
  if (r < 0)
  {
    // Couldn't prepare the context
    engine->ReturnContext(coctx);
    return 0;
  }

  // Set the context manager as user data with the context so it
  // can be retrieved by the functions registered with the engine
  coctx->SetUserData(this, CONTEXT_MGR);

  // Find the current context thread info
  // TODO: Start with the current thread so that we can find the group faster
  for (asUINT n = 0; n < m_threads.size(); n++)
  {
    if (m_threads[n]->coRoutines[m_threads[n]->currentCoRoutine] == currCtx)
    {
      // Add the coRoutine to the list
      m_threads[n]->coRoutines.push_back(coctx);
    }
  }

  return coctx;
}

Int32
ContextManager::executeScripts() {
  // TODO: Should have an optional time out for this function. If not all scripts executed before the
  //       time out, the next time the function is called the loop should continue
  //       where it left off.

  // TODO: There should be a time out per thread as well. If a thread executes for too
  //       long, it should be aborted. A group of co-routines count as a single thread.

  // Check if the system time is higher than the time set for the contexts
  //asUINT time = m_getTimeFunc ? m_getTimeFunc() : asUINT(-1);
  if (!ScriptEngine::isStarted()) {
    ScriptEngine::startUp();
  }
  asUINT time = Time::getElapsedMilli();
  for (m_currentThread = 0; m_currentThread < m_threads.size(); m_currentThread++)
  {
    SContextInfo *thread = m_threads[m_currentThread];
    if (thread->sleepUntil < time)
    {
      int currentCoRoutine = thread->currentCoRoutine;

      // Gather some statistics from the GC
      asIScriptEngine *engine = thread->coRoutines[currentCoRoutine]->GetEngine();
      asUINT gcSize1, gcSize2, gcSize3;
      engine->GetGCStatistics(&gcSize1);

      // Execute the script for this thread and co-routine
      int r = thread->coRoutines[currentCoRoutine]->Execute();

      // Determine how many new objects were created in the GC
      engine->GetGCStatistics(&gcSize2);
      m_numGCObjectsCreated += gcSize2 - gcSize1;
      m_numExecutions++;

      if (r != asEXECUTION_SUSPENDED)
      {
        // The context has terminated execution (for one reason or other)
        // Unless the application has requested to keep the context we'll return it to the pool now
        if (thread->keepCtxAfterExecution != thread->coRoutines[currentCoRoutine])
          engine->ReturnContext(thread->coRoutines[currentCoRoutine]);
        thread->coRoutines[currentCoRoutine] = 0;

        thread->coRoutines.erase(thread->coRoutines.begin() + thread->currentCoRoutine);
        if (thread->currentCoRoutine >= thread->coRoutines.size())
          thread->currentCoRoutine = 0;

        // If this was the last co-routine terminate the thread
        if (thread->coRoutines.size() == 0)
        {
          m_freeThreads.push_back(thread);
          m_threads.erase(m_threads.begin() + m_currentThread);
          m_currentThread--;
        }
      }

      // Destroy all known garbage if any new objects were created
      if (gcSize2 > gcSize1)
      {
        engine->GarbageCollect(asGC_FULL_CYCLE | asGC_DESTROY_GARBAGE);

        // Determine how many objects were destroyed
        engine->GetGCStatistics(&gcSize3);
        m_numGCObjectsDestroyed += gcSize3 - gcSize2;
      }

      // TODO: If more objects are created per execution than destroyed on average
      //       then it may be necessary to run more iterations of the detection of
      //       cyclic references. At the startup of an application there is usually
      //       a lot of objects created that will live on through out the application
      //       so the average number of objects created per execution will be higher
      //       than the number of destroyed objects in the beginning, but afterwards
      //       it usually levels out to be more or less equal.

      // Just run an incremental step for detecting cyclic references
      engine->GarbageCollect(asGC_ONE_STEP | asGC_DETECT_GARBAGE);
    }
  }

  return int(m_threads.size());
}

void
ContextManager::setSleeping(asIScriptContext *ctx,
                            asUINT milliSeconds) {
  if(m_getTimeFunc == 0) {
    return;
  }

  // Find the context and update the timeStamp
  // for when the context is to be continued

  // TODO: Start with the current thread

  for (asUINT n = 0; n < m_threads.size(); n++)
  {
    if (m_threads[n]->coRoutines[m_threads[n]->currentCoRoutine] == ctx)
    {
      m_threads[n]->sleepUntil = (m_getTimeFunc ? m_getTimeFunc() : 0) + milliSeconds;
    }
  }
}

void
ContextManager::abortAll() {
  // Abort all contexts and release them.The script engine will make
  // sure that all resources held by the scripts are properly released.

  for (asUINT n = 0; n < m_threads.size(); n++)
  {
    for (asUINT c = 0; c < m_threads[n]->coRoutines.size(); c++)
    {
      asIScriptContext *ctx = m_threads[n]->coRoutines[c];
      if (ctx)
      {
        ctx->Abort();
        ctx->GetEngine()->ReturnContext(ctx);
        ctx = 0;
      }
    }
    m_threads[n]->coRoutines.resize(0);

    m_freeThreads.push_back(m_threads[n]);
  }

  m_threads.resize(0);

  m_currentThread = 0;
}

void 
ScriptSleep(asUINT milliSeconds)
{
  // Get a pointer to the context that is currently being executed
  asIScriptContext *ctx = asGetActiveContext();
  if (ctx)
  {
    // Get the context manager from the user data
    ContextManager *ctxMgr = reinterpret_cast<ContextManager*>(ctx->GetUserData(CONTEXT_MGR));
    if (ctxMgr)
    {
      // Suspend its execution. The VM will continue until the current
      // statement is finished and then return from the Execute() method
      ctx->Suspend();

      // Tell the context manager when the context is to continue execution
      ctxMgr->setSleeping(ctx, milliSeconds);
    }
  }
}

void 
ScriptYield()
{
  // Get a pointer to the context that is currently being executed
  asIScriptContext *ctx = asGetActiveContext();
  if (ctx)
  {
    // Get the context manager from the user data
    ContextManager *ctxMgr = reinterpret_cast<ContextManager*>(ctx->GetUserData(CONTEXT_MGR));
    if (ctxMgr)
    {
      // Let the context manager know that it should run the next co-routine
      ctxMgr->nextCoRoutine();

      // The current context must be suspended so that VM will return from
      // the Execute() method where the context manager will continue.
      ctx->Suspend();
    }
  }
}

void 
ScriptCreateCoRoutine(asIScriptFunction *func, CScriptDictionary *arg)
{
  if (func == 0)
    return;

  asIScriptContext *ctx = asGetActiveContext();
  if (ctx)
  {
    // Get the context manager from the user data
    ContextManager *ctxMgr = reinterpret_cast<ContextManager*>(ctx->GetUserData(CONTEXT_MGR));
    if (ctxMgr)
    {
      // Create a new context for the co-routine
      asIScriptContext *coctx = ctxMgr->addContextForCoRoutine(ctx, func);

      // Pass the argument to the context
      coctx->SetArgObject(0, arg);

      // The context manager will call Execute() on the context when it is time
    }
  }
}

}