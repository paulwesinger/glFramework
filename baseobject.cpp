//#include <SDL.h>
#include "baseobject.h"
/*
 * File:   baseobject.cpp
 * Author: paul
 *
 * Created on 11. August 2018, 16:31
 */
#include "logger.h"
#include "utils.h"
#include "imageloader.h"
//#include "../vecmath/vecmath.h"
#include "vartypes.h"
#include "defaults.h"

const glm::vec3 VEC3_ZERO = glm::vec3(0.0,0.0,0.0);

BaseObject::BaseObject() {

    _Position = VEC3_ZERO;
    _Rotate = VEC3_ZERO;
    _Scale = VEC3_ZERO;
    _Color = glm::vec4(VEC3_ZERO,1.0);
    init();
}

BaseObject::BaseObject(const BaseObject& orig) {

    _Position = orig._Position;
    _Rotate = orig._Rotate;
    _Scale  = orig._Scale;
    _Translate = orig._Translate;
    init();
}

BaseObject::BaseObject(vec3 pos,vec3 rotate,vec3 scale , vec4 col) {

    _Position = pos;
    _Rotate = rotate;
    _Scale = scale;
    _Color = col;
    init();
}

BaseObject::~BaseObject() {
}

void BaseObject::init( ) {
    _IsOrtho        = false;
    _FirstTranslate = true;
    _HasTextures    = false;
    _CountTextures  = 0;
    _HasAlpha       = false;
    _Elapsed        = 0;

    // Standard wert für Alpha: Black
    _AlphaColor.r   = 0.0;
    _AlphaColor.g   = 0.0;
    _AlphaColor.b   = 0.0;

    _DrawMode       = GL_TRIANGLE_STRIP;
    _PolgonMode     = GL_FILL;
    _HasAnimation   = false;
    //_Light          = nullptr;
}

//void BaseObject::Draw(Camera * ){}

// void BaseObject::addLight(light *l) {
//     _Light = l;
// }

bool BaseObject::addTexture(std::string path, uint activetexture) {

    _HasTextures = false;
    if (activetexture > 4) {
        log.logwarn("Value for activeTexture to high, choose [0..4] ", "BaseObject::addTexture");
        return false;
    }

    SDL_Surface * surface = ImageLoader::getSurface(IMAGEPATH::ROOT + path, "BaseObject::addTexture");
    if ( surface ) {

        int width = surface->w;
        int height = surface->h;
        char * data = static_cast<char*>(surface->pixels);

        if ( data ) {
            _HasTextures = true;
            GLuint tex;
            glGenTextures(1,&tex);

            switch (activetexture) {
            case 0: glActiveTexture(GL_TEXTURE0); glBindTexture(GL_TEXTURE_2D,tex); break;
            case 1: glActiveTexture(GL_TEXTURE1); glBindTexture(GL_TEXTURE_2D,tex); break;
            case 2: glActiveTexture(GL_TEXTURE2); glBindTexture(GL_TEXTURE_2D,tex); break;
            case 3: glActiveTexture(GL_TEXTURE3); glBindTexture(GL_TEXTURE_2D,tex); break;
            case 4: glActiveTexture(GL_TEXTURE4); glBindTexture(GL_TEXTURE_2D,tex); break;
            }
            // *******************************************
            // Generate Textures
            // *******************************************
            _Textures[activetexture] = tex;
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

            log.logimage("Texture " + path + " geladen. Added to ActiveTexture" + IntToString(activetexture));

            _CountTextures ++ ;
            return true;
        } // data
    } // surface
    return  false;
}

bool BaseObject::addTexture(std::vector<std::string> path, std::string obj) {


    _HasTextures = false;
    if (path.size() > 5 ) {
        log.logwarn("Anzahl Texturen überschreitet maximum(" + IntToString(MAX_TEXTURES) +")"," BaseObject::addTexture");
        log.logwarn("Keine Texturen geladen !");
        return false;
    }

    for ( uint i = 0; i< path.size(); i++) {

        SDL_Surface * surface = ImageLoader::getSurface(IMAGEPATH::ROOT + path[i], "BaseObject::Init");
        if ( surface ) {
            int width = surface->w;
            int height = surface->h;
            char * data = static_cast<char*>(surface->pixels);

            if ( data ) {
                _HasTextures = true;
                GLuint tex;
                glGenTextures(1,&tex);
                switch (i) {
                case 0: glActiveTexture(GL_TEXTURE0); glBindTexture(GL_TEXTURE_2D,tex); break;
                case 1: glActiveTexture(GL_TEXTURE1); glBindTexture(GL_TEXTURE_2D,tex); break;
                case 2: glActiveTexture(GL_TEXTURE2); glBindTexture(GL_TEXTURE_2D,tex); break;
                case 3: glActiveTexture(GL_TEXTURE3); glBindTexture(GL_TEXTURE_2D,tex); break;
                case 4: glActiveTexture(GL_TEXTURE4); glBindTexture(GL_TEXTURE_2D,tex); break;
                }
                // *******************************************
                // Generate Textures
                // *******************************************
                _Textures[i] = tex;
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

                log.logimage("Texture "+ IMAGEPATH::ROOT + path[i]+ " geladen. Index = " + IntToString(i),obj);
                _CountTextures ++ ;
            }  // if data
            else
                log.logwarn("Texture  nicht geladen !"," BaseObject::addTexture");
        } //if surface
    }  // for
    log.loginfo("addTexture :: _CountTextures: " + IntToString(_CountTextures-1),"Baseobjec::Init");
    if (_HasTextures)
        return true;
    return  false;
}

void BaseObject::initShader(ShaderType s, GLuint prog) {
    switch (s) {

    case COLOR_SHADER:          perspectiveColorShader = prog; break;
    case TEXTURE_SHADER:        perspectiveTextureShader = prog; break;
    case LIGHT_SHADER:          lightshader = prog; break;
    case LIGHT_COLOR_SHADER:    lightcolorshader = prog; break;
    case LIGHT_TEXTURE_SHADER:  lighttextureshader = prog; break;
    case GLASS_SHADER        :  glasshader = prog; break;
    default:
        perspectiveColorShader = prog;
    }
}

void BaseObject::setActiveShader(ShaderType t){

    switch (t) {
        case COLOR_SHADER       : currentShader = perspectiveColorShader; break;
        case TEXTURE_SHADER     : currentShader = perspectiveTextureShader; break;
        case LIGHT_SHADER       : currentShader = lightshader; break;
        case LIGHT_COLOR_SHADER : currentShader = lightcolorshader; break;
        case LIGHT_TEXTURE_SHADER   : currentShader = lighttextureshader;break;
        case GLASS_SHADER           : currentShader = glasshader; break;
        default:
            currentShader = perspectiveColorShader;
    }
}
// Transormatioons

vec3 BaseObject::Rotatation()       { return _Rotate; }
vec3 BaseObject::ScaleSize()        { return _Scale ; }
vec3 BaseObject::Translation ()    { return _Translate;}

void BaseObject::Rotate(vec3 rotate)    { _Rotate    = rotate; }
void BaseObject::Scale(vec3 scale)      {  _Scale     = scale;  }
void BaseObject::Translate(vec3 trans)  {  _Translate = trans;  }

void BaseObject::SetColor( vec4 col)       { _Color = col;         }
vec4 BaseObject::Color()                { return _Color;        }

void BaseObject::setGlasShader(bool useglas) {
    if (useglas) {
        _UseGlasshader = true;
        oldshader = currentShader;
        setActiveShader(GLASS_SHADER);
    }
    else {
        currentShader = oldshader;
        _UseGlasshader = false;
    }

}

void BaseObject::StepTranslate(vec3 step, uint elapsed ) { _Translate += (step * static_cast<float>(elapsed)); }
void BaseObject::StepScale(vec3 step, uint elapsed)      { _Scale += (step *static_cast<float>(elapsed));      }
void BaseObject::StepRotate(vec3 step, uint elapsed )    { _Rotate += (step * static_cast<float>(elapsed));    }

//----------------------------------
// for overrides in animate child
//----------------------------------
void BaseObject::AnimateRotate(uint) {}
void BaseObject::AnimateTranslate(uint) {}
void BaseObject::AnimateScale(uint) {}

void BaseObject::SetFirstTranslate(bool ok){ _FirstTranslate = ok;    }
bool BaseObject::GetFirstTranslate()       { return _FirstTranslate;  }

void BaseObject::SetProjection(glm::mat4 pro, bool orthomode )  {      _Projection = pro; _IsOrtho = orthomode;  }
glm::mat4 & BaseObject::GetProjection()    {  return _Projection;      }

bool BaseObject::HasTextures()              { return  _HasTextures; }
void BaseObject::SetHasTextures(bool hasATexture) { _HasTextures = hasATexture; }

void BaseObject::SetHasAlpha(bool yes) { _HasAlpha = yes;  }
bool BaseObject::HasAlpha() {return _HasAlpha; }

void BaseObject::SetAlphaColor(glm::vec3 alphacol) { _AlphaColor = alphacol; }
glm::vec3 BaseObject::AlphaColor() {return _AlphaColor; }

void BaseObject::SetFrontFace(uint mode) { _FrontFace_Mode = mode; }

void BaseObject::setDrawMode(GLuint mode) {    _DrawMode = mode; }
void BaseObject::setPolygonMode(GLuint mode) { _PolgonMode = mode;}
bool BaseObject::HasAnimation() {     return _HasAnimation; }
void BaseObject::SetUseBlending(bool useblending) {_UseBlending = useblending; }

bool BaseObject::UseGlasShader() { return _UseGlasshader;}
bool BaseObject::UseBlending(){ return _UseBlending; }

bool BaseObject::UseBlinn(bool b) {
    _UseBlinn = b;
}
void BaseObject::SetPosition(vec3 pos) { _Position = pos; }



