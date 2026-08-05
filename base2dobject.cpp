#include "base2dobject.h"
#include <SDL_image.h>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include "imageloader.h"



const int STD_RECT_WIDTH = 200;
const int STD_RECT_HEIGHT = 50;

static const GLushort vertex_indices[] =
    {
        0, 1, 2, 3,4, 5
};

ENGINE::BaseObject2D::BaseObject2D() {}

ENGINE::BaseObject2D::BaseObject2D(int resx, int resy, Shader * sh)
{

    setImage("");

    _Size.w = STD_RECT_WIDTH;
    _Size.h = STD_RECT_HEIGHT;
    shader = sh;
    Init(resx, resy);
}

ENGINE::BaseObject2D::BaseObject2D(int resx, int resy, std::string path, Shader * sh)
{
    setImage(path);
    shader = sh;

    _Size.w = STD_RECT_WIDTH;
    _Size.h = STD_RECT_HEIGHT;

    Init(resx,resy);
}


ENGINE::BaseObject2D::BaseObject2D(const BaseObject2D& orig)
{
}

ENGINE::BaseObject2D::~BaseObject2D() {}

void ENGINE::BaseObject2D::setShaderPtr(Shader*pt) {
    // Pointer from initGL
    shader = pt;
}

Shader * ENGINE::BaseObject2D::getShaderPtr() {
    return shader;
}

void ENGINE::BaseObject2D::useShader(int type) {
    if ( type == (TEXTURE_SHADER &&  _ImagePath == "") ) {
        _CurrentShader = _ColorShader;
        return;
    }

    switch( type) {
    case TEXTURE_SHADER: _CurrentShader = shader->getTexture2DShader(); break;
    case COLOR_SHADER: _CurrentShader = shader->getColor2DShader();  break;
    default:
        _CurrentShader = shader->getTexture2DShader();
    }
}

bool ENGINE::BaseObject2D::Init(int resx, int resy)
{
    // ---------------------------
    // Color Init
    //----------------------------
    _Color          = glm::vec4(1.0, 1.0, 1.0, 1.0);
    _DisableColor   = glm::vec4(0.5, 0.5, 0.5, 0.5);
    _Enable = true;

    // ---------------------------
    // Texture Shader init :
    // ---------------------------
    _ResX = resx;
    _ResY = resy;

    if ( _ImagePath != "") {

        ImageLoader imgload;
        SDL_Surface * surface = imgload.getSurface( _ImagePath,"Base2D");

        char * data = nullptr;
        if ( surface ) {

            data = static_cast<char*>( surface ->pixels);
            _Size.w  = surface-> w;
            _Size.h = surface-> h;
        }

        if ( data ) {

            glGenerateMipmap(GL_TEXTURE_2D);
            glActiveTexture(GL_TEXTURE0);
            glGenTextures(1,&_Texture);
            glBindTexture(GL_TEXTURE_2D,_Texture);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            // set texture filtering parameters
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            glTexImage2D(GL_TEXTURE_2D, 0,  GL_RGBA, surface->w, surface->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
            glBindTexture(GL_TEXTURE_2D, 0);

            if (shader != nullptr)
                _CurrentShader = shader->getTexture2DShader();
            else
                _CurrentShader = 0;
        }
    }
    else {
        _Size.w = STD_RECT_WIDTH;
        _Size.h = STD_RECT_HEIGHT;

        if (shader != nullptr)
            _CurrentShader = shader->getColor2DShader();
        else
            _CurrentShader = 0;

    }

    // -----------------------------------------------
    // VertexArraobject und VertexBufferObject
    // -----------------------------------------------
    glGenVertexArrays(1,&_VAO);
    glBindVertexArray(_VAO);
    glGenBuffers(1,&_VBO);
    glBindBuffer(GL_ARRAY_BUFFER,_VBO);

    glBufferData(GL_ARRAY_BUFFER,
                 //                 sizeof(vertex_positions),
                 //                 vertex_positions,
                 sizeof(GLfloat) * 6 * 4,
                 nullptr,
                 GL_DYNAMIC_DRAW);

    glVertexAttribPointer(0,2,GL_FLOAT,GL_FALSE, 4 * sizeof(GLfloat),(void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1,2,GL_FLOAT,GL_FALSE, 4 * sizeof(GLfloat),(void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // --------------   Index
    glGenBuffers(1,&_EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,_EBO);
    glBufferData (GL_ELEMENT_ARRAY_BUFFER,
                 sizeof (vertex_indices),
                 vertex_indices,
                 GL_DYNAMIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
    glBindBuffer(GL_ARRAY_BUFFER,0);
    glBindVertexArray(0);

    return true;
}

void ENGINE::BaseObject2D::setImage(std::string path) {
    _ImagePath = path;
}

void ENGINE::BaseObject2D::SetDragable() { _Dragable = true; }
void ENGINE::BaseObject2D::ResetDragable() { _Dragable = false;}
bool ENGINE::BaseObject2D::IsDragAble() { return _Dragable; }
void ENGINE::BaseObject2D::OnClick(){}   // Im child überschreiben
int ENGINE::BaseObject2D::ID(){}

void ENGINE::BaseObject2D::OnRelease() {}

void ENGINE::BaseObject2D::OnMouseHover(){}
// ---------------------------------------------------
// Render Function
// param in [x] - X Position in screencoords
// param in [y] - y Position in screnecoords
// ---------------------------------------------------
void ENGINE::BaseObject2D::Render( ) {


    if (_CurrentShader) {
        glUseProgram(_CurrentShader);

        glActiveTexture(GL_TEXTURE0);
        _Projection =  glm::ortho(0.0f,static_cast<float>(_ResX),static_cast<float>(_ResY), 0.0f,  -1.0f, 1.0f);

        _Uniform_mv_projectloc = glGetUniformLocation(_CurrentShader,"projection");

        _Uniform_colorloc   = glGetUniformLocation(_CurrentShader,"col2D");

        glUniform4f(_Uniform_colorloc, _Color.r, _Color.g, _Color.b, _Color.a);

        GLfloat w = _Size.w;
        GLfloat h = _Size.h;

        GLfloat px = static_cast<GLfloat>(_Pos.x);
        GLfloat py = static_cast<GLfloat>(_Pos.y);


        GLfloat vertices[6][4] = {
            { px,     py  + h,        0.0, 1.0},//0.0 },
            { px,     py,             0.0, 0.0},//1.0 },
            { px + w, py,             1.0, 0.0},//1.0 },

            { px, py + h,             0.0, 1.0},//0.0 }, // w muss weg für 6  uv = 0,0 !!
            { px + w, py,             1.0, 0.0},//1.0 },
            { px + w, py + h,         1.0, 1.0} //0.0 }
        };

        glm::mat4 Model(1.0f);
        glm::mat4 mvp = _Projection * Model ;
        glUniformMatrix4fv(_Uniform_mv_projectloc, 1, GL_FALSE, glm::value_ptr(mvp)); //projection));


        glBindVertexArray(_VAO);
        glBindBuffer(GL_ARRAY_BUFFER,_VBO);
        glBufferSubData(GL_ARRAY_BUFFER,0,sizeof(vertices),vertices);
        glBindBuffer(GL_ARRAY_BUFFER,0);
        // Texture
        glBindTexture(GL_TEXTURE_2D,_Texture);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,_EBO);
        glDrawElements( GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, nullptr);


        // Aufräumen
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
        glBindVertexArray(0);
        glBindTexture(GL_TEXTURE_2D,0);
    }
}

// static inline mat4 ortho(float left, float right, float bottom, float top, float n, float f)
// {
//     return mat4( vec4(2.0f / (right - left), 0.0f, 0.0f, 0.0f),
//                 vec4(0.0f, 2.0f / (top - bottom), 0.0f, 0.0f),
//                 vec4(0.0f, 0.0f, 2.0f / (n - f), 0.0f),
//                 vec4((left + right) / (left - right), (bottom + top) / (bottom - top), (n + f) / (f - n), 1.0f) );
// }
