#include "DELinkedList.h"
#include "Types.h"


Node::Node(Tile value, Node *next) : value(value), next(next)
{
}

LinkedList::LinkedList()
{
   head = nullptr;
   tail = nullptr;
   size = 0;
}

LinkedList::~LinkedList()
{
   clear();
}

int LinkedList::getSize() const
{
   return this->size;
}

// void LinkedList::addFront(Tile value)
// {
//    Node *toAdd = new Node(value, nullptr);
//    if (head == nullptr)
//    {
//       head = toAdd;
//    }
//    else
//    {
//       head->prev = toAdd;
//    }
//    ++size;
// }

void LinkedList::addBack(Tile value)
{
   Node *toAdd = new Node(value, nullptr);

   if (tail == nullptr)
   {
      head = toAdd;
   }
   else
   {
      tail->next = toAdd;
   }
   tail = toAdd;
   ++size;
}

// void LinkedList::removeBack()
// {
//    if (tail != nullptr)
//    {
//       Node *toDelete = head;
//       tail = tail->prev;

//       if (tail == nullptr)
//       {
//          head = nullptr;
//       }
//       else
//       {
//          tail->next = nullptr;
//       }

//       delete toDelete;
//       --size;
//    }
// }

Tile LinkedList::removeFront()
{
   Tile tile = ' ';
   if (head != nullptr)
   {
      Node *toDelete = head;
      tile = head->value;
      if (tail == toDelete) {
         tail = nullptr;
      }
      head = head->next;
      delete toDelete;
      --size;
   }

   return tile;
}

std::string LinkedList::toString()
{
   std::string result;
   Node* current = head;
   while( current != nullptr)
   {
      result += std::string(1, current->value);
      current = current->next;
   }
   return result;
}

void LinkedList::swap(int i, int j) {
   if (i <= size && j <= size) {
      if (j < i) {
         int temp = i;
         i = j;
         j = temp;
      } // to iterate to the smaller value first, swap i and j

      Node *nodeI = nullptr;
      Node *nodeJ = nullptr;
      Node *current = head;
      int currNode = 1;
      while (current != nullptr && currNode <= j+1) {
         if (currNode == i) {
            nodeI = current;
         }
         if (currNode == j) {
            nodeJ = current;
         }
         current = current->next;
         ++currNode;
      }

      // swap tiles
      Tile tempTile = nodeI->value;
      nodeI->value = nodeJ->value;
      nodeJ->value = tempTile;
   }
}

void LinkedList::clear()
{
   Node *current = head;
   while (current != nullptr)
   {
      Node *next = current->next;
      delete current;
      current = next;
   }
   head = nullptr;
   tail = nullptr;
   size = 0;
}
