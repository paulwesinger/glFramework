#ifndef BASE2DOBJECT_H
#define BASE2DOBJECT_H

#include "base.h"
#include "shader.h"
#include <string>

namespace ENGINE {

    class BaseObject2D : public Base
    {
    public:
        BaseObject2D();
        BaseObject2D(int resx, int resy, Shader * sh);
        BaseObject2D(int resx, int resy, std::string path, Shader * sh);
        BaseObject2D(const BaseObject2D& orig);
        virtual ~BaseObject2D();

        bool Init(int resx,int resy);
        void setImage(std::string path);
        void useShader(int type);
        void setShaderPtr(Shader * pt);

        void SetDragable();
        void ResetDragable();
        bool IsDragAble();

        Shader * getShaderPtr();


        virtual void Render() override;
        virtual void OnClick();
        virtual void OnRelease();
        virtual void OnMouseHover();
        virtual int ID();

    protected:

        GLuint _VAO;
        GLuint _VBO;
        GLuint _EBO;
        //-----------------
        // Shaders
        //-----------------
        Shader * shader;

        // ints for shader returns
        GLuint _TextureShader;  // the linked shaders
        GLuint _ColorShader;
        GLuint _CurrentShader;
        GLuint _GlyphShader;

        // ---------------------------------
        // Parent member for kaskaded close
        // ..Like Menu with submenu..
        // ---------------------------------

        unsigned int _Texture;
        std::string _ImagePath;

        GLint _Uniform_mv_projectloc;
        GLint _Uniform_colorloc;
        glm::mat4 _Projection;

        bool _Dragable = false;

    };
}

#endif // BASE2DOBJECT_H
