/*
Nathan Obert M03134502
*/
#ifndef NODE_H
#define NODE_H
using namespace std;

template <class T>
struct node
{
    T value;
    node *left;
    node *right;

    //constructors
    node(){}
    node(T obj);
};

template <class T>
node<T>::node(T obj)
{
    value = obj;
    left = nullptr;
    right = nullptr;
}
#endif