//
// Created by uriy4 on 24/11/2022.
//

#ifndef WET1_T_NODE_H
#define WET1_T_NODE_H

class UnionFind;

template <class T>
class t_Node {
public:

    t_Node() :
            m_father(nullptr), m_left_son(nullptr),
            m_right_son(nullptr), m_data(nullptr),
            m_height(0), m_balance_Factor(0), m_rank(0)
            {}
    t_Node(T* data) :
            m_father(nullptr), m_left_son(nullptr),
            m_right_son(nullptr), m_data(data),
            m_height(0), m_balance_Factor(0), m_rank(1)
            {}

    t_Node(const t_Node& a) = default;
    t_Node &operator=(const t_Node& a) = default;
    ~t_Node()=default;

    bool isLeaf ();

    t_Node* m_father;
    t_Node* m_left_son;
    t_Node* m_right_son;

    T* m_data ;
    int m_height;
    int m_balance_Factor;
    int m_rank;
};

template<class T>
bool t_Node<T>::isLeaf (){
    return ((this->m_left_son == nullptr) && (this->m_right_son == nullptr));
}

#endif //WET1_T_NODE_H