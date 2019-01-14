#ifndef IMGUIDOCK_H_
#define IMGUIDOCK_H_

// based on https://github.com/nem0/LumixEngine/blob/master/external/imgui/imgui_dock.h
// Lumix Engine Dock. From: https://github.com/nem0/LumixEngine/blob/master/src/editor/imgui/imgui_dock.h
/*
The MIT License (MIT)

Copyright (c) 2013-2016 Mikulas Florek

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/
// modified from https://bitbucket.org/duangle/liminal/src/tip/src/liminal/imgui_dock.h

// USAGE:
/*
        // Outside any ImGuiWindow:

        // Windowed:
        if (ImGui::Begin("imguidock window (= lumix engine's dock system)",NULL,ImVec2(500, 500),0.95f,ImGuiWindowFlags_NoScrollbar)) {
            ImGui::BeginDockspace();
            static char tmp[128];
            for (int i=0;i<10;i++)  {
                sprintf(tmp,"Dock %d",i);
                if (i==9) ImGui::SetNextDock(ImGuiDockSlot_Bottom);// optional
                if(ImGui::BeginDock(tmp))  {
                    ImGui::Text("Content of dock window %d goes here",i);
                }
                ImGui::EndDock();
            }
            ImGui::EndDockspace();
        }
        ImGui::End();


        // Fullscreen (without visual artifacts):
        ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize);
        const ImGuiWindowFlags flags =  (ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoTitleBar);
        const float oldWindowRounding = ImGui::GetStyle().WindowRounding;ImGui::GetStyle().WindowRounding = 0;
        const bool visible = ImGui::Begin("imguidock window (= lumix engine's dock system)",NULL,ImVec2(0, 0),1.0f,flags);
        ImGui::GetStyle().WindowRounding = oldWindowRounding;
        if (visible) {
            ImGui::BeginDockspace();
            static char tmp[128];
            for (int i=0;i<10;i++)  {
                sprintf(tmp,"Dock %d",i);
                if (i==9) ImGui::SetNextDock(ImGuiDockSlot_Bottom);// optional
                if(ImGui::BeginDock(tmp))  {
                    ImGui::Text("Content of dock window %d goes here",i);
                }
                ImGui::EndDock();
            }
            ImGui::EndDockspace();
        }
        ImGui::End();
*/

#ifndef IMGUI_API
#include <imgui.h>
#endif //IMGUI_API

#include "imgui_internal.h"
//#include "dr_imgui_helper.h"
 
typedef enum ImGuiDockSlot {
    ImGuiDockSlot_Left=0,
    ImGuiDockSlot_Right,
    ImGuiDockSlot_Top,
    ImGuiDockSlot_Bottom,
    ImGuiDockSlot_Tab,

    ImGuiDockSlot_Float,
    ImGuiDockSlot_None
} ImGuiDockSlot;

namespace ImGui {
  
struct DockContext
{
  enum EndAction_ {
    EndAction_None,
    EndAction_Panel,
    EndAction_End,
    EndAction_EndChild
  };
  
  enum Status_ {
    Status_Docked,
    Status_Float,
    Status_Dragged
  };
  
  struct Dock
  {
    Dock();
    
    ~Dock();
    
    ImVec2
    getMinSize() const;

    bool
    isHorizontal() const;
    
    void
    setParent(Dock* dock);
    
    Dock&
    getRoot();

    Dock&
    getSibling();

    Dock&
    getFirstTab();

    void
    setActive();
    
    bool
    hasChildren() const;
    
    void
    setChildrenPosSize(const ImVec2& _pos, const ImVec2& _size);
    
    void
    setPosSize(const ImVec2& _pos, const ImVec2& _size);

    char* label;
    ImU32 id;
    Dock* next_tab;
    Dock* prev_tab;
    Dock* children[2];
    Dock* parent;
    bool active;
    ImVec2 pos;
    ImVec2 size;
    ImVec2 floatmode_size;
    Status_ status;
    int last_frame;
    int invalid_frames;
    char location[16];
    bool opened;
    bool first;
  };

  ImVector<Dock*> m_docks;
  ImVec2 m_drag_offset;
  Dock* m_current;
  Dock *m_next_parent;
  int m_last_frame;
  EndAction_ m_end_action;
  bool m_is_begin_open;
  ImVec2 m_workspace_pos;
  ImVec2 m_workspace_size;
  ImGuiDockSlot m_next_dock_slot;
  bool m_is_first_call;
  
  DockContext();

  ~DockContext();

  void
  Shutdown();

  Dock&
  getDock(const char* label,
          bool opened,
          const ImVec2& default_size,
          const ImVec2& default_pos);

  void
  putInBackground();

  void
  splits();

  void
  checkNonexistent();

  Dock*
  getDockAt(const ImVec2& /*pos*/) const;

  static ImRect
  getDockedRect(const ImRect& rect, ImGuiDockSlot dock_slot);

  static ImRect
  getSlotRect(ImRect parent_rect, ImGuiDockSlot dock_slot);

  static ImRect
  getSlotRectOnBorder(ImRect parent_rect, ImGuiDockSlot dock_slot);

  Dock*
  getRootDock();

  bool
  dockSlots(Dock& dock, Dock* dest_dock, const ImRect& rect, bool on_border);

  void
  handleDrag(Dock& dock);

  void
  fillLocation(Dock& dock);

  void
  doUndock(Dock& dock);

  void
  drawTabbarListButton(Dock& dock);

  bool
  tabbar(Dock& dock, bool close_button);

  static void
  setDockPosSize(Dock& dest,
                 Dock& dock,
                 ImGuiDockSlot dock_slot,
                 Dock& container);

  Dock
  *findNonContainer(Dock *dock);

  void
  doDock(Dock& dock, Dock* dest, ImGuiDockSlot dock_slot);

  void
  rootDock(const ImVec2& pos, const ImVec2& size);

  void
  setDockActive();

  static ImGuiDockSlot
  getSlotFromLocationCode(char code);

  static char
  getLocationCode(Dock* dock);

  void
  tryDockToStoredLocation(Dock& dock);

  void
  cleanDocks();
  
  bool
  begin(const char* label,
        bool* opened,
        ImGuiWindowFlags extra_flags,
        const ImVec2& default_size,
        const ImVec2& default_pos);
  
  void
  end();

  void
  debugWindow();
    
  int
  getDockIndex(Dock* dock);
};


// Create, destroy and change dock contexts (*).

// EXAMPLE USAGE:
/* ImGui::DockContext* myDockContext=NULL; // global variable

   // When you init your application:
   myDockContext = ImGui::CreateDockContext();
   ImGui::SetCurrentDockContext(myDockContext);
   // From now on you can use imguidock [calling BeginDockspace()/EndDockspace() and so on].

   // When you destroy your application:
   ImGui::DestroyDockContext(myDockContext);myDockContext=NULL;
*/

// (*)  This is really mandatory only if you're not using an IMGUI_USE_XXX_BINDING, or if you don't know
//      what IMGUI_USE_XXX_BINDING is (because otherwise the code above is already called for you in addons/imguibindings/imguibindings.cpp).

// Each created context must be set current using SetCurrentDockContext and destroyed using DestroyDockContext.
IMGUI_API DockContext* CreateDockContext();
IMGUI_API void DestroyDockContext(DockContext* dock);

IMGUI_API void SetCurrentDockContext(DockContext* dock);
IMGUI_API DockContext* GetCurrentDockContext();

IMGUI_API void RootDock(const ImVec2& pos, const ImVec2& size);

IMGUI_API void BeginDockspace();
IMGUI_API void EndDockspace();
IMGUI_API void ShutdownDock();
IMGUI_API void SetNextDock(ImGuiDockSlot slot);
// 'default_size', when positive, will be used as the initial size of the Window when in floating/undocked mode.
// When the floating/undocked window is manually resized, the last modified window size is kept (and the passed argument is ignored).
// If 'default_size' is negative, any manual resizing (of the floating window) will be lost when the window is re-docked.
// Please note that if you LoadDock(...) the last saved value will be used (so 'default_size' can still be ignored).
IMGUI_API bool BeginDock(const char* label, bool* opened = NULL, ImGuiWindowFlags extra_flags = 0, const ImVec2& default_size = ImVec2(0,0), const ImVec2& default_pos = ImVec2(0,0));
IMGUI_API void EndDock();
IMGUI_API void SetDockActive();
IMGUI_API void DockDebugWindow();

// Ported from the original "Lua binding" code
#if (defined(IMGUIHELPER_H_) && !defined(NO_IMGUIHELPER_SERIALIZATION))
#   ifndef NO_IMGUIHELPER_SERIALIZATION_SAVE
    IMGUI_API bool SaveDock(ImGuiHelper::Serializer& s);
    IMGUI_API bool SaveDock(const char* filename);
#   endif //NO_IMGUIHELPER_SERIALIZATION_SAVE
#   ifndef NO_IMGUIHELPER_SERIALIZATION_LOAD
    IMGUI_API bool LoadDock(ImGuiHelper::Deserializer& d,const char ** pOptionalBufferStart=NULL);
    IMGUI_API bool LoadDock(const char* filename);
#   endif //NO_IMGUIHELPER_SERIALIZATION_LOAD
#endif //(defined(IMGUIHELPER_H_) && !defined(NO_IMGUIHELPER_SERIALIZATION))

} // namespace ImGui

extern bool gImGuiDockReuseTabWindowTextureIfAvailable; // [true] (used only when available)


#endif //IMGUIDOCK_H_