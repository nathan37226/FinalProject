/*
Nathan Obert M03134502
My AVL Tree implementation
*/
#ifndef AVL_H
#define AVL_H
#include "node.h"
#include <iostream>
#include <algorithm>
#include <string>
#include <vector>
#include <fstream>
using namespace std;

template <class T>
class AVLTree
{
private:
    node<T> *root;
    int nodeCount;

protected:
    void addMidpoints(vector<T> &valueList, int start, int end); //used in creating from file
    void destroySubRoot(node<T> *&subRoot); //used in the destructor
    int height(node<T> *&subRoot) const; //returns height attribute of a given node, or 0 if applicable
    int getNodeBalance(node<T> *subRoot) const; //whether node's subtrees are balanced
    int leafCount(node<T> *subRoot) const;
    void displayInOrder(node<T> *subRoot) const;
    void displayPreOrder(node<T> *subRoot) const;
    void displayPostOrder(node<T> *subRoot) const;
    void displayInOrderWithHeight(node<T> *subRoot) const;
    node<T>* insertHelper(node<T> *&subRoot, node<T> *&newNode); 
    node<T>* searchHelper(T val, node<T> *subRoot) const;
    node<T>* delHelper(node<T> *&subRoot, T value);
    node<T>* rightmost(node<T> *&subRoot); //used in removing; from node to remove, left tree's largest value takes spot
    void performRotations(node<T> *&subRoot); //will perform one of the 4 types of rotations if necessary
    node<T>* rightRotation(node<T> *&x); //returned node is new root of subtree
    node<T>* leftRotation(node<T> *&y);
    void saveHelper(node<T> *traversalNode, ofstream &outFile) const;

public:
    AVLTree();
    AVLTree(string filename);
    ~AVLTree();
    
    bool isBalanced() const;
    int getLeaves() const;
    void display(int option = 1) const;
    void insert(T value);
    void search(T value) const;
    void del(T value);
    void saveInfo(string filename) const;
    int getHeight() const;
    int getNodeCount() const;
};

#include "AVL.cpp" //implementation file
#endif