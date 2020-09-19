#ifndef LINKED_LIST_H
#define LINKED_LIST_H 

class Node {
public:
   Node(char value, Node* next);

   char value;
   Node* next;
   Node* prev;
};

class LinkedList {
public:
   LinkedList();
   LinkedList(const LinkedList& other);
   ~LinkedList();

   /**
    * Return the current size of the Linked List.
    */
   unsigned  char size() const;


   /**
    * Add the value to the back of the Linked List
    */
   void addBack(char value);

   /**
    * Add the value to the front of the Linked List
    */
   void addFront(char value);

   /**
    * Remove the value at the back of the Linked List
    */
   void removeBack();

   /**
    * Remove the value at the front of the Linked List
    */
   char removeFront();

   /**
    * Removes all values from the Linked List
    */
   void clear();

private:

   Node* head;
   Node* tail;
};

#endif // LINKED_LIST_H
