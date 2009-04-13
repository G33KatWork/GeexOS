#ifndef _TEST_CLASS_H
#define _TEST_CLASS_H

#include <lib/types.h>

@interface TestClass {
    int var;
}
 
- (void) setVar:(int)value;
- (int) getVar;
- (void) increment;
- (void) decrement;
- (id)init;
@end

#endif
