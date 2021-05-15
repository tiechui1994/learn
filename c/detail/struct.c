#include "struct.h"
#include <stdio.h>

int main() {
    Name name = {
            name:"qq",
            age:12
    };

    struct Alias alias = {
            name:"ww",
            age:100
    };

    Alias alias1 = {
            name:"alias1",
            age:101
    };


    Name* np = (Name*) (&alias);


    printf("this is Alias, name:%s, age:%d\n", alias.name, alias.age);
    printf("this is Name, name:%s, age:%d\n", name.name, name.age);
}
