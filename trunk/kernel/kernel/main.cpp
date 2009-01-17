/*
=========================================================================
	main.cpp

	main kernel program
=========================================================================
*/

#include <hal.h>
#include <bootinfo.h>
#include "DebugDisplay.h"
#include "exception.h"
#include "paging.h"#include "kheap.h"
#include "fs.h"
#include "initrd.h"
#include "assert.h"

#include "debugIrqHandler.h"

extern unsigned placement_address;

void setupDebugHandler();

int kmain (multiboot_info* bootinfo) {
	hal_initialize ();

	ASSERT(bootinfo->m_modsCount > 0);
	unsigned initrd_location = *((unsigned*)bootinfo->m_modsAddr);
	unsigned initrd_end = *(unsigned*)(bootinfo->m_modsAddr+4);
	// Don't trample our module with placement accesses, please!
	placement_address = initrd_end;
	
	DebugClrScr (0x18);
	DebugSetColor (0x19);
	paging_initialize();
	
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
	//setvect (14,(void (&)(void))page_fault);
	setvect (16,(void (&)(void))fpu_fault);
	setvect (17,(void (&)(void))alignment_check_fault);
	setvect (18,(void (&)(void))machine_check_abort);
	setvect (19,(void (&)(void))simd_fpu_fault);
	setupDebugHandler();
	
	uint32_t memSize = bootinfo->m_memoryLo + bootinfo->m_memoryHi;
	uint32_t multibootFlags = bootinfo->m_flags;

	/*DebugGotoXY (0,0);
	DebugSetColor (0x70);
	DebugPrintf (" GeexOs Kernel preparing to load...                                             ");
	DebugGotoXY (0,1);
	DebugSetColor (0x19);
	DebugPrintf (" GeexOS Starting Up...\n");
	DebugPrintf (" Installed memory: %iKB\n", memSize);
	DebugPrintf (" Multiboot flags: %x\n", multibootFlags);
	DebugPrintf (" Kernel commandline: %s\n", (const char*)(bootinfo->m_cmdLine));
	DebugPrintf (" Processor vendor: %s\n\n", get_cpu_vendor());*/

	fs_root = initialise_initrd(initrd_location);

	// list the contents of /
    /*unsigned i = 0;
    struct dirent *node = 0;
    while ( (node = readdir_fs(fs_root, i)) != 0)
    {
        DebugPrintf("Found file %s", node->name);
        fs_node_t *fsnode = finddir_fs(fs_root, node->name);

        if ((fsnode->flags&0x7) == FS_DIRECTORY)
        {
            DebugPrintf("\n\t(directory)\n");
        }
        else
        {
            DebugPrintf("\n\t contents: \"");
            char buf[256];
            unsigned sz = read_fs(fsnode, 0, 256, buf);
            unsigned j;
            for (j = 0; j < sz; j++)
                DebugPutc(buf[j]);
            
            DebugPrintf("\"\n");
        }
        i++;
    }*/

	for(;;) {
		unsigned bla = kmalloc(1);
		DebugGotoXY (50,10);
		DebugPrintf("%x", bla);
	}

	return 0;
}

void setupDebugHandler() {
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
}

