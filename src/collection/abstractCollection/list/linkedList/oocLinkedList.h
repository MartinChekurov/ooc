/**
 * @file oocLinkedList.h
 * @brief LinkedList implementation for the OOC (Object-Oriented C) framework
 *
 * This header defines the LinkedList class, a doubly-linked list implementation
 * of the List interface. It provides efficient insertion and deletion operations
 * at any position, with O(1) operations at the head and tail.
 */

#ifndef OOC_LINKED_LIST_H_
#define OOC_LINKED_LIST_H_

/**
 * @brief Returns the LinkedList class descriptor
 * @return Pointer to the LinkedList class object
 *
 * Constructor parameters:
 * - (none)
 *
 * Example:
 * @code
 *   void* list = ooc_new(ooc_linkedListClass());
 * @endcode
 */
void* ooc_linkedListClass();

#endif
