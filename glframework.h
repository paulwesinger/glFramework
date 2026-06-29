#ifndef GLFRAMEWORK_H
#define GLFRAMEWORK_H

#include <string>
#include </usr/include/SDL2/SDL.h>
#include "GLFrameWork_global.h"
#include "vartypes.h"

#include "rendertext.h"

using namespace std;

typedef enum{
    SDL_CONTEXT_OK = 0,
    SDL_CONTEXT_FAILED,
    SDL_WINDOWCREATE_OK,
    SDL_WINDOWCREATE_FAILED,

    SDL_ADD_CONTROL_OK,
    SDL_ADD_CONTROL_FAILED,
    SDL_STATE_UNDEFINED

}INIT_STATES;

class GLFRAMEWORK_EXPORT GLFrameWork
{
public:
    GLFrameWork(int resx, int resy);
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

    SDL_GLContext  SDL_Context();
    SDL_Window * SDLWindow();

    // ***********************************
    // setters
    // ***********************************
    void setClearColor(float4 col);
    void setClearColor(float r,float g, float b,float a= 1.0f);

    // ***********************************
    // init shader, clearcolor usw..
    // ***********************************
    bool initViewElements();

protected:
    void sdl_die(string msg);
    bool HandleMessage();

    string Logtext;

    SDL_GLContext glContext;
    SDL_Window * GLWindow =  nullptr;
    SDL_DisplayMode DesktopDisplayMode;

    bool AddTextDisplay();

    int _ResX;
    int _ResY;
    bool _Quit;

    Shader * _Shader;

    SDL_Event _Event;
    float4 _ClearColor;
    // State Handling
    INIT_STATES sdlstate;

    //----------------------------------------------
    // View - Elements: in listen packen.
    // zb.: TextFenster, Controlls, 3D objects
    // ---------------------------------------------
    std::vector<RenderText*> _Texts;
};

#endif // GLFRAMEWORK_H
