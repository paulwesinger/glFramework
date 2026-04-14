#include "errorlogs.h"

void ErrorLogs::Warning(bool isExpressionTrue,std::string message) {
    if ( ! isExpressionTrue) {
        Logger::logwarn(message);
    }
}

void ErrorLogs::Failed(bool isExpressionTrue,const std::string message, bool & errorflag) {
    if (! isExpressionTrue) {
        errorflag = true;
        Logger::logwarn(message);
        return;
    }
    errorflag = false;
}
