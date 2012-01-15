#include <types.h>
#include <arch.h>
#include <print.h>

int kmain(void);

int kmain()
{
    arch_machine_setup();
    printf("GXLDR Stage 2\r\n");
    
    /*int a = 1;
    int b = 2;
    
    int c = a/(b-2);*/

    return 0;
}
