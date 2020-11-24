/*
Nathan Obert CSC 232
I have built up this linked list thoughout the labs (and my own time as a form of studying).
*/
#ifndef LINKEDLIST_H
#define LINKEDLIST_H
#include <iostream>
using namespace std;

template <class T>
struct Node
{
    T value;
    Node<T> *next; //points to next value
    Node<T> *prev; //points to previous value
};

template <class T>
class LinkedList
{
private:
    Node<T> *head; //first node
    Node<T> *tail; //last node
    int maxCapacity; //helps institute max capacity, if wanted
    int currentItems; //helps prevent adding items beyond max capacity

public:
    LinkedList(int capacity = -1);    
    LinkedList(const LinkedList &list); //deep copy constructor, but it's easier to use 'list = otherList' for deep copying
    ~LinkedList();

    bool search(T &itemValue) const;
    void push(T itemValue); //adds on to beginning of linked list
    void append(T itemValue); //adds on directly to end of the linked list
    void remove(T itemValue);
    void delFirst(); //deletes the first node
    void delLast(); //deletes the last node
    void peekFirst(T &item) const; //makes argument take head's value
    void peekLast(T &item) const; //makes argument take tail's value
    void displayList() const; //displays from head to tail
    void rDisplayList() const; //displays from tail to head
    bool isFull() const;
    bool isEmpty() const;
    int size() const;
    void clear(); //removes all values in the linked list
    void operator = (const LinkedList &list); //deep copy esque for assignment operator overriding, usful for copy constructors of other classes invloving a linked list member
};

template <class T>
LinkedList<T>::LinkedList(int capacity) //the default argument is taken care of above
{
    head = nullptr;
    tail = nullptr;
    maxCapacity = capacity;
    currentItems = 0; //nothing initially
}

template <class T>
LinkedList<T>::LinkedList(const LinkedList &list)
{
    head = nullptr;
    tail = nullptr;
    currentItems = 0;
    maxCapacity = list.maxCapacity;

    Node<T>* traversalNode = list.head;
    while(traversalNode != nullptr)
    {
        append(traversalNode->value); //adds new node to the current list
        traversalNode = traversalNode->next;
    }
}

template <class T>
LinkedList<T>::~LinkedList()
{
    Node<T> *nodePtr = head; //for traversing the list
    Node<T> *nodeToDelete = nullptr;

    while (nodePtr != nullptr)
    {
        nodeToDelete = nodePtr;
        nodePtr = nodePtr->next; //goes on to next node
        delete nodeToDelete;
    }
    nodeToDelete = nullptr;

    head = nullptr;
    tail = nullptr;
}

template <class T>
bool LinkedList<T>::search(T &itemValue) const
{
    if (!isEmpty())
    {
        Node<T> *traversalNode = head;
        while (traversalNode->value != itemValue)
        {
            traversalNode = traversalNode->next;
            if (!traversalNode) //if reached the end
            {
                //cout << "The key could not be found!" << endl;
                return false;
            }
        }
        itemValue = traversalNode->value; //only will take a diff value if found
        return true;
    }
    else
    {
        //cout << "The key could not be found!" << endl;
        return false;
    }
    
}

template <class T>
void LinkedList<T>::push(T itemValue)
{
    if (isFull())
    {
        cout << "The list is full" << endl;
    }
    else
    {
        Node<T>* newNode = new Node<T>;
        newNode->value = itemValue;
        newNode->prev = nullptr;

        if (isEmpty())
        {
            head = newNode;
            head->next = nullptr;
            tail = newNode; //head and tail are same Node with 1 element
            tail->next = nullptr;
        }
        else
        {
            Node<T>* oldHead = head;
            head = newNode;
            oldHead->prev = head;
            head->next = oldHead;
        }
        currentItems++;
    }
}

template <class T>
void LinkedList<T>::append(T itemValue)
{
    if (isFull())
    {
        cout << "The list is full" << endl;
    }
    else
    {
        Node<T>* newNode = new Node<T>;
        newNode->value = itemValue;
        newNode->next = nullptr;

        if (head == nullptr) //if nothing in list
        {
            head = newNode;
            head->prev = nullptr;
            tail = newNode; //beginning and end point to same node since only one node exists 
            tail->next = nullptr;
            tail->prev = nullptr;
        }
        else //if contents in list exist
        {        
            Node<T>* oldTail = tail;
            tail = newNode;
            newNode->prev = oldTail;
            oldTail->next = tail;
        }
        currentItems++; //added new item, so must keep track of the addition
    }
}

template <class T>
void LinkedList<T>::remove(T itemValue)
{
    if (isEmpty())
    {
        cout << "No values to remove" << endl;
    }
    else
    {
        Node<T> *nodePtr = head;
        while (nodePtr->value != itemValue)
        {
            nodePtr = nodePtr->next;
            if (nodePtr == nullptr)
            {
                cout << "The value was not found" << endl;
                return; //exits function
            }
        }

        if (nodePtr == head)
        {
            delFirst(); //will handle
        }
        else
        {
            Node<T> *before = nodePtr->prev; //moving ptrs to go around node to be deleted
            before->next = nodePtr->next;

            if (nodePtr == tail)
            {
                tail = before;
            }

            delete nodePtr;
            nodePtr = nullptr;
            currentItems--;
        }
    }
}

template <class T>
void LinkedList<T>::delFirst()
{
    if (isEmpty())
    {
        cout << "No values to delete" << endl;
    }
    else
    {
        Node<T>* nodeToDelete = head;
        if (head == tail) //i.e. only one Node
        {
            delete nodeToDelete;
            tail = nullptr;
            head = nullptr;
        }
        else //i.e. more than one node exists
        {
            head = nodeToDelete->next;
            head->prev = nullptr;
            delete nodeToDelete;
        }
        nodeToDelete = nullptr;
        currentItems--; //decrease item count
    }
}

template <class T>
void LinkedList<T>::delLast()
{
    if (isEmpty())
    {
        cout << "No values to delete" << endl;
    }
    else
    {
        Node<T>* nodeToDelete = tail;
        if (nodeToDelete == head) //i.e. only one Node
        {
            delete nodeToDelete;
            tail = nullptr;
            head = nullptr;
        }
        else //i.e. more than one node exists
        {
            tail = nodeToDelete->prev;
            tail->next = nullptr;
            delete nodeToDelete;
        }
        nodeToDelete = nullptr;
        currentItems--; //decrease item count
    }
}

template <class T>
void LinkedList<T>::peekFirst(T &item) const
{
    if (isEmpty())
    {
        cout << "No current values exist" << endl;
    }
    else
    {
        item = head->value;
    }
}

template <class T>
void LinkedList<T>::peekLast(T &item) const
{
    if (isEmpty())
    {
        cout << "No current values exist" << endl;
    }
    else
    {
        item = tail->value;
    }
}

template <class T>
void LinkedList<T>::displayList() const
{
    if (!isEmpty())
    {
        Node<T>* nodePtr = head; //initial location in list
        while (nodePtr != nullptr)
        {
            cout << nodePtr->value << " ";
            nodePtr = nodePtr->next; //traverse forward a node
        }
        cout << endl;
    }
    else
    {
        cout << "The list has no values to display!" << endl;
    }
}

template <class T>
void LinkedList<T>::rDisplayList() const
{
    if (!isEmpty())
    {
        Node<T>* nodePtr = tail; //initial location in list
        while (nodePtr != nullptr)
        {
            cout << nodePtr->value << " ";
            nodePtr = nodePtr->prev; //traverse forward a node
        }
        cout << endl;
    }
    else
    {
        cout << "The list has no values to display!" << endl;
    }
}

template <class T>
bool LinkedList<T>::isFull() const
{
    bool status = (maxCapacity == currentItems) ? true : false;
    return status;
}

template <class T>
bool LinkedList<T>::isEmpty() const
{
    bool status = (currentItems == 0) ? true : false;
    return status;
}

template <class T>
int LinkedList<T>::size() const
{
    return currentItems;
}

template <class T>
void LinkedList<T>::clear()
{
    //will delete each node in the list to prevent memory leaks
    Node<T>* nodePtr = head; //for traversing the list
    Node<T>* nodeToDelete = nullptr;

    while (nodePtr != nullptr)
    {
        nodeToDelete = nodePtr;
        nodePtr = nodePtr->next; //goes on to next node
        delete nodeToDelete;
    }
    nodeToDelete = nullptr;

    //now to reset initial values
    head = nullptr;
    tail = nullptr;
    currentItems = 0;
}

//Used for 'list = otherList;' -- list takes on otherList's values while remaining seperate in memory, like a deep copy constructor
template <class T>
void LinkedList<T>::operator = (const LinkedList &list)
{
    clear(); //clear existing list for futher redefining
    maxCapacity = list.maxCapacity;

    Node<T>* traversalNode = list.head;
    while(traversalNode != nullptr)
    {
        append(traversalNode->value); //adds new node to the current list
        traversalNode = traversalNode->next;
    }
}
#endif