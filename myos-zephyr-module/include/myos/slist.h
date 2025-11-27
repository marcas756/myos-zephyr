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
 * @file
 * @copyright  Copyright 2013-2023 Marco Bacchi <marco@bacchi.at>
 * @license    This file is released under the MIT License.\n
 *             https://opensource.org/licenses/MIT
 *
 * @brief      Circular singly linked list
 *
 * @details    In computer science, a linked list is a linear collection of data elements,
 *             whose order is not given by their physical placement in memory.
 *             Instead, each element points to the next. It is a data structure consisting
 *             of a collection of nodes which together represent a sequence. In its most basic form,
 *             each node contains: data, and a reference (in other words, a link) to the next node
 *             in the sequence. This structure allows for efficient insertion or removal of elements
 *             from any position in the sequence during iteration.
 *             More info at https://en.wikipedia.org/wiki/Linked_list
 *
 *             Singly linked lists contain nodes which have a data field as well as 'next' field,
 *             which points to the next node in line of nodes. Operations that can be performed on
 *             singly linked lists include insertion, deletion, and traversal.
 *             More info at https://en.wikipedia.org/wiki/Linked_list#Singly_linked_list
 *
 *             In the last node of a list, the link field often contains a null reference, a
 *             special value used to indicate the lack of further nodes. A less common convention
 *             is to make it point to the first node of the list; in that case, the list is said to
 *             be 'circular' or 'circularly linked'; otherwise, it is said to be 'open' or 'linear'.
 *             It is a list where the last pointer points to the first node.
 *             More info at https://en.wikipedia.org/wiki/Linked_list#Circular_linked_list
 *
 *             Comparison Singly Linked List vs. Arrays:
 *
 *             | Arrays                                      | Singly Linked List                                                             |
 *             | ------------------------------------------- | ------------------------------------------------------------------------------ |
 *             | Arrays are stored in a continuous location. | Linked lists are not stored in contiguous locations.                          |
 *             | Fixed size.                                 | Dynamic in size.                                                               |
 *             | Memory is allocated at compile time.        | Memory is allocated at run time (not necessarily true).                       |
 *             | Uses less memory than linked lists.         | Uses more memory because it stores both data and the address of the next node.|
 *             | Elements can be accessed easily.            | Insertion and deletion operations are faster.                                  |
 *
 *             Nomenclature (applies for slist and dlist):
 *
 *             ![Nomenclature (applies for slist and dlist)](slist/slist_nomenclature.png){html: width=640px, latex: width=5cm}
 *
 *             This file was documented in doxygen style format by using ChatGPT 4.
 *
 */

#ifndef SLIST_H_
#define SLIST_H_

#include <stddef.h>
#include <stdbool.h>


#define SLIST_NODE_TYPE slist_node_t slist_node


/**
 * @brief Macro to obtain the next node in a circular singly linked list.
 *
 * This macro is used to access the next node of a given node in a circular singly linked list.
 * It retrieves the 'next' pointer of the specified node (`node`), which points to the following node in the list.
 * In a circular singly linked list, each node contains a pointer to the next node, forming a circular structure.
 * The head node of the list also acts as a node in the list, pointing to the first data-containing node or to itself if the list is empty.
 *
 * Usage:
 * @code
 * slist_node_t *head = create_list(); // Create and initialize the list
 * slist_node_t *current = head->next; // Start with the first node
 * while (current != head) {           // Iterate until the list loops back to the head
 *     // Process the current node
 *     current = slist_next(head, current);
 * }
 * @endcode
 *
 * @param slist A pointer to the head of the circular singly linked list.
 * @param node A pointer to the current node in the list.
 * @return A pointer to the next node in the list.
 */
#define slist_next(slist,node) \
    (((slist_node_t*)(node))->next)

/**
 * @brief Initializes a circular singly linked list.
 *
 * This macro initializes a circular singly linked list. It sets up the list
 * such that the 'next' pointer of the head node points to the head itself,
 * indicating an empty list. In a circular singly linked list, this condition
 * signifies that there are no other nodes in the list, and the list loops back
 * to the head. This initialization is crucial for the proper functioning of
 * the list, as it establishes the fundamental circular structure of the list.
 *
 * Proper initialization of the list is essential before performing any other
 * operations on the list, as it ensures that the list is in a known and empty state.
 *
 * Usage:
 * @code
 * slist_node_t mylist;
 * slist_init(&mylist);
 * // Now 'mylist' is initialized and ready for operations
 * @endcode
 *
 * @param slist A pointer to the head of the circular singly linked list to be initialized.
 */
#define slist_init(slist) \
    do{slist_next(slist,slist) = (slist);}while(0)

/**
 * @brief Returns an iterator to the beginning of a circular singly linked list.
 *
 * This macro provides an iterator pointing to the first element in a circular
 * singly linked list. Since the head node of the list also serves as a part
 * of the list, this macro returns the 'next' pointer of the head node. If the
 * list is empty (i.e., the head's 'next' pointer points back to the head),
 * this iterator will point to the head node itself, indicating that there are
 * no elements in the list.
 *
 * The `slist_begin` macro is useful for accessing the first element of the
 * list, especially when iterating over the list or performing operations on
 * the elements. It is important to check if the list is empty before
 * dereferencing the returned iterator.
 *
 * Usage:
 * @code
 * slist_node_t mylist;
 * slist_init(&mylist);
 * // Add elements to the list
 * slist_node_t *begin = slist_begin(&mylist);
 * if (begin != &mylist) {
 *     // The list is not empty, process the first element
 * }
 * @endcode
 *
 * @param slist A pointer to the head of the circular singly linked list.
 * @return An iterator (pointer) to the first element in the list, or to the head node itself if the list is empty.
 */
#define slist_begin(slist) ((slist)->next)

/**
 * @brief Retrieves the first element in a circular singly linked list.
 *
 * This macro provides access to the first data-containing node in a circular
 * singly linked list. It utilizes the `slist_begin` macro to obtain an iterator
 * pointing to the first element. If the list is empty, this iterator will point
 * back to the head node itself, which should not be treated as a valid data-containing
 * node.
 *
 * The `slist_front` macro simplifies accessing the first element of the list.
 * Users should verify that the list is not empty before attempting to use the
 * returned iterator to access list data.
 *
 * Usage:
 * @code
 * slist_node_t mylist;
 * slist_init(&mylist);
 * // Add elements to the list
 * slist_node_t *front = slist_front(&mylist);
 * if (front != &mylist) {
 *     // The list is not empty, access the data of the first element
 * }
 * @endcode
 *
 * @param slist A pointer to the head of the circular singly linked list.
 * @return An iterator (pointer) to the first element in the list, or to the head node itself if the list is empty.
 */
#define slist_front(slist) (slist_begin(slist))

/**
 * @brief Returns an iterator to the past-the-end element in a circular singly linked list.
 *
 * This macro provides an iterator referring to the past-the-end element
 * in a circular singly linked list. The past-the-end element is a theoretical
 * element that follows the last element in the list. It is not a valid element
 * and thus shall not be dereferenced. This iterator is commonly used in
 * combination with `slist_begin` to specify a range that includes all the elements
 * in the container.
 *
 * In a circular singly linked list, the past-the-end element is represented by
 * the list node itself. As such, this macro returns the list node, signifying
 * the end of the list. This is particularly useful for loop conditions and
 * iterator comparisons where a sentinel value is needed to represent the end
 * of the list.
 *
 * Usage:
 * @code
 * slist_node_t mylist;
 * slist_init(&mylist);
 * // Add elements to the list
 * for (slist_node_t *it = slist_begin(&mylist); it != slist_end(&mylist); it = slist_next(&mylist, it)) {
 *     // Process each element
 * }
 * @endcode
 *
 * @param slist A pointer to the head of the circular singly linked list.
 * @return An iterator referring to the past-the-end element, which is the list node itself.
 */
#define slist_end(slist) (slist)


/**
 * @brief Iterates through all elements in a circular singly linked list.
 * @details This macro sets up a loop that iterates from the first to the last element
 *          in a circular singly linked list. It initializes the iterator to the first
 *          element in the list using `slist_begin`, and then continues to iterate
 *          over each element until it reaches the end of the list, indicated by
 *          `slist_end`. This macro is useful for performing operations on each
 *          element in the list, such as searching, modifying, or printing values.
 *
 *          The macro uses a for loop to traverse the list, making it easy to
 *          apply a block of code to each element. It is important to ensure
 *          that the list is properly initialized before using this macro to avoid
 *          undefined behavior.
 *
 * Usage:
 * @code
 * slist_node_t mylist;
 * slist_init(&mylist);
 * // Add elements to the list
 * slist_node_t *iterator;
 * slist_foreach(&mylist, iterator) {
 *     // Process each element pointed to by iterator
 * }
 * @endcode
 *
 * @param slist A pointer to the head of the circular singly linked list to be iterated over.
 * @param iterator A pointer that will be used as an iterator over the list.
 */
#define slist_foreach(slist,iterator)                   \
    for(iterator = (void*)slist_begin(slist);           \
        ((slist_node_t*)iterator) != slist_end(slist);  \
        iterator=(void*)slist_next(slist,iterator))


/**
 * @brief Tests whether a circular singly linked list is empty.
 * @details This macro checks if a circular singly linked list is empty by
 *          comparing the 'next' pointer of the head node (`slist`) to the
 *          address of the head node itself. In an empty circular singly linked
 *          list, the 'next' pointer of the head node points to the head node,
 *          indicating that there are no other nodes in the list. This quick
 *          check allows for determining if the list contains any elements
 *          without traversing the list.
 *
 *          It is particularly useful for checking the state of the list
 *          before performing operations that require the list to contain
 *          elements.
 *
 * Usage:
 * @code
 * slist_node_t mylist;
 * slist_init(&mylist);
 * if (slist_empty(&mylist)) {
 *     // The list is empty
 * } else {
 *     // The list contains elements
 * }
 * @endcode
 *
 * @param slist A pointer to the head of the circular singly linked list.
 * @return An integer: 0 if the list is not empty, or 1 if it is empty.
 */
#define slist_empty(slist) ((slist)->next==(slist))



/**
 * @brief Inserts an element at the beginning of a circular singly linked list.
 * @details This macro inserts a new node (`node_to_add`) at the beginning of
 *          the circular singly linked list, right after the head node. The insertion
 *          is achieved by setting the 'next' pointer of `node_to_add` to the
 *          current first element of the list, and then updating the 'next' pointer
 *          of the head node (`slist`) to point to `node_to_add`. After the insertion,
 *          `node_to_add` becomes the new first element in the list.
 *
 *          This operation is efficient as it does not require traversing the list
 *          and directly modifies the necessary pointers to insert the new node.
 *          It is important to ensure that `node_to_add` is properly allocated and
 *          initialized before using this macro.
 *
 * Usage:
 * @code
 * slist_node_t mylist;
 * slist_init(&mylist);
 * slist_node_t *new_node = create_node(data); // Assume create_node allocates and initializes a new node
 * slist_push_front(&mylist, new_node);
 * // Now 'new_node' is the first element of 'mylist'
 * @endcode
 *
 * @param slist A pointer to the head of the circular singly linked list.
 * @param node_to_add A pointer to the node to be added to the beginning of the list.
 */
#define slist_push_front(slist,node_to_add) \
    do{ \
        ((slist_node_t*)(node_to_add))->next = (slist)->next; \
        (slist)->next=((slist_node_t*)(node_to_add)); \
    }while(0);

/**
 * @brief Removes the first element from a circular singly linked list.
 * @details This macro removes the first element in the circular singly linked
 *          list. It does this by setting the 'next' pointer of the head node
 *          (`slist`) to point to the second node in the list, effectively
 *          removing the first node from the list. This operation is efficient
 *          as it requires only a modification of the head node's 'next' pointer.
 *
 *          It is important to ensure that the list is not empty before calling
 *          this macro. If the list is empty (i.e., `slist->next` is `slist`),
 *          calling `slist_pop_front` will have no effect. Users should also
 *          handle memory management for the removed node if it was dynamically
 *          allocated. This macro only removes the node from the list structure;
 *          it does not free the memory associated with the node.
 *
 * Usage:
 * @code
 * slist_node_t mylist;
 * slist_init(&mylist);
 * // Add elements to the list
 * if (!slist_empty(&mylist)) {
 *     slist_pop_front(&mylist);
 *     // The first node is now removed from 'mylist'
 * }
 * @endcode
 *
 * @param slist A pointer to the head of the circular singly linked list.
 */
#define slist_pop_front(slist) \
    do{(slist)->next = (slist)->next->next;}while(0)


/**
 * @brief Inserts an element at the end of a circular singly linked list.
 * @details This macro inserts a new node (`node_to_add`) at the end of the circular
 *          singly linked list. It first finds the current last node in the list
 *          using `slist_back(slist)`, which traverses the entire list. Once the last
 *          node is identified, the macro sets the 'next' pointer of this node to
 *          `node_to_add`, and then adjusts the 'next' pointer of `node_to_add` to
 *          point to the head of the list (`slist`), maintaining the circular nature
 *          of the list.
 *
 *          This operation requires traversing the entire list to find the last node,
 *          which has a linear time complexity with respect to the number of elements
 *          in the list. Users should ensure that `node_to_add` is properly allocated
 *          and initialized before using this macro.
 *
 * Usage:
 * @code
 * slist_node_t mylist;
 * slist_init(&mylist);
 * slist_node_t *new_node = create_node(data); // Assume create_node allocates and initializes a new node
 * slist_push_back(&mylist, new_node);
 * // Now 'new_node' is the last element of 'mylist'
 * @endcode
 *
 * @param slist A pointer to the head of the circular singly linked list.
 * @param node_to_add A pointer to the node to be added to the end of the list.
 */
#define slist_push_back(slist,node_to_add) \
    do { \
        ((slist_node_t*)(node_to_add))->next = slist_back(slist); \
        ((slist_node_t*)(node_to_add))->next->next = ((slist_node_t*)(node_to_add)); \
        ((slist_node_t*)(node_to_add))->next = slist_end(slist);\
    }while(0)




/**
 * @brief Inserts a node after a specified position in a circular singly linked list.
 * @details This macro inserts a new node (`nodeptr`) into the circular singly
 *          linked list immediately after a specified node (`posptr`). The insertion
 *          is achieved by adjusting the 'next' pointer of `nodeptr` to point to
 *          the node that follows `posptr`, and then updating the 'next' pointer
 *          of `posptr` to point to `nodeptr`. This operation effectively places
 *          `nodeptr` into the list directly after `posptr`.
 *
 *          The operation is efficient as it only involves modifying the pointers
 *          of `posptr` and `nodeptr`, without the need to traverse the entire list.
 *          It is crucial that both `posptr` and `nodeptr` are valid pointers to nodes
 *          and that `posptr` is an existing member of the list for the macro to function
 *          correctly. Improper usage may lead to undefined behavior.
 *
 * Usage:
 * @code
 * slist_node_t mylist;
 * slist_init(&mylist);
 * slist_node_t *position_node = find_node(mylist, some_criteria); // Assume find_node returns a valid node
 * slist_node_t *new_node = create_node(data); // Assume create_node allocates and initializes a new node
 * slist_insert_after(&mylist, position_node, new_node);
 * // Now 'new_node' is inserted immediately after 'position_node' in 'mylist'
 * @endcode
 *
 * @param slistptr A pointer to the head of the circular singly linked list.
 * @param posptr A pointer to the node after which the new node will be inserted.
 * @param nodeptr A pointer to the new node to be inserted into the list.
 */
#define slist_insert_after(slistptr,posptr,nodeptr) \
    do{ \
        ((slist_node_t*)(nodeptr))->next = ((slist_node_t*)(posptr))->next; \
        ((slist_node_t*)(posptr))->next = ((slist_node_t*)(nodeptr)); \
    }while(0)



/**
 * @brief Inserts a node before a specified position in a circular singly linked list.
 * @details This macro inserts a new node (`nodeptr`) into the circular singly
 *          linked list immediately before a specified node (`posptr`). The insertion
 *          is achieved by adjusting the 'next' pointer of the node that precedes
 *          `posptr`, found using `slist_prev(slist, posptr)`, to point to `nodeptr`,
 *          and then setting the 'next' pointer of `nodeptr` to `posptr`. This
 *          operation effectively places `nodeptr` into the list directly before
 *          `posptr`.
 *
 *          The operation requires finding the previous node, which involves a partial
 *          traversal of the list up to `posptr`. This macro assumes that both `posptr`
 *          and `nodeptr` are valid pointers to nodes and that `posptr` is an existing
 *          member of the list. Improper usage may lead to undefined behavior.
 *
 * Usage:
 * @code
 * slist_node_t mylist;
 * slist_init(&mylist);
 * slist_node_t *position_node = find_node(mylist, some_criteria); // Assume find_node returns a valid node
 * slist_node_t *new_node = create_node(data); // Assume create_node allocates and initializes a new node
 * slist_insert_before(&mylist, position_node, new_node);
 * // Now 'new_node' is inserted immediately before 'position_node' in 'mylist'
 * @endcode
 *
 * @param slist A pointer to the head of the circular singly linked list.
 * @param posptr A pointer to the node before which the new node will be inserted.
 * @param nodeptr A pointer to the new node to be inserted into the list.
 */
#define slist_insert_before(slist,posptr,nodeptr) \
    do{ \
        slist_prev(slist,posptr)->next = (slist_node_t*)(nodeptr); \
        ((slist_node_t*)nodeptr))->next = posptr; \
    }while(0)



/**
 * @brief Clears a circular singly linked list.
 * @details This macro clears the contents of a circular singly linked list.
 *          It reinitializes the list by setting the 'next' pointer of the
 *          head node to point to itself, effectively marking the list as empty.
 *          This operation resets the list to its initial state, with no nodes
 *          linked to the head.
 *
 *          It is important to note that this macro only resets the list structure;
 *          it does not free the memory associated with the nodes in the list. If
 *          the nodes were dynamically allocated, the user should ensure that they
 *          are properly freed before clearing the list to avoid memory leaks.
 *
 * Usage:
 * @code
 * slist_node_t mylist;
 * slist_init(&mylist);
 * // Add elements and perform operations on the list
 * slist_clear(&mylist);
 * // Now 'mylist' is cleared and reset to its initial state
 * @endcode
 *
 * @param slistptr A pointer to the circular singly linked list to be cleared.
 */
#define  slist_clear(slistptr) \
    slist_init(slistptr)

/**
 * @brief Removes a specific node from a circular singly linked list.
 * @details This macro removes a specified node (`nodeptr`) from the circular
 *          singly linked list. It accomplishes this by finding the node that
 *          precedes `nodeptr` in the list using `slist_prev(slistptr, nodeptr)`
 *          and then adjusting its 'next' pointer to bypass `nodeptr`, pointing
 *          instead to the node that follows `nodeptr`. This operation effectively
 *          removes `nodeptr` from the list.
 *
 *          The operation requires finding the previous node, which involves a
 *          partial traversal of the list up to `nodeptr`. It is important to
 *          ensure that `nodeptr` is a valid and existing member of the list
 *          before using this macro. Improper usage may lead to undefined behavior.
 *
 *          Note that this macro only removes `nodeptr` from the list structure;
 *          it does not free the memory associated with `nodeptr`. If `nodeptr`
 *          was dynamically allocated, users are responsible for managing its memory.
 *
 * Usage:
 * @code
 * slist_node_t mylist;
 * slist_init(&mylist);
 * // Add elements to the list
 * slist_node_t *node_to_remove = find_node(mylist, some_criteria); // Assume find_node returns a valid node
 * if (node_to_remove) {
 *     slist_erase(&mylist, node_to_remove);
 *     // 'node_to_remove' is now removed from 'mylist'
 *     // Free node_to_remove if necessary
 * }
 * @endcode
 *
 * @param slistptr A pointer to the head of the circular singly linked list.
 * @param nodeptr A pointer to the node to be removed from the list.
 */
#define slist_erase(slistptr,nodeptr)                                               \
    do{                                                                             \
        slist_prev(slistptr,nodeptr)->next = ((slist_node_t*)(nodeptr))->next;      \
    }while(0)

/*!
 * @typedef slist_node_t
 * @brief Typedef for the structure representing a node in a circular singly linked list.
 */
typedef struct slist_node_t slist_node_t;

 /*!
  * @struct slist_node_t
  * @brief Structure representing a node in a circular singly linked list.
  *
  * @details This structure represents a single node in a circular singly linked list.
  *          Each node contains a pointer to the next node in the list, forming a
  *          circular structure. The circular nature allows traversal of the list
  *          from any node, and the last node links back to the head of the list.
  *
  * @var slist_node_t::next
  * Pointer to the next node in the list. In the last node of the list, this pointer
  * points back to the head of the list, completing the circular structure.
  */
struct slist_node_t {
    slist_node_t *next;     /*!< Pointer to the next node in the list. */
};

/*!
* @typedef slist_t
* @brief Typedef for the circular singly linked list.
* @details This typedef is used to define a type for the circular singly linked list.
*          It establishes `slist_node_t` as the type for the head of the list. In a circular
*          singly linked list defined by this typedef, the head node itself is a member of the list,
*          pointing to the first data-containing node or back to itself if the list is empty.
*          This setup simplifies certain list operations by providing a consistent starting point
*          for list traversal and manipulation.
*
* Usage:
* @code
* slist_t mylist; // Declare a list
* slist_init(&mylist); // Initialize the list
* // The list can now be used for various operations
* @endcode
*/
typedef slist_node_t slist_t; /*!< List instance is also a member of the list */


/**
 * @brief Finds the previous node in a circular singly linked list.
 * @details This function searches for the node that precedes a specified node (`node`)
 *          in a circular singly linked list. Since the list is singly linked, the function
 *          traverses the list starting from the specified node and continues until it loops
 *          back to the same node. The node immediately before this loop-back is the predecessor
 *          of the given node.
 *
 *          This traversal is necessary due to the singly linked nature of the list, as there
 *          are no direct pointers to previous nodes. The function is useful for operations
 *          that require knowledge of a node's predecessor, such as certain insertion or deletion
 *          operations.
 *
 * @note The function assumes that both the list and the node are properly initialized. If
 *       the node is not part of the list, the behavior is undefined. This function has a linear
 *       time complexity with respect to the number of elements in the list.
 *
 * Usage:
 * @code
 * slist_t mylist;
 * slist_init(&mylist);
 * // Add elements to the list
 * slist_node_t *node = find_node(mylist, some_criteria); // Assume find_node returns a valid node
 * slist_node_t *prev_node = slist_prev(&mylist, node);
 * if (prev_node) {
 *     // 'prev_node' is the predecessor of 'node'
 * }
 * @endcode
 *
 * @param slist A pointer to the head of the circular singly linked list.
 * @param node A pointer to the node whose predecessor is to be found.
 * @return A pointer to the predecessor of the specified node, or NULL if the node is the head of the list.
 */
slist_node_t* slist_prev(slist_t *slist, void *existing_node);

/**
 * @brief Counts the number of nodes in a circular singly linked list.
 * @details This function calculates and returns the number of elements (nodes)
 *          present in a circular singly linked list, excluding the head node itself.
 *          It traverses the list starting from the first element after the head
 *          node and counts each node until it reaches the head node again.
 *
 *          The traversal makes use of the `slist_foreach` macro, which iterates
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
 * slist_t mylist;
 * slist_init(&mylist);
 * // Add elements to the list
 * size_t list_size = slist_size(&mylist);
 * printf("List contains %zu elements\n", list_size);
 * @endcode
 *
 * @param slist A pointer to the head of the circular singly linked list.
 * @return The number of elements in the list, excluding the head node itself.
 */
size_t slist_size(slist_t *slist);

/**
 * @brief Returns the last node in a circular singly linked list.
 * @details This function searches for the last node in a circular singly linked list.
 *          It starts from the end of the list, which is the head node itself (represented by `slist_end(slist)`),
 *          and iterates through the list until it finds a node whose 'next' pointer points back to the head node.
 *          This node is identified as the last node in the list.
 *
 *          This approach is necessary because the list is singly linked and does not have direct pointers to
 *          previous nodes. The function is useful for operations that require access to the last element of the list,
 *          such as appending elements or performing certain modifications.
 *
 * @note The function assumes that the list is properly initialized. If the list is empty,
 *       the function will return the head node itself. The function has linear time complexity,
 *       as it may need to traverse the entire list to find the last node.
 *
 * Usage:
 * @code
 * slist_t mylist;
 * slist_init(&mylist);
 * // Add elements to the list
 * slist_node_t *last_node = slist_back(&mylist);
 * if (last_node != &mylist) {
 *     // The list is not empty, and 'last_node' is the last node in the list
 * }
 * @endcode
 *
 * @param slist A pointer to the head of the circular singly linked list.
 * @return A pointer to the last node in the list, or the head node itself if the list is empty.
 */
slist_node_t* slist_back(slist_t* slist);

/**
 * @brief Finds the node two steps behind a specified node in a circular singly linked list.
 * @details This function searches for the node that is two positions before a specified node (`node`)
 *          in a circular singly linked list. It traverses the list, checking for the node whose 'next'
 *          node's 'next' pointer equals the specified node. If the specified node is at the beginning
 *          of the list, or the list does not contain enough elements, the function returns the list
 *          node itself, which is a well-defined behavior for this list structure.
 *
 *          This function is useful in operations where the node two steps behind a given node needs
 *          to be identified, such as certain deletion operations. Due to the singly linked nature of
 *          the list, the function iterates through the list to find the appropriate node.
 *
 * @note This function has a linear time complexity with respect to the number of elements in the list.
 *       It is important to ensure that the list is properly initialized and the node is a part of the list.
 *
 * Usage:
 * @code
 * slist_t mylist;
 * slist_init(&mylist);
 * // Add elements to the list
 * slist_node_t *node = find_node(mylist, some_criteria); // Assume find_node returns a valid node
 * slist_node_t *prev_prev_node = slist_prev_prev(&mylist, node);
 * // 'prev_prev_node' is two nodes behind 'node', or the list node itself if not enough elements
 * @endcode
 *
 * @param slist A pointer to the head of the circular singly linked list.
 * @param node A pointer to the node whose two-step predecessor is to be found.
 * @return A pointer to the node two positions before the specified node, or the list node itself if such a node does not exist.
 */
slist_node_t* slist_prev_prev(slist_t *slist, void* node);


/**
 * @brief Removes the last element from a circular singly linked list.
 * @details This macro removes the last element in the circular singly linked
 *          list. It locates the node that is two positions behind the head node
 *          (which is effectively the last node) using `slist_prev_prev(slist, slist)`,
 *          and then sets its 'next' pointer to point to the head node (`slist`).
 *          This operation removes the last node from the list.
 *
 *          Since this operation requires finding the second-to-last node, it
 *          involves traversing almost the entire list, which makes it less
 *          efficient, particularly for longer lists. The macro assumes that
 *          the list contains at least one element other than the head node.
 *          Users should check that the list is not empty before performing
 *          this operation to avoid undefined behavior.
 *
 * Usage:
 * @code
 * slist_t mylist;
 * slist_init(&mylist);
 * // Add elements to the list
 * if (!slist_empty(&mylist)) {
 *     slist_pop_back(&mylist);
 *     // The last node is now removed from 'mylist'
 * }
 * @endcode
 *
 * @param slist A pointer to the head of the circular singly linked list.
 */
#define slist_pop_back(slist)                            \
    do{                                                  \
        slist_prev_prev(slist,slist)->next = (slist);    \
    }while(0)


/**
 * @brief Searches for a specific node in a circular singly linked list.
 * @details This function iterates through the circular singly linked list to find
 *          a specified node (`node`). The search is conducted by comparing each node's
 *          address in the list with the address of `node`. If the specified node is
 *          found in the list, the function returns a pointer to that node; otherwise,
 *          it returns NULL.
 *
 *          The search uses the `slist_foreach` macro to traverse each node in the
 *          list. This function is useful for verifying whether a specific node is
 *          present in the list and for operations that depend on locating a particular
 *          node within the list.
 *
 * @note The function runs in linear time complexity with respect to the number of
 *       elements in the list. It is important to ensure that the list is properly
 *       initialized before using this function.
 *
 * Usage:
 * @code
 * slist_t mylist;
 * slist_init(&mylist);
 * // Add elements to the list
 * slist_node_t *search_node = create_node(data); // Assume create_node allocates and initializes a new node
 * slist_push_back(&mylist, search_node);
 * // Now search for the node
 * slist_node_t *found_node = slist_find(&mylist, search_node);
 * if (found_node) {
 *     printf("Node found in the list\n");
 * } else {
 *     printf("Node not found in the list\n");
 * }
 * @endcode
 *
 * @param slist A pointer to the head of the circular singly linked list.
 * @param node The node to search for in the list.
 * @return A pointer to the node if it exists in the list, NULL otherwise.
 */
slist_node_t* slist_find(slist_t *slist, void *node);

#endif /* SLIST_H_ */
