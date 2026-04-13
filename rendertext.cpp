#include "rendertext.h"

// Element Indices
static const GLushort vertex_indices[] =
    {
        0, 1, 2, 3, 4, 5
};

//----------------------------
// Shader für Text Rendering
// ---------------------------
static const GLchar * vs2D_src = {
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

static const GLchar * fs2D_src = {
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
    "   if(texel.r == 0.0 && texel.g == 0.0 && texel.b == 0)        \n"
    "       discard;                                                \n"
    "   else {                                                      \n"
    "       vec4 sampled = vec4(1.0, 1.0, 1.0, texture(text, fs_in.uv).r); \n"
    "       fragcolor =    sampled * col2D;                         \n"
    "   }                                                           \n"
    "}                                                                "
};





RenderText::RenderText(int resx, int resy,Shader * sh){
    initConstructor(resx,resy,sh);
}

RenderText::RenderText(int resx, int resy, sPoint pos, Shader * sh){
    initConstructor(resx,resy,sh);
    _Pos = pos;
}


void RenderText::initConstructor(int resx, int resy, Shader *sh){
    _Font = GNU_DEFAULT_FONT;
    _Shader = sh;
    _ResX = resx;
    _ResY = resy;

    _BackgroundColor = glm::vec4(0.0f,0.0f,0.8f,0.3f);
    _TextColor = glm::vec4(1.0f,1.0f,1.0f,1.0f);
    _Scale = 1.0f;
    _Pixelsize = 16;
    _MarginLeft = 5.0f;
    _MarginRight= 5.0f;
    _MarginY = 5.0f;
}


bool RenderText::Init(int resx, int resy){
    //_ResX = resx;
    //_ResY = resy;
    // _RenderBottom = false;
    // _RenderHeader = false;
    // _HasBackground = false;
    // _HasTexture = false;
    // _AlignRight = false;
    // _BackgroundColor = glm::vec4(0.0f,0.0f,0.8f,0.3f);
    // _TextColor = glm::vec4(1.0f,1.0f,1.0f,1.0f);
    // _Scale = 1.0f;
    // _Pixelsize = 16;
    // _MarginLeft = 5.0f;
    // _MarginRight= 5.0f;
    // _MarginY = 5.0f;

    // ---------------------------------------
    //  Freetype2 init.
    // ---------------------------------------
    if (FT_Init_FreeType(&ft))
        log.logwarn("Konnte Freetype nicht initialisieren !!","TextRender::Init");
    else
        log.loginfo("Freetype2 initialisiert ","TextRender::Init");


    if (FT_New_Face(ft, _Font.c_str(), 0, &face)) {
        log.logwarn("Konnte Freetype2 Face nicht initialisieren","RenderText::Init");
        return false;
    }
    else
        log.loginfo("Freetype2 Face initialisiert ... ");


    FT_Set_Pixel_Sizes(face, 0, _Pixelsize);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // Disable byte-alignment restriction
    // Char 0 .. 127 einlesen
    glActiveTexture(GL_TEXTURE0);

    for (GLubyte c = 0; c < 128; c++)    {
        // Load character glyph
        if (FT_Load_Char(face, c, FT_LOAD_RENDER))
        {
            log.logwarn("ERROR::FREETYTPE: Failed to load Glyph","TextRender::Init");
            continue;
        }
        // Generate texture
        GLuint texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(
            GL_TEXTURE_2D,
            0,
            GL_RED,
            static_cast<GLsizei>(face->glyph->bitmap.width),
            static_cast<GLsizei>(face->glyph->bitmap.rows),
            0,
            GL_RED,
            GL_UNSIGNED_BYTE,
            face->glyph->bitmap.buffer
            );
        // Set texture options
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        // Now store character for later use
        Character character = {
            texture,
            glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
            glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
            face->glyph->advance.x,
            face->glyph->bitmap.rows
        };
        Characters.insert(std::pair<GLchar, Character>(c, character));
    } // for GLubyte

    glBindTexture(GL_TEXTURE_2D, 0);
    // Destroy FreeType once we're finished
    FT_Done_Face(face);
    FT_Done_FreeType(ft);
    //  -----------------------------------------------
    // VertexArraobject und VertexBufferObject für Text
    // ------------------------------------------------
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
    // TextureCoordinates
    glVertexAttribPointer(1,2,GL_FLOAT,GL_FALSE, 4 * sizeof(GLfloat),(void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // Nicht benutzt - wäre für Color !
    //glVertexAttribPointer(2,2,GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat),(void*)(5* sizeof(float)));
    //glEnableVertexAttribArray(2);
    // --------------   Index
    glGenBuffers(1,&_EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,_EBO);
    glBufferData (GL_ELEMENT_ARRAY_BUFFER,
                 sizeof (vertex_indices),
                 vertex_indices,
                 GL_DYNAMIC_DRAW);



    //-------------------------------------------------------------------


    // ---------------------------------------------------------------
    // Das ganze jetzt für Background
    // Es werden 5 Texturen benötigt --> Links oben, Rechts oben, Rahmen, Links unten, Rechts unten.
    // An die Textlänge wird nur der Mittelteil angepast.
    // ----------------------------------------------------------------
    // VertexArray für Headline
    // glGenVertexArrays(1,&_bgVAO);
    // glBindVertexArray(_bgVAO);
    // // ****************************************************************
    // // ----------------------------------------------------------------
    // // Das ganze jetzt noch mal für den Mittelteil und die Eckteile
    // // ----------------------------------------------------------------
    // // VertexBuffer
    // glGenBuffers(1,&_bgVBO);
    // glBindBuffer(GL_ARRAY_BUFFER,_bgVBO);
    // glBufferData(GL_ARRAY_BUFFER,sizeof(GLfloat)*6*4, NULL, GL_DYNAMIC_DRAW);  // wird in paint angepasst
    // // Mittel Teil
    // glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (void*)0);
    // glEnableVertexAttribArray(0);
    // //TexturCoordinates
    // glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat),(void*)(2 * sizeof(float)) );
    // glEnableVertexAttribArray(1);
    // //-------------------
    // //Elementbuffer
    // //-------------------
    // glGenBuffers(1,&_bgEBO);
    // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _bgEBO);
    // glBufferData(GL_ELEMENT_ARRAY_BUFFER,
    //              sizeof(vertex_indices),
    //              vertex_indices,
    //              GL_DYNAMIC_DRAW);
    // //----------
    // // Aufräumen
    // //----------
    // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
    // glBindBuffer(GL_ARRAY_BUFFER,0);
    // glBindVertexArray(0);





    return true;
}
