#include <sys/types.h>
#include <iostream>
#include <cstdlib>

typedef unsigned long Address;

#define hex std::hex
#define dec std::dec

#define OBJ_SYS_DEBUG_MGS(x)        \
do {                                \
    std::cout << x << std::endl;    \
} while(false);
