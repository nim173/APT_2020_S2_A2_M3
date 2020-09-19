
#include "DELinkedList.h"

#include <exception>
#include <iostream>
#include <fstream>
#include <limits>

Node::Node(char value, Node *next) : value(value), next(next)
{
}

LinkedList::LinkedList()
{
   head = nullptr;
   tail = nullptr;
}

LinkedList::LinkedList(const LinkedList &other)
{
   // TODO
}

LinkedList::~LinkedList()
{
   clear();
}

 unsigned char LinkedList::size() const
{
   int count = 0;
   Node *current = head;
   while (current != nullptr)
   {
      ++count;
      current = current->next;
   }

   return count;
}


void LinkedList::addFront(char value)
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
}

void LinkedList::addBack(char value)
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

}

char LinkedList::removeFront()
{
   char tile = ' ';
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
   return tile;
}

void LinkedList::clear()
{
   while (head != nullptr)
   {
      removeFront();
   }
}
