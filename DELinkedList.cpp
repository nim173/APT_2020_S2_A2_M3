#include "DELinkedList.h"
#include "Types.h"

#include <exception>
#include <iostream>
#include <fstream>
#include <limits>

Node::Node(Tile value, Node *next) : value(value), next(next)
{
}

LinkedList::LinkedList()
{
   head = nullptr;
   tail = nullptr;
   size = 0;
}

LinkedList::LinkedList(const LinkedList &other)
{
   // TODO
}

LinkedList::~LinkedList()
{
   clear();
}

 int LinkedList::getSize() const
{
  return this->size;
}



void LinkedList::addFront(Tile value)
{
   Node *toAdd = new Node(value, nullptr);
   if (head == nullptr)
   {
      head = toAdd;
   }
   else
   {
      head->prev = toAdd; 
   }
   ++size;
}

void LinkedList::addBack(Tile value)
{
   Node *toAdd = new Node(value, nullptr);

   if (tail == nullptr)
   {
      tail = toAdd;
   }
   else
   {
      Node *current = tail;
      while (current->prev != nullptr)
      {
         current = current->prev;
      }

      current->prev = toAdd;
   }
   ++size;
}

void LinkedList::removeBack()
{
     if (tail != nullptr)
   {
      Node* toDelete = head;
      tail = tail->prev;

      if (tail==nullptr)
      {
         head = nullptr;
      } else 
      {
         tail->next = nullptr;
      }

      delete toDelete;
   }
   --size;

}

Tile LinkedList::removeFront()
{
   Tile tile = ' ';
   if (head != nullptr)
   {
      Node* toDelete = head;
      tile = head->value;
      head = head->next;

      if (head==nullptr)
      {
         tail = nullptr;
      } else 
      {
         head->prev = nullptr;
      }
      delete toDelete;
   }
   --size;
   return tile;
}

void LinkedList::clear()
{
   while (head != nullptr)
   {
      removeFront();
   }
}
