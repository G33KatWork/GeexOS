#ifndef DATASTRUCTURES_DOUBLYLINKEDLIST_H_
#define DATASTRUCTURES_DOUBLYLINKEDLIST_H_

#include <types.h>
#include <kernel/global.h>

namespace DataStructures
{
    struct ListItem
    {
        struct ListItem *next, *previous;
    };
    
    class DoublyLinkedList
    {
    private:
        struct ListItem listHead;
        
        static void addInternal(struct ListItem *newItem, struct ListItem *previous, struct ListItem *next)
        {
            next->previous = newItem;
        	newItem->next = next;
        	newItem->previous = previous;
        	previous->next = newItem;
        }
    
    public:
        DoublyLinkedList()
        {
            listHead.next = &listHead;
            listHead.previous = &listHead;
        }
        
        ~DoublyLinkedList(){}
        
        void AddInFront(struct ListItem* newItem)
        {
            addInternal(newItem, &listHead, listHead.next);
        }
        
        void AddToTail(struct ListItem* newItem)
        {
            addInternal(newItem, listHead.previous, &listHead);
        }
        
        void Remove(struct ListItem* toRemove)
        {
            toRemove->next->previous = toRemove->previous;
            toRemove->previous->next = toRemove->next;
            toRemove->next = NULL;
            toRemove->previous = NULL;
        }
        
        bool IsEmpty()
        {
            return listHead.next == &listHead;
        }
    };
}

#endif
