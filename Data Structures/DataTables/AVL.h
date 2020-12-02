/*
Nathan Obert M03134502
This AVL Tree implementation functions as a dictionary. The node has inside a vector, which means
the same key can be inserted with different associated values, and this vector will hold all of the
values. For the bank, this means the name "Nathan" can be inserted and the associated values can be
all of the accounts with the first name "Nathan" on it. 

While this tree is of a template type, not all functions will work as such. For the purposes of this
project, this tree will only ever be of type string. The saving and building functions are heavily
related to the objects being of type string, and other types may cause it to break.
*/
#ifndef AVL_H
#define AVL_H
#include "node.h"
#include <iostream>
#include <algorithm>
#include <string>
#include <vector>
#include <fstream>
#include <cmath>
#include "EncryptionBox.h"
using namespace std;

template <class T>
class AVLTree
{
private:
    node<T> *root;
    int nodeCount;
    int mappedItemCount;

protected:
    void addMidpoints(vector<node<T>*> &nodeList, int start, int end); //used in creating from file
    void destroySubRoot(node<T> *&subRoot); //used in the destructor
    int height(node<T> *&subRoot) const; //returns height attribute of a given node, or 0 if applicable
    int getNodeBalance(node<T> *subRoot) const; //whether node's subtrees are balanced
    int leafCount(node<T> *subRoot) const;
    void displayInOrder(node<T> *subRoot) const;
    void displayInOrderWithAdmins(node<T> *subRoot) const;
    void displayInOrderWithOfficials(node<T> *subRoot) const;
    void displayInOrderWithHeight(node<T> *subRoot) const;
    void displayInOrderWithMappedItems(node<T> *subRoot) const;
    node<T>* insertHelper(node<T> *&subRoot, node<T> *&newNode); 
    node<T>* insertWithItemHelper(node<T> *&subRoot, node<T> *&newNode);
    node<T>* searchHelper(T val, node<T> *subRoot) const;
    node<T>* delHelper(node<T> *&subRoot, T value);
    node<T>* rightmost(node<T> *&subRoot); //used in removing; from node to remove, left tree's largest value takes spot
    node<T>* delMappedItemHelper(node<T> *&subRoot, T value, T mappedItem);
    void performRotations(node<T> *&subRoot); //will perform one of the 4 types of rotations if necessary
    node<T>* rightRotation(node<T> *&x); //returned node is new root of subtree
    node<T>* leftRotation(node<T> *&y);
    void saveHelper(node<T> *traversalNode, ofstream &outFile) const;
    node<T>* buildHelper(string entries);

public:
    AVLTree();
    ~AVLTree();
    
    bool isBalanced() const;
    int getLeaves() const;
    void display(int option = 5) const;
    void insert(T value); //inserts the node
    void insertWithItem(T value, T mappedItem); //inserts a node with an item associated with the value. Like, phoneNum -> acct
    vector<T> returnMappedItems(T value) const;
    void insertWithList(T value, vector<T> mappedItems);
    void search(T value) const;
    void del(T value);
    void delMappedItem(T value, T mappedItem); //deletes a specific item out of list in node. If nothing left after, deletes node.
    void saveInfo(string filename) const;
    void buildTree(string filename);
    int getHeight() const;
    int getNodeCount() const;
    void getStatistics() const;
};

#include "AVL.cpp" //implementation file
#endif
