//
// Created by uriy4 on 24/11/2022.
//

#ifndef WET1_L_NODE_H
#define WET1_L_NODE_H

template<class T>
class t_Node;

template<class T>
class l_Node {
public:

    l_Node() = default;
    explicit l_Node(T* data) : m_data(data), m_prev(nullptr), m_next(nullptr) {}
    l_Node(const l_Node& a) = default;
    l_Node &operator=(const l_Node& a) = default;
    ~l_Node()=default;

    T* m_data;
    l_Node<T> *m_prev;
    l_Node<T> *m_next;

};

#endif //WET1_L_NODE_H

