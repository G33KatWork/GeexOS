#import "TestClass.h"

@implementation TestClass

- (void) setVar:(int)value
{
    var = value;
}

- (int) getVar
{
    return var;
}

- (void) increment
{
    var++;
}

- (void) decrement
{
    var--;
}

- (id)init
{
    return self;
}

@end
