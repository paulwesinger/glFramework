#include <glew.h>
#include "rendertext.h"
#include <gtc/type_ptr.hpp>
#include <gtc/matrix_transform.hpp>
#include "gl_global.h"
#include <iostream>


// Element Indices
static const GLushort vertex_indices[] =
    {
        0, 1, 2, 3, 4, 5
};

//--------------------------------------------
// Construct / Destruct
// -------------------------------------------

ENGINE::RenderText::RenderText(int resx, int resy,Shader * sh){
    _Shader = sh;

    sPoint p(10,10);
    _Pos = p;
    posX = p.x;
    posY = p.y;

    _DisplayID = 0;
    _DisplayName = "Noname";

    initConstructor(resx,resy,p,sh);
    Init();
}

ENGINE::RenderText::RenderText(int resx, int resy, sPoint pos, Shader * sh,int id,string name){

    _DisplayID = id;
    _DisplayName = name;

    _Shader = sh;
    _Pos = pos;
    posX = pos.x;
    posY = pos.y;

    _DisplayID;
    _DisplayName;
    initConstructor(resx,resy,pos,sh);
    Init();

}

ENGINE::RenderText::~RenderText(){

}


void ENGINE::RenderText::SetHasBackground(bool hasbg){
    _HasBackGround = hasbg;
}

void ENGINE::RenderText::SetText(string text, int index){
    try{

        if (index < 0){ // add to list
           _StringList.push_back(text);
        }
        else {
            if(index < _StringList.size())
                _StringList.at(index) = text;
        }

    }
    catch(const std::exception& e)
    {
        log.logwarn(e.what());
    }
}

string ENGINE::RenderText::GetText(int index){

    try{
        return _StringList[0];
    }
    catch(const std::exception e){
        return e.what();
    }
}

void ENGINE::RenderText::SetGlyphShader(GLuint s) {
    _GlyphShader = s;
}

bool ENGINE::RenderText::Init(){


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
    //glActiveTexture(GL_TEXTURE0);

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
        sCharacter character = {
            texture,
            glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
            glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
            face->glyph->advance.x,
            face->glyph->bitmap.rows
        };
        Characters.insert(std::pair<GLchar, sCharacter>(c, character));
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


    // ---------------------------------------------------------------
    // Das ganze jetzt für Background ohne textur nur color
    // An die Textlänge wird die breite angepasst
    // ----------------------------------------------------------------

    glGenVertexArrays(1,&_bgVAO);
    glBindVertexArray(_bgVAO);
    // ****************************************************************
    // VertexBuffer
    glGenBuffers(1,&_bgVBO);
    glBindBuffer(GL_ARRAY_BUFFER,_bgVBO);
    glBufferData(GL_ARRAY_BUFFER,sizeof(GLfloat)*6*4, NULL, GL_DYNAMIC_DRAW);  // wird in paint angepasst

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (void*)0);
    glEnableVertexAttribArray(0);
    //TexturCoordinates
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat),(void*)(2 * sizeof(float)) );
    glEnableVertexAttribArray(1);
    //------------------------
    //Elementbuffer
    //---------------------
    glGenBuffers(1,&_bgEBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _bgEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                 sizeof(vertex_indices),
                 vertex_indices,
                 GL_DYNAMIC_DRAW);

    //----------
    // Aufräumen
    //----------
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
    glBindBuffer(GL_ARRAY_BUFFER,0);
    glBindVertexArray(0);

    return true;

}

void ENGINE::RenderText::RenderPaintarea(GLfloat x, GLfloat y, GLfloat height) {

    // Standard Masse der images !!
    GLfloat w = _Textfeld.w;
    //GLfloat h = 16.0f;

    GLfloat vertices[6][4] = {

    { x,     y  + height,          0.0, 0.0 },
    { x,     y,                    0.0, 1.0 },
    { x + w, y,                    1.0, 1.0 },

    { x, y  +  height,             0.0, 0.0 }, // w muss weg für 6  uv = 0,0 !!
    { x + w, y,                    1.0, 1.0 },
    { x + w, y + height,           1.0, 0.0 }
};

// if (_HasTexture) {
//     glUniform4f(framecolor_loc,1.0,1.0,1.0, 0.5); // Textfeld hat 50% Alpha -> sieht cool aus
//     glBindTexture(GL_TEXTURE_2D,texPaintarea);
// }
// else
    glUniform4f(    framecolor_loc,_BackgroundColor.r,_BackgroundColor.g,_BackgroundColor.b,_BackgroundColor.a);
    glBufferSubData(GL_ARRAY_BUFFER,0,sizeof(vertices),vertices);

    glDrawElements( GL_TRIANGLE_STRIP, 6, GL_UNSIGNED_SHORT, 0);
}


void ENGINE::RenderText::CalcSize(int &weite, int &height) {
    // Breite ermitteln:
    std::string::const_iterator c;
    GLfloat feldweite = 0.0f;
    GLfloat feldhoehe = 20.0f; // Standard höhe 16 Pixel
    GLfloat width = 0.0f;
    int count = 0;
    for ( uint i = 0; i < _StringList.size(); i ++ ) {
        for (c = _StringList[i].begin(); c != _StringList[i].end(); c++) {
            sCharacter ch = Characters[*c];
            feldweite += static_cast<GLfloat> ( (ch.Advance >> 6) * _Scale);
        }
        count ++;

        if ( width < feldweite )
            width = feldweite;
        feldweite = 0.0f;
    }

    weite = width;
    height = feldhoehe * count * _Scale;
}

void ENGINE::RenderText::Draw(){

    GLfloat _x = posX;
    GLfloat _y = posY;

    glEnable(GL_BLEND);
    glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);

    // Breite ermitteln:
    std::string::const_iterator c;
    int width;
    int height;

    CalcSize(width,height);
  //  std::cout <<"width: "<< width << " height: " << height << endl;
    _Textfeld.x = posX;
    _Textfeld.y = posY;
    _Textfeld.w = width * _Scale + _MarginLeft + _MarginRight;
    _Textfeld.h = height;
    GLfloat newX;
    if ( _AlignRight )
        newX = _ResX - _Textfeld.w;
    else
        newX = _x;

    // std::cout <<"x: "<< _Textfeld.x << " w: " << _Textfeld.w << "y: " << _Textfeld.y << "h: " << _Textfeld.h << endl;


    // --------------------------------
    // Erstmal alles fürs TextFenster
    //---------------------------------
    // if (_HasTexture)
    //     _CurrentShader = _Shader ->getTexture2DShader();
    // else
    _CurrentShader = _Shader->getColor2DShader();

    glUseProgram(_CurrentShader);

    projection_loc = glGetUniformLocation(_CurrentShader,"projection");//"projection_textfeld");
    framecolor_loc = glGetUniformLocation(_CurrentShader,"col2D");    //"color");
    // IDentity
    glm::mat4 Model(1.0f);

    MatOrtho = glm::orthoRH(0.0f,static_cast<GLfloat>(_ResX), static_cast<GLfloat>(_ResY), 0.0f,  -100.0f, 100.0f);

    glm::mat4 mp = MatOrtho;//* Model ;
    glUniformMatrix4fv(projection_loc, 1, GL_FALSE, glm::value_ptr(mp)); // projection matrix im shader init.
    //  glUniform4f(framecolor_loc,_BackgroundColor.r,_BackgroundColor.g,_BackgroundColor.b,_BackgroundColor.a);


    if (_HasBackGround) {
    //     // ALle Buffers binden ....
        glBindVertexArray(_bgVAO);
        glBindBuffer(GL_ARRAY_BUFFER,_bgVBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,_bgEBO);

        RenderPaintarea(newX, _y - (16 - _MarginY*2), height);

    //     // Alles Rendern
    //     if (_RenderHeader)
    //         RenderFrame(newX, _y - (16.0f - _MarginY*2), texHeadline );

    //     if (_RenderBottom)
    //         RenderFrame(newX, _y + height, texBottom );
    //     // ... und aushängen
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }


    _x = newX + _MarginLeft;
    GLfloat startX  = _x;
    GLfloat row     = _y + 16.0f;//_MarginY;
    //--------------------
    // Text Rendern
    //--------------------
    glActiveTexture(GL_TEXTURE0);
    GLuint s = _Shader->getGlyphShader();

    SetGlyphShader(s);
    glUseProgram( s); //   shader->getGlyphShader());


    glBindVertexArray(_VAO);
    mv_projectloc = glGetUniformLocation(_GlyphShader,"projection");
    uniform_colorloc   = glGetUniformLocation(_GlyphShader,"col2D");

    //glm::mat4 Model(1.0f);
    glm::mat4 mvp = MatOrtho * Model ;

    glUniformMatrix4fv(mv_projectloc, 1, GL_FALSE, glm::value_ptr(mvp)); //projection));
    glUniform4f(uniform_colorloc,_TextColor.r, _TextColor.g, _TextColor.b, _TextColor.a);

    // Iterate through all characters
    // std::string::const_iterator c;
    for (uint i = 0; i < _StringList.size(); i++ ) {
        for (c = _StringList[i].begin(); c != _StringList[i].end(); c++)
        {
            sCharacter ch = Characters[*c];

            GLfloat xpos = _x + ch.Bearing.x * _Scale;
            //GLfloat ypos = row - ((ch.Size.y - ch.Bearing.y) * _Scale) ;

            GLfloat ypos;
            ypos = row + ((ch.Size.y - ch.Bearing.y) * _Scale) ;

            GLfloat w = ch.Size.x * _Scale;
            GLfloat h = ch.Size.y * _Scale;
            // Update VBO for each character
            GLfloat vertices[6][4] = {
                { xpos,     ypos - h - 6.0f,   0.0, 0.0 },  // alles 16 war 6 !!
                { xpos,     ypos - 6.0f,       0.0, 1.0 },
                { xpos + w, ypos - 6.0f,       1.0, 1.0 },

                { xpos,     ypos - h - 6.0f,   0.0, 0.0 },
                { xpos + w, ypos - 6.0f,       1.0, 1.0 },
                { xpos + w, ypos - h - 6.0f,   1.0, 0.0 }
            };
            // Render glyph texture over quad
            glBindTexture(GL_TEXTURE_2D,ch.TextureID);
            // Update content of VBO memory
            glBindBuffer(GL_ARRAY_BUFFER, _VBO);
            glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); // Be sure to use glBufferSubData and not glBufferData
            // Render quad
            //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,_EBO);
            //glDrawElements( GL_TRIANGLE_STRIP, 6, GL_UNSIGNED_SHORT, 0);
            glDrawArrays(GL_TRIANGLES, 0, 6);
            // Now advance cursors for next glyph (note that advance is number of 1/64 pixels)
            _x += (ch.Advance >> 6) * _Scale; // Bitshift by 6 to get value in pixels (2^6 = 64 (divide amount of 1/64th pixels by 64 to get amount of pixels))
        }
        _x =     startX;
        row +=  18.0f *_Scale; //  + bearingdiff ;  // 16
    }
    // Aufräumen
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
    glBindBuffer(GL_ARRAY_BUFFER,0);
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D,0);
}

bool ENGINE::RenderText::GenTextfeldSegment(std::string image, unsigned int &tex){
    // std::string imagepath = image;
    // //glBindTexture(GL_TEXTURE_2D,tex);
    // SDL_Surface * surface;
    // char * data;
    // surface = CLoadImage::getSurface(imagepath,"TextRender::GenTextFeldSegment");

    // if (surface) {
    //     data = static_cast<char*>( surface ->pixels);
    //     _Textfeld.w = surface -> w;
    //     _Textfeld.h = surface -> h;
    // }
    // else
    //     return false;

    // if (data) {
    //     glGenTextures(1,&tex);
    //     glBindTexture(GL_TEXTURE_2D,tex);
    //     //Flags
    //     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    //     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    //     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    //     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    //     glTexImage2D(GL_TEXTURE_2D, 0,  GL_RGB, surface->w, surface->h, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    //     glBindTexture(GL_TEXTURE_2D, 0);
    //     return true;
    // }
    // return false;
    return true;
}

// -----------------------------------------
// privates:
// -----------------------------------------

void ENGINE::RenderText::initConstructor(int resx, int resy, sPoint pos, Shader *sh){

    //SDL_GL_MakeCurrent(GLWindow,glContext);

    _Font = GNU_DEFAULT_FONT;
    _Shader = sh;
    _ResX = resx;
    _ResY = resy;

    _BackgroundColor = glm::vec4(0.2f,0.2f,0.2f,0.3f);
    _TextColor = glm::vec4(1.0f,1.0f,1.0f,1.0f);
    _Scale = 1.0f;
    _Pixelsize = 16;
    _MarginLeft = 5.0f;
    _MarginRight= 5.0f;
    _MarginY = 5.0f;
    _AlignRight = false;
}

void ENGINE::RenderText::RenderFrame(GLfloat x, GLfloat y) {
    // Standard Masse der images !!
    GLfloat w = _Textfeld.w;
    GLfloat h = 16.0f;

    interSectHeadline.x = x;
    interSectHeadline.w = x + w;
    interSectHeadline.y = y - h;
    interSectHeadline.h = y;

    GLfloat vertices[6][4] = {
        { x,     y  - h,        0.0, 0.0 },
        { x,     y,             0.0, 1.0 },
        { x + w, y,             1.0, 1.0 },

        { x, y - h,             0.0, 0.0 }, // w muss weg für 6  uv = 0,0 !!
        { x + w, y,             1.0, 1.0 },
        { x + w, y - h,         1.0, 0.0 }
    };


    glUniform4f(framecolor_loc,1.0,1.0,1.0,1.0);
    glBufferSubData(GL_ARRAY_BUFFER,0,sizeof(vertices),vertices);
//    glBindTexture(GL_TEXTURE_2D,tex);
    glDrawElements( GL_TRIANGLE_STRIP, 6, GL_UNSIGNED_SHORT, 0);
//    glBindTexture(GL_TEXTURE_2D, 0);
}


