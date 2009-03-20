#include "utils.h"

#define cpuid(func,ax,bx,cx,dx)\
	__asm__ __volatile__ ("cpuid":\
	"=a" (ax), "=b" (bx), "=c" (cx), "=d" (dx) : "a" (func));

char* get_cpu_vendor () {

	static char	vendor[13] = {0};
	int a;
	
	int v1;
	int v2;
	int v3;

	cpuid(0x0, a, v1, v2, v3);
	
	vendor[3] = (char)(v1 >> 24);
	vendor[2] = (char)(v1 >> 16);
	vendor[1] = (char)(v1 >> 8);
	vendor[0] = (char)(v1);
	
	vendor[7] = (char)(v3 >> 24);
	vendor[6] = (char)(v3 >> 16);
	vendor[5] = (char)(v3 >> 8);
	vendor[4] = (char)(v3);
	
	vendor[11] = (char)(v2 >> 24);
	vendor[10] = (char)(v2 >> 16);
	vendor[9] = (char)(v2 >> 8);
	vendor[8] = (char)v2;

	return vendor;
}

uint8_t inportb (uint16_t portid)
{
	uint8_t ret;
	asm volatile("inb %1, %0" : "=a" (ret) : "dN" (portid));	
	return ret;
}

void outportb (uint16_t portid, uint8_t value)
{
	asm volatile ("outb %1, %0" : : "dN" (portid), "a" (value));
}
