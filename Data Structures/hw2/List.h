//
// Created by Shaked on 23/11/2022.
//

#ifndef WET1_LIST_H
#define WET1_LIST_H

#include "l_Node.h"
#include "uf_Node.h"

template<class T>
class List {
public:
    List() : m_size(0), m_head(nullptr), m_rear(nullptr) {}
    List(const List& a) = delete;
    List &operator=(const List* a) = delete;
    ~List()= default;

    void connectLeftToRight(l_Node<T>* a, l_Node<T>* b);
    l_Node<T>* listInsertAfter(T* a, l_Node<T>* b);
    l_Node<T>*  listRemove(l_Node<T>* b);

    int m_size;
    l_Node<T> *m_head;
    l_Node<T> *m_rear;

};

template<class T>
void List<T>::connectLeftToRight (l_Node<T>* a, l_Node<T>* b){
    a->m_next = b;
    b->m_prev = a;
}

template<class T>
l_Node<T>* List<T>::listInsertAfter(T* a, l_Node<T>* b){
    l_Node<T>* c = new l_Node<T>(a);
    if (this->m_size == 0){
        m_head = c;
        m_rear = c;
    }
    else{
        l_Node<T>* d = b->m_next;
        connectLeftToRight(b, c);
        if (d != nullptr){
            connectLeftToRight(c, d);
        }
        else {
            c->m_next = nullptr;
            m_rear = c;
        }
    }
    m_size++;
    return c;
}

template<class T>
l_Node<T>*  List<T>::listRemove(l_Node<T>* b) {
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
    return b;
}


#endif //WET1_LIST_H