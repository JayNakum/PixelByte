#ifndef PIXELBYTE_LOG_H
#define PIXELBYTE_LOG_H

#include <stdio.h>
#include <string.h>
#include <time.h>

static inline const char* timeNow(void)
{
    static char buffer[16];
    time_t t = time(NULL);

    struct tm tmInfo;

#ifdef PXB_WINDOWS
    localtime_s(&tmInfo, &t);
#elif PXB_LINUX
    tmInfo = *localtime(&t);
#endif

    strftime(buffer, sizeof(buffer), "%H:%M:%S", &tmInfo);
    return buffer;
}

#define PXB_LOG_FORMAT "[%s] %s | %s | %s:%d | "
#define PXB_LOG_ARGS timeNow(), __FILE__, __func__, __LINE__

#define PXB_LOG(category, messageFormat, ...) \
    do \
    { \
        printf(PXB_LOG_FORMAT messageFormat "\n", #category, PXB_LOG_ARGS __VA_OPT__(,) __VA_ARGS__); \
        fflush(stdout); \
    } while(0)

#define PXB_LOG_MSG(category, message) PXB_LOG(category, "%s", message)

#define PXB_ASSERT(expression) \
    do \
    { \
        if (!(expression)) \
        { \
            PXB_LOG(ASSERT, "Expression Failed %s", #expression); \
            abort(); \
        } \
    } while(0)

#endif // PIXELBYTE_LOG_H
