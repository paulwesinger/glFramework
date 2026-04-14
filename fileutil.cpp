#include "fileutil.h"
#include "utils.h"

using namespace std;

const int MAXLINES = 100;

FileUtil::FileUtil() {
    clear();
}

FileUtil::FileUtil(const FileUtil& orig) {
}

FileUtil::~FileUtil() {
}

void FileUtil::clear() {
    flags = fstream::app ; // standard Flag
    fileopen = false;
    filename = "";
}
bool FileUtil::readShaderSource(string & s, string path){
    FILE * pFile;
    char buffer[100];
    pFile = fopen(path.c_str(),"r");

    if ( pFile == nullptr)
        return false;

    while ( ! feof(pFile) ) {
        if ( fgets (buffer , 100 , pFile) == NULL ) break;
        std::string st(buffer);
        s += st;
    }
    return true;
}

bool FileUtil::writeline(string datei,string text) {
    ofstream os (datei.c_str(), flags);
    if ( os.is_open()) {
        char * buffer = new char[text.size()];
        int size = text.size();
        text.copy(buffer,size);
        os.write(buffer,size);
        os.close();
        log.loginfo ("Writing Text to file " + datei + " ... done","fileUtil");
        log.loginfo ("File " + datei + " ... closed","fileUtil");
        return true;
    }
    else
        log.logwarn("File " + datei + " was not open ... failure!" );

    return false;
}

std::string FileUtil::Parse(std::string line, std::string delString, std::string spacestring) {
    std::string help = delString + spacestring;

    return help.erase(0,help.length());
}

int FileUtil::ParseToint(std::string line, std::string delString, std::string spacestring) {
    std::string help = Parse(line,delString,spacestring);
    return StringToInt(help);
}

bool FileUtil::readLine(string datei,std::vector<std::string> &lines) {
    ifstream is;
    string line;

    is.open(datei.c_str());
    if (is.is_open() ) {
        int i = 0;
        while (getline(is,line)) {
            lines.push_back(line);
            i++;
        }
        is.close();

        log.loginfo("Loaded " + IntToString(i) + " entries","fileUtil :: readline" );
        log.loginfo ("Reading Data from file " + datei + " ... done","fileUtil :: readLine");
        log.logEmptyLine(1);

        return  true;
    }
    else {
        log.logwarn("Warning : Konnte Datei " + datei + " nicht öffnen","fileutil");
        return  false;
    }
}

void FileUtil::setFlags(ifstream::openmode fileflags) {  // falls trunc gewünscht
    flags = fileflags;
}
