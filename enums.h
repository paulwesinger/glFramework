#ifndef ENUMS_H
#define ENUMS_H
enum ShaderType {
    COLOR_SHADER = 0,
    TEXTURE_SHADER,
    LIGHT_SHADER,
    LIGHT_COLOR_SHADER,
    LIGHT_TEXTURE_SHADER,
    GLASS_SHADER
};

enum LineType {
    DASHED       = 0,
    POINT,
    SOLID
};
#endif // ENUMS_H
