#ifndef IMAGELOADER_H
#define IMAGELOADER_H

#include <string>
#include <SDL.h>
#include <SDL_image.h>


#include <logger.h>

using namespace std;

namespace ENGINE{
const string STANDARDIMAGE = "images/standard/errorAlpha.png";


class ImageLoader
{
public:
    ImageLoader();
    ImageLoader(string path);
    ~ImageLoader();
    SDL_Surface * getSurface(string  path, string obj);
private:
    string _Path;
    SDL_Surface * loadStandardSurface();  // loads a "error" image to avoid crash in opengl

    Logger log;
};
}
#endif // IMAGELOADER_H
