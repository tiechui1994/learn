/***********************************************************************************************************************
 * 限定符
 *
 * 通常使用类型和存储类别来描述一个变量. C90增加了两个属性: 恒常性(constancy) 和 易变性(volatility). 这两个属性可以分别使用关键字
 * const 和 volatile 来声明, 以这两个关键字创建的类型是限定类型.
 * C99新增了第3个限定符: restrict, 用于提高编译器优化. C11增加了第4个限定符: _Atomic
 *
 *
 * const 类型限定符
 * 以const关键字声明的对象, 其值不能通过赋值或递增, 递减来修改.
 *
 * - 在指针和形参声明中使用const
 * 声明普通变量和数组时使用const关键字很简单. 指针相对复杂一些, 因为要区分是限定指针本身为const还是限定指针指向的值为const.
 * const float* p1; //
 * float const* p2; // p1, p2, 指针指向的数据无法修改. 可以修改指针指向的地址.
 *
 * float* const p3; // p3, 指针指向的地址无法修改. 可以修改指针指向值.
 * // 简而言之, const 在 * 左侧任意位置, 限定了指针指向的数据不能改变. const在 * 的右侧, 限定了指针本身不能改变.
 *
 *
 * - 对全局数据使用const
 *
 *
 * volatile
 * volatile 限定符告知计算机, 代理可以改变该变量的值. 通常, 它被用于硬件地址以及在其他程序或同时运行的线程中共享数据.
 * 例如, 一个地址上可能储存着当前的时钟信息, 无论程序做什么, 地址上的值都随时间的变化而变化. 或者一个地址用于接受另一台计算机传入的
 * 信息.
 *
 *
 * restrict
 * restrict 关键字允许编译器优化某部分以更好地支持计算. 它只能用于指针, 表明该指针类型是访问数据对象的唯一且初始非方式.
 *
 *
 ***********************************************************************************************************************/

#include <stdio.h>

void main() {
    char* ch1 = "www.qq.com";
    // 指针本身不能改变
    char* const ch2 = "const addr, volatile value"; // 指向的地址无法修改, 可以修改指向的值
    printf("ch2:%s\n", ch2);

    int i0 = 100;
    int* const i1 = &i0;
    *i1 = 101;
    printf("i1:%d\n", *i1);

    // 指针指向的数据不能更改
    char const* ch4 = "";
    const char* ch3 = "const value, volatile addr"; // 指向的值无法修改, 但是可以修改指向的地址
    printf("ch3 addr:%p\n", &ch3);
    ch3 = "";
    printf("ch3 addr:%p\n", &ch3);


    char* volatile p1 = "Java";
    char volatile* p2 = "C++";
    volatile char* p3 = "Go";

    printf("p1:%p, p2:%p, p3:%p\n", &p1, &p2, &p3);
    p2 = p3 = p1 = "C";
    printf("p1:%p, p2:%p, p3:%p\n", &p1, &p2, &p3);
}
