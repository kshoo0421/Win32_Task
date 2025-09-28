#define _CRTDBG_MAP_ALLOC
#include <cstdlib>
#include <crtdbg.h>
#include <iostream>
#include "String.h"

int main()
{
    String a("Hello");
    String b("World");
    String c = a + b;
    std::cout << c << std::endl;

    // 종료 시점 릭 체크
    _CrtDumpMemoryLeaks();
    return 0;
}
