#ifndef DATASTRUCTURES_LIST_H_
#define DATASTRUCTURES_LIST_H_

#include <types.h>

namespace DataStructures
{
    template<class T>
    class List
    {
    private:
        struct node
        {
            T data;
            node *link;
        }*p;
 
    public:
        List()
        {
            p = NULL;
        }
           
        void append(T data)
        {
            node *q,*t;

            if(p == NULL)
            {
                p = new node;
                p->data = data;
                p->link = NULL;
            }
            else
            {
                q = p;
                while( q->link != NULL )
                    q = q->link;

                t = new node;
                t->data = data;
                t->link = NULL;
                q->link = t;
            }
        }
       
        void addFirst(T data)
        {
            node *q;

            q = new node;
            q->data = data;
            q->link = p;
            p = q;
        }
        
        void addAfter(int element, T data)
        {
            //list head?
            if(element == 0)
            {
                addFirst(data);
                return;
            }
            
            node *q,*t;
            int i;
            
            for(i=0, q=p; i<element; i++)
            {
                q = q->link;
                if(q == NULL) //list end reached?
                {
                    append(data);
                    return;
                }
            }

            t = new node;
            t->data = data;
            t->link = q->link;
            q->link = t;
        }
           
        void remove(T data)
        {
            node *q,*r;
            q = p;
            
            if(q->data == data)
            {
                p = q->link;
                //delete q;
                return;
            }

            r = q;
            while(q != NULL)
            {
                if(q->data == data)
                {
                    r->link = q->link;
                    //delete q;
                    return;
                }

                r = q;
                q = q->link;
            }
        }
        
        /*void removeAt(int element)
        {
            node *q, *t, *u;
            int i;
            
            for(i=0, q=p; i<element - 1; i++)
            {
                q = q->link;
                if(q == NULL || q->link == NULL)
                    return;
            }
            
            t = q->link;
            u = t->link;
            delete t;
            q->link = u;
        }*/
        
        int count()
        {
            node *q;
            int c=0;
            for(q=p; q != NULL; q = q->link)
                c++;

            return c;
        }
        
        T getAt(int element)
        {
            node *q;
            int i;
            
            if(element >= count())
                return NULL;
            
            for(i=0, q=p; i<element; i++)
                q = q->link;
            
            return q->data;
        }
        
        ~List()
        {
            node *q;
            if( p == NULL )
                return;

            while( p != NULL )
            {
                q = p->link;
                delete p;
                p = q;
            }
        }
    };
}

#endif
