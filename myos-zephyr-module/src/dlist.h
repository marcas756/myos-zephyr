/*! \copyright
 
   https://opensource.org/license/mit/

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
 * @file
 * @copyright  Copyright 2013-2023 Marco Bacchi <marco@bacchi.at>
 * @license    This file is released under the MIT License.\n
 *             https://opensource.org/licenses/MIT
 *
 * @brief      Circular doubly linked list
 *
 * @details    In computer science, a linked list is a linear collection of data elements,
 *             whose order is not given by their physical placement in memory.
 *             Instead, each element points to the next. It is a data structure consisting
 *             of a collection of nodes which together represent a sequence. In its most basic form,
 *             each node contains: data, and a reference (in other words, a link) to the next node
 *             in the sequence. This structure allows for efficient insertion or removal of elements
 *             from any position in the sequence during iteration.
 *             [https://en.wikipedia.org/wiki/Linked_list]
 *
 *             In a 'doubly linked list', each node contains, besides the next-node link, a second link field
 *             pointing to the 'previous' node in the sequence. The two links may be called 'forward('s') and
 *             'backwards', or 'next' and 'prev'('previous').
 *             [https://en.wikipedia.org/wiki/Linked_list#Doubly_linked_list]
 *
 *             In the last node of a list, the link field often contains a null reference, a
 *             special value is used to indicate the lack of further nodes. A less common convention
 *             is to make it point to the first node of the list; in that case, the list is said to
 *             be 'circular' or 'circularly linked'; otherwise, it is said to be 'open' or 'linear'.
 *             It is a list where the last pointer points to the first node.
 *             [https://en.wikipedia.org/wiki/Linked_list#Circular_linked_list]
 *
 *             Comparison Singly Linked List vs. Doubly Linked List:
 *
 *             | Singly Linked List                                             | Doubly Linked List                                                   |
 *             | -------------------------------------------------------------- | -------------------------------------------------------------------- |
 *             | Linked lists are not stored in contiguous location.            | Linked lists are not stored in contiguous location.                  |
 *             | Dynamic in size.                                               | Dynamic in size.                                                     |
 *             | Memory is allocated at run time (not necessarily true).        | Memory is allocated at run time (not necessarily true).              |
 *             | Uses less memory than a doubly linked list.                    | Uses more memory as it also stores the address of the previous node. |
 *             | Element accessing requires the traversal of whole linked list. | Element accessing requires the traversal of whole linked list.       |
 *             | Insertion and deletion operation is slower.                    | Insertion and deletion operation is faster.                          |
 *
 *
 *             This file was documented in doxygen style format by using ChatGPT 4.
 *
 *
 */

#ifndef DLIST_H_
#define DLIST_H_

#include <stddef.h>

/**
 * @brief Macro to embed a list node in a structure for a circular doubly linked list.
 *
 * Add `DLIST_NODE_TYPE` as the first member of a structure to make it suitable
 * for use as a node in a circular doubly linked list. This macro defines an embedded
 * `dlist_node_t` structure, allowing any structure that includes it to be linked
 * into a circular doubly linked list. The embedded `dlist_node_t` structure is named
 * `dlist_node`, and it contains the necessary pointers to the next and previous nodes in the list.
 *
 * This approach allows for the creation of nodes that can store additional data
 * while still being compatible with circular doubly linked list operations.
 *
 * Usage:
 * @code
 * typedef struct {
 *     DLIST_NODE_TYPE; // Embeds the list node
 *     int data;        // Additional data field
 * } custom_node_t;
 * @endcode
 *
 * With this definition, `custom_node_t` can be used as a node in a circular doubly linked list,
 * and additional data can be associated with each node.
 */
#define DLIST_NODE_TYPE dlist_node_t dlist_node


/*!
 * @brief Retrieves the next node in a circular doubly linked list.
 * @details This macro provides an efficient way to access the successor of
 *          a given node (`existing_node`) in a circular doubly linked list.
 *          It returns the node that immediately follows the specified node
 *          in the list. This is particularly useful for iterating over a
 *          circular doubly linked list, where each node is connected to both
 *          its predecessor and successor.
 *
 *          The macro assumes that `existing_node` is not NULL and is a valid
 *          member of the list. If `existing_node` is the last node in the list,
 *          `dlist_next` returns the list node itself, as the next node in the
 *          circular structure.
 *
 * Usage:
 * @code
 * dlist_node_t *current = get_some_node(dlist);
 * dlist_node_t *next_node = dlist_next(dlist, current);
 * // Process next_node as needed
 * @endcode
 *
 * @param dlist A pointer to the circular doubly linked list.
 * @param existing_node A pointer to the current node in the list.
 * @return A pointer to the next node in the list, which could be the list node itself.
 */
#define dlist_next(dlist,existing_node) \
    (((dlist_node_t*)(existing_node))->next)

/*!
 * @brief Retrieves the previous node in a circular doubly linked list.
 * @details This macro is designed to access the predecessor of a given node
 *          (`existing_node`) in a circular doubly linked list. It returns the
 *          node that immediately precedes the specified node in the list. This
 *          macro is particularly useful for reverse iteration over a circular
 *          doubly linked list, where each node is linked to both its next and
 *          previous nodes.
 *
 *          The macro assumes that `existing_node` is not NULL and is a valid
 *          member of the list. If `existing_node` is the first node in the list
 *          (after the list node), `dlist_prev` returns the list node itself,
 *          which is the predecessor in the circular structure of the list.
 *
 * Usage:
 * @code
 * dlist_node_t *current = get_some_node(dlist);
 * dlist_node_t *previous_node = dlist_prev(dlist, current);
 * // Process previous_node as needed
 * @endcode
 *
 * @param dlist A pointer to the circular doubly linked list.
 * @param existing_node A pointer to the current node in the list.
 * @return A pointer to the previous node in the list, which could be the list node itself.
 */
#define dlist_prev(dlist,existing_node) \
    (((dlist_node_t*)(existing_node))->prev)

/*!
 * @brief Initializes a circular doubly linked list.
 * @details This macro is used to initialize a circular doubly linked list, setting
 *          it up as an empty list. In an empty list, both the 'next' and 'prev'
 *          pointers of the list node point to the list node itself, creating a
 *          circular structure. Initializing the list in this way is crucial, as
 *          it ensures that the list is in a known state, preventing undefined
 *          behavior when performing other operations on the list.
 *
 *          Proper initialization of the list is essential before using any other
 *          list functions or function-like macros, as they rely on the list being
 *          correctly initialized to a circular structure.
 *
 * Usage:
 * @code
 * dlist_t mylist;
 * dlist_init(&mylist);
 * // Now 'mylist' is initialized and ready for operations
 * @endcode
 *
 * @param dlist A pointer to the circular doubly linked list to be initialized.
 */
#define dlist_init(dlist) \
    do{ \
        (dlist)->next = (dlist); \
        (dlist)->prev = (dlist); \
    }while(0)

/*!
 * @brief Returns an iterator to the beginning of the list.
 * @details This macro provides an iterator pointing to the first element in
 *          a circular doubly linked list. It returns the node immediately following
 *          the list node, which is considered the first element of the list. If the
 *          list is empty, this iterator will point back to the list node itself.
 *
 *          It is important to note that if the list is empty, the returned iterator
 *          should not be dereferenced, as it would point to the list node instead of
 *          a valid data node. Users should check whether the list is empty (e.g., using
 *          `dlist_empty`) before dereferencing the iterator.
 *
 * Usage:
 * @code
 * dlist_t mylist;
 * dlist_init(&mylist);
 * // Add elements to the list
 * dlist_node_t *begin = dlist_begin(&mylist);
 * if (begin != &mylist) {
 *     // The list is not empty, process the first element
 * }
 * @endcode
 *
 * @param dlist A pointer to the circular doubly linked list.
 * @return An iterator (pointer) to the first node in the list, or to the list node itself if the list is empty.
 */
#define dlist_begin(dlist) ((dlist)->next)

/*!
 * @brief Retrieves the first element in a circular doubly linked list.
 * @details This macro provides access to the first data-containing node in
 *          a circular doubly linked list. It utilizes the `dlist_begin` macro
 *          to obtain an iterator pointing to the first element. If the list is
 *          empty, it returns an iterator pointing to the list node itself,
 *          which should not be dereferenced.
 *
 *          The `dlist_front` macro is useful for accessing the first element
 *          of the list in a direct and concise way. However, users should
 *          verify that the list is not empty before attempting to use the
 *          returned iterator to access list data.
 *
 * Usage:
 * @code
 * dlist_t mylist;
 * dlist_init(&mylist);
 * // Add elements to the list
 * dlist_node_t *front = dlist_front(&mylist);
 * if (front != &mylist) {
 *     // The list is not empty, access the data of the first element
 * }
 * @endcode
 *
 * @param dlist A pointer to the circular doubly linked list.
 * @return An iterator (pointer) to the first element in the list, or to the list node itself if the list is empty.
 */
#define dlist_front(dlist) (dlist_begin(dlist))

/*!
 * @brief Returns an iterator to the past-the-end element in a circular doubly linked list.
 * @details This macro provides an iterator referring to the past-the-end element
 *          in a circular doubly linked list. The past-the-end element is a
 *          theoretical element that follows the last element in the list. It is
 *          not a valid element and thus shall not be dereferenced. This iterator
 *          is commonly used in combination with `dlist_begin` to specify a range
 *          that includes all the elements in the container.
 *
 *          In a circular doubly linked list, the successor of the last element
 *          is the list node itself, signifying the end of the list. Therefore,
 *          this macro returns the list node to represent the past-the-end element.
 *          It is particularly useful for loop conditions and iterator comparisons
 *          where a sentinel value is needed to represent the end of the list.
 *
 * Usage:
 * @code
 * dlist_t mylist;
 * dlist_init(&mylist);
 * // Add elements to the list
 * for (dlist_node_t *it = dlist_begin(&mylist); it != dlist_end(&mylist); it = dlist_next(&mylist, it)) {
 *     // Process each element
 * }
 * @endcode
 *
 * @param dlist A pointer to the circular doubly linked list.
 * @return An iterator referring to the past-the-end element, which is the list node itself.
 */
#define dlist_end(dlist) (dlist)

/*!
 * @brief Returns the last node in a circular doubly linked list.
 * @details This macro provides an iterator to the last node in a circular
 *          doubly linked list. It uses the `dlist_prev` macro to find the node
 *          that precedes the list node itself (`dlist`). In a circular doubly
 *          linked list, the 'prev' pointer of the list node points to the last
 *          element, making this an efficient method to access the last node.
 *
 *          This is particularly useful for operations that require access to
 *          the end of the list, such as appending elements or performing
 *          operations in reverse order. It is important to note that if the
 *          list is empty, the macro will return the list node itself, which
 *          should not be treated as a valid data-containing node.
 *
 * Usage:
 * @code
 * dlist_t mylist;
 * dlist_init(&mylist);
 * // Add elements to the list
 * dlist_node_t *last_node = dlist_back(&mylist);
 * if (last_node != &mylist) {
 *     // The list is not empty, process the last node
 * }
 * @endcode
 *
 * @param dlist A pointer to the circular doubly linked list.
 * @return An iterator to the last node in the list, or the list node itself if the list is empty.
 */
#define dlist_back(dlist) (dlist_prev(dlist,dlist))


/*!
 * @brief Iterates through all elements in a circular doubly linked list.
 * @details This macro sets up a loop that iterates from the first to the last element
 *          in a circular doubly linked list. It initializes the iterator to the first
 *          element in the list using `dlist_begin`, and then continues to iterate
 *          over each element until it reaches the end of the list, indicated by
 *          `dlist_end`. This macro is useful for performing operations on each
 *          element in the list, such as searching, modifying, or printing values.
 *
 *          The macro uses a for loop to traverse the list, making it easy to
 *          apply a block of code to each element. It is important to ensure
 *          that the list is properly initialized before using this macro to avoid
 *          undefined behavior.
 *
 * Usage:
 * @code
 * dlist_t mylist;
 * dlist_init(&mylist);
 * // Add elements to the list
 * dlist_node_t *iterator;
 * dlist_foreach(&mylist, iterator) {
 *     // Process each element pointed to by iterator
 * }
 * @endcode
 *
 * @param dlist A pointer to the circular doubly linked list to be iterated over.
 * @param iterator A pointer that will be used as an iterator over the list.
 */
#define dlist_foreach(dlist,iterator) \
    for(iterator = (void*)dlist_begin(dlist); ((dlist_node_t*)iterator) != dlist_end(dlist); iterator=(void*)dlist_next(dlist,iterator))


/*!
 * @brief Tests whether a circular doubly linked list is empty.
 * @details This macro checks if a circular doubly linked list is empty by
 *          comparing the 'next' pointer of the list node (`dlist`) to the
 *          address of the list node itself. In an empty circular doubly linked
 *          list, the 'next' and 'prev' pointers of the list node point to the
 *          list node itself. If the 'next' pointer points to the list node,
 *          the list is empty. Otherwise, it contains at least one element.
 *
 *          This is a quick and efficient way to determine if the list has any
 *          elements without traversing the entire list. It is particularly
 *          useful for checking list state before performing operations that
 *          require the list to have elements.
 *
 * Usage:
 * @code
 * dlist_t mylist;
 * dlist_init(&mylist);
 * // Operations that may modify the list
 * if (dlist_empty(&mylist)) {
 *     // The list is empty, handle accordingly
 * } else {
 *     // The list is not empty, proceed with further operations
 * }
 * @endcode
 *
 * @param dlist A pointer to the circular doubly linked list.
 * @return An integer: 0 if the list is not empty, or 1 if it is empty.
 */
#define dlist_empty(dlist) ((dlist)->next==(dlist))


/*!
 * @brief Inserts an element at the beginning of a circular doubly linked list.
 * @details This macro inserts a new node (`node_to_add`) at the beginning of
 *          the circular doubly linked list, right before its current first element.
 *          The insertion is done by adjusting the 'next' and 'prev' pointers of
 *          the node to add, as well as the 'next' pointer of the list node and
 *          the 'prev' pointer of the original first element of the list.
 *
 *          This operation is efficient as it does not require traversing the list
 *          and directly modifies the necessary pointers to insert the new node.
 *          After insertion, the new node becomes the first element of the list.
 *
 *          It is important to ensure that `node_to_add` is properly allocated and
 *          initialized before using this macro. Additionally, the list must be
 *          correctly initialized for this macro to function as expected.
 *
 * Usage:
 * @code
 * dlist_t mylist;
 * dlist_init(&mylist);
 * dlist_node_t *new_node = create_node(data); // Assume create_node allocates and initializes a new node
 * dlist_push_front(&mylist, new_node);
 * // Now 'new_node' is the first element of 'mylist'
 * @endcode
 *
 * @param dlist A pointer to the circular doubly linked list.
 * @param node_to_add A pointer to the node to be added to the beginning of the list.
 */
#define dlist_push_front(dlist,node_to_add) \
    do{ \
        ((dlist_node_t*)(node_to_add))->next = (dlist)->next; \
        ((dlist_node_t*)(node_to_add))->prev = (dlist); \
        (dlist)->next->prev = ((dlist_node_t*)(node_to_add)); \
        (dlist)->next = ((dlist_node_t*)(node_to_add)); \
    }while(0);

/*!
 * @brief Deletes the first node in a circular doubly linked list.
 * @details This macro removes the first node of the circular doubly linked
 *          list. It does this by setting the 'next' pointer of the list node
 *          (`dlist`) to point to the second node in the list, and then updating
 *          the 'prev' pointer of the new first node to point back to the list node.
 *          This effectively removes the original first node from the list.
 *
 *          The operation is efficient as it directly manipulates the pointers
 *          without needing to traverse the list. However, it is important to
 *          ensure that the list is not empty before calling this macro. If the
 *          list is empty (i.e., `dlist->next` is `dlist`), calling `dlist_pop_front`
 *          will have no effect.
 *
 *          Users should also handle memory management for the removed node if
 *          it was dynamically allocated. This macro only removes the node from
 *          the list structure; it does not free the memory associated with the node.
 *
 * Usage:
 * @code
 * dlist_t mylist;
 * dlist_init(&mylist);
 * // Add elements to the list
 * if (!dlist_empty(&mylist)) {
 *     dlist_pop_front(&mylist);
 *     // The first node is now removed from 'mylist'
 * }
 * @endcode
 *
 * @param dlist A pointer to the circular doubly linked list.
 */
#define dlist_pop_front(dlist) \
    do{ \
        (dlist)->next = (dlist)->next->next; \
        (dlist)->next->prev = (dlist); \
    }while(0);



/*!
 * @brief Inserts an element at the end of a circular doubly linked list.
 * @details This macro inserts a new node (`node_to_add`) at the end of
 *          the circular doubly linked list, right after the current last node.
 *          The insertion is achieved by adjusting the 'next' and 'prev' pointers
 *          of the new node, as well as the 'prev' pointer of the list node and
 *          the 'next' pointer of the original last node in the list.
 *
 *          This operation is efficient as it directly manipulates the pointers
 *          without needing to traverse the list. After insertion, the new node
 *          becomes the last element of the list. It is important to ensure
 *          that `node_to_add` is properly allocated and initialized before using
 *          this macro. Additionally, the list must be correctly initialized
 *          for this macro to function as expected.
 *
 * Usage:
 * @code
 * dlist_t mylist;
 * dlist_init(&mylist);
 * dlist_node_t *new_node = create_node(data); // Assume create_node allocates and initializes a new node
 * dlist_push_back(&mylist, new_node);
 * // Now 'new_node' is the last element of 'mylist'
 * @endcode
 *
 * @param dlist A pointer to the circular doubly linked list.
 * @param node_to_add A pointer to the node to be added to the end of the list.
 */
#define dlist_push_back(dlist, node_to_add) \
    do { \
        ((dlist_node_t*)(node_to_add))->next = (dlist); \
        ((dlist_node_t*)(node_to_add))->prev = (dlist)->prev; \
        (dlist)->prev->next = ((dlist_node_t*)(node_to_add)); \
        (dlist)->prev = ((dlist_node_t*)(node_to_add)); \
    }while(0)



/*!
 * @brief Removes the last element from a circular doubly linked list.
 * @details This macro removes the last element in the circular doubly linked
 *          list, effectively reducing the container's size by one. The removal
 *          is done by adjusting the 'prev' pointer of the list node (`dlist`)
 *          to point to the second-to-last node, and then setting the 'next'
 *          pointer of this new last node to point back to the list node.
 *          This effectively removes the original last node from the list.
 *
 *          The operation is efficient as it directly manipulates the pointers
 *          without needing to traverse the entire list. However, it is important
 *          to ensure that the list is not empty before calling this macro. If
 *          the list is empty (i.e., `dlist->prev` is `dlist`), calling
 *          `dlist_pop_back` will have no effect.
 *
 *          Users should also handle memory management for the removed node if
 *          it was dynamically allocated. This macro only removes the node from
 *          the list structure; it does not free the memory associated with the node.
 *
 * Usage:
 * @code
 * dlist_t mylist;
 * dlist_init(&mylist);
 * // Add elements to the list
 * if (!dlist_empty(&mylist)) {
 *     dlist_pop_back(&mylist);
 *     // The last node is now removed from 'mylist'
 * }
 * @endcode
 *
 * @param dlist A pointer to the circular doubly linked list.
 */
#define dlist_pop_back(dlist) \
    do { \
        (dlist)->prev = (dlist)->prev->prev; \
        (dlist)->prev->next = (dlist); \
    }while(0)


/*!
 * @brief Removes a specific node from a circular doubly linked list.
 * @details This macro removes a specific node (`existing_node`) from the
 *          circular doubly linked list. It achieves this by adjusting the 'next'
 *          pointer of the node's predecessor and the 'prev' pointer of the node's
 *          successor to bypass `existing_node`, effectively removing it from
 *          the list.
 *
 *          This operation is efficient as it directly manipulates the pointers
 *          of the neighboring nodes, and does not require traversing the list.
 *          It is crucial to ensure that `existing_node` is indeed a member of
 *          the list; removing a node that is not part of the list will result
 *          in undefined behavior.
 *
 *          Users should also handle memory management for the removed node if
 *          it was dynamically allocated. This macro only removes the node from
 *          the list structure; it does not free the memory associated with the node.
 *
 * Usage:
 * @code
 * dlist_t mylist;
 * dlist_init(&mylist);
 * // Add elements to the list
 * dlist_node_t *node_to_remove = find_node(mylist, some_criteria); // Assume find_node returns a valid node
 * if (node_to_remove) {
 *     dlist_erase(&mylist, node_to_remove);
 *     // Now 'node_to_remove' is removed from 'mylist'
 *     // Free node_to_remove if necessary
 * }
 * @endcode
 *
 * @param dlist A pointer to the circular doubly linked list.
 * @param existing_node A pointer to the node to be removed from the list.
 */
#define dlist_erase(dlist, existing_node) \
    do{ \
        ((dlist_node_t*)existing_node)->prev->next = ((dlist_node_t*)existing_node)->next; \
        ((dlist_node_t*)existing_node)->next->prev = ((dlist_node_t*)existing_node)->prev; \
    } while(0)


/*!
 * @brief Inserts an element after a specified node in a circular doubly linked list.
 * @details This macro inserts a new node (`node_to_insert`) into the circular
 *          doubly linked list immediately after a specified node (`existing_node`).
 *          The insertion is achieved by adjusting the 'next' and 'prev' pointers
 *          of `node_to_insert`, as well as the 'next' pointer of `existing_node`
 *          and the 'prev' pointer of the original successor of `existing_node`.
 *
 *          This operation allows for efficient insertion in the list without
 *          needing to traverse it. It is crucial that both `existing_node` and
 *          `node_to_insert` are valid pointers to nodes properly allocated.
 *          The macro assumes that `existing_node` is a part of the list and
 *          `node_to_insert` is not yet in the list. Improper usage may lead
 *          to undefined behavior.
 *
 *          Users should ensure that `node_to_insert` is correctly initialized
 *          before using this macro, as it only handles the linking of the node
 *          into the list structure.
 *
 * Usage:
 * @code
 * dlist_t mylist;
 * dlist_init(&mylist);
 * dlist_node_t *existing_node = find_node(mylist, some_criteria); // Assume find_node returns a valid node
 * dlist_node_t *new_node = create_node(data); // Assume create_node allocates and initializes a new node
 * dlist_insert_after(&mylist, existing_node, new_node);
 * // Now 'new_node' is inserted after 'existing_node' in 'mylist'
 * @endcode
 *
 * @param dlist A pointer to the circular doubly linked list.
 * @param existing_node A pointer to the node in the list after which the new node will be inserted.
 * @param node_to_insert A pointer to the new node to be inserted into the list.
 */
#define dlist_insert_after(dlist, existing_node, node_to_insert) \
    do{ \
        ((dlist_node_t*)node_to_insert)->next = ((dlist_node_t*)existing_node)->next; \
        ((dlist_node_t*)node_to_insert)->prev = existing_node; \
        ((dlist_node_t*)existing_node)->next->prev = node_to_insert; \
        ((dlist_node_t*)existing_node)->next = node_to_insert; \
    } while(0)

/*!
 * @brief Inserts an element before a specified node in a circular doubly linked list.
 * @details This macro inserts a new node (`node_to_insert`) into the circular
 *          doubly linked list immediately before a specified node (`existing_node`).
 *          The insertion is achieved by adjusting the 'next' and 'prev' pointers
 *          of `node_to_insert`, as well as the 'prev' pointer of `existing_node`
 *          and the 'next' pointer of the original predecessor of `existing_node`.
 *
 *          This operation allows for efficient insertion in the list without
 *          needing to traverse it. It is crucial that both `existing_node` and
 *          `node_to_insert` are valid pointers to nodes properly allocated.
 *          The macro assumes that `existing_node` is a part of the list and
 *          `node_to_insert` is not yet in the list. Improper usage may lead
 *          to undefined behavior.
 *
 *          Users should ensure that `node_to_insert` is correctly initialized
 *          before using this macro, as it only handles the linking of the node
 *          into the list structure.
 *
 * Usage:
 * @code
 * dlist_t mylist;
 * dlist_init(&mylist);
 * dlist_node_t *existing_node = find_node(mylist, some_criteria); // Assume find_node returns a valid node
 * dlist_node_t *new_node = create_node(data); // Assume create_node allocates and initializes a new node
 * dlist_insert_before(&mylist, existing_node, new_node);
 * // Now 'new_node' is inserted before 'existing_node' in 'mylist'
 * @endcode
 *
 * @param dlist A pointer to the circular doubly linked list.
 * @param existing_node A pointer to the node in the list before which the new node will be inserted.
 * @param node_to_insert A pointer to the new node to be inserted into the list.
 */
#define dlist_insert_before(dlist, existing_node, node_to_insert) \
    do{ \
        ((dlist_node_t*)node_to_insert)->next = existing_node; \
        ((dlist_node_t*)node_to_insert)->prev = ((dlist_node_t*)existing_node)->prev; \
        ((dlist_node_t*)existing_node)->prev->next = node_to_insert; \
        ((dlist_node_t*)existing_node)->prev = node_to_insert; \
    } while(0)

/*!
 * @brief Typedef for the structure representing a node in a circular doubly linked list.
 */
typedef struct dlist_node_t dlist_node_t;

 /*!
  * @struct dlist_node_t
  * @brief Structure representing a node in a circular doubly linked list.
  *
  * @details This structure represents a single node in a circular doubly linked list.
  *          Each node contains pointers to the next and previous nodes in the list,
  *          allowing for bidirectional traversal. This structure is fundamental for
  *          the implementation of a circular doubly linked list.
  *
  * @var dlist_node_t::next
  * Pointer to the next node in the list.
  *
  * @var dlist_node_t::prev
  * Pointer to the previous node in the list.
  */
 struct dlist_node_t {
     dlist_node_t *next; /*!< Pointer to the next node in the list */
     dlist_node_t *prev; /*!< Pointer to the previous node in the list */
 };

 /*!
  * @brief Typedef for the circular doubly linked list.
  * @details This typedef is used to create an instance of a circular doubly linked list.
  *          It defines a list node type (`dlist_node_t`) which also acts as the head
  *          of the list. The list instance itself is a member of the list, simplifying
  *          list operations by eliminating special cases for the head of the list.
  */
typedef dlist_node_t dlist_t; /*!< List instance is also a member of the list */

/*!
 * @brief Counts the number of nodes in a circular doubly linked list.
 * @details This function calculates and returns the number of elements (nodes)
 *          present in a circular doubly linked list, excluding the list node
 *          itself. It traverses the list starting from the first element after
 *          the list node and counts each node until it reaches the list node again.
 *
 *          This traversal makes use of the `dlist_foreach` macro, which iterates
 *          over each node in the list. The function is useful for determining
 *          the size of the list for various operations such as allocation,
 *          serialization, or applying algorithms that depend on the number of
 *          elements.
 *
 * @note The function assumes that the list is properly initialized. An uninitialized
 *       or improperly formed list might lead to undefined behavior. The function
 *       runs in linear time complexity, as it traverses each element of the list.
 *
 * Usage:
 * @code
 * dlist_t mylist;
 * dlist_init(&mylist);
 * // Add elements to the list
 * size_t list_size = dlist_size(&mylist);
 * printf("List contains %zu elements\n", list_size);
 * @endcode
 *
 * @param dlist A pointer to the circular doubly linked list.
 * @return The number of elements in the list, excluding the list node itself.
 */
size_t dlist_size(dlist_t *dlist);


/*!
 * @brief Checks if a node is in the circular doubly linked list.
 * @details This function iterates through the circular doubly linked list
 *          and checks if a specific node (`node`) is present in the list.
 *          It uses a linear search, comparing each node's address with
 *          the address of the specified node. If the node is found, the
 *          function returns a pointer to that node; otherwise, it returns NULL.
 *
 *          The search is performed using the `dlist_foreach` macro, which
 *          iterates over each node in the list. This function is useful for
 *          verifying the presence of a node in the list before performing
 *          operations that depend on its existence in the list.
 *
 * @note The function assumes that the list is properly initialized and the
 *       `node` pointer is valid. The function runs in linear time complexity,
 *       as it may need to traverse the entire list to find the node or
 *       determine its absence.
 *
 * Usage:
 * @code
 * dlist_t mylist;
 * dlist_init(&mylist);
 * // Add elements to the list
 * dlist_node_t *search_node = create_node(data); // Assume create_node allocates and initializes a new node
 * dlist_push_back(&mylist, search_node);
 * // Now search for the node
 * dlist_node_t *found_node = dlist_find(&mylist, search_node);
 * if (found_node) {
 *     printf("Node found in the list\n");
 * } else {
 *     printf("Node not found in the list\n");
 * }
 * @endcode
 *
 * @param dlist A pointer to the circular doubly linked list.
 * @param node The node to search for in the list.
 * @return A pointer to the node if it exists in the list, NULL otherwise.
 */
dlist_node_t* dlist_find(dlist_t* dlist, void* node);

#endif /* DLIST_H_ */



