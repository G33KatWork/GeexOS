/*
=========================================================================
	main.cpp

	main kernel program
=========================================================================
*/

#include <hal.h>
#include "DebugDisplay.h"
#include "exception.h"

void irq0(void){DebugClrScr(0x18); DebugGotoXY(0,0); DebugSetColor(0x1e); DebugPuts("irq0"); for(;;) ;}
void irq1(void){DebugClrScr(0x18); DebugGotoXY(0,0); DebugSetColor(0x1e); DebugPuts("irq1"); for(;;) ;}
void irq2(void){DebugClrScr(0x18); DebugGotoXY(0,0); DebugSetColor(0x1e); DebugPuts("irq2"); for(;;) ;}
void irq3(void){DebugClrScr(0x18); DebugGotoXY(0,0); DebugSetColor(0x1e); DebugPuts("irq3"); for(;;) ;}
void irq4(void){DebugClrScr(0x18); DebugGotoXY(0,0); DebugSetColor(0x1e); DebugPuts("irq4"); for(;;) ;}
void irq5(void){DebugClrScr(0x18); DebugGotoXY(0,0); DebugSetColor(0x1e); DebugPuts("irq5"); for(;;) ;}
void irq6(void){DebugClrScr(0x18); DebugGotoXY(0,0); DebugSetColor(0x1e); DebugPuts("irq6"); for(;;) ;}
void irq7(void){DebugClrScr(0x18); DebugGotoXY(0,0); DebugSetColor(0x1e); DebugPuts("irq7"); for(;;) ;}
void irq8(void){DebugClrScr(0x18); DebugGotoXY(0,0); DebugSetColor(0x1e); DebugPuts("irq8"); for(;;) ;}
void irq9(void){DebugClrScr(0x18); DebugGotoXY(0,0); DebugSetColor(0x1e); DebugPuts("irq9"); for(;;) ;}
void irq10(void){DebugClrScr(0x18); DebugGotoXY(0,0); DebugSetColor(0x1e); DebugPuts("irq10"); for(;;) ;}
void irq11(void){DebugClrScr(0x18); DebugGotoXY(0,0); DebugSetColor(0x1e); DebugPuts("irq11"); for(;;) ;}
void irq12(void){DebugClrScr(0x18); DebugGotoXY(0,0); DebugSetColor(0x1e); DebugPuts("irq12"); for(;;) ;}
void irq13(void){DebugClrScr(0x18); DebugGotoXY(0,0); DebugSetColor(0x1e); DebugPuts("irq13"); for(;;) ;}
void irq14(void){DebugClrScr(0x18); DebugGotoXY(0,0); DebugSetColor(0x1e); DebugPuts("irq14"); for(;;) ;}
void irq15(void){DebugClrScr(0x18); DebugGotoXY(0,0); DebugSetColor(0x1e); DebugPuts("irq15"); for(;;) ;}
void irq16(void){DebugClrScr(0x18); DebugGotoXY(0,0); DebugSetColor(0x1e); DebugPuts("irq16"); for(;;) ;}
void irq17(void){DebugClrScr(0x18); DebugGotoXY(0,0); DebugSetColor(0x1e); DebugPuts("irq17"); for(;;) ;}
void irq18(void){DebugClrScr(0x18); DebugGotoXY(0,0); DebugSetColor(0x1e); DebugPuts("irq18"); for(;;) ;}
void irq19(void){DebugClrScr(0x18); DebugGotoXY(0,0); DebugSetColor(0x1e); DebugPuts("irq19"); for(;;) ;}
void irq20(void){DebugClrScr(0x18); DebugGotoXY(0,0); DebugSetColor(0x1e); DebugPuts("irq20"); for(;;) ;}
void irq21(void){DebugClrScr(0x18); DebugGotoXY(0,0); DebugSetColor(0x1e); DebugPuts("irq21"); for(;;) ;}
void irq22(void){DebugClrScr(0x18); DebugGotoXY(0,0); DebugSetColor(0x1e); DebugPuts("irq22"); for(;;) ;}
void irq23(void){DebugClrScr(0x18); DebugGotoXY(0,0); DebugSetColor(0x1e); DebugPuts("irq23"); for(;;) ;}
void irq24(void){DebugClrScr(0x18); DebugGotoXY(0,0); DebugSetColor(0x1e); DebugPuts("irq24"); for(;;) ;}
void irq25(void){DebugClrScr(0x18); DebugGotoXY(0,0); DebugSetColor(0x1e); DebugPuts("irq25"); for(;;) ;}
void irq26(void){DebugClrScr(0x18); DebugGotoXY(0,0); DebugSetColor(0x1e); DebugPuts("irq26"); for(;;) ;}
void irq27(void){DebugClrScr(0x18); DebugGotoXY(0,0); DebugSetColor(0x1e); DebugPuts("irq27"); for(;;) ;}
void irq28(void){DebugClrScr(0x18); DebugGotoXY(0,0); DebugSetColor(0x1e); DebugPuts("irq28"); for(;;) ;}
void irq29(void){DebugClrScr(0x18); DebugGotoXY(0,0); DebugSetColor(0x1e); DebugPuts("irq29"); for(;;) ;}
void irq30(void){DebugClrScr(0x18); DebugGotoXY(0,0); DebugSetColor(0x1e); DebugPuts("irq30"); for(;;) ;}
void irq31(void){DebugClrScr(0x18); DebugGotoXY(0,0); DebugSetColor(0x1e); DebugPuts("irq31"); for(;;) ;}
void irq32(void){DebugClrScr(0x18); DebugGotoXY(0,0); DebugSetColor(0x1e); DebugPuts("irq32"); for(;;) ;}
void irq33(void){DebugClrScr(0x18); DebugGotoXY(0,0); DebugSetColor(0x1e); DebugPuts("irq33"); for(;;) ;}
void irq34(void){DebugClrScr(0x18); DebugGotoXY(0,0); DebugSetColor(0x1e); DebugPuts("irq34"); for(;;) ;}
void irq35(void){DebugClrScr(0x18); DebugGotoXY(0,0); DebugSetColor(0x1e); DebugPuts("irq35"); for(;;) ;}
void irq36(void){DebugClrScr(0x18); DebugGotoXY(0,0); DebugSetColor(0x1e); DebugPuts("irq36"); for(;;) ;}
void irq37(void){DebugClrScr(0x18); DebugGotoXY(0,0); DebugSetColor(0x1e); DebugPuts("irq37"); for(;;) ;}
void irq38(void){DebugClrScr(0x18); DebugGotoXY(0,0); DebugSetColor(0x1e); DebugPuts("irq38"); for(;;) ;}
void irq39(void){DebugClrScr(0x18); DebugGotoXY(0,0); DebugSetColor(0x1e); DebugPuts("irq39"); for(;;) ;}
void irq40(void){DebugClrScr(0x18); DebugGotoXY(0,0); DebugSetColor(0x1e); DebugPuts("irq40"); for(;;) ;}
void irq41(void){DebugClrScr(0x18); DebugGotoXY(0,0); DebugSetColor(0x1e); DebugPuts("irq41"); for(;;) ;}
void irq42(void){DebugClrScr(0x18); DebugGotoXY(0,0); DebugSetColor(0x1e); DebugPuts("irq42"); for(;;) ;}
void irq43(void){DebugClrScr(0x18); DebugGotoXY(0,0); DebugSetColor(0x1e); DebugPuts("irq43"); for(;;) ;}
void irq44(void){DebugClrScr(0x18); DebugGotoXY(0,0); DebugSetColor(0x1e); DebugPuts("irq44"); for(;;) ;}
void irq45(void){DebugClrScr(0x18); DebugGotoXY(0,0); DebugSetColor(0x1e); DebugPuts("irq45"); for(;;) ;}
void irq46(void){DebugClrScr(0x18); DebugGotoXY(0,0); DebugSetColor(0x1e); DebugPuts("irq46"); for(;;) ;}
void irq47(void){DebugClrScr(0x18); DebugGotoXY(0,0); DebugSetColor(0x1e); DebugPuts("irq47"); for(;;) ;}
void irq48(void){DebugClrScr(0x18); DebugGotoXY(0,0); DebugSetColor(0x1e); DebugPuts("irq48"); for(;;) ;}
void irq49(void){DebugClrScr(0x18); DebugGotoXY(0,0); DebugSetColor(0x1e); DebugPuts("irq49"); for(;;) ;}
void irq50(void){DebugClrScr(0x18); DebugGotoXY(0,0); DebugSetColor(0x1e); DebugPuts("irq50"); for(;;) ;}
void irq51(void){DebugClrScr(0x18); DebugGotoXY(0,0); DebugSetColor(0x1e); DebugPuts("irq51"); for(;;) ;}
void irq52(void){DebugClrScr(0x18); DebugGotoXY(0,0); DebugSetColor(0x1e); DebugPuts("irq52"); for(;;) ;}
void irq53(void){DebugClrScr(0x18); DebugGotoXY(0,0); DebugSetColor(0x1e); DebugPuts("irq53"); for(;;) ;}
void irq54(void){DebugClrScr(0x18); DebugGotoXY(0,0); DebugSetColor(0x1e); DebugPuts("irq54"); for(;;) ;}
void irq55(void){DebugClrScr(0x18); DebugGotoXY(0,0); DebugSetColor(0x1e); DebugPuts("irq55"); for(;;) ;}
void irq56(void){DebugClrScr(0x18); DebugGotoXY(0,0); DebugSetColor(0x1e); DebugPuts("irq56"); for(;;) ;}
void irq57(void){DebugClrScr(0x18); DebugGotoXY(0,0); DebugSetColor(0x1e); DebugPuts("irq57"); for(;;) ;}
void irq58(void){DebugClrScr(0x18); DebugGotoXY(0,0); DebugSetColor(0x1e); DebugPuts("irq58"); for(;;) ;}
void irq59(void){DebugClrScr(0x18); DebugGotoXY(0,0); DebugSetColor(0x1e); DebugPuts("irq59"); for(;;) ;}
void irq60(void){DebugClrScr(0x18); DebugGotoXY(0,0); DebugSetColor(0x1e); DebugPuts("irq60"); for(;;) ;}
void irq61(void){DebugClrScr(0x18); DebugGotoXY(0,0); DebugSetColor(0x1e); DebugPuts("irq61"); for(;;) ;}
void irq62(void){DebugClrScr(0x18); DebugGotoXY(0,0); DebugSetColor(0x1e); DebugPuts("irq62"); for(;;) ;}
void irq63(void){DebugClrScr(0x18); DebugGotoXY(0,0); DebugSetColor(0x1e); DebugPuts("irq63"); for(;;) ;}
void irq64(void){DebugClrScr(0x18); DebugGotoXY(0,0); DebugSetColor(0x1e); DebugPuts("irq64"); for(;;) ;}
void irq65(void){DebugClrScr(0x18); DebugGotoXY(0,0); DebugSetColor(0x1e); DebugPuts("irq65"); for(;;) ;}
void irq66(void){DebugClrScr(0x18); DebugGotoXY(0,0); DebugSetColor(0x1e); DebugPuts("irq66"); for(;;) ;}
void irq67(void){DebugClrScr(0x18); DebugGotoXY(0,0); DebugSetColor(0x1e); DebugPuts("irq67"); for(;;) ;}
void irq68(void){DebugClrScr(0x18); DebugGotoXY(0,0); DebugSetColor(0x1e); DebugPuts("irq68"); for(;;) ;}
void irq69(void){DebugClrScr(0x18); DebugGotoXY(0,0); DebugSetColor(0x1e); DebugPuts("irq69"); for(;;) ;}
void irq70(void){DebugClrScr(0x18); DebugGotoXY(0,0); DebugSetColor(0x1e); DebugPuts("irq70"); for(;;) ;}
void irq71(void){DebugClrScr(0x18); DebugGotoXY(0,0); DebugSetColor(0x1e); DebugPuts("irq71"); for(;;) ;}
void irq72(void){DebugClrScr(0x18); DebugGotoXY(0,0); DebugSetColor(0x1e); DebugPuts("irq72"); for(;;) ;}
void irq73(void){DebugClrScr(0x18); DebugGotoXY(0,0); DebugSetColor(0x1e); DebugPuts("irq73"); for(;;) ;}
void irq74(void){DebugClrScr(0x18); DebugGotoXY(0,0); DebugSetColor(0x1e); DebugPuts("irq74"); for(;;) ;}
void irq75(void){DebugClrScr(0x18); DebugGotoXY(0,0); DebugSetColor(0x1e); DebugPuts("irq75"); for(;;) ;}
void irq76(void){DebugClrScr(0x18); DebugGotoXY(0,0); DebugSetColor(0x1e); DebugPuts("irq76"); for(;;) ;}
void irq77(void){DebugClrScr(0x18); DebugGotoXY(0,0); DebugSetColor(0x1e); DebugPuts("irq77"); for(;;) ;}
void irq78(void){DebugClrScr(0x18); DebugGotoXY(0,0); DebugSetColor(0x1e); DebugPuts("irq78"); for(;;) ;}
void irq79(void){DebugClrScr(0x18); DebugGotoXY(0,0); DebugSetColor(0x1e); DebugPuts("irq79"); for(;;) ;}
void irq80(void){DebugClrScr(0x18); DebugGotoXY(0,0); DebugSetColor(0x1e); DebugPuts("irq80"); for(;;) ;}
void irq81(void){DebugClrScr(0x18); DebugGotoXY(0,0); DebugSetColor(0x1e); DebugPuts("irq81"); for(;;) ;}
void irq82(void){DebugClrScr(0x18); DebugGotoXY(0,0); DebugSetColor(0x1e); DebugPuts("irq82"); for(;;) ;}
void irq83(void){DebugClrScr(0x18); DebugGotoXY(0,0); DebugSetColor(0x1e); DebugPuts("irq83"); for(;;) ;}
void irq84(void){DebugClrScr(0x18); DebugGotoXY(0,0); DebugSetColor(0x1e); DebugPuts("irq84"); for(;;) ;}
void irq85(void){DebugClrScr(0x18); DebugGotoXY(0,0); DebugSetColor(0x1e); DebugPuts("irq85"); for(;;) ;}
void irq86(void){DebugClrScr(0x18); DebugGotoXY(0,0); DebugSetColor(0x1e); DebugPuts("irq86"); for(;;) ;}
void irq87(void){DebugClrScr(0x18); DebugGotoXY(0,0); DebugSetColor(0x1e); DebugPuts("irq87"); for(;;) ;}
void irq88(void){DebugClrScr(0x18); DebugGotoXY(0,0); DebugSetColor(0x1e); DebugPuts("irq88"); for(;;) ;}
void irq89(void){DebugClrScr(0x18); DebugGotoXY(0,0); DebugSetColor(0x1e); DebugPuts("irq89"); for(;;) ;}
void irq90(void){DebugClrScr(0x18); DebugGotoXY(0,0); DebugSetColor(0x1e); DebugPuts("irq90"); for(;;) ;}
void irq91(void){DebugClrScr(0x18); DebugGotoXY(0,0); DebugSetColor(0x1e); DebugPuts("irq91"); for(;;) ;}
void irq92(void){DebugClrScr(0x18); DebugGotoXY(0,0); DebugSetColor(0x1e); DebugPuts("irq92"); for(;;) ;}
void irq93(void){DebugClrScr(0x18); DebugGotoXY(0,0); DebugSetColor(0x1e); DebugPuts("irq93"); for(;;) ;}
void irq94(void){DebugClrScr(0x18); DebugGotoXY(0,0); DebugSetColor(0x1e); DebugPuts("irq94"); for(;;) ;}
void irq95(void){DebugClrScr(0x18); DebugGotoXY(0,0); DebugSetColor(0x1e); DebugPuts("irq95"); for(;;) ;}
void irq96(void){DebugClrScr(0x18); DebugGotoXY(0,0); DebugSetColor(0x1e); DebugPuts("irq96"); for(;;) ;}
void irq97(void){DebugClrScr(0x18); DebugGotoXY(0,0); DebugSetColor(0x1e); DebugPuts("irq97"); for(;;) ;}
void irq98(void){DebugClrScr(0x18); DebugGotoXY(0,0); DebugSetColor(0x1e); DebugPuts("irq98"); for(;;) ;}
void irq99(void){DebugClrScr(0x18); DebugGotoXY(0,0); DebugSetColor(0x1e); DebugPuts("irq99"); for(;;) ;}
void irq100(void){DebugClrScr(0x18); DebugGotoXY(0,0); DebugSetColor(0x1e); DebugPuts("irq100"); for(;;) ;}
void irq101(void){DebugClrScr(0x18); DebugGotoXY(0,0); DebugSetColor(0x1e); DebugPuts("irq101"); for(;;) ;}
void irq102(void){DebugClrScr(0x18); DebugGotoXY(0,0); DebugSetColor(0x1e); DebugPuts("irq102"); for(;;) ;}
void irq103(void){DebugClrScr(0x18); DebugGotoXY(0,0); DebugSetColor(0x1e); DebugPuts("irq103"); for(;;) ;}
void irq104(void){DebugClrScr(0x18); DebugGotoXY(0,0); DebugSetColor(0x1e); DebugPuts("irq104"); for(;;) ;}
void irq105(void){DebugClrScr(0x18); DebugGotoXY(0,0); DebugSetColor(0x1e); DebugPuts("irq105"); for(;;) ;}
void irq106(void){DebugClrScr(0x18); DebugGotoXY(0,0); DebugSetColor(0x1e); DebugPuts("irq106"); for(;;) ;}
void irq107(void){DebugClrScr(0x18); DebugGotoXY(0,0); DebugSetColor(0x1e); DebugPuts("irq107"); for(;;) ;}
void irq108(void){DebugClrScr(0x18); DebugGotoXY(0,0); DebugSetColor(0x1e); DebugPuts("irq108"); for(;;) ;}
void irq109(void){DebugClrScr(0x18); DebugGotoXY(0,0); DebugSetColor(0x1e); DebugPuts("irq109"); for(;;) ;}
void irq110(void){DebugClrScr(0x18); DebugGotoXY(0,0); DebugSetColor(0x1e); DebugPuts("irq110"); for(;;) ;}
void irq111(void){DebugClrScr(0x18); DebugGotoXY(0,0); DebugSetColor(0x1e); DebugPuts("irq111"); for(;;) ;}
void irq112(void){DebugClrScr(0x18); DebugGotoXY(0,0); DebugSetColor(0x1e); DebugPuts("irq112"); for(;;) ;}
void irq113(void){DebugClrScr(0x18); DebugGotoXY(0,0); DebugSetColor(0x1e); DebugPuts("irq113"); for(;;) ;}
void irq114(void){DebugClrScr(0x18); DebugGotoXY(0,0); DebugSetColor(0x1e); DebugPuts("irq114"); for(;;) ;}
void irq115(void){DebugClrScr(0x18); DebugGotoXY(0,0); DebugSetColor(0x1e); DebugPuts("irq115"); for(;;) ;}
void irq116(void){DebugClrScr(0x18); DebugGotoXY(0,0); DebugSetColor(0x1e); DebugPuts("irq116"); for(;;) ;}
void irq117(void){DebugClrScr(0x18); DebugGotoXY(0,0); DebugSetColor(0x1e); DebugPuts("irq117"); for(;;) ;}
void irq118(void){DebugClrScr(0x18); DebugGotoXY(0,0); DebugSetColor(0x1e); DebugPuts("irq118"); for(;;) ;}
void irq119(void){DebugClrScr(0x18); DebugGotoXY(0,0); DebugSetColor(0x1e); DebugPuts("irq119"); for(;;) ;}
void irq120(void){DebugClrScr(0x18); DebugGotoXY(0,0); DebugSetColor(0x1e); DebugPuts("irq120"); for(;;) ;}
void irq121(void){DebugClrScr(0x18); DebugGotoXY(0,0); DebugSetColor(0x1e); DebugPuts("irq121"); for(;;) ;}
void irq122(void){DebugClrScr(0x18); DebugGotoXY(0,0); DebugSetColor(0x1e); DebugPuts("irq122"); for(;;) ;}
void irq123(void){DebugClrScr(0x18); DebugGotoXY(0,0); DebugSetColor(0x1e); DebugPuts("irq123"); for(;;) ;}
void irq124(void){DebugClrScr(0x18); DebugGotoXY(0,0); DebugSetColor(0x1e); DebugPuts("irq124"); for(;;) ;}
void irq125(void){DebugClrScr(0x18); DebugGotoXY(0,0); DebugSetColor(0x1e); DebugPuts("irq125"); for(;;) ;}
void irq126(void){DebugClrScr(0x18); DebugGotoXY(0,0); DebugSetColor(0x1e); DebugPuts("irq126"); for(;;) ;}
void irq127(void){DebugClrScr(0x18); DebugGotoXY(0,0); DebugSetColor(0x1e); DebugPuts("irq127"); for(;;) ;}
void irq128(void){DebugClrScr(0x18); DebugGotoXY(0,0); DebugSetColor(0x1e); DebugPuts("irq128"); for(;;) ;}
void irq129(void){DebugClrScr(0x18); DebugGotoXY(0,0); DebugSetColor(0x1e); DebugPuts("irq129"); for(;;) ;}
void irq130(void){DebugClrScr(0x18); DebugGotoXY(0,0); DebugSetColor(0x1e); DebugPuts("irq130"); for(;;) ;}
void irq131(void){DebugClrScr(0x18); DebugGotoXY(0,0); DebugSetColor(0x1e); DebugPuts("irq131"); for(;;) ;}
void irq132(void){DebugClrScr(0x18); DebugGotoXY(0,0); DebugSetColor(0x1e); DebugPuts("irq132"); for(;;) ;}
void irq133(void){DebugClrScr(0x18); DebugGotoXY(0,0); DebugSetColor(0x1e); DebugPuts("irq133"); for(;;) ;}
void irq134(void){DebugClrScr(0x18); DebugGotoXY(0,0); DebugSetColor(0x1e); DebugPuts("irq134"); for(;;) ;}
void irq135(void){DebugClrScr(0x18); DebugGotoXY(0,0); DebugSetColor(0x1e); DebugPuts("irq135"); for(;;) ;}
void irq136(void){DebugClrScr(0x18); DebugGotoXY(0,0); DebugSetColor(0x1e); DebugPuts("irq136"); for(;;) ;}
void irq137(void){DebugClrScr(0x18); DebugGotoXY(0,0); DebugSetColor(0x1e); DebugPuts("irq137"); for(;;) ;}
void irq138(void){DebugClrScr(0x18); DebugGotoXY(0,0); DebugSetColor(0x1e); DebugPuts("irq138"); for(;;) ;}
void irq139(void){DebugClrScr(0x18); DebugGotoXY(0,0); DebugSetColor(0x1e); DebugPuts("irq139"); for(;;) ;}
void irq140(void){DebugClrScr(0x18); DebugGotoXY(0,0); DebugSetColor(0x1e); DebugPuts("irq140"); for(;;) ;}
void irq141(void){DebugClrScr(0x18); DebugGotoXY(0,0); DebugSetColor(0x1e); DebugPuts("irq141"); for(;;) ;}
void irq142(void){DebugClrScr(0x18); DebugGotoXY(0,0); DebugSetColor(0x1e); DebugPuts("irq142"); for(;;) ;}
void irq143(void){DebugClrScr(0x18); DebugGotoXY(0,0); DebugSetColor(0x1e); DebugPuts("irq143"); for(;;) ;}
void irq144(void){DebugClrScr(0x18); DebugGotoXY(0,0); DebugSetColor(0x1e); DebugPuts("irq144"); for(;;) ;}
void irq145(void){DebugClrScr(0x18); DebugGotoXY(0,0); DebugSetColor(0x1e); DebugPuts("irq145"); for(;;) ;}
void irq146(void){DebugClrScr(0x18); DebugGotoXY(0,0); DebugSetColor(0x1e); DebugPuts("irq146"); for(;;) ;}
void irq147(void){DebugClrScr(0x18); DebugGotoXY(0,0); DebugSetColor(0x1e); DebugPuts("irq147"); for(;;) ;}
void irq148(void){DebugClrScr(0x18); DebugGotoXY(0,0); DebugSetColor(0x1e); DebugPuts("irq148"); for(;;) ;}
void irq149(void){DebugClrScr(0x18); DebugGotoXY(0,0); DebugSetColor(0x1e); DebugPuts("irq149"); for(;;) ;}
void irq150(void){DebugClrScr(0x18); DebugGotoXY(0,0); DebugSetColor(0x1e); DebugPuts("irq150"); for(;;) ;}
void irq151(void){DebugClrScr(0x18); DebugGotoXY(0,0); DebugSetColor(0x1e); DebugPuts("irq151"); for(;;) ;}
void irq152(void){DebugClrScr(0x18); DebugGotoXY(0,0); DebugSetColor(0x1e); DebugPuts("irq152"); for(;;) ;}
void irq153(void){DebugClrScr(0x18); DebugGotoXY(0,0); DebugSetColor(0x1e); DebugPuts("irq153"); for(;;) ;}
void irq154(void){DebugClrScr(0x18); DebugGotoXY(0,0); DebugSetColor(0x1e); DebugPuts("irq154"); for(;;) ;}
void irq155(void){DebugClrScr(0x18); DebugGotoXY(0,0); DebugSetColor(0x1e); DebugPuts("irq155"); for(;;) ;}
void irq156(void){DebugClrScr(0x18); DebugGotoXY(0,0); DebugSetColor(0x1e); DebugPuts("irq156"); for(;;) ;}
void irq157(void){DebugClrScr(0x18); DebugGotoXY(0,0); DebugSetColor(0x1e); DebugPuts("irq157"); for(;;) ;}
void irq158(void){DebugClrScr(0x18); DebugGotoXY(0,0); DebugSetColor(0x1e); DebugPuts("irq158"); for(;;) ;}
void irq159(void){DebugClrScr(0x18); DebugGotoXY(0,0); DebugSetColor(0x1e); DebugPuts("irq159"); for(;;) ;}
void irq160(void){DebugClrScr(0x18); DebugGotoXY(0,0); DebugSetColor(0x1e); DebugPuts("irq160"); for(;;) ;}
void irq161(void){DebugClrScr(0x18); DebugGotoXY(0,0); DebugSetColor(0x1e); DebugPuts("irq161"); for(;;) ;}
void irq162(void){DebugClrScr(0x18); DebugGotoXY(0,0); DebugSetColor(0x1e); DebugPuts("irq162"); for(;;) ;}
void irq163(void){DebugClrScr(0x18); DebugGotoXY(0,0); DebugSetColor(0x1e); DebugPuts("irq163"); for(;;) ;}
void irq164(void){DebugClrScr(0x18); DebugGotoXY(0,0); DebugSetColor(0x1e); DebugPuts("irq164"); for(;;) ;}
void irq165(void){DebugClrScr(0x18); DebugGotoXY(0,0); DebugSetColor(0x1e); DebugPuts("irq165"); for(;;) ;}
void irq166(void){DebugClrScr(0x18); DebugGotoXY(0,0); DebugSetColor(0x1e); DebugPuts("irq166"); for(;;) ;}
void irq167(void){DebugClrScr(0x18); DebugGotoXY(0,0); DebugSetColor(0x1e); DebugPuts("irq167"); for(;;) ;}
void irq168(void){DebugClrScr(0x18); DebugGotoXY(0,0); DebugSetColor(0x1e); DebugPuts("irq168"); for(;;) ;}
void irq169(void){DebugClrScr(0x18); DebugGotoXY(0,0); DebugSetColor(0x1e); DebugPuts("irq169"); for(;;) ;}
void irq170(void){DebugClrScr(0x18); DebugGotoXY(0,0); DebugSetColor(0x1e); DebugPuts("irq170"); for(;;) ;}
void irq171(void){DebugClrScr(0x18); DebugGotoXY(0,0); DebugSetColor(0x1e); DebugPuts("irq171"); for(;;) ;}
void irq172(void){DebugClrScr(0x18); DebugGotoXY(0,0); DebugSetColor(0x1e); DebugPuts("irq172"); for(;;) ;}
void irq173(void){DebugClrScr(0x18); DebugGotoXY(0,0); DebugSetColor(0x1e); DebugPuts("irq173"); for(;;) ;}
void irq174(void){DebugClrScr(0x18); DebugGotoXY(0,0); DebugSetColor(0x1e); DebugPuts("irq174"); for(;;) ;}
void irq175(void){DebugClrScr(0x18); DebugGotoXY(0,0); DebugSetColor(0x1e); DebugPuts("irq175"); for(;;) ;}
void irq176(void){DebugClrScr(0x18); DebugGotoXY(0,0); DebugSetColor(0x1e); DebugPuts("irq176"); for(;;) ;}
void irq177(void){DebugClrScr(0x18); DebugGotoXY(0,0); DebugSetColor(0x1e); DebugPuts("irq177"); for(;;) ;}
void irq178(void){DebugClrScr(0x18); DebugGotoXY(0,0); DebugSetColor(0x1e); DebugPuts("irq178"); for(;;) ;}
void irq179(void){DebugClrScr(0x18); DebugGotoXY(0,0); DebugSetColor(0x1e); DebugPuts("irq179"); for(;;) ;}
void irq180(void){DebugClrScr(0x18); DebugGotoXY(0,0); DebugSetColor(0x1e); DebugPuts("irq180"); for(;;) ;}
void irq181(void){DebugClrScr(0x18); DebugGotoXY(0,0); DebugSetColor(0x1e); DebugPuts("irq181"); for(;;) ;}
void irq182(void){DebugClrScr(0x18); DebugGotoXY(0,0); DebugSetColor(0x1e); DebugPuts("irq182"); for(;;) ;}
void irq183(void){DebugClrScr(0x18); DebugGotoXY(0,0); DebugSetColor(0x1e); DebugPuts("irq183"); for(;;) ;}
void irq184(void){DebugClrScr(0x18); DebugGotoXY(0,0); DebugSetColor(0x1e); DebugPuts("irq184"); for(;;) ;}
void irq185(void){DebugClrScr(0x18); DebugGotoXY(0,0); DebugSetColor(0x1e); DebugPuts("irq185"); for(;;) ;}
void irq186(void){DebugClrScr(0x18); DebugGotoXY(0,0); DebugSetColor(0x1e); DebugPuts("irq186"); for(;;) ;}
void irq187(void){DebugClrScr(0x18); DebugGotoXY(0,0); DebugSetColor(0x1e); DebugPuts("irq187"); for(;;) ;}
void irq188(void){DebugClrScr(0x18); DebugGotoXY(0,0); DebugSetColor(0x1e); DebugPuts("irq188"); for(;;) ;}
void irq189(void){DebugClrScr(0x18); DebugGotoXY(0,0); DebugSetColor(0x1e); DebugPuts("irq189"); for(;;) ;}
void irq190(void){DebugClrScr(0x18); DebugGotoXY(0,0); DebugSetColor(0x1e); DebugPuts("irq190"); for(;;) ;}
void irq191(void){DebugClrScr(0x18); DebugGotoXY(0,0); DebugSetColor(0x1e); DebugPuts("irq191"); for(;;) ;}
void irq192(void){DebugClrScr(0x18); DebugGotoXY(0,0); DebugSetColor(0x1e); DebugPuts("irq192"); for(;;) ;}
void irq193(void){DebugClrScr(0x18); DebugGotoXY(0,0); DebugSetColor(0x1e); DebugPuts("irq193"); for(;;) ;}
void irq194(void){DebugClrScr(0x18); DebugGotoXY(0,0); DebugSetColor(0x1e); DebugPuts("irq194"); for(;;) ;}
void irq195(void){DebugClrScr(0x18); DebugGotoXY(0,0); DebugSetColor(0x1e); DebugPuts("irq195"); for(;;) ;}
void irq196(void){DebugClrScr(0x18); DebugGotoXY(0,0); DebugSetColor(0x1e); DebugPuts("irq196"); for(;;) ;}
void irq197(void){DebugClrScr(0x18); DebugGotoXY(0,0); DebugSetColor(0x1e); DebugPuts("irq197"); for(;;) ;}
void irq198(void){DebugClrScr(0x18); DebugGotoXY(0,0); DebugSetColor(0x1e); DebugPuts("irq198"); for(;;) ;}
void irq199(void){DebugClrScr(0x18); DebugGotoXY(0,0); DebugSetColor(0x1e); DebugPuts("irq199"); for(;;) ;}
void irq200(void){DebugClrScr(0x18); DebugGotoXY(0,0); DebugSetColor(0x1e); DebugPuts("irq200"); for(;;) ;}
void irq201(void){DebugClrScr(0x18); DebugGotoXY(0,0); DebugSetColor(0x1e); DebugPuts("irq201"); for(;;) ;}
void irq202(void){DebugClrScr(0x18); DebugGotoXY(0,0); DebugSetColor(0x1e); DebugPuts("irq202"); for(;;) ;}
void irq203(void){DebugClrScr(0x18); DebugGotoXY(0,0); DebugSetColor(0x1e); DebugPuts("irq203"); for(;;) ;}
void irq204(void){DebugClrScr(0x18); DebugGotoXY(0,0); DebugSetColor(0x1e); DebugPuts("irq204"); for(;;) ;}
void irq205(void){DebugClrScr(0x18); DebugGotoXY(0,0); DebugSetColor(0x1e); DebugPuts("irq205"); for(;;) ;}
void irq206(void){DebugClrScr(0x18); DebugGotoXY(0,0); DebugSetColor(0x1e); DebugPuts("irq206"); for(;;) ;}
void irq207(void){DebugClrScr(0x18); DebugGotoXY(0,0); DebugSetColor(0x1e); DebugPuts("irq207"); for(;;) ;}
void irq208(void){DebugClrScr(0x18); DebugGotoXY(0,0); DebugSetColor(0x1e); DebugPuts("irq208"); for(;;) ;}
void irq209(void){DebugClrScr(0x18); DebugGotoXY(0,0); DebugSetColor(0x1e); DebugPuts("irq209"); for(;;) ;}
void irq210(void){DebugClrScr(0x18); DebugGotoXY(0,0); DebugSetColor(0x1e); DebugPuts("irq210"); for(;;) ;}
void irq211(void){DebugClrScr(0x18); DebugGotoXY(0,0); DebugSetColor(0x1e); DebugPuts("irq211"); for(;;) ;}
void irq212(void){DebugClrScr(0x18); DebugGotoXY(0,0); DebugSetColor(0x1e); DebugPuts("irq212"); for(;;) ;}
void irq213(void){DebugClrScr(0x18); DebugGotoXY(0,0); DebugSetColor(0x1e); DebugPuts("irq213"); for(;;) ;}
void irq214(void){DebugClrScr(0x18); DebugGotoXY(0,0); DebugSetColor(0x1e); DebugPuts("irq214"); for(;;) ;}
void irq215(void){DebugClrScr(0x18); DebugGotoXY(0,0); DebugSetColor(0x1e); DebugPuts("irq215"); for(;;) ;}
void irq216(void){DebugClrScr(0x18); DebugGotoXY(0,0); DebugSetColor(0x1e); DebugPuts("irq216"); for(;;) ;}
void irq217(void){DebugClrScr(0x18); DebugGotoXY(0,0); DebugSetColor(0x1e); DebugPuts("irq217"); for(;;) ;}
void irq218(void){DebugClrScr(0x18); DebugGotoXY(0,0); DebugSetColor(0x1e); DebugPuts("irq218"); for(;;) ;}
void irq219(void){DebugClrScr(0x18); DebugGotoXY(0,0); DebugSetColor(0x1e); DebugPuts("irq219"); for(;;) ;}
void irq220(void){DebugClrScr(0x18); DebugGotoXY(0,0); DebugSetColor(0x1e); DebugPuts("irq220"); for(;;) ;}
void irq221(void){DebugClrScr(0x18); DebugGotoXY(0,0); DebugSetColor(0x1e); DebugPuts("irq221"); for(;;) ;}
void irq222(void){DebugClrScr(0x18); DebugGotoXY(0,0); DebugSetColor(0x1e); DebugPuts("irq222"); for(;;) ;}
void irq223(void){DebugClrScr(0x18); DebugGotoXY(0,0); DebugSetColor(0x1e); DebugPuts("irq223"); for(;;) ;}
void irq224(void){DebugClrScr(0x18); DebugGotoXY(0,0); DebugSetColor(0x1e); DebugPuts("irq224"); for(;;) ;}
void irq225(void){DebugClrScr(0x18); DebugGotoXY(0,0); DebugSetColor(0x1e); DebugPuts("irq225"); for(;;) ;}
void irq226(void){DebugClrScr(0x18); DebugGotoXY(0,0); DebugSetColor(0x1e); DebugPuts("irq226"); for(;;) ;}
void irq227(void){DebugClrScr(0x18); DebugGotoXY(0,0); DebugSetColor(0x1e); DebugPuts("irq227"); for(;;) ;}
void irq228(void){DebugClrScr(0x18); DebugGotoXY(0,0); DebugSetColor(0x1e); DebugPuts("irq228"); for(;;) ;}
void irq229(void){DebugClrScr(0x18); DebugGotoXY(0,0); DebugSetColor(0x1e); DebugPuts("irq229"); for(;;) ;}
void irq230(void){DebugClrScr(0x18); DebugGotoXY(0,0); DebugSetColor(0x1e); DebugPuts("irq230"); for(;;) ;}
void irq231(void){DebugClrScr(0x18); DebugGotoXY(0,0); DebugSetColor(0x1e); DebugPuts("irq231"); for(;;) ;}
void irq232(void){DebugClrScr(0x18); DebugGotoXY(0,0); DebugSetColor(0x1e); DebugPuts("irq232"); for(;;) ;}
void irq233(void){DebugClrScr(0x18); DebugGotoXY(0,0); DebugSetColor(0x1e); DebugPuts("irq233"); for(;;) ;}
void irq234(void){DebugClrScr(0x18); DebugGotoXY(0,0); DebugSetColor(0x1e); DebugPuts("irq234"); for(;;) ;}
void irq235(void){DebugClrScr(0x18); DebugGotoXY(0,0); DebugSetColor(0x1e); DebugPuts("irq235"); for(;;) ;}
void irq236(void){DebugClrScr(0x18); DebugGotoXY(0,0); DebugSetColor(0x1e); DebugPuts("irq236"); for(;;) ;}
void irq237(void){DebugClrScr(0x18); DebugGotoXY(0,0); DebugSetColor(0x1e); DebugPuts("irq237"); for(;;) ;}
void irq238(void){DebugClrScr(0x18); DebugGotoXY(0,0); DebugSetColor(0x1e); DebugPuts("irq238"); for(;;) ;}
void irq239(void){DebugClrScr(0x18); DebugGotoXY(0,0); DebugSetColor(0x1e); DebugPuts("irq239"); for(;;) ;}
void irq240(void){DebugClrScr(0x18); DebugGotoXY(0,0); DebugSetColor(0x1e); DebugPuts("irq240"); for(;;) ;}
void irq241(void){DebugClrScr(0x18); DebugGotoXY(0,0); DebugSetColor(0x1e); DebugPuts("irq241"); for(;;) ;}
void irq242(void){DebugClrScr(0x18); DebugGotoXY(0,0); DebugSetColor(0x1e); DebugPuts("irq242"); for(;;) ;}
void irq243(void){DebugClrScr(0x18); DebugGotoXY(0,0); DebugSetColor(0x1e); DebugPuts("irq243"); for(;;) ;}
void irq244(void){DebugClrScr(0x18); DebugGotoXY(0,0); DebugSetColor(0x1e); DebugPuts("irq244"); for(;;) ;}
void irq245(void){DebugClrScr(0x18); DebugGotoXY(0,0); DebugSetColor(0x1e); DebugPuts("irq245"); for(;;) ;}
void irq246(void){DebugClrScr(0x18); DebugGotoXY(0,0); DebugSetColor(0x1e); DebugPuts("irq246"); for(;;) ;}
void irq247(void){DebugClrScr(0x18); DebugGotoXY(0,0); DebugSetColor(0x1e); DebugPuts("irq247"); for(;;) ;}
void irq248(void){DebugClrScr(0x18); DebugGotoXY(0,0); DebugSetColor(0x1e); DebugPuts("irq248"); for(;;) ;}
void irq249(void){DebugClrScr(0x18); DebugGotoXY(0,0); DebugSetColor(0x1e); DebugPuts("irq249"); for(;;) ;}
void irq250(void){DebugClrScr(0x18); DebugGotoXY(0,0); DebugSetColor(0x1e); DebugPuts("irq250"); for(;;) ;}
void irq251(void){DebugClrScr(0x18); DebugGotoXY(0,0); DebugSetColor(0x1e); DebugPuts("irq251"); for(;;) ;}
void irq252(void){DebugClrScr(0x18); DebugGotoXY(0,0); DebugSetColor(0x1e); DebugPuts("irq252"); for(;;) ;}
void irq253(void){DebugClrScr(0x18); DebugGotoXY(0,0); DebugSetColor(0x1e); DebugPuts("irq253"); for(;;) ;}
void irq254(void){DebugClrScr(0x18); DebugGotoXY(0,0); DebugSetColor(0x1e); DebugPuts("irq254"); for(;;) ;}
void irq255(void){DebugClrScr(0x18); DebugGotoXY(0,0); DebugSetColor(0x1e); DebugPuts("irq255"); for(;;) ;}

int kmain () {

	hal_initialize ();
	
	//! install our exception handlers
	setvect (0,(void (&)(void))divide_by_zero_fault);
	setvect (1,(void (&)(void))single_step_trap);
	setvect (2,(void (&)(void))nmi_trap);
	setvect (3,(void (&)(void))breakpoint_trap);
	setvect (4,(void (&)(void))overflow_trap);
	setvect (5,(void (&)(void))bounds_check_fault);
	setvect (6,(void (&)(void))invalid_opcode_fault);
	setvect (7,(void (&)(void))no_device_fault);
	setvect (8,(void (&)(void))double_fault_abort);
	setvect (10,(void (&)(void))invalid_tss_fault);
	setvect (11,(void (&)(void))no_segment_fault);
	setvect (12,(void (&)(void))stack_fault);
	setvect (13,(void (&)(void))general_protection_fault);
	setvect (14,(void (&)(void))page_fault);
	setvect (16,(void (&)(void))fpu_fault);
	setvect (17,(void (&)(void))alignment_check_fault);
	setvect (18,(void (&)(void))machine_check_abort);
	setvect (19,(void (&)(void))simd_fpu_fault);

		setvect (9,(void (&)(void))irq9);
		setvect (15,(void (&)(void))irq15);
		setvect (16,(void (&)(void))irq16);
		setvect (20,(void (&)(void))irq20);
		setvect (21,(void (&)(void))irq21);
		setvect (22,(void (&)(void))irq22);
		setvect (23,(void (&)(void))irq23);
		setvect (24,(void (&)(void))irq24);
		setvect (25,(void (&)(void))irq25);
		setvect (26,(void (&)(void))irq26);
		setvect (27,(void (&)(void))irq27);
		setvect (28,(void (&)(void))irq28);
		setvect (29,(void (&)(void))irq29);
		setvect (30,(void (&)(void))irq30);
		setvect (31,(void (&)(void))irq31);
		setvect (33,(void (&)(void))irq33);
		setvect (34,(void (&)(void))irq34);
		setvect (35,(void (&)(void))irq35);
		setvect (36,(void (&)(void))irq36);
		setvect (37,(void (&)(void))irq37);
		setvect (38,(void (&)(void))irq38);
		setvect (39,(void (&)(void))irq39);
		setvect (40,(void (&)(void))irq40);
		setvect (41,(void (&)(void))irq41);
		setvect (42,(void (&)(void))irq42);
		setvect (43,(void (&)(void))irq43);
		setvect (44,(void (&)(void))irq44);
		setvect (45,(void (&)(void))irq45);
		setvect (46,(void (&)(void))irq46);
		setvect (47,(void (&)(void))irq47);
		setvect (48,(void (&)(void))irq48);
		setvect (49,(void (&)(void))irq49);
		setvect (50,(void (&)(void))irq50);
		setvect (51,(void (&)(void))irq51);
		setvect (52,(void (&)(void))irq52);
		setvect (53,(void (&)(void))irq53);
		setvect (54,(void (&)(void))irq54);
		setvect (55,(void (&)(void))irq55);
		setvect (56,(void (&)(void))irq56);
		setvect (57,(void (&)(void))irq57);
		setvect (58,(void (&)(void))irq58);
		setvect (59,(void (&)(void))irq59);
		setvect (60,(void (&)(void))irq60);
		setvect (61,(void (&)(void))irq61);
		setvect (62,(void (&)(void))irq62);
		setvect (63,(void (&)(void))irq63);
		setvect (64,(void (&)(void))irq64);
		setvect (65,(void (&)(void))irq65);
		setvect (66,(void (&)(void))irq66);
		setvect (67,(void (&)(void))irq67);
		setvect (68,(void (&)(void))irq68);
		setvect (69,(void (&)(void))irq69);
		setvect (70,(void (&)(void))irq70);
		setvect (71,(void (&)(void))irq71);
		setvect (72,(void (&)(void))irq72);
		setvect (73,(void (&)(void))irq73);
		setvect (74,(void (&)(void))irq74);
		setvect (75,(void (&)(void))irq75);
		setvect (76,(void (&)(void))irq76);
		setvect (77,(void (&)(void))irq77);
		setvect (78,(void (&)(void))irq78);
		setvect (79,(void (&)(void))irq79);
		setvect (80,(void (&)(void))irq80);
		setvect (81,(void (&)(void))irq81);
		setvect (82,(void (&)(void))irq82);
		setvect (83,(void (&)(void))irq83);
		setvect (84,(void (&)(void))irq84);
		setvect (85,(void (&)(void))irq85);
		setvect (86,(void (&)(void))irq86);
		setvect (87,(void (&)(void))irq87);
		setvect (88,(void (&)(void))irq88);
		setvect (89,(void (&)(void))irq89);
		setvect (90,(void (&)(void))irq90);
		setvect (91,(void (&)(void))irq91);
		setvect (92,(void (&)(void))irq92);
		setvect (93,(void (&)(void))irq93);
		setvect (94,(void (&)(void))irq94);
		setvect (95,(void (&)(void))irq95);
		setvect (96,(void (&)(void))irq96);
		setvect (97,(void (&)(void))irq97);
		setvect (98,(void (&)(void))irq98);
		setvect (99,(void (&)(void))irq99);
		setvect (100,(void (&)(void))irq100);
		setvect (101,(void (&)(void))irq101);
		setvect (102,(void (&)(void))irq102);
		setvect (103,(void (&)(void))irq103);
		setvect (104,(void (&)(void))irq104);
		setvect (105,(void (&)(void))irq105);
		setvect (106,(void (&)(void))irq106);
		setvect (107,(void (&)(void))irq107);
		setvect (108,(void (&)(void))irq108);
		setvect (109,(void (&)(void))irq109);
		setvect (110,(void (&)(void))irq110);
		setvect (111,(void (&)(void))irq111);
		setvect (112,(void (&)(void))irq112);
		setvect (113,(void (&)(void))irq113);
		setvect (114,(void (&)(void))irq114);
		setvect (115,(void (&)(void))irq115);
		setvect (116,(void (&)(void))irq116);
		setvect (117,(void (&)(void))irq117);
		setvect (118,(void (&)(void))irq118);
		setvect (119,(void (&)(void))irq119);
		setvect (120,(void (&)(void))irq120);
		setvect (121,(void (&)(void))irq121);
		setvect (122,(void (&)(void))irq122);
		setvect (123,(void (&)(void))irq123);
		setvect (124,(void (&)(void))irq124);
		setvect (125,(void (&)(void))irq125);
		setvect (126,(void (&)(void))irq126);
		setvect (127,(void (&)(void))irq127);
		setvect (128,(void (&)(void))irq128);
		setvect (129,(void (&)(void))irq129);
		setvect (130,(void (&)(void))irq130);
		setvect (131,(void (&)(void))irq131);
		setvect (132,(void (&)(void))irq132);
		setvect (133,(void (&)(void))irq133);
		setvect (134,(void (&)(void))irq134);
		setvect (135,(void (&)(void))irq135);
		setvect (136,(void (&)(void))irq136);
		setvect (137,(void (&)(void))irq137);
		setvect (138,(void (&)(void))irq138);
		setvect (139,(void (&)(void))irq139);
		setvect (140,(void (&)(void))irq140);
		setvect (141,(void (&)(void))irq141);
		setvect (142,(void (&)(void))irq142);
		setvect (143,(void (&)(void))irq143);
		setvect (144,(void (&)(void))irq144);
		setvect (145,(void (&)(void))irq145);
		setvect (146,(void (&)(void))irq146);
		setvect (147,(void (&)(void))irq147);
		setvect (148,(void (&)(void))irq148);
		setvect (149,(void (&)(void))irq149);
		setvect (150,(void (&)(void))irq150);
		setvect (151,(void (&)(void))irq151);
		setvect (152,(void (&)(void))irq152);
		setvect (153,(void (&)(void))irq153);
		setvect (154,(void (&)(void))irq154);
		setvect (155,(void (&)(void))irq155);
		setvect (156,(void (&)(void))irq156);
		setvect (157,(void (&)(void))irq157);
		setvect (158,(void (&)(void))irq158);
		setvect (159,(void (&)(void))irq159);
		setvect (160,(void (&)(void))irq160);
		setvect (161,(void (&)(void))irq161);
		setvect (162,(void (&)(void))irq162);
		setvect (163,(void (&)(void))irq163);
		setvect (164,(void (&)(void))irq164);
		setvect (165,(void (&)(void))irq165);
		setvect (166,(void (&)(void))irq166);
		setvect (167,(void (&)(void))irq167);
		setvect (168,(void (&)(void))irq168);
		setvect (169,(void (&)(void))irq169);
		setvect (170,(void (&)(void))irq170);
		setvect (171,(void (&)(void))irq171);
		setvect (172,(void (&)(void))irq172);
		setvect (173,(void (&)(void))irq173);
		setvect (174,(void (&)(void))irq174);
		setvect (175,(void (&)(void))irq175);
		setvect (176,(void (&)(void))irq176);
		setvect (177,(void (&)(void))irq177);
		setvect (178,(void (&)(void))irq178);
		setvect (179,(void (&)(void))irq179);
		setvect (180,(void (&)(void))irq180);
		setvect (181,(void (&)(void))irq181);
		setvect (182,(void (&)(void))irq182);
		setvect (183,(void (&)(void))irq183);
		setvect (184,(void (&)(void))irq184);
		setvect (185,(void (&)(void))irq185);
		setvect (186,(void (&)(void))irq186);
		setvect (187,(void (&)(void))irq187);
		setvect (188,(void (&)(void))irq188);
		setvect (189,(void (&)(void))irq189);
		setvect (190,(void (&)(void))irq190);
		setvect (191,(void (&)(void))irq191);
		setvect (192,(void (&)(void))irq192);
		setvect (193,(void (&)(void))irq193);
		setvect (194,(void (&)(void))irq194);
		setvect (195,(void (&)(void))irq195);
		setvect (196,(void (&)(void))irq196);
		setvect (197,(void (&)(void))irq197);
		setvect (198,(void (&)(void))irq198);
		setvect (199,(void (&)(void))irq199);
		setvect (200,(void (&)(void))irq200);
		setvect (201,(void (&)(void))irq201);
		setvect (202,(void (&)(void))irq202);
		setvect (203,(void (&)(void))irq203);
		setvect (204,(void (&)(void))irq204);
		setvect (205,(void (&)(void))irq205);
		setvect (206,(void (&)(void))irq206);
		setvect (207,(void (&)(void))irq207);
		setvect (208,(void (&)(void))irq208);
		setvect (209,(void (&)(void))irq209);
		setvect (210,(void (&)(void))irq210);
		setvect (211,(void (&)(void))irq211);
		setvect (212,(void (&)(void))irq212);
		setvect (213,(void (&)(void))irq213);
		setvect (214,(void (&)(void))irq214);
		setvect (215,(void (&)(void))irq215);
		setvect (216,(void (&)(void))irq216);
		setvect (217,(void (&)(void))irq217);
		setvect (218,(void (&)(void))irq218);
		setvect (219,(void (&)(void))irq219);
		setvect (220,(void (&)(void))irq220);
		setvect (221,(void (&)(void))irq221);
		setvect (222,(void (&)(void))irq222);
		setvect (223,(void (&)(void))irq223);
		setvect (224,(void (&)(void))irq224);
		setvect (225,(void (&)(void))irq225);
		setvect (226,(void (&)(void))irq226);
		setvect (227,(void (&)(void))irq227);
		setvect (228,(void (&)(void))irq228);
		setvect (229,(void (&)(void))irq229);
		setvect (230,(void (&)(void))irq230);
		setvect (231,(void (&)(void))irq231);
		setvect (232,(void (&)(void))irq232);
		setvect (233,(void (&)(void))irq233);
		setvect (234,(void (&)(void))irq234);
		setvect (235,(void (&)(void))irq235);
		setvect (236,(void (&)(void))irq236);
		setvect (237,(void (&)(void))irq237);
		setvect (238,(void (&)(void))irq238);
		setvect (239,(void (&)(void))irq239);
		setvect (240,(void (&)(void))irq240);
		setvect (241,(void (&)(void))irq241);
		setvect (242,(void (&)(void))irq242);
		setvect (243,(void (&)(void))irq243);
		setvect (244,(void (&)(void))irq244);
		setvect (245,(void (&)(void))irq245);
		setvect (246,(void (&)(void))irq246);
		setvect (247,(void (&)(void))irq247);
		setvect (248,(void (&)(void))irq248);
		setvect (249,(void (&)(void))irq249);
		setvect (250,(void (&)(void))irq250);
		setvect (251,(void (&)(void))irq251);
		setvect (252,(void (&)(void))irq252);
		setvect (253,(void (&)(void))irq253);
		setvect (254,(void (&)(void))irq254);
		setvect (255,(void (&)(void))irq255);

	DebugClrScr (0x18);

	DebugGotoXY (0,0);
	DebugSetColor (0x70);
	DebugPrintf (" GeexOs Kernel preparing to load...                                             ");
	DebugGotoXY (0,1);
	DebugSetColor (0x19);
	DebugPrintf (" GeexOS Starting Up...\n");
	
	DebugGotoXY(0, 10);
	DebugPrintf(" Processor vendor: ");
	DebugPrintf(get_cpu_vendor());
	
	for(;;) {
		DebugGotoXY (0,14);
		DebugPrintf (" Current tick count: %i", get_tick_count());
	}

	return 0;
}



