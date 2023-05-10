//
// Created by uriy4 on 24/11/2022.
//

#ifndef WET1_L_NODE_H
#define WET1_L_NODE_H

//////////////Class l_Node//////////////
template<class T>
class t_Node;

template<class T>
class l_Node {
public:
    l_Node() = default;
    l_Node(T* data) : m_data(data), m_prev(nullptr), m_next(nullptr) , m_place_in_tree(nullptr){}
    l_Node(const l_Node& a) = default; //to delete
    l_Node &operator=(const l_Node& a) = default; //to delete
    ~l_Node()=default;
//    {
//        m_place_in_tree=nullptr;
//        m_next=nullptr;
//        m_prev= nullptr;
//        delete this;
//    }
    T* m_data;
    l_Node<T> *m_prev;
    l_Node<T> *m_next;
    t_Node<T> *m_place_in_tree;

};

#endif //WET1_L_NODE_H

