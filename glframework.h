#ifndef GLFRAMEWORK_H
#define GLFRAMEWORK_H

#include "GLFrameWork_global.h"

#include <string>
#include <chrono>
#include </usr/include/SDL2/SDL.h>

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

// ++++++++++++++++++++++++++
// Keyboard Buttons - Pressed
// ++++++++++++++++++++++++++

typedef enum{
    BTN_PRESS_NO = 0,
    BTN_PRESS_LEFT_KEY,
    BTN_PRESS_RIGHT_KEY,
    BTN_PRESS_UP_KEY,
    BTN_PRESS_DOWN_KEY,
    BTN_PRESS_SPACE_KEY,
}BTN_PRESS;


// ++++++++++++++++++++++++++
// Keyboard Buttons - Up
// ++++++++++++++++++++++++++
typedef enum{
    BTN_UP_NO = 100,
    BTN_UP_LEFT_KEY,
    BTN_UP_RIGHT_KEY,
    BTN_UP_UP_KEY,
    BTN_UP_DOWN_KEY,
    BTN_UP_F1_KEY,      // Coin 1€
    BTN_UP_F2_KEY,      // Coin 2€
    BTN_UP_SPACE_KEY,
    BTN_UP_ESC,

    BTN_UP_A,
    BTN_UP_B,
    BTN_UP_C,
    BTN_UP_D,
    BTN_UP_E,
    BTN_UP_F,
    BTN_UP_G,
    BTN_UP_H,
    BTN_UP_I,
    BTN_UP_J,
    BTN_UP_K,
    BTN_UP_L,
    BTN_UP_M,
    BTN_UP_N,
    BTN_UP_O,
    BTN_UP_P,
    BTN_UP_Q,
    BTN_UP_R,
    BTN_UP_S,
    BTN_UP_T,
    BTN_UP_U,
    BTN_UP_V,
    BTN_UP_W,
    BTN_UP_X,
    BTN_UP_Y,
    BTN_UP_Z,
    // Ziffern
    BTN_UP_0,
    BTN_UP_1,
    BTN_UP_2,
    BTN_UP_3,
    BTN_UP_4,
    BTN_UP_5,
    BTN_UP_6,
    BTN_UP_7,
    BTN_UP_8,
    BTN_UP_9

}BTN_UP;

union KEYBOARDSTATE{
    BTN_UP BtnStateUP;
    BTN_PRESS BtnStatePress;
};

namespace CLOCK {

typedef std::chrono::high_resolution_clock Clock;

class GameClock {
public:
    GameClock();
    void Start();
    void Stop();

    uint64_t Elapsed();

    string FPSasString(uint64_t el,string text);
    double FPS(uint64_t el);

    void StartTimer(uint64_t timetoshot,FP callback);

protected:
    std::chrono::duration<double>  _Elapsed;
    Clock::time_point _Start;
    Clock::time_point _End;

    FP TimerCallback = nullptr;

    Clock::time_point _TimerStart;


    double fps = 0;
};

}

namespace ENGINE{

class GLFRAMEWORK_EXPORT GLFrameWork
{
public:
    GLFrameWork(int resx, int resy);
    bool InitSDL();
    virtual void DestroySDL();
    virtual void Run();

    virtual void HandleMessage();
    virtual KEYBOARDSTATE KeyState();

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

    std::chrono::duration<double> Elapsed();

    // ***********************************
    // setters
    // ***********************************
    void setClearColor(sFloat4 col);
    void setClearColor(float r,float g, float b,float a= 1.0f);

    // ***********************************
    // init shader, clearcolor usw..
    // ***********************************
    bool initViewElements();
    virtual bool AddTextDisplay(int x,int y,int id, string name);
    virtual bool AddTextDisplayWithBackground(int x, int y,int id,string name);
protected:
    void sdl_die(string msg);


    void SwapWindow();
    void Restore3D();
    void Prepare2D();

    void ReleaseEngine();
  //  virtual void GetKey() = 0;

    string Logtext;

    SDL_GLContext glContext;
    SDL_Window * GLWindow =  nullptr;
    SDL_DisplayMode DesktopDisplayMode;

    int _ResX;
    int _ResY;
    bool _Quit;

    Shader * _Shader;
    SDL_Event _Event;

    sFloat4 _ClearColor;
    // State Handling
    INIT_STATES sdlstate;

    // +++++++++++++++++++++++++++++++++
    //
    KEYBOARDSTATE KeyboardState;

    //----------------------------------------------
    // View - Elements: in listen packen.
    // zb.: TextFenster, Controlls, 3D objects
    // ---------------------------------------------
    std::vector<RenderText*> _Displays;

    // ---------------------------------------------
    // Chrone für zeit per frame
    // ---------------------------------------------
    std::chrono::duration<double> _Elapsed;


};
}

#endif // GLFRAMEWORK_H
