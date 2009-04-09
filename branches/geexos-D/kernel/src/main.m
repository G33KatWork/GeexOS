//#import "TestClass.h"

int gp;

int main(void)
{   
    char* video = (char*)0xC00B8000;
    for(int i = 0; i < 24*80*2; i++)
        video[i] = 0;
 
    //TestClass* c = [TestClass init];
        
    return 0; 
}
