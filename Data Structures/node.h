/*
Nathan Obert M03134502
*/
#ifndef NODE_H
#define NODE_H
#include <vector>
using namespace std;

template <class T>
struct node
{
    T value;
    vector<T> list; //where to store each occurence of the value
    int height;
    node *left;
    node *right;

    //constructors
    node(){}
    node(T obj);
    node(T obj, T mappedItem);
    node(T obj, vector<T> mappedItems);
};

template <class T>
node<T>::node(T obj)
{
    value = obj;
    left = nullptr;
    right = nullptr;
    list = {}; //nothing initially
    height = 1;
}

template <class T>
node<T>::node(T obj, T mappedItem)
{
    value = obj;
    left = nullptr;
    right = nullptr;
    list = {mappedItem};
    height = 1;
}

template <class T>
node<T>::node(T obj, vector<T> mappedItems)
{
    value = obj;
    left = nullptr;
    right = nullptr;
    list = mappedItems;
    height = 1;
}
#endif