#include "def.h"

#include <iostream>

int main()
{
    vec2 v1{10.l, 10.l};
    vec2 v2 = v1 * 2.l; 
    vec2 v3 = v1 + v2;

    std::cout << v3.x << ", " << v3.y << std::endl;

    return 0;
}