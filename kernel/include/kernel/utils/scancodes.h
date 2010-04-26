//http://www.tu-chemnitz.de/informatik/RA/lehre/mop/dos_stuff/scancodes.html

// Unshifted characters
char unshifted[][2] = {
    {0x00,' '},
    {0x01,' '},
    {0x02,'1'},
    {0x03,'2'},
    {0x04,'3'},
    {0x05,'4'},
    {0x06,'5'},
    {0x07,'6'},
    {0x08,'7'},
    {0x09,'8'},
    {0x0A,'9'},
    {0x0B,'0'},
    {0x0C,'�'},
    {0x0D,'�'},
    {0x0E,'\r'},
    {0x0F,'\t'},
    {0x10,'q'},
    {0x11,'w'},
    {0x12,'e'},
    {0x13,'r'},
    {0x14,'t'},
    {0x15,'z'},
    {0x16,'u'},
    {0x17,'i'},
    {0x18,'o'},
    {0x19,'p'},
    {0x1a,'�'},
    {0x1b,'+'},
    {0x1c,'\n'}, //ENTER
    {0x1d,' '}, //CTRL Left
    {0x1e,'a'},
    {0x1f,'s'},
    {0x20,'d'},
    {0x21,'f'},
    {0x22,'g'},
    {0x23,'h'},
    {0x24,'j'},
    {0x25,'k'},
    {0x26,'l'},
    {0x27,'�'},
    {0x28,'�'},
    {0x29,'<'},
    {0x2a,' '}, //SHIFT Left
    {0x2b,'#'},
    {0x2c,'y'},
    {0x2d,'x'},
    {0x2e,'c'},
    {0x2f,'v'},
    {0x30,'b'},
    {0x31,'n'},
    {0x32,'m'},
    {0x33,','},
    {0x34,'.'},
    {0x35,'-'},
    {0x36,' '}, //SHIFT Right
    {0x37,' '}, //PRINT
    {0x38,' '}, //ALT Left
    {0x39,' '},
    {0x3a,' '}, //CAPSLOCK
    {0x3b,' '}, //F1
    {0x3c,' '}, //F2
    {0x3d,' '}, //F3
    {0x3e,' '}, //F4
    {0x3f,' '}, //F5
    {0x40,' '}, //F6
    {0x41,' '}, //F7
    {0x42,' '}, //F8
    {0x43,' '}, //F9
    {0x44,' '}, //F10
    {0x45,' '}, //NUM LOCK
    {0x46,' '}, //Rollen
    {0x47,' '}, //POS1
    {0x48,' '}, //UP
    {0x49,' '}, //PAGE UP
    {0x4a,'-'},
    {0x4b,' '}, //LEFT
    {0x4c,'5'},
    {0x4d,' '}, //RIGHT
    {0x4e,'+'},
    {0x4f,' '}, //END
    {0x50,' '}, //DOWN
    {0x51,' '}, //PAGE DOWN
    {0x52,' '}, //INSERT
    {0x53,' '}, //DELETE
    {0x54,' '}, //???
    //{0xd9,'+'}, //F11
    //{0xda,'+'}, //F12
};

// Shifted characters
char shifted[][2] = {
    {0x00,' '},
    {0x01,' '},
    {0x02,'!'},
    {0x03,'"'},
    {0x04,'�'},
    {0x05,'$'},
    {0x06,'%'},
    {0x07,'&'},
    {0x08,'/'},
    {0x09,'('},
    {0x0A,')'},
    {0x0B,'='},
    {0x0C,'?'},
    {0x0D,'`'},
    {0x0E,'\r'},
    {0x0F,'\t'},
    {0x10,'Q'},
    {0x11,'W'},
    {0x12,'E'},
    {0x13,'R'},
    {0x14,'T'},
    {0x15,'Z'},
    {0x16,'U'},
    {0x17,'I'},
    {0x18,'O'},
    {0x19,'P'},
    {0x1a,'�'},
    {0x1b,'*'},
    {0x1c,'\n'}, //ENTER
    {0x1d,' '}, //CTRL Left
    {0x1e,'A'},
    {0x1f,'S'},
    {0x20,'D'},
    {0x21,'F'},
    {0x22,'G'},
    {0x23,'H'},
    {0x24,'J'},
    {0x25,'K'},
    {0x26,'L'},
    {0x27,'�'},
    {0x28,'�'},
    {0x29,'>'},
    {0x2a,' '}, //SHIFT Left
    {0x2b,'\''},
    {0x2c,'Y'},
    {0x2d,'X'},
    {0x2e,'C'},
    {0x2f,'V'},
    {0x30,'B'},
    {0x31,'N'},
    {0x32,'M'},
    {0x33,';'},
    {0x34,':'},
    {0x35,'_'},
    {0x36,' '}, //SHIFT Right
    {0x37,' '}, //PRINT
    {0x38,' '}, //ALT Left
    {0x39,' '},
    {0x3a,' '}, //CAPSLOCK
    {0x3b,' '}, //F1
    {0x3c,' '}, //F2
    {0x3d,' '}, //F3
    {0x3e,' '}, //F4
    {0x3f,' '}, //F5
    {0x40,' '}, //F6
    {0x41,' '}, //F7
    {0x42,' '}, //F8
    {0x43,' '}, //F9
    {0x44,' '}, //F10
    {0x45,' '}, //NUM LOCK
    {0x46,' '}, //Rollen
    {0x47,'7'},
    {0x48,'8'},
    {0x49,'9'},
    {0x4a,'-'},
    {0x4b,'4'},
    {0x4c,'5'},
    {0x4d,'6'},
    {0x4e,'+'},
    {0x4f,'1'},
    {0x50,'2'},
    {0x51,'3'},
    {0x52,'0'},
    {0x53,','},
    {0x54,' '}, //???
    //{0xd9,'+'}, //F11
    //{0xda,'+'}, //F12
};