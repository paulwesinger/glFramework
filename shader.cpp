#include "shader.h"
#include <iostream>
#include <string>
#include <logger.h>
#include <errorlogs.h>

//--------------------------------------------------------------------
// Standard Shaders
// -------------------------------------------------------------------
static const GLchar * Standard2D_VertexSrc = {
    "#version 440 core                                              \n"
    "layout (location = 0) in vec2 vertex;                          \n"
    "layout (location = 1) in vec2 tex;                             \n"

    "uniform mat4 projection;                                       \n"
    "out VS_OUT {                                                   \n"
    "   vec2 uv;                                                    \n"
    "} vs_out;                                                      \n"

    "void main()                                                    \n"
    "{                                                              \n"
    "   gl_Position = projection * vec4(vertex,0.0,1.0);            \n"
    "   vs_out.uv =  tex;                                           \n"
    "}                                                               \n"
};

static const GLchar * Standard2DTexture_FragmentSrc = {
    "#version 440 core                                              \n"

    "in VS_OUT {                                                    \n"
    "   vec2 uv;                                                    \n"
    "} fs_in;                                                       \n"

    "out vec4 fragcolor;                                            \n"
    "uniform sampler2D text;                                        \n"
    "uniform vec4 col2D;                                            \n"

    "void main()                                                    \n"
    "{                                                              \n"
    "   vec4 texel = texture(text,fs_in.uv);                        \n"
    "   if(texel.a == 0.0)                                          \n"
    "       discard;                                                \n"
    "   fragcolor =   col2D * texel ;                               \n"
    "}                                                                "
};

static const GLchar * Standard2DGlyph_FragmentSrc = {
    "#version 440 core                                              \n"

    "in VS_OUT {                                                    \n"
    "   vec2 uv;                                                    \n"
    "} fs_in;                                                       \n"

    "out vec4 fragcolor;                                            \n"
    "uniform sampler2D text;                                        \n"
    "uniform vec4 col2D;                                            \n"

    "void main()                                                    \n"
    "{                                                              \n"
    "   vec4 texel = texture(text,fs_in.uv);                        \n"
    "   //if (texel.a == 0)                                           \n"
    "    if(texel.r == 0.0 && texel.g == 0.0 && texel.b == 0)        \n"
    "       discard;                                                \n"
    "   else {                                                      \n"
    "       vec4 sampled = vec4(1.0, 1.0, 1.0, texture(text, fs_in.uv).r); \n"
    "       fragcolor =     col2D;                         \n"
    "   }                                                           \n"
    "}                                                                "
};

static const GLchar * Standard2DColor_FragmentSrc = {
    "#version 440 core                                              \n"

    "out vec4 fragcolor;                                            \n"
    "uniform vec4 col2D;                                            \n"

    "void main()                                                    \n"
    "{                                                              \n"
    "   fragcolor =   col2D;                                        \n"
    "}                                                                "
};

//-------------------------------------------------
// Fragment-Shader für Color Rendering mit Normale
// ------------------------------------------------
const GLchar * Standard3DColor_FragmentLightSrc = {
    "#version 450 core                                      \n"

    "in VS_OUT {                                            \n"
    "        vec3 N;                                        \n"
    "        vec3 L;                                        \n"
    "        vec3 V;                                        \n"
    "        vec3 fragpos;                                  \n"
    "        vec3 color;                                    \n"
    "        vec2 tex;                                      \n"
    "} fs_in;                                               \n"
    "uniform vec3 lightPos;                                 \n"
    "uniform vec3 viewPos;                                  \n"
    "uniform bool blinn;                                    \n"
    "uniform vec4 triangleColor;                            \n"
    "                                                       \n"
    "out vec4 FragColor;                                    \n"
    "                                                       \n"
    "void main()                                            \n"
    "{                                                      \n"
    "    vec3 color;                                        \n"
    "    color = triangleColor.rgb * fs_in.color;           \n"
    "    vec3 ambient = 0.5 * color;   // ambient           \n"
    "    vec3 lightDir = normalize(lightPos - fs_in.fragpos);\n"
    "    vec3 normal = normalize(fs_in.N);                  \n"
    "    float diff = max(dot(lightDir,normal), 0.0);       \n"
    "    vec3 diffuse = diff * color;        // diffus      \n"
    "    vec3 viewDir = normalize(viewPos - fs_in.fragpos); \n"
    "    vec3 reflectDir = reflect(-lightDir, normal);      \n"
    "    float spec = 0.0;                                  \n"
    "    if(blinn)                                          \n"
    "    {                                                  \n"
    "       vec3 halfwayDir = normalize(lightDir + viewDir);                        \n"
    "       spec = pow(max(dot(normal, halfwayDir), 0.0), 1.0);                     \n"
    "    }                                                   \n"
    "    else                                                \n"
    "    {                                                   \n"
    "       vec3 reflectDir = reflect(-lightDir, normal);    \n"
    "       spec = pow(max(dot(viewDir, reflectDir), 0.0), 1.0);                    \n"
    "    }                                                                          \n"
    "    vec3 specular = vec3(1.0) * spec; // assuming bright white light color     \n"
    "    FragColor = vec4(ambient + diffuse + specular,1.0 ) * vec4(color,1.0);     \n"
    "}                                                                              \n"
};

//-------------------------------------------------
// Shaders für rendering mit normalen und Licht
// ------------------------------------------------
//-------------------------------------------------
// Vertex-Shader für Texture Rendering mit normalen
// ------------------------------------------------
const GLchar * Standard3D_VertexNormalsSrc = {
    "    #version 450 core                                  \n"
    "    layout (location = 0) in vec3 position;            \n"
    "    layout (location = 1) in vec3 normal;              \n"
    "    layout (location = 2) in vec3 color;               \n"
    "    layout (location = 3) in vec2 tex;                 \n"

    "    out vec3 fragpos;                                  \n"
    "    out vec3 normal_out;                               \n"
    "    out vec2 TexCoord;                                 \n"

    "    uniform mat4 mv_matrix;                            \n"

    "    uniform mat4 model;                                \n"
    "    uniform mat4 projection;                           \n"
    "    uniform mat4 view;                                 \n"

    "    uniform vec3 Light;                                \n"

    "    out VS_OUT {                                       \n"
    "        vec3 N;                                        \n"
    "        vec3 L;                                        \n"
    "        vec3 V;                                        \n"
    "        vec3 fragpos;                                  \n"
    "        vec3 color;                                    \n"
    "        vec2 tex;                                      \n"
    "    } vs_out;                                          \n"


    "    void main(void)                                    \n"
    "    {                                                  \n"
    "    //------------------------------------------       \n"
    "        vec4 p = mv_matrix * vec4(position,1.0);       \n"

    "        vs_out.N = mat3(mv_matrix) * normal;           \n"
    "        vs_out.L = Light - p.xyz;                      \n"
    "        vs_out.V = -p.xyz;                             \n"
    "        vs_out.color = color;                          \n"
    "        vs_out.tex = tex;                              \n"
    "        vs_out.fragpos = vec3(model * vec4(position,1.0));    \n"

    "        gl_Position = projection * view * vec4(vs_out.fragpos,1.0);\n"
    "        //TexCoord = tex;                                \n"
    "    }                                                  \n"
};

const GLchar * Standard3D_FragmentNormalsSrc = {
    "#version 450 core                                       \n"
    "layout(binding=0) uniform sampler2D texture1;          \n"
    "layout(binding=1) uniform sampler2D texture2;          \n"
    "out vec4 FragColor;                                    \n"
    "in VS_OUT {                                            \n"
    "        vec3 N;                                        \n"
    "        vec3 L;                                        \n"
    "        vec3 V;                                        \n"
    "        vec3 fragpos;                                  \n"
    "        vec3 color;                                    \n"
    "        vec2 tex;                                      \n"
    "} fs_in;                                               \n"
    "uniform vec3 lightPos;                                 \n"
    "uniform vec3 viewPos;                                  \n"
    "uniform bool blinn;                                    \n"
    "uniform vec4 triangleColor;                            \n"
    "uniform bool useTexture_2;                             \n"
    "uniform bool hasTexture;                               \n"
    "void main()                                            \n"
    "{                                                      \n"
    "    vec3 color;                                        \n"
    "    if (hasTexture) {                                  \n"
    "        if ( ! useTexture_2)                           \n"
    "            color = texture(texture1,fs_in.tex).rgb;   \n"
    "         else                                          \n"
    "            color = mix(texture(texture1, fs_in.tex), texture(texture2, fs_in.tex), 0.5).rgb; \n"
    "    }                                                  \n"
    "    else                                               \n"
    "    {                                                  \n"
    "        color = triangleColor.rgb * fs_in.color;       \n"
    "    }                                                  \n"
    "    vec3 ambient = 0.5 * color;   // ambient           \n"
    "    vec3 lightDir = normalize(lightPos - fs_in.fragpos);\n"
    "    vec3 normal = normalize(fs_in.N);                  \n"
    "    float diff = max(dot(lightDir,normal), 0.0);       \n"
    "    vec3 diffuse = diff * color;        // diffus      \n"
    "    vec3 viewDir = normalize(viewPos - fs_in.fragpos); \n"
    "    vec3 reflectDir = reflect(-lightDir, normal);      \n"
    "    float spec = 0.0;                                  \n"
    "    if(blinn)                                          \n"
    "    {                                                  \n"
    "       vec3 halfwayDir = normalize(lightDir + viewDir);        \n"
    "       spec = pow(max(dot(normal, halfwayDir), 0.0), 1.0);     \n"
    "    }                                                   \n"
    "    else                                                \n"
    "    {                                                   \n"
    "       vec3 reflectDir = reflect(-lightDir, normal);    \n"
    "       spec = pow(max(dot(viewDir, reflectDir), 0.0), 1.0);    \n"
    "    }                                                   \n"
    "    vec3 specular = vec3(1.0) * spec; // assuming bright white light color\n"
    "    FragColor = vec4(ambient + diffuse + specular,1.0 ) * vec4(color,1.0);   \n"
    "}                                                       \n"

};



//-------------------------------------------------
// Vertex-Shader für Texture Rendering ohne Normale
// ------------------------------------------------
const GLchar * Standard3D_VertexSrc = {
    "#version 450 core                                          \n"

    "layout (location = 0) in vec3 position;                    \n"
    "layout (location = 1) in vec3 normal;                      \n"
    "layout (location = 2) in vec3 veccolor;                    \n"
    "layout (location = 3) in vec2 tex;                         \n"

    "out VS_OUT{                                                \n"
    "   vec4 color;                                             \n"
    "   vec2 TexCoord;                                          \n"
    "}  vs_out;                                                 \n"

    "uniform mat4 mv_matrix;                                    \n"

    "uniform mat4 model;                                        \n"
    "uniform mat4 projection;                                   \n"
    "uniform mat4 view;                                         \n"

    "void main(void)                                            \n"
    "{                                                          \n"
    "   vec3 fragpos = vec3(model * vec4(position,1.0));        \n"
    "   gl_Position = projection * view * vec4(fragpos,1.0);    \n"

    "   vs_out.color = vec4(veccolor,1.0);                      \n"
    "   vs_out.TexCoord = tex;                                  \n"
    "}                                                          \n"
};



const GLchar * Standard3DTextured_FragmentSrc = {

"#version 450 core                                          \n"

    "layout(binding=0) uniform sampler2D texture1;              \n"
    "layout(binding=1) uniform sampler2D texture2;              \n"

    "in VS_OUT{                                                 \n"
    "   vec4 color;                                             \n"
    "   vec2 TexCoord;                                          \n"
    "} fs_in;                                                   \n"

    "uniform bool useTexture_2;                                 \n"
    "uniform vec4 triangleColor;                                \n"
    "uniform bool hasTexture;                                   \n"

    "out vec4 FragColor;                                        \n"

    "void main(void) {                                          \n"

    "    vec4 outcolor;                                         \n"

    "    if ( hasTexture) {                                    \n"

    "       if (useTexture_2)                                 \n"
    "            outcolor = mix(texture(texture1, fs_in.TexCoord), texture(texture2, fs_in.TexCoord), 0.5);\n"
    "        else                                               \n"
    "            outcolor = texture(texture1,fs_in.TexCoord);   \n"
    "    }                                                      \n"
    "    else {                                                 \n"
    "        outcolor =  triangleColor * fs_in.color;                         \n"
    "    }                                                      \n"
    "    FragColor =  outcolor;                                 \n"
    "}                                                          \n"

};

const GLchar * Standard3DColored_FragmentSrc = {
    "#version 450 core                                          \n"

    "layout (location = 1) in vec3 veccolor;                    \n"
    "layout (location = 2) in vec2 tex;                         \n"
    "out vec4 color;                                            \n"
    "uniform vec3 triangleColor;                                \n"

    "in VS_OUT                                                  \n"
    "{                                                          \n"
    "   vec4 color;                                             \n"
    "   vec2 TexCoord;                                          \n"
    "} fs_in;                                                   \n"

    "void main(void)                                            \n"
    "{                                                          \n"
    "   if (triangleColor.r == 0 && triangleColor.g == 0 && triangleColor.b == 0)   \n"
    "       color = fs_in.color;                                \n"
    "   else                                                    \n"
    "       color = vec4(triangleColor,1);                      \n"
    "}                                                          \n"
};


const GLchar * Standard3DGLasshader_Vertexsrc = {
    "    #version 450 core                                          \n"
    "    layout (location = 0) in vec3 position;                    \n"
    "    layout (location = 1) in vec3 veccolor;                    \n"
    "    layout (location = 2) in vec2 tex;                         \n"

    "    out VS_OUT{                                                \n"
    "        vec4 color;                                            \n"
    "        vec2 TexCoord;                                         \n"
    "    } vs_out;                                                  \n"

    "    uniform mat4 mv_matrix;                                    \n"

    "    uniform mat4 model;                                        \n"
    "    uniform mat4 projection;                                   \n"
    "    uniform mat4 view;                                         \n"

    "    void main(void)                                            \n"
    "    {                                                          \n"
    "        vec3 fragpos = vec3(model * vec4(position,1.0));       \n"
    "        gl_Position = projection * view * vec4(fragpos,1.0);   \n"

    "        vs_out.color = vec4(veccolor,1.0);                     \n"
    "        vs_out.TexCoord = tex;                                 \n"
    "    }                                                          \n"
};

const GLchar * Standard3DGLasshader_Fragmentsrc = {
    "    #version 450 core                                                                  \n"

    "    layout(binding=0) uniform sampler2D texture1;                                      \n"
    "    layout(binding=1) uniform sampler2D texture2;                                      \n"

    "    in VS_OUT{                                                                         \n"
    "       vec4 color;                                                                     \n"
    "       vec2 TexCoord;                                                                  \n"
    "    } fs_in;                                                                           \n"

    "    uniform vec4 triangleColor;                                                        \n"
    "    uniform bool useTexture_2;                                                         \n"
    "    uniform bool hasTexture;                                                           \n"

    "    out vec4 FragColor;                                                                \n"

    "    void main(void) {                                                                  \n"

    "        vec4 outcolor;                                                                 \n"
    "        if ( hasTexture ) {                                                            \n"
    "            if (useTexture_2)                                                          \n"
    "               outcolor =  mix(texture(texture1, fs_in.TexCoord), texture(texture2, fs_in.TexCoord), 0.5); \n"
    "            else                                                                       \n"
    "               outcolor = texture(texture1,fs_in.TexCoord);                            \n"
    "        }                                                                              \n"
    "        else {                                                                         \n"
    "             outcolor = triangleColor;                                                 \n"
    "        }                                                                              \n"
    "        if (outcolor.r == 0.0 && outcolor.g == 0.0 && outcolor.b == 0.0)               \n"
    "            discard;                                                                   \n"
    "        else                                                                           \n"
    "        {                                                                              \n"
    "            FragColor =  outcolor * triangleColor;                                     \n"
    "        }                                                                              \n"
    "    }                                                                                  \n"
};




Shader::Shader() {
    _FAILED_GlyphShader         = false;
    _FAILED_3DTextureShader     = false;
    _FAILED_3DColorShader       = false;
    _FAILED_3DLightShader       = false;
    _FAILED_3DLightColorShader  = false;
    _FAILED_3DGlasShader        = false;
    _FAILED_2DColorShader       = false;
    _FAILED_2DTextureShader     = false;


    CreateStandardShaders();
}

void Shader::CreatStandardShaderFromFile(std::string /*path*/) {
    // for later use ...

}

void Shader::CreateStandardShaders() {

    //------------------------------------------------------
    //Create Shader for Text printing and 2D
    //------------------------------------------------------
    ErrorLogs::Failed(CreateStandardGlyphShader(),"Creating GlyphShader failed",_FAILED_GlyphShader);
    ErrorLogs::Failed(CreateStandard2DTextureShader(),"Creating 2D TextureShader failed !", _FAILED_2DTextureShader);
    ErrorLogs::Failed(CreateStandard2DColorShader(),"Creating 2D ColorShader failed !", _FAILED_2DColorShader);

    //------------------------------------------------------
    //Create Shader for 3D rendering with texture, no lights
    //------------------------------------------------------
    ErrorLogs::Failed(CreateStandard3DTextureShader(),"Creating Standard3DTextureShader failed !", _FAILED_3DTextureShader);

    //------------------------------------------------------
    //Create Shader for 3D rendering with color, no lights
    //------------------------------------------------------
    ErrorLogs::Failed(CreateStandard3DColorShader(),"Creating Standard3DColorShader failed !", _FAILED_3DColorShader);
    ErrorLogs::Failed(CreateStandard3DLightShader(),"Creating Standard3DLightShader failed !", _FAILED_3DLightShader);
    ErrorLogs::Failed(CreateStandard3DLightColorShader(),"Creating Standard3DLightColorShader failed !", _FAILED_3DLightColorShader);
    ErrorLogs::Failed(CreateStandard3DGlasShader(),"Creating Standard3DGlasShader failed !", _FAILED_3DGlasShader);
}



bool Shader::CreateShaderProgram(GLuint & prg, const GLchar * vertexsource, const GLchar * fragmentsource) {
    //------------------------------------------------------
    //Shader for text printing
    //------------------------------------------------------
    int vs = compileShader(vertexsource,GL_VERTEX_SHADER);
    int fs = compileShader(fragmentsource,GL_FRAGMENT_SHADER);
    prg = CreateProgram(vs,fs);

    return  (vs == 0 ||  fs == 0  || prg == 0 ) ? false : true;
}

bool Shader::CreateStandardGlyphShader() {

    //------------------------------------------------------
    //Shader for text printing
    //------------------------------------------------------
    return CreateShaderProgram(_GlyphShader2D,Standard2D_VertexSrc, Standard2DGlyph_FragmentSrc);
}

bool Shader::CreateStandard2DTextureShader() {

    //------------------------------------------------------
    //Shader for text printiing and background with alpha
    //------------------------------------------------------
    return CreateShaderProgram(_TextureShader2D,Standard2D_VertexSrc, Standard2DTexture_FragmentSrc);
}

bool Shader::CreateStandard2DColorShader() {

    //------------------------------------------------------
    //Shader for 2d Color
    //------------------------------------------------------
    return CreateShaderProgram(_ColorShader2D,Standard2D_VertexSrc, Standard2DColor_FragmentSrc);
}

bool Shader::CreateStandard3DTextureShader() {
    //------------------------------------------------------
    //Shader for text printing
    //------------------------------------------------------
    return CreateShaderProgram(_TextureShader3D,Standard3D_VertexSrc, Standard3DTextured_FragmentSrc);
}

bool Shader::CreateStandard3DColorShader() {
    //------------------------------------------------------
    //Shader for 3DColorShader
    //------------------------------------------------------
    return CreateShaderProgram(_ColorShader3D,Standard3D_VertexSrc, Standard3DColored_FragmentSrc);
}

bool Shader::CreateStandard3DLightShader() {
    //------------------------------------------------------
    //Shader for 3D LightShader
    //------------------------------------------------------
    return CreateShaderProgram(_LightShader3D,Standard3D_VertexNormalsSrc, Standard3D_FragmentNormalsSrc);
}

bool Shader::CreateStandard3DLightColorShader() {
    //------------------------------------------------------
    //Shader for 3D LightShader
    //------------------------------------------------------
    return CreateShaderProgram(_LightShaderColor3D,Standard3D_VertexNormalsSrc, Standard3DColor_FragmentLightSrc);
}

bool Shader::CreateStandard3DGlasShader() {
    //------------------------------------------------------
    //Shader for 3D LightShader
    //------------------------------------------------------
    return CreateShaderProgram(_GlasShader3D,Standard3DGLasshader_Vertexsrc , Standard3DGLasshader_Fragmentsrc);
}

GLuint Shader::getTexture3DShader() {
    return ( _FAILED_3DTextureShader) ?  0 : _TextureShader3D;
}

GLuint Shader::getColor3DShader() {
    return ( _FAILED_3DColorShader) ?  0 : _ColorShader3D;
}

GLuint Shader::getLightShader() {
    return (_FAILED_3DLightShader) ? 0 : _LightShader3D;
}

GLuint Shader::getLightColorShader() {
    return (_FAILED_3DLightColorShader) ? 0  : _LightShaderColor3D;
}

GLuint Shader::getGlyphShader() {
    return ( _FAILED_GlyphShader) ?  0 : _GlyphShader2D;
}

GLuint Shader::getColor2DShader() {
    return (_FAILED_2DColorShader) ? 0 : _ColorShader2D;
}

GLuint Shader::getTexture2DShader() {
    return (_FAILED_2DTextureShader) ? 0 : _TextureShader2D;
}

GLuint Shader::getGlasShader () {
    return (_FAILED_3DGlasShader) ? 0 : _GlasShader3D;
}

Shader::~Shader(){

}

int Shader::compileShader(const char* source, GLint type){

    int ret = glCreateShader(type); // VERTEX,FRAGMENT or GEOMETRY

    glShaderSource(ret,1,&source,NULL);
    glCompileShader(ret);
    int ok;
    char log[512];
    std::string  s = "";

    glGetShaderiv(ret,GL_COMPILE_STATUS,&ok);
    if (  ! ok )
    {

        if (type == GL_VERTEX_SHADER)
            s= " VertexShader ";
        else
            if (type == GL_FRAGMENT_SHADER)
                s = "FragmentShader ";
            else
                s = "<undefined !>";

        glGetShaderInfoLog(ret,512,NULL,log);
        Logger::logwarn( s + "FAILED : " +(std::string)log);
        return 0;
    }

    Logger::loginfo( s + ": compiled ","Shader::compileShader");
    return ret;
}


int Shader::compileVertexShader(const char* source)
{
    return compileShader(source,GL_VERTEX_SHADER);
}
int Shader::compileFragmentShader(const char * source)
{
    return compileShader(source,GL_FRAGMENT_SHADER);
}



int Shader::CreateProgram(int &vs,int &fs)
{
    GLuint prog = glCreateProgram();
    glAttachShader(prog,vs);
    glAttachShader(prog,fs);
    glLinkProgram(prog);
    glDeleteShader(vs);
    glDeleteShader(fs);
    return prog;
}

// NEue Funktionen für CustomShader from File
int Shader::compileVertexShaderFromFile(std::string path, FileUtil* reader) {
    std::string shadertext;
    if ( ! reader->readShaderSource(shadertext,path)  ) {
        Logger::logwarn("Shader Source nicht gefunden: " + path,"Shader::compileVertexShaderFromFile");
        return -1;
    }
    Logger::loginfo("Source " + path + " geladen !","Shader::compileVertexShaderFromFile");

    int ret = glCreateShader(GL_VERTEX_SHADER);
    const GLchar* ch = (const GLchar* )shadertext.c_str();

    glShaderSource(ret,1,&ch ,NULL );
    glCompileShader(ret);

    int ok;
    char log[512];
    glGetShaderiv(ret,GL_COMPILE_STATUS,&ok);
    if ( ok ) {
        Logger::loginfo("VertexShader " + path + " erfolgreich compiliert !","Shader::compileVertexShaderFromFile");
        return ret;
    }
    else {
        glGetShaderInfoLog(ret,512,NULL,log);
        Logger::logwarn("Warning : "+ path + "failed:" + (std::string) log,"Shader::compileVertexShaderFromCource");
        return -1;
    }
}

int Shader::compileFragmentShaderFromFile(std::string path, FileUtil* reader) {
    std::string shadertext;
    if ( ! reader->readShaderSource(shadertext,path)  ) {
        Logger::logwarn("Shader Source nicht gefunden: " + path,"Shader::compileFragmentShaderFromFile");
        return -1;
    }
    Logger::loginfo("Source " + path + "  geladen !","Shader::compileFragmentShaderFromFile");
    int ret = glCreateShader(GL_FRAGMENT_SHADER);
    const GLchar* ch = (const GLchar* )shadertext.c_str();

    glShaderSource(ret,1, &ch, NULL );
    glCompileShader(ret);

    int ok;
    char log[512];
    glGetShaderiv(ret,GL_COMPILE_STATUS,&ok);
    if ( ok ) {
        Logger::loginfo("FragmentShader: " + path + " erfolgreich compiliert !","Shader::compileFragmentShaderFromFile");
        return ret;
    }
    else {
        glGetShaderInfoLog(ret,512,NULL,log);
        Logger::logwarn("Warning :" + path + " Log: " +(std::string) log,"Shader::compileFragmentShaderFromCource");
        return -1;
    }
}

void Shader::CreateCustomShader(GLuint &progID ){
    progID = glCreateProgram();
    Logger::loginfo("Erstelle ShaderID ........ done","Shader::CreateCustomShader");
}

void Shader::AttachCustomShader( GLuint &prog, int &shader) {
    glAttachShader(prog,shader);
}

void Shader::CreateCustomProgram(GLuint &prog) {
    glLinkProgram(prog);
}

void Shader::deleteShader(int& shader) {
    glDeleteShader(shader);
}


