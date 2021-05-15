#include "log.h"
#include <unistd.h>
#include <pthread.h>
#include <fcntl.h>
#include <string.h>
#include <stdarg.h>

#define MAX_FILE_LOG_BYTE     (5000000)

int LOG_LEVEL = 4;
int enbale_log_file = 0;
static int log_fd = -1;
static pthread_mutex_t log_file_mutex = PTHREAD_MUTEX_INITIALIZER;

char* time_now(char* p) {
    time_t t;
    struct tm* tm_now;

    time(&t);
    tm_now = localtime(&t);

    snprintf(p, 24, "%04d-%02d-%02d %02d:%02d:%02d",
             tm_now->tm_year + 1900, tm_now->tm_mon, tm_now->tm_mday,
             tm_now->tm_hour, tm_now->tm_min, tm_now->tm_sec);
    return p;

}

static inline void logmutex_lock() {
    pthread_mutex_lock(&log_file_mutex);
}

static inline void logmutex_unlock() {
    pthread_mutex_unlock(&log_file_mutex);
}

// loop write
static int log_write_file(int fd, char* buf, int len) {
    int writed_len = 0;
    ssize_t n;

    while (writed_len < len) {
        n = write(fd, (void*) (buf + writed_len), (size_t) (len - writed_len));
        if (n < 0 && errno == EINTR) {
            continue;
        }

        if (n < 0) {
            enbale_log_file = 0;
            return -1;
        }

        writed_len += n;
    }

    return len;
}

int log_ini() {
    if (!enbale_log_file) {
        return 0;
    }

    log_fd = open(log_file_path, O_WRONLY | O_CREAT /*| O_APPEND*/, 0666);
    if (log_fd < 0) {
        loge("failed to open %s for logging: %s", log_file_path, strerror(errno));
        enbale_log_file = 0;
        return -1;
    }

    return 0;
}

extern void log2file(const char* timestr, const char* logtag, const char* file,
                     const char* func, int linenum, char* format, ...) {
#if 1

    char buf[LOG_BUF_SIZE] = {0};
    char fmt_buf[LOG_BUF_SIZE] = {0};

    va_list ap;
    va_list args;


    logmutex_lock();
    if (!enbale_log_file || log_fd < 0) {
        goto exit;
    }


    int len = snprintf(fmt_buf, LOG_BUF_SIZE, "%s %s %s %s() [%d] %s \n", timestr, logtag, file, func, linenum, format);
    va_start(args, format);

    long max_byte = lseek(log_fd, 0, SEEK_END);
    if (max_byte >= MAX_FILE_LOG_BYTE) {
        ftruncate(log_fd, 0);
        lseek(log_fd, 0, SEEK_SET);
    }

    len = vsnprintf(buf, LOG_BUF_SIZE, fmt_buf, args);
    log_write_file(log_fd, buf, len);

    va_end(args);

    exit:
    logmutex_unlock();
#endif
}


