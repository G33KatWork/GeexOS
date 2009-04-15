#ifndef _GLOBAL_H
#define _GLOBAL_H

#include <lib/types.h>
#include <kernel/IO/Monitor.h>

extern class IO::Monitor kout;

#define ASSERT(b)    ((b) ? (void)0 : panic_assert(__FILE__, __LINE__, #b))
#define PANIC(msg)   panic(msg)

#ifdef DEBUG
#define DEBUG_MSG(msg) \
    Color foreground = kout.GetForeground(); \
    Color background = kout.GetBackground(); \
    kout.SetBackground(Black); \
    kout.SetForeground(LightBlue); \
    kout.PrintString("[DEBUG\t] "); \
    kout << msg << endl; \
    kout.SetBackground(background); \
    kout.SetForeground(foreground);
#else
#define DEBUG_MSG(msg)
#endif

extern void panic(const char *message);
extern void panic_assert(const char *file, unsigned int line, const char *desc);

#endif
