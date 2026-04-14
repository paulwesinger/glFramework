#ifndef VARTYPES_H
#define VARTYPES_H

typedef struct{
    float x;
    float y;
    float z;
    float w;

}float4;

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

typedef struct _float2{
    float x;
    float y;
} FLOAT2;

typedef struct _float3{
    float x;
    float y;
    float z;
}FLOAT3;

typedef struct _float4{
    float x;
    float y;
    float z;
    float w;
}FLOAT4;


#endif // VARTYPES_H
