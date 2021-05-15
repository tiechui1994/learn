#ifndef ADVANCE_LOG_H
#define ADVANCE_LOG_H

#include <stdio.h>
#include <time.h>
#include <errno.h>

#define LOG_FILE_NAME_LEN 128
#define LOG_BUF_SIZE 1024

extern int LOG_LEVEL;
extern int enbale_log_file;
extern char log_file_path[LOG_FILE_NAME_LEN];

extern char* time_now(char* p);

extern int log_ini();

extern void log2file(const char* timestr, const char* logtag, const char* file, const char* func,
                     int linenum, char* format, ...);

#define loge(format, ...) \
    do {                     \
        if (LOG_LEVEL >= 1){\
            char t[24] = {0}; \
            time_now(t);  \
            printf("%s [ERROR] %s() [%d] "#format"\n",t,__FUNCTION__,__LINE__ ,##__VA_ARGS__);\
            log2file(t ,"[ERROR]", __FILE__,__FUNCTION__,__LINE__, format, ##__VA_ARGS__); \
        }\
    }while(0)


#define logw(format, ...) \
    do {                     \
        if (LOG_LEVEL >= 2){\
            char t[24] = {0}; \
            time_now(t);  \
            printf("%s [WARN] %s() [%d] "#format"\n",t,__FUNCTION__,__LINE__ ,##__VA_ARGS__);\
            log2file(t ,"[WARN]", __FILE__,__FUNCTION__,__LINE__, format, ##__VA_ARGS__); \
        }\
    }while(0)

#define logi(format, ...) \
    do {                     \
        if (LOG_LEVEL >= 3){\
            char t[24] = {0}; \
            time_now(t);  \
            printf("%s [INFO] %s() [%d] "#format"\n",t,__FUNCTION__,__LINE__ ,##__VA_ARGS__);\
            log2file(t ,"[INFO]", __FILE__,__FUNCTION__,__LINE__, format, ##__VA_ARGS__); \
        }\
    }while(0)

#define log(format, ...) \
    do {                     \
        if (LOG_LEVEL >= 4){\
            char t[24] = {0}; \
            time_now(t);  \
            printf("%s [DEBUG] %s() [%d] "#format"\n",t,__FUNCTION__,__LINE__ ,##__VA_ARGS__);\
            log2file(t ,"[DEBUG]", __FILE__,__FUNCTION__,__LINE__, format, ##__VA_ARGS__); \
        }\
    }while(0)
#define logd(format, ...) \
    do {                     \
        if (LOG_LEVEL >= 4){\
            char t[24] = {0}; \
            time_now(t);  \
            printf("%s [DEBUG] %s() [%d] "#format"\n",t,__FUNCTION__,__LINE__ ,##__VA_ARGS__);\
            log2file(t ,"[DEBUG]", __FILE__,__FUNCTION__,__LINE__, format, ##__VA_ARGS__); \
        }\
    }while(0)

#endif