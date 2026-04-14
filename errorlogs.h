#ifndef ERRORLOGS_H
#define ERRORLOGS_H

#include <string>
#include <logger.h>

using namespace std;

class ErrorLogs
{
public:
    static void Warning(bool isExpressionTrue,const string message);
    static void Failed(bool isExpressionTrue,const string message, bool & errorflag );

private:

    ErrorLogs() {}
    Logger log;
};

#endif // ERRORLOGS_H
