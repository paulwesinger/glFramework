#include "imageloader.h"

ImageLoader::ImageLoader(string path) { _Path = path; }

ImageLoader::~ImageLoader(){}

SDL_Surface * ImageLoader::getSurface(string  path,string obj){

    SDL_Surface * surf = IMG_Load( path.c_str() );
    if ( surf ) {
        log.loginfo("Succes:  Image " + path + " loaded",obj);
        return surf;
    }
    else {
        surf = loadStandardSurface();
        Logger::logwarn("Warning: Image " + path + " not loaded ! created a template image ! Please Check Path.", obj);
        return  surf;
    }

}

SDL_Surface * ImageLoader::loadStandardSurface(){

    return IMG_Load(STANDARDIMAGE.c_str());
}
