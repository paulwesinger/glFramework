#ifndef SHADER_H
#define SHADER_H

#include <glew.h>
#include <string>
#include <cstdio>
#include <fileutil.h>

class Shader
{
public:
    Shader();

    // Standard shaders for rendering
    void CreateStandardShaders();
    void CreatStandardShaderFromFile(std::string path);


    GLuint getGlyphShader();
    GLuint getTexture2DShader();
    GLuint getColor2DShader();

    GLuint getTexture3DShader();
    GLuint getColor3DShader();
    GLuint getLightShader();
    GLuint getLightColorShader();
    GLuint getGlasShader();


    int compileShader(const char* source, GLint type);
    int compileVertexShader(const char* source); // Vertex und Fragment Shader von Datei
    int compileFragmentShader(const char* source);
    int CreateProgram(int &vs,int &fs); //in vertexshader fragmentshader ret Programmindex
    // Vertex und Fragment Shader von Datei
    int compileVertexShaderFromFile(std::string path,FileUtil * reader);
    int compileFragmentShaderFromFile(std::string path,FileUtil * reader);
    void CreateCustomShader(GLuint &progID );  // Muss VOR AttachCustomShader aufgerufen werden!!
    void AttachCustomShader( GLuint &prog, int &shader);  //Shader dazuhängen
    void CreateCustomProgram(GLuint &prog); // Die ID des Shaders retour
    void deleteShader(int &shader);

    ~Shader();
protected:
    // Shaders for text and 2D
    GLuint _GlyphShader2D,_TextureShader2D,_ColorShader2D;

    //Shaders for 3D
    GLuint _TextureShader3D,_ColorShader3D,_LightShader3D, _LightShaderColor3D, _GlasShader3D;

private:

    bool CreateShaderProgram(GLuint & prg,const GLchar * vertexsource, const GLchar * fragmentsource);
    bool CreateStandardGlyphShader();
    bool CreateStandard2DColorShader();
    bool CreateStandard2DTextureShader();

    bool CreateStandard3DTextureShader();
    bool CreateStandard3DColorShader();
    bool CreateStandard3DLightShader();
    bool CreateStandard3DLightColorShader();
    bool CreateStandard3DGlasShader();

    bool _FAILED_GlyphShader;
    bool _FAILED_2DTextureShader;
    bool _FAILED_2DColorShader;

    bool _FAILED_3DTextureShader;
    bool _FAILED_3DColorShader;
    bool _FAILED_3DLightShader;
    bool _FAILED_3DLightColorShader;
    bool _FAILED_3DGlasShader;
};

#endif // SHADER_H
