#include <rand.h>

static uint32_t rnd_lcg1_xn = 1;

void srand(uint32_t seed)
{
	rnd_lcg1_xn = seed;
}

uint32_t rand()
{
	rnd_lcg1_xn *= 3039177861U;
	return rnd_lcg1_xn;
}
