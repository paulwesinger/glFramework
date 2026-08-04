#ifndef BASEOBJECT_H
#define BASEOBJECT_H

#include <gl.h>

#include <vec3.hpp>
#include <vec4.hpp>
#include <mat4x4.hpp>

#include <vector>
#include <string>
#include <logger.h>

#include "vartypes.h"

using namespace glm;


uint const  MAX_TEXTURES = 5;

class BaseObject
{
public:

    BaseObject();
    BaseObject(const BaseObject& orig);
    BaseObject(vec3 pos,vec3 rotate,vec3 scale,vec4 col );
    virtual ~BaseObject();

    // transormations
    virtual vec3 Translation();
    virtual vec3 Rotatation();
    virtual vec3 ScaleSize();
    virtual bool HasAnimation();

    virtual vec4 Color();
    virtual void SetColor(vec4 col);

   // virtual void Draw( Camera * cam);

    virtual void Translate(vec3 trans);
    virtual void Rotate(vec3 rotate);  // ACHTUN : Hält nur den aktuell zu drehenden Winkel für jede Achse
    virtual void Scale(vec3 scale);
    virtual void SetPosition(vec3 pos);

    virtual void StepTranslate(vec3 step, uint elapsed);
    virtual void StepRotate(vec3 step,  uint elapsed);
    virtual void StepScale(vec3 step, uint elapsed);

    virtual void AnimateRotate(uint elapsed);
    virtual void AnimateTranslate(uint elapsed);
    virtual void AnimateScale(uint elapsed);


    virtual bool addTexture(std::vector<std::string> path, std::string obj);
    virtual bool addTexture(std::string path, uint activtexture = 0);

    virtual void setActiveShader(ShaderType t);
    virtual void initShader(ShaderType s,GLuint prog);
    //virtual void addLight(light * l);
    virtual void setDrawMode(GLuint mode); // For drawing poligons
    virtual void setPolygonMode(GLuint mode);
    virtual void setGlasShader(bool useglas);

    // Flags
    void SetFirstTranslate(bool ok);
    void SetAlphaColor(glm::vec3 alphacol);
    void SetHasTextures(bool hasATexture);
    void SetHasAlpha(bool yes);
    void SetFrontFace(uint mode);
    void SetUseBlending(bool useblending);

    bool HasAlpha();
    bool UseGlasShader();
    bool UseBlending();
    bool GetFirstTranslate();
    bool HasTextures();
    bool UseBlinn(bool b);

    vec3 Position();
    vec3 AlphaColor();
    void SetProjection(glm::mat4 pro,bool orthomode = false);
    mat4 & GetProjection();


protected:
    bool _HasAnimation; // Flag für abgeleitete Objekte

    GLuint
        perspectiveTextureShader,
        perspectiveColorShader,
        lightshader,
        lightcolorshader,
        lighttextureshader,
        glasshader,
        oldshader,
        currentShader;

    uint _FrontFace_Mode;

    // This vectors holds the current translation,rotation,scale
    vec3 _Rotate;
    vec3 _Translate;
    vec3 _Scale;
    vec3 _Position;

    vec4 _Color; // inkl. Alpha
    bool _FirstTranslate;  // Flag ,ob erst veschoben oder rotiert werden soll

    glm::mat4 _Projection;
    bool _IsOrtho;
    bool _HasTextures;
    bool _HasAlpha;
    bool _UseGlasshader;
    bool _UseBlending;
    bool _UseBlinn;
    glm::vec3 _AlphaColor;

    uint _Elapsed;


    uint _CountTextures;  // die Anzehl = Listen länge
    GLuint  _Textures[5];

    GLuint _DrawMode;
    GLuint _PolgonMode;

    // Light
   // light *  _Light;

private:
    void init();

    Logger log;
};

#endif // BASEOBJECT_H
