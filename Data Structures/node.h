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
};

template <class T>
node<T>::node(T obj)
{
    value = obj;
    left = nullptr;
    right = nullptr;
    vector<T> list = {}; //nothing initially
    height = 1;
}

template <class T>
node<T>::node(T obj, T mappedItem)
{
    value = obj;
    left = nullptr;
    right = nullptr;
    vector<T> list = {mappedItem};
    height = 1;
}
#endif