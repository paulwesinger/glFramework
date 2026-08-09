#include <glew.h>
#include <sstream>
#include <iomanip>
#include <iostream>
#include <chrono>


#include "glframework.h"

using namespace  ENGINE;
using namespace  CLOCK;
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Game Clock
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++



GameClock::GameClock(){

}

void GameClock::Start(){
    _Start = Clock::now();
}


string GameClock::FPSasString(uint64_t el, string text){

    double fps = FPS(el);
    string ret;
    if (fps > 0){
        std::ostringstream os;
        os << std::fixed << std::setprecision(2)<< fps;
        ret = "FPS " + os.str();
    }
    else
        ret = text;

    return ret;
}

double GameClock::FPS(uint64_t el){

    static double framecount = 0;
    static uint64_t elapsedtotal;

    framecount ++;
    elapsedtotal += el;

    // +++++++++++++++++++++++
    // Berechnung in millisecs
    // +++++++++++++++++++++++
    if (elapsedtotal >= 1000) {
        elapsedtotal = 0;
        double tmp = framecount;
        framecount = 0;
        return tmp;
    }
    else
        return 0;

}

void GameClock::Stop()
{

}

uint64_t GameClock::Elapsed(){
    _End = Clock::now();
    uint64_t ret = std::chrono::duration_cast<std::chrono::milliseconds>(_End - _Start).count();
    _Start = _End;
    return ret;

}

// ++++++++++++++++++++ End Game Clock +++++++++++++++++++




GLFrameWork::GLFrameWork(int resx,int resy) {

    _ResX = resx;
    _ResY = resy;

    _Quit = false;

}


bool GLFrameWork::initViewElements(){

    Logtext += "Init Shader";
    _Shader = new Shader();

    // **************************
    // Standard grau
    // **************************
    _ClearColor.x = 0.5f;
    _ClearColor.y = 0.5f;
    _ClearColor.z = 0.5f;
    _ClearColor.w = 1.0f;
    glClearColor(_ClearColor.x, _ClearColor.y, _ClearColor.z, _ClearColor.w);

    return true;
}

SDL_Window* GLFrameWork:: GetGLWindow(){
    return GLWindow;
}

void GLFrameWork::PrintDisplayModes(){

    int numDisplaymodes = SDL_GetNumDisplayModes(0);
    Logtext += "Num Display modes: " + std::to_string(numDisplaymodes);

    cout << Logtext << endl;
    // Alle Display modes auflisten:

    for(int j = 0; j < SDL_GetNumVideoDisplays(); j++)
    {
        for (int i = 0; i < numDisplaymodes; i++ ) {
            SDL_DisplayMode dpm;
            SDL_GetDisplayMode(j,i,&dpm);
            std::string w = std::to_string(dpm.w);
            std::string h = std::to_string(dpm.h);
            std::string index = std::to_string(i);
            try {

                // todo: Pixelformat auswerten
                Logtext += "Display : " + std::to_string(j) + " Resolution Mode[" + index+ "] : " + w + "x" + h + "\n";
                cout << "Display : " << std::to_string(j) << " Resolution Mode[" << index << "] : " <<  w << "x" << h << std::endl;

            }
            catch ( ...) {
                Logtext += "Konnte Display Mode[" + index + "] nicht ermitteln\n";
            }
        }
    }
}

bool GLFrameWork::InitSDL(){

    int ret = SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
    if (ret != 0)
        return ret;

    atexit(SDL_Quit);
    Logtext = "SDL-Init Ok\n";
    // Attribute
    SDL_GL_LoadLibrary(NULL);
    SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL,1);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION,4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION,5);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER,1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE,24);

    // **********************************
    // SDL Window init:
    // **********************************

    GLWindow = CreateGLWindow();

    return GLWindow != nullptr;
}

SDL_Window * GLFrameWork::CreateGLWindow(bool fullscreen,std::string caption){
    if ( fullscreen) {
        GLWindow = SDL_CreateWindow(
            caption.c_str(),
            0, //SDL_WINDOWPOS_UNDEFINED,
            0, //SDL_WINDOWPOS_UNDEFINED,
            _ResX,_ResY,
            //SDL_WINDOW_FULLSCREEN_DESKTOP | SDL_WINDOW_OPENGL
            SDL_WINDOW_OPENGL
            );
        SDL_SetWindowFullscreen(GLWindow,SDL_WINDOW_FULLSCREEN);

    }
    else {

        GLWindow = SDL_CreateWindow(
            caption.c_str(),
            0,
            0,
            _ResX,_ResY,
            SDL_WINDOW_OPENGL);
    }
    glViewport(0,0,_ResX, _ResY);

    if ( GLWindow == nullptr)  {
        sdlstate = INIT_STATES::SDL_WINDOWCREATE_FAILED;
        sdl_die("Konnte Fenster nicht erzeugen");
        return nullptr;
    }

    glContext = SDL_GL_CreateContext(GLWindow);
    if ( ! glContext) {
        sdlstate = INIT_STATES::SDL_CONTEXT_FAILED;
        sdl_die (" Konnte Context nicht erzeugen ");
        return nullptr;
    }

    glewExperimental = GL_TRUE;
    glewInit();

    SDL_GL_SetSwapInterval(1);


    DesktopDisplayMode.w = _ResX;
    DesktopDisplayMode.h = _ResY;
    DesktopDisplayMode.format = SDL_PIXELFORMAT_RGBA8888;
    SDL_SetWindowDisplayMode(GLWindow,&DesktopDisplayMode);

    sdlstate = INIT_STATES::SDL_CONTEXT_OK;
    return GLWindow;
}

bool GLFrameWork::AddTextDisplayWithBackground(int x, int y,int id,string name){

    try{
        sPoint p(x,y);
        RenderText * t = new RenderText(_ResX,_ResY,p,_Shader,id,name);
        t->SetHasBackground(true);
        _Displays.push_back(t);
        return true;
    }
    catch(exception e){
        std::cout << e.what() << std::endl;
        return false;
    }
}

bool GLFrameWork::AddTextDisplay(int x, int y,int id,string name){

    try{
        sPoint p(x,y);
        RenderText * t = new RenderText(_ResX,_ResY,p,_Shader,id,name);
        t->SetHasBackground(false);
        _Displays.push_back(t);
        return true;
    }
    catch(exception e){
        std::cout << e.what() << std::endl;
        return false;
    }
}


void GLFrameWork::Prepare2D() {
    glDisable(GL_CULL_FACE);
    glDisable(GL_DEPTH_TEST);
    glDepthMask(0);
}

void GLFrameWork::Restore3D() {
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);

    glFrontFace(GL_CCW);
    glEnable(GL_BLEND);
    glDepthMask(1);
}

// ***********************************************
// set - functions
// ***********************************************
void GLFrameWork::setClearColor(sFloat4 col){
    _ClearColor = col;
}

void GLFrameWork::setClearColor(float r, float g, float b, float a){
    sFloat4 col= {r,g,b,a};
    _ClearColor = col;
}

// ***********************************************
// get - functions
// ***********************************************


std::chrono::duration<double> GLFrameWork::Elapsed() {
    return _Elapsed;
}


INIT_STATES GLFrameWork::getInitState(){
    return sdlstate;
}

SDL_DisplayMode GLFrameWork::getCurrentDisplayMode(){
    return DesktopDisplayMode;
}

string GLFrameWork::getCurrentDisplayModeAsString(){
    stringstream stream;

    stream << "Aktueller Display- Modus:\n" <<
        to_string(DesktopDisplayMode.w) << " x " << to_string(DesktopDisplayMode.h)  <<
        " Format: " << DesktopDisplayMode.format;

    return stream.str();
}

// void GLFrameWork::GetKey(){
// }

KEYBOARDSTATE GLFrameWork::KeyState(){
     const Uint8 * keystate = SDL_GetKeyboardState(NULL);

    if (keystate[SDL_SCANCODE_W] || keystate[SDL_SCANCODE_UP]) {
        KeyboardState.BtnStatePress = BTN_PRESS_UP_KEY;
    }
    else
    if (keystate[SDL_SCANCODE_S] || keystate[SDL_SCANCODE_DOWN]) {
        KeyboardState.BtnStatePress = BTN_PRESS_DOWN_KEY;
    }
    else
    if (keystate[SDL_SCANCODE_A] || keystate[SDL_SCANCODE_LEFT]) {
        KeyboardState.BtnStatePress = BTN_PRESS_LEFT_KEY;
    }
    else
    if (keystate[SDL_SCANCODE_D] || keystate[SDL_SCANCODE_RIGHT]) {
        KeyboardState.BtnStatePress = BTN_PRESS_RIGHT_KEY;
    }
    else
    if (keystate[SDL_SCANCODE_KP_SPACE]) {
        KeyboardState.BtnStatePress = BTN_PRESS_SPACE_KEY;
    }
    else
        KeyboardState.BtnStatePress = BTN_PRESS_NO;

    return KeyboardState;


}
// **********************************************
// event handling
// **********************************************
void GLFrameWork::HandleMessage(){

    while (SDL_PollEvent(&_Event))
    {
        switch(_Event.type) {          

            case SDL_KEYUP:
            switch(_Event.key.keysym.sym) {
                case SDLK_ESCAPE:
                _Quit =true; break;
                    //KeyboardState.BtnStateUP = BTN_UP_ESC; GetKey(); break;
                case SDLK_LEFT:
                    //KeyboardState.BtnStateUP = BTN_UP_LEFT_KEY; GetKey(); break;
                case SDLK_RIGHT:
                    //KeyboardState.BtnStateUP = BTN_UP_RIGHT_KEY; GetKey(); break;
                case SDLK_DOWN:
                    //KeyboardState.BtnStateUP = BTN_UP_DOWN_KEY; GetKey(); break;
                case SDLK_UP:
                    //KeyboardState.BtnStateUP = BTN_UP_UP_KEY; GetKey(); break;
                case SDLK_SPACE:
                    //KeyboardState.BtnStateUP = BTN_UP_SPACE_KEY; GetKey(); break;
                default:
                    break;
                    //KeyboardState.BtnStateUP = BTN_UP_NO; GetKey();
            }   //switch
        }
    }
}

void GLFrameWork::SwapWindow(){
    SDL_GL_SwapWindow(GLWindow);
}

void GLFrameWork::Run(){

}

void GLFrameWork::PrintOpenGLData(){

    printf("Vendor :  %s\n",glGetString(GL_VENDOR));
    printf("Renderer  %s\n",glGetString(GL_RENDERER));
    printf("Version   %s\n",glGetString(GL_VERSION));
}


void GLFrameWork::ReleaseEngine(){

    for(RenderText* elem: _Displays)
    {
        delete elem;
    }
    _Displays.clear();
}

void GLFrameWork::DestroySDL(){
    if(GLWindow != nullptr)
        SDL_DestroyWindow(GLWindow);

}

void GLFrameWork::sdl_die(std::string msg){
    Logtext += msg + "\n";
}
