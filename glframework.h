#ifndef GLFRAMEWORK_H
#define GLFRAMEWORK_H

#include <string>
#include </usr/include/SDL2/SDL.h>
#include "GLFrameWork_global.h"
#include "vartypes.h"

using namespace std;

typedef enum{
    SDL_CONTEXT_OK = 0,
    SDL_CONTEXT_FAILED,
    SDL_WINDOWCREATE_OK,
    SDL_WINDOWCREATE_FAILED,
    SDL_STATE_UNDEFINED

}INIT_STATES;

class GLFRAMEWORK_EXPORT GLFrameWork
{
public:
    GLFrameWork();
    bool InitSDL();
    virtual void DestroySDL();
    void Run();

    string Log();
    INIT_STATES getInitState();

    void PrintDisplayModes();
    void PrintOpenGLData();

    // ***********************************
    // GL Context und Window Creation
    // ***********************************
    SDL_Window * CreateGLWindow(bool fullscreen = false,string caption = "GLEngine");
    SDL_Window * GetGLWindow();

    // ***********************************
    // Display data
    // ***********************************
    SDL_DisplayMode getCurrentDisplayMode();
    string getCurrentDisplayModeAsString();

protected:
    void sdl_die(string msg);
    bool HandleMessage();

    string Logtext;

    SDL_GLContext glContext;
    SDL_Window * GLWindow =  nullptr;
    SDL_DisplayMode DesktopDisplayMode;

    int _ResX;
    int _ResY;
    bool _Quit;

    SDL_Event _Event;
    float4 _ClearColor;

    // State Handling
    INIT_STATES sdlstate;
};

#endif // GLFRAMEWORK_H
