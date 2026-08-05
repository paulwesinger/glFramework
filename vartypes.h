#ifndef VARTYPES_H
#define VARTYPES_H

#include <glm.hpp>
#include "enums.h"
#include <string>

typedef void (*FP)();  // Zeiger auf Funktion wird für eventhandling gebraucht
typedef void (*FPB)(bool);

typedef struct{
    float x;
    float y;
} sFloat2;

typedef struct{
    float x;
    float y;
    float z;
}sFloat3;

typedef struct{
    float x;
    float y;
    float z;
    float w;

}sFloat4;

typedef struct {
    glm::vec3 vector;
    glm::vec3 color;
    glm::vec2 tex;
} sVertexTexture ;

typedef struct {
    glm::vec3 vector;
    glm::vec3 normals;
    glm::vec3 color;
    glm::vec2 tex;
} sVertexNormals ;

typedef struct {
    glm::vec3 vector;
    glm::vec3 color;
} sVertexColor;

// ++++++++++++++++++++++++++++++++++++++++++++++
// union für die obigen 3 stucts:
// Zur verwendung für 2D und 3D
// ++++++++++++++++++++++++++++++++++++++++++++++

union VectorUnion {
    sVertexTexture sVertTexture;
    sVertexNormals sVertNormals;
    sVertexColor sVertColor;
};

typedef struct {
    int x;
    int y;
    int w;
    int h;
} sRect;

struct stPoint{
    int x;
    int y;
    stPoint(){}
    stPoint(int _x, int _y)
    {
        x =_x, y = _y;
    }

};
typedef stPoint sPoint;

struct POINT {

    float x;
    float y;
    float z = 0.0f;
    POINT(){}
    POINT(float _x, float _y, float _z) :  x(_x), y(_y),z(_z)  {}
};
typedef struct POINT Point;

struct sSize {
    int w;
    int h;
    sSize(){w = 0; h = 0;}
    sSize(int width, int height){ w = width, h = height; }
} ;

typedef struct{
    long id;
    float x;
    float y;
    float z;
}sVector3;

typedef struct{
    long id;
    float x;
    float y;
}sVector2;

typedef struct{
    sFloat3 translate;
    sFloat3 rotate;
    sFloat3 scale;
}sTransform;

typedef struct{
    short p0;
    short p1;
    short p2;
}sFace;

typedef struct{
    float u;
    float v;
}sTextureCoords;

typedef struct _color{
    sFloat4 color;
}sColor;

typedef struct{
    sFloat3 ambient;
    sFloat3 diffuse;
    sFloat3 specular;
    float shiness;
} sMaterial;

// ################################
// structs zum laden von Datei
// ++++++++++++++++++++++++++++++++
typedef struct {

    bool firstTranslate;
    bool hasLight;
    sTransform trans;
    sFloat3 origin;
    sFloat4 color;
    std::string textures;// for list

    std::string texture0;
    std::string texture1;
    std::string texture2;
    std::string texture3;
    std::string texture4;

}s3DFileStruct;

// ------------------------------------------------------
// needed for loading 2D and 3D objects from a file
// ------------------------------------------------------
typedef struct {

    s3DFileStruct filestruct;

    // zusätzlich für kugel:
    float radius;
    int meshpoints;

}sSphereFileStruct;

typedef  struct {
    int patchX;
    int patchZ;
    float rasterX;
    float rasterZ;
    std::string textures;
    std::string texture0;
    std::string texture1;
    std::string texture2;
    std::string texture3;
    std::string texture4;
    sFloat4 color;
    sTransform trans;
    bool hasLight;
}sLandscape ;

#endif // VARTYPES_H
