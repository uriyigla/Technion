//
// Created by Shaked on 23/11/2022.
//

#ifndef WET1_LIST_H
#define WET1_LIST_H

#include "l_Node.h"

template <class T>
class t_Node;
//////////////Class List//////////////
template<class T>
class List {
public:
    List() : m_size(0), m_head(nullptr), m_rear(nullptr) , m_iterator(nullptr),m_good_leaves(0){}
    List(const List& a) = delete; //to delete
    List &operator=(const List& a) = delete; //to delete
    ~List()= default;

    void connectLeftToRight(l_Node<T>* a, l_Node<T>* b);
    l_Node<T>* listInsertAfter(T *a,l_Node<T>* b);
    l_Node<T>* listInsertInFront(T *a, l_Node<T>* b);
    void listRemove(l_Node<T>* b);
    void mergeList(List<T>* a, List<T>* b);
    void mergeListOther(List<T>* a, List<T>* b);

    int m_size;
    l_Node<T> *m_head;
    l_Node<T> *m_rear;
    l_Node<T> *m_iterator;
    int m_good_leaves;
};

template<class T>
void List<T>::connectLeftToRight (l_Node<T>* a, l_Node<T>* b){
    a->m_next=b;
    b->m_prev=a;
}

template<class T>
l_Node<T>* List<T>::listInsertAfter(T *a,l_Node<T>* b){
    l_Node<T> *c = new l_Node<T>(a);
    if (this->m_size==0){
        m_head=c;
        m_rear=c;
    }
    else{
        l_Node<T>* d=b->m_next;
        connectLeftToRight(b, c);
        if (d!=nullptr){
            connectLeftToRight(c, d);
        }
        else {
            c->m_next=nullptr;
            m_rear=c;
        }
    }
    m_size++;
    return c;
}

template<class T>
l_Node<T>* List<T>::listInsertInFront(T *a, l_Node<T>* b){
    l_Node<T> *c = new l_Node<T>(a);
    if (this->m_size==0){
        m_head=c;
        m_rear=c;
    }
    else{
        l_Node<T>* d=b->m_prev;
        connectLeftToRight(c, b);
        if (d!=nullptr){
            connectLeftToRight(d, c);
        }
        else {
            c->m_prev=nullptr;
            m_head=c;
        }
    }
    m_size++;
    return c;
}

template<class T>
void List<T>::listRemove(l_Node<T>* b) {
    if (m_size == 1) {
        this->m_head = nullptr;
        this->m_rear = nullptr;
    }
    else {
        l_Node<T> *a = b->m_prev;
        l_Node<T> *c = b->m_next;
        if (a == nullptr) {
            m_head = c;
            c->m_prev = nullptr;
            b->m_next = nullptr;
        } else {
            if (c == nullptr) {
                m_rear = a;
                a->m_next = nullptr;
                b->m_prev = nullptr;
            }
            else {
                connectLeftToRight(a, c);
            }
        }
    }
    m_size--;
    delete (b);
}

template<class T>
void List<T>::mergeList(List<T>* a, List<T>* b) {
    l_Node<T> *nodeList1 = a->m_head;
    l_Node<T> *nodeList2 = b->m_head;
    int length=a->m_size+b->m_size;
    l_Node<T>* newInsertNode=nullptr;
    l_Node<T>* dataOriginNode=nullptr;

    for (int i=0;i<length;i++){
        if (nodeList1 == nullptr) {
            newInsertNode = this->listInsertAfter(nodeList2->m_data,m_rear);
            dataOriginNode = nodeList2;
            nodeList2 = nodeList2->m_next;
        }
        else if (nodeList2 == nullptr) {
            newInsertNode = this->listInsertAfter(nodeList1->m_data,m_rear);
            dataOriginNode = nodeList1;
            nodeList1 = nodeList1->m_next;
        }
        else {
            if ((int)(*nodeList1->m_data)>(int)(*nodeList2->m_data)){
                newInsertNode = this->listInsertAfter(nodeList2->m_data,m_rear);
                dataOriginNode = nodeList2;
                nodeList2 = nodeList2->m_next;
            }
            else {
                newInsertNode = this->listInsertAfter(nodeList1->m_data,m_rear);
                dataOriginNode = nodeList1;
                nodeList1 = nodeList1->m_next;
            }
        }
        newInsertNode->m_place_in_tree=dataOriginNode->m_place_in_tree->m_place_other_tree;
    }
}

template<class T>
void List<T>::mergeListOther(List<T>* a, List<T>* b) {
    l_Node<T> *nodeList1 = a->m_head;
    l_Node<T> *nodeList2 = b->m_head;
    int length=a->m_size+b->m_size;
    l_Node<T>* newInsertNode=nullptr;
    l_Node<T>* dataOriginNode=nullptr;

    for (int i=0;i<length;i++){
        if (nodeList1 == nullptr) {
            newInsertNode = this->listInsertAfter(nodeList2->m_data,m_rear);
            dataOriginNode = nodeList2;
            nodeList2 = nodeList2->m_next;
        }
        else if (nodeList2 == nullptr) {
            newInsertNode = this->listInsertAfter(nodeList1->m_data,m_rear);
            dataOriginNode = nodeList1;
            nodeList1 = nodeList1->m_next;
        }
        else {
            if ((*nodeList1->m_data)>(*nodeList2->m_data)){
                newInsertNode = this->listInsertAfter(nodeList2->m_data,m_rear);
                dataOriginNode = nodeList2;
                nodeList2 = nodeList2->m_next;
            }
            else {
                newInsertNode = this->listInsertAfter(nodeList1->m_data,m_rear);
                dataOriginNode = nodeList1;
                nodeList1 = nodeList1->m_next;
            }
        }
        newInsertNode->m_place_in_tree=dataOriginNode->m_place_in_tree->m_place_other_tree;
    }
}

#endif //WET1_LIST_H
