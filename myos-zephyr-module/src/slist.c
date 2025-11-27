/*
   Copyright 2013-2023 Marco Bacchi <marco@bacchi.at>
   
   Permission is hereby granted, free of charge, to any person
   obtaining a copy of this software and associated documentation
   files (the "Software"), to deal in the Software without
   restriction, including without limitation the rights to use,
   copy, modify, merge, publish, distribute, sublicense, and/or sell
   copies of the Software, and to permit persons to whom the
   Software is furnished to do so, subject to the following
   conditions:
   
   The above copyright notice and this permission notice shall be
   included in all copies or substantial portions of the Software.
   
   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
   EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
   OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
   NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
   HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
   WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
   FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
   OTHER DEALINGS IN THE SOFTWARE.
*/

/*!
    \file       slist.c

    \brief      Circular singly linked list

    \details    In computer science, a linked list is a linear collection of data elements,
                whose order is not given by their physical placement in memory.
                Instead, each element points to the next. It is a data structure consisting
                of a collection of nodes which together represent a sequence. In its most basic form,
                each node contains: data, and a reference (in other words, a link) to the next node
                in the sequence. This structure allows for efficient insertion or removal of elements
                from any position in the sequence during iteration.
                [https://en.wikipedia.org/wiki/Linked_list]

                Singly linked lists contain nodes which have a data field as well as 'next' field,
                which points to the next node in line of nodes. Operations that can be performed on
                singly linked lists include insertion, deletion and traversal.
                [https://en.wikipedia.org/wiki/Linked_list#Singly_linked_list]

                In the last node of a list, the link field often contains a null reference, a
                special value is used to indicate the lack of further nodes. A less common convention
                is to make it point to the first node of the list; in that case, the list is said to
                be 'circular' or 'circularly linked'; otherwise, it is said to be 'open' or 'linear'.
                It is a list where the last pointer points to the first node.
                [https://en.wikipedia.org/wiki/Linked_list#Circular_linked_list]
*/

#include <stdlib.h>
#include "slist.h"

slist_node_t* slist_find(slist_t *slist, void *node)
{
   slist_node_t* iterator;

   slist_foreach(slist,iterator)
   {
       if ( iterator == node )
       {
           return node;
       }
   }

   return NULL;
}

slist_node_t* slist_back(slist_t* slist)
{
    slist_node_t *iterator = slist_end(slist);

    while( slist_next(slist,iterator) != slist_end(slist) )
    {
        iterator = slist_next(slist,iterator);
    }

    return iterator;
}



slist_node_t* slist_prev_prev(slist_t *slist, void* node)
{
    slist_node_t *iterator;
    slist_foreach(slist,iterator)
    {
        if( slist_next(slist,slist_next(slist,iterator)) == node )
        {
            return iterator;
        }
    }
    return iterator;
}



slist_node_t* slist_prev(slist_t *slist, void* node)
{
    slist_node_t *iterator = node;

    while( slist_next(slist,iterator) != node )
    {
        iterator = slist_next(slist,iterator);
    }

    return iterator;
}


size_t slist_size(slist_t *slist)
{
    slist_node_t *iterator;
    size_t size = 0;

    slist_foreach(slist,iterator)
    {
        size++;
    }

    return size;
}


