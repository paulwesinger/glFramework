#ifndef FILEUTIL_H
#define FILEUTIL_H

#include <fstream>
#include <vector>
#include <string>
#include <logger.h>

class FileUtil
{
public:
    FileUtil();
    FileUtil(const FileUtil& orig);
    virtual ~FileUtil();

    virtual bool writeline(std::string datei,std::string text);
    virtual bool readLine(std::string datei, std::vector<std::string> & lines);
    void setFlags(std::ifstream::openmode fileflags);
    bool readShaderSource(std::string & s, std::string path);
    void clear();
    std::string Parse(std::string line, std::string delString,std::string spacestring= " ");
    int ParseToint(std::string line, std::string delString,std::string spacestring= " ");
private:

    std::string filename;   // globaler  Dateiname
    std::ifstream::openmode flags;
    bool fileopen;
    Logger log;

};

#endif // FILEUTIL_H
