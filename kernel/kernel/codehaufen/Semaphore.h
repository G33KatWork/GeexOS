#ifndef _SEMAPHORE_H_
#define _SEMAPHORE_H_

typedef int semaphore;

namespace Processes
{
    class Semaphore
    {
        public:
            static void up(semaphore* sem);
            static int down(semaphore* sem);
    };
}

#endif