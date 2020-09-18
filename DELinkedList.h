#ifndef LINKED_LIST_H
#define LINKED_LIST_H 

class Node {
public:
   Node(int value, Node* next);

   int value;
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
   unsigned int size() const;

   /**
    * output: Get the value at the given index.
    * input: Index must be >=0 and < size()
    * Version that gives error code.
    * 
    */
   bool get(const unsigned int index, int& returnValue) const;


   /**
    * Add the value to the back of the Linked List
    */
   void addBack(int value);

   /**
    * Add the value to the front of the Linked List
    */
   void addFront(int value);

   /**
    * Remove the value at the back of the Linked List
    */
   void removeBack();

   /**
    * Remove the value at the front of the Linked List
    */
   void removeFront();

   /**
    * Removes all values from the Linked List
    */
   void clear();

private:

   Node* head;
   Node* tail;
};

#endif // LINKED_LIST_H
