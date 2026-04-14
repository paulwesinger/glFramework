#include "utils.h"


std::string IntToString ( int value) {
    char buf [50];
    // return wert wird nicht gebraucht
    snprintf(buf,50,"%d", value);
    return (std::string(buf));
}

std::string FloatToString ( float value) {

    std::string num_text = std::to_string(value);
    std::string rounded = num_text.substr(0, num_text.find(".")+3);

    //std::string s = std::format("{:.2f}", 3.14159265359);     c++20 aktualisieren
    return rounded;
}

float StringToFloat( std::string value) {
    std::string::size_type sz;
    return std::stof(value,&sz);
}

int StringToInt(std::string value) {
    std::string::size_type sz;
    return std::stoi(value,&sz);
}
