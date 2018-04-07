#pragma once
#include "dr_script_prerequisites.h"
#include "dr_time.h"
#include <vector>

namespace driderSDK {

class CScriptDictionary;

typedef asUINT(*TIMEFUNC_t)();

const asPWORD CONTEXT_MGR = 1002;

struct SContextInfo
{
  asUINT                    sleepUntil;
  std::vector<asIScriptContext*> coRoutines;
  asUINT                    currentCoRoutine;
  asIScriptContext *        keepCtxAfterExecution;
};

static void
ScriptSleep(asUINT milliSeconds);

static void
ScriptYield();

void
ScriptCreateCoRoutine(asIScriptFunction *func,
                      CScriptDictionary *arg);

class DR_SCRIPT_EXPORT ContextManager : public Module<ContextManager> {
  public:
    ContextManager();

    ~ContextManager();
    

    /**
    * Set the function that the manager will use to obtain the time in milliseconds
    */
    //void 
    //setGetTimeCallback(TIMEFUNC_t func);

    /*
    * Registers the following:
    * 
    *  void sleep(uint milliseconds)
    * 
    * The application must set the get time callback for this to work
    */
    void 
    registerThreadSupport(asIScriptEngine *engine);
    
    /**
    * Registers the following:
    * 
    *  funcdef void coroutine(dictionary@)
    *  void createCoRoutine(coroutine @func, dictionary @args)
    *  void yield()
    */
    void 
    registerCoRoutineSupport(asIScriptEngine *engine);
    
    /**
    * Create a new context, prepare it with the function id, then return 
    * it so that the application can pass the argument values. The context
    * will be released by the manager after the execution has completed.
    * Set keepCtxAfterExecution to true if the application needs to retrieve
    * information from the context after it the script has finished. 
    */
    asIScriptContext *
    addContext(asIScriptEngine *engine,
               TString moduleName);

    /**
    * If the context was kept after the execution, this method must be 
    * called when the application is done with the context so it can be
    * returned to the pool for reuse.
    */
    void 
    doneWithContext(asIScriptContext *ctx);
    
    /**
    * Create a new context, prepare it with the function id, then return
    * it so that the application can pass the argument values. The context
    * will be added as a co-routine in the same thread as the currCtx.
    */
    asIScriptContext *
    addContextForCoRoutine(asIScriptContext *currCtx, 
                           asIScriptFunction *func);
    /**
    * Execute each script that is not currently sleeping. The function returns after 
    * each script has been executed once. The application should call this function
    * for each iteration of the message pump, or game loop, or whatever.
    * Returns the number of scripts still in execution.
    */
    Int32 
    executeScripts();

    /**
    * Put a script to sleep for a while
    */
    void 
    setSleeping(asIScriptContext *ctx,
                asUINT milliSeconds);

    /**
    * Switch the execution to the next co-routine in the group.
    * Returns true if the switch was successful.
    */
    void 
    nextCoRoutine();

    /*
    * Abort all scripts
    */
    void 
    abortAll();
  
  protected:
    std::vector<SContextInfo*> m_threads;
    std::vector<SContextInfo*> m_freeThreads;
    asUINT                     m_currentThread;
    TIMEFUNC_t m_getTimeFunc;

    // Statistics for Garbage Collection
    asUINT   m_numExecutions;
    asUINT   m_numGCObjectsCreated;
    asUINT   m_numGCObjectsDestroyed;

  public:
      
};

}