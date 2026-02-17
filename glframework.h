#ifndef GLFRAMEWORK_H
#define GLFRAMEWORK_H

#include <string>
#include </usr/include/SDL2/SDL.h>
#include "GLFrameWork_global.h"
#include "vartypes.h"


class GLFRAMEWORK_EXPORT GLFrameWork
{    

public:
    GLFrameWork();
    bool InitSDL();
    virtual void DestroySDL();
    void Run();
    std::string Log();
    void PrintDisplayModes();
    void PrintOpenGLData();

    // ***********************************
    // GL Context und Window Creation
    // ***********************************
    SDL_Window * CreateGLWindow(bool fullscreen = false,std::string caption = "GLEngine");
    SDL_Window * GetGLWindow();
protected:
    void sdl_die(std::string msg);
    bool HandleMessage();

    std::string Logtext;
    bool InitSDL_done;

    SDL_GLContext glContext;
    SDL_Window * GLWindow =  nullptr;
    SDL_DisplayMode DesktopDisplayMode;

    int _ResX;
    int _ResY;
    bool _Quit;

    SDL_Event _Event;

    float4 _ClearColor;

};

#endif // GLFRAMEWORK_H
