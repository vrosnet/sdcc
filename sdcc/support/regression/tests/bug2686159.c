/*
   bug2686159.c
 */

#include <testfwk.h>

#ifdef __SDCC_pic16
# define ADDRESS(x) (0x02 ## x)
#else
# define ADDRESS(x) (0xCA ## x)
#endif

/* This produces incorrect code */
#define REG_1 (*(__xdata unsigned char*)ADDRESS(00))

void incorrect(void)
{
	REG_1 |= 1;
	REG_1 |= 2;
}

/* This produces correct code */
__xdata unsigned char __at(ADDRESS(01)) REG_2;

void correct(void)
{
	REG_2 |= 1;
	REG_2 |= 2;
}

void testBug(void)
{
#ifdef __SDCC
	REG_1 = 0x40;
	incorrect();
	ASSERT (REG_1 == 0x43);
#endif
	REG_2 = 0x50;
	correct();
	ASSERT (REG_2 == 0x53);
}
