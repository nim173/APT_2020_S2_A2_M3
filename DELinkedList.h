#ifndef LINKED_LIST_H
#define LINKED_LIST_H
#include "Types.h"

class Node
{
public:
   Node(Tile value, Node *next);

   Tile value;
   Node *next;
   Node *prev;
};

class LinkedList
{
public:
   LinkedList();
   LinkedList(const LinkedList &other);
   ~LinkedList();

   /**
    * Return the current size of the Linked List.
    */
   int getSize() const;

   /**
    * Add the value to the back of the Linked List
    */
   void addBack(Tile value);

   /**
    * Add the value to the front of the Linked List
    */
   void addFront(Tile value);

   /**
    * Remove the value at the back of the Linked List
    */
   void removeBack();

   /**
    * Remove the value at the front of the Linked List
    */
   Tile removeFront();

   /**
    * Removes all values from the Linked List
    */
   void clear();

private:
   Node *head;
   Node *tail;
   int size;
};



#endif // LINKED_LIST_H
