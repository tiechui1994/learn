#include "util/log.h"
#include <memory.h>


int main() {
    static char* p1 = "Hello World";
    static char* p2 = "This is P2";
    printf("point:  p1:%p, p2:%p\n", &p1, &p2);
    printf("content: p1:%s, p2:%s\n", p1, p2);

    p2 = p1;
    printf("point: p2:%p\n", &p2);
    printf("content: p2:%s\n", p2);


    strcpy(log_file_path, "./log.log");
    log_ini();
    loge("the debug is notice");
}