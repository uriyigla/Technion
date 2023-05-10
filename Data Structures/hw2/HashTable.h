//
// Created by Shaked on 21/12/2022.
//

#ifndef WET2_HASHTABLE_H
#define WET2_HASHTABLE_H
#include "List.h"
#include "l_Node.h"

template <class T>
class HashTable {
public:
    HashTable() : m_arraySize(3), m_counter(0) {
        m_array = new List<T>[3];
    }

    HashTable(const HashTable &a) = delete;

    HashTable &operator=(const HashTable &a) = delete;

    ~HashTable() = default;

    void increaseArray();

    void checkCounter();

    void insert(T *a);

    Player* findInArray(int id);

    void print ();

    int m_arraySize;
    int m_counter;
    List<T> *m_array;
};

template<class T>
void HashTable<T>::increaseArray() {
    int count = m_arraySize;
    m_arraySize = (m_arraySize * 2) + 1;
    List<T> *newArray = new List<T>[m_arraySize];
    for (int i = 0; i < count; i++) {
        while (m_array[i].m_size != 0) {
            l_Node<T> *temp = m_array[i].listRemove(m_array[i].m_head);
            int k = (int) (*(temp->m_data)) % (m_arraySize);
            newArray[k].listInsertAfter(temp->m_data, newArray[k].m_rear);
            temp->m_data = nullptr;
            delete temp;
        }
    }
    delete[] m_array;
    m_array = newArray;
}

template<class T>
void HashTable<T>::checkCounter() {
    if (m_counter * 2 >= m_arraySize) {
        increaseArray();
    }
}

template<class T>
void HashTable<T>::insert(T *a) {
    int k = ((int) (*a) % (m_arraySize));
    m_array[k].listInsertAfter(a, m_array[k].m_rear);
    m_counter++;
    checkCounter();
}

template<class T>
Player* HashTable<T>::findInArray(int id) {
    int k = (id) % (m_arraySize);
    if (m_array[k].m_head != nullptr) {
        l_Node<T> *iterator = m_array[k].m_head;
        while (iterator != nullptr) {
            if ((int) (*(iterator->m_data)) == id) {
                return iterator->m_data;
            }
            iterator = iterator->m_next;
        }
    }
    return nullptr;
}

#endif //WET2_HASHTABLE_H