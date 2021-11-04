#include "Log.h"

#include <cstdarg>
#include <cstdio>

void Log::Print(char const* message, ...)
{
    va_list argumentList;
    va_start(argumentList, message);
    vprintf(message, argumentList);
    va_end(argumentList);
}
