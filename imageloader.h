#ifndef IMAGELOADER_H
#define IMAGELOADER_H

#include <string>
#include <SDL_image.h>
#include <SDL.h>

#include <logger.h>

using namespace std;
const string STANDARDIMAGE = "images/standard/errorAlpha.png";


class ImageLoader
{
public:
    ImageLoader(string path);
    ~ImageLoader();
    static SDL_Surface * getSurface(string  path, string obj);
private:
    string _Path;
    static SDL_Surface * loadStandardSurface();  // loads a "error" image to avoid crash in opengl

    static Logger log;
};

#endif // IMAGELOADER_H
