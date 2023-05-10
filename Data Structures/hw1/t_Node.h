//
// Created by uriy4 on 24/11/2022.
//

#ifndef WET1_T_NODE_H
#define WET1_T_NODE_H

#include <memory>
#include "l_Node.h"


template <class T>
class t_Node {
public:

    t_Node* m_father;
    t_Node* m_left_son;
    t_Node* m_right_son;
    t_Node* m_place_other_tree;
    l_Node<T>* m_place_in_List;
    T* m_data ;
    int m_height;
    int m_balance_Factor;

    t_Node() :
            m_father(nullptr), m_left_son(nullptr),
            m_right_son(nullptr), m_place_other_tree(nullptr),
            m_place_in_List(nullptr), m_data(nullptr),
            m_height(0), m_balance_Factor(0)
    {}
    t_Node(T* data) :
            m_father(nullptr), m_left_son(nullptr),
            m_right_son(nullptr), m_place_other_tree(nullptr),
            m_place_in_List(nullptr), m_data(data),
            m_height(0), m_balance_Factor(0)
            {}

    t_Node(const t_Node& a) = default; //to delete
    t_Node &operator=(const t_Node& a) = default; //to delete
    ~t_Node()=default;
//    {
//        m_left_son=nullptr;
//        m_father=nullptr;
//        m_right_son= nullptr;
//        m_place_in_List= nullptr;
//        delete this;
//    }
    bool isLeaf ();

};

template<class T>
void right_son_father (t_Node<T>* right_son,t_Node<T>* father){
    father->m_right_son=right_son;
    right_son->m_father=father;
}

template<class T>
void left_son_father (t_Node<T>* left_son,t_Node<T>* father){
    father->m_left_son=left_son;
    left_son->m_father=father;
}

template<class T>
bool t_Node<T>::isLeaf (){
    return ((this->m_left_son==nullptr)&&(this->m_right_son==nullptr));
}




#endif //WET1_T_NODE_H