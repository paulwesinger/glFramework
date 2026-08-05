#include "imageloader.h"

ENGINE::ImageLoader::ImageLoader(string path) { _Path = path; }
ENGINE::ImageLoader::ImageLoader() { _Path = ""; }
ENGINE::ImageLoader::~ImageLoader(){}

SDL_Surface * ENGINE::ImageLoader::getSurface(string  path,string obj){
    try{

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
    catch(exception e){
        Logger::logError( e.what());
        return nullptr;
    }
    return nullptr;

}

SDL_Surface * ENGINE::ImageLoader::loadStandardSurface(){

    return IMG_Load(STANDARDIMAGE.c_str());
}
