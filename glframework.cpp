#include <glew.h>
#include "glframework.h"
#include <sstream>


GLFrameWork::GLFrameWork(int resx,int resy) {

    _ResX = resx;
    _ResY = resy;

    _Quit = false;

    if (initViewElements())
        sdlstate = SDL_CONTEXT_OK;
    else
        sdlstate = SDL_CONTEXT_FAILED;

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

  //  Logtext += "InitViewElements: Stelle Hintergrund auf (Grau) R: " + std::to_string(_ClearColor.x) + "G: " + std::to_string(_ClearColor.y) +
    //           "B: " + std::to_string(_ClearColor.z);


    return true;
}

std::string GLFrameWork::Log(){
    return Logtext;
}

SDL_Window* GLFrameWork:: GetGLWindow(){
    return GLWindow;
}

void GLFrameWork::PrintDisplayModes(){

    int numDisplaymodes = SDL_GetNumDisplayModes(0);
    Logtext += "Num Display modes: " + std::to_string(numDisplaymodes);

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
            }
            catch ( ...) {
                Logtext += "Konnte Display Mode[" + index + "] nicht ermitteln\n";
            }

        }
    }
}

bool GLFrameWork::InitSDL(){

    int ret = SDL_Init(SDL_INIT_VIDEO);
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


    SDL_GL_MakeCurrent(GLWindow,glContext);



    SDL_GL_SetSwapInterval(1);


    DesktopDisplayMode.w = _ResX;
    DesktopDisplayMode.h = _ResY;
    DesktopDisplayMode.format = SDL_PIXELFORMAT_RGBA8888;
    SDL_SetWindowDisplayMode(GLWindow,&DesktopDisplayMode);

    sdlstate = INIT_STATES::SDL_CONTEXT_OK;
    return GLWindow;
}

bool GLFrameWork::AddTextDisplay(){

    sPoint p(10,10);
    RenderText * t = new RenderText(_ResX,_ResY,p,_Shader);



    // TextDraw * t = new TextDraw(_ResX,_ResY,p,_Shader);

    _Texts.push_back(t);

    //  string sCount = std::to_string( _Displays.size());


    return true;
}

SDL_GLContext GLFrameWork::SDL_Context(){
    return glContext;
}

SDL_Window* GLFrameWork::SDLWindow(){
    return GLWindow;
}

// ***********************************************
// get - functions
// ***********************************************
void GLFrameWork::setClearColor(float4 col){
    _ClearColor = col;
}

void GLFrameWork::setClearColor(float r, float g, float b, float a){
    float4 col= {r,g,b,a};
    _ClearColor = col;
}

// ***********************************************
// get - functions
// ***********************************************

INIT_STATES GLFrameWork::getInitState(){
    return sdlstate;
}

SDL_DisplayMode GLFrameWork::getCurrentDisplayMode(){
    return DesktopDisplayMode;
}

string GLFrameWork::getCurrentDisplayModeAsString(){
    stringstream stream;

    stream  <<"Aktueller Display- Modus:\n" <<
        to_string(DesktopDisplayMode.w) << " x " << to_string(DesktopDisplayMode.h)  <<
        " Format: " << DesktopDisplayMode.format;

    return stream.str();
}

// **********************************************
// event handling
// **********************************************
bool GLFrameWork::HandleMessage(){


    while (SDL_PollEvent(&_Event))
    {
        switch(_Event.type) {
        case SDL_KEYDOWN:
            switch(_Event.key.keysym.sym) {
            case SDLK_ESCAPE:
                _Quit =true;
                break;
            }
            break;
        case SDL_KEYUP:
            break;
        default:
            break;
        }
    }
    return true;
}

void GLFrameWork::Run(){

    glEnable(GL_DEPTH_TEST);

    while (! _Quit) {

        HandleMessage();

        glDepthFunc(GL_LEQUAL);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor( _ClearColor.x, _ClearColor.y, _ClearColor.z, _ClearColor.w);

        for(RenderText* elems:_Texts) {
             elems->Draw();
         }



        SDL_GL_SwapWindow(GLWindow);
        //_Quit =true;
    }
}

void GLFrameWork::PrintOpenGLData(){

    printf("Vendor :  %s\n",glGetString(GL_VENDOR));
    printf("Renderer  %s\n",glGetString(GL_RENDERER));
    printf("Version   %s\n",glGetString(GL_VERSION));
}

void GLFrameWork::DestroySDL(){
    if(GLWindow != nullptr)
        SDL_DestroyWindow(GLWindow);

}

void GLFrameWork::sdl_die(std::string msg){
    Logtext += msg + "\n";
}
