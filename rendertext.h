#ifndef RENDERTEXT_H
#define RENDERTEXT_H

#include <ft2build.h>
#include FT_FREETYPE_H

#include <string>
#include <vector>
#include <map>

#include <glew.h>
#include <gl.h>
#include <glm.hpp>
#include "shader.h"
#include "vartypes.h"
#include <logger.h>


using namespace std;

const string GNU_DEFAULT_FONT = "/usr/share/fonts/gnu-free/FreeSans.ttf";
const string GNU_DEFAULT_FONT_BOLD = "/usr/share/fonts/gnu-free/FreeSansBold.ttf";

const string GNU_DEFAULT_OBLIQUE_FONT = "/usr/share/fonts/gnu-free/FreeSansOblique.ttf";
const string GNU_DEFAULT_OBLIQUE_FONT_BOLD = "/usr/share/fonts/gnu-free/FreeSansBoldOblique.ttf";

struct sCharacter {
    GLuint     TextureID;  // ID handle of the glyph texture
    glm::ivec2 Size;       // Size of glyph
    glm::ivec2 Bearing;    // Offset from baseline to left/top of glyph
    FT_Pos     Advance;    // Offset to advance to next glyph
    GLuint     Height;
};

struct sTextfeld{
    GLfloat x;
    GLfloat y;
    GLfloat w;
    GLfloat h;
};


namespace ENGINE {
    class RenderText
    {
    public:
        RenderText(int resx, int resy,Shader * sh);
        RenderText(int resx, int resy, sPoint pos, Shader * sh,int id,string name);

        ~RenderText();
        void initConstructor(int resx,int resy,sPoint pos,Shader * sh);
        bool Init();
        void SetText(std::string text, int index = -1 );
        string GetText(int index = 0);

        void SetHasBackground(bool hasbg);
        void Draw();
    protected:
        bool _AlignRight;
        bool _FAILED;
        GLfloat _Scale;

        glm::vec4 _TextColor;
        glm::vec4 _BackgroundColor;

        int _ResX;
        int _ResY;

        sPoint _Pos;

        GLfloat posX;
        GLfloat posY;

        GLuint _VAO;
        GLuint _VBO;
        GLuint _EBO;

        // Buffers und arrays für Background
        GLuint _bgVAO;
        GLuint _bgVBO;
        GLuint _bgEBO;

        Shader * _Shader;
        // ints for shader returns
        int vs;
        int fs;

        // -----------------------
        // Shader für das Textfeld
        //------------------------
        int vs_textfeld;
        int fs_textfeld;
        uint _TextureShader,_GlyphShader,_ColorShader,_CurrentShader;

        GLint mv_projectloc;
        GLint uniform_colorloc;
        // uniformlocations in textfeldshader --> nur einmal setzen ,
        // für alle 3 Renderer(Headline Paintarea, Bottom)
        GLuint projection_loc, framecolor_loc;

        glm::mat4 MatOrtho;

        bool _HasTexture;
        bool _HasBackGround;

        // -------------------------------
        // Includes und libs für Freetype2
        // -------------------------------
        FT_Library ft;
        FT_Face face;
        std::string _Font;

        // Einige Felder für look and feel:
        FT_UInt _Pixelsize;
        GLfloat _MarginLeft,_MarginRight, _MarginY;

        std::map<GLchar, sCharacter> Characters;
        //Hilfsfunktion zum einlesen der 5 Texturen für Textfeld
        bool GenTextfeldSegment(std::string image, unsigned int &tex);
        void SetGlyphShader(GLuint s);
        sRect _Textfeld;
        std::vector<std::string> _StringList;

        int _DisplayID;
        std::string  _DisplayName;


    private:
        // hlpvars for drag
        int distX;
        int distY;
        sRect  interSectHeadline;
        bool   _Dragging;
        Logger log;


        void CalcSize(int &weite, int &height);
        void RenderPaintarea(GLfloat x, GLfloat y, GLfloat height);
        void RenderFrame(GLfloat x, GLfloat y);

    };
}
#endif // RENDERTEXT_H
