#include <stdio.h>

/**
 * const char*, char const*, char* const 的区别
 *
 * 把声明从右往左读.
 *
 * char* const cp;  cp是一个const pointer to char
 *
 * char const* cc;  cc是一个pointer to const char
 * const char* cc;
 *
 * const TYPE <=> TYPE const
 * eg. const int, int const
 *
 **/

int main() {
    char str[] = "Hello World";

    // 指向 "字符串常量" 的 指针. *constchar1的值是const, 不能修改, 但是可以修改constchar1的指向
    const char* addr1 = str;
    char const* addr2 = str;
    printf("const char* : %s\n", addr1);
    printf("char const* : %s\n", addr2);

    addr1 = "const char*";
    addr2 = "char const*";
    printf("const char* : %s\n", addr1);
    printf("char const* : %s\n", addr2);

    printf("\n============================\n\n");

    // 常量的 指向 "字符" 的 指针; 即const指针. 不能修改addr指针, 但是可以修改该指针指向的内容
    char* const addr3 = str;
    printf("char* const :%s\n", addr3);

    addr3[0] = 'Z';
    printf("char* const :%s\n", addr3);


    /**
     * 字符串是 char[] 或者 const char*
     *
     * 字符串不是char*
     *
     * char[] ==> char* 但是 char* =\=> char[]
     *
     * 作为char* 或者 char[], 可以修改字符串内容.
     * 作为const char*, 无法修改字符串内容.
     *
     **/

    char c1[] = "Hello";
    char const* c2 = "String";
    printf("char[] => %s | char const* => %s \n", c1, c2);


    char c3[] = "WWW";
    char* c4 = c3;

    c3[0] = 'Z';
    c4[0] = 'Q';
    printf("%d\n", c3 == c4);
    return 0;
}