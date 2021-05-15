#include <zconf.h>

/**
 * typedef struct  VS  struct
 *
 * C 中定义结构体类型使用 typedef struct
 *
 * typedef struct Student {
 *     int a;
 * } Stu;
 *
 * 这里的Stu实际上是 struct Student 的别名. Stu == struct Student
 *
 * 声明变量方式:
 *      Stu stu;
 *      struct  Student stu;
 *
 * typedef struct {
 *     int a;
 * } Stu;
 *
 * 声明变量方式:
 *      Stu stu;
 *
 *----------------------------------------------------------------------------------------------------------------------
 *
 * C++中定义结构体:
 * struct Student {
 *     int a;
 * };
 *
 * 声明变量: Student stu
 *
 * struct Student {
 *     int a;
 * } stu;
 *
 * 这里的 stu 是一个变量. (important!!!)
 *
 *
 * NOTE: typedef struct defined
 * typedef struct Student {
 *     int a;
 * } Stu;
 *
 * 这里的 Stu 是一个结构体类型 = struct Student
 *
 * 声明变量:  Stu stu;
 *          Student stu;
 *          struct Student stu;
 *----------------------------------------------------------------------------------------------------------------------
 *
 * C当中的特殊情况:
 * typedef struct {
 *     num int;
 * } a, b, c;
 *
 * a, b, c 是相同的类型
 */

// 带有别名的方式
typedef struct Alias {
    const char* name;
    uint age;
} Alias;

// 不带别名的方式
typedef struct {
    const char* name;
    uint age;
} Name;
