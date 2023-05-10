#ifndef WET1_AVLTREE_H
#define WET1_AVLTREE_H
#include "t_Node.h"

template<class T>
class AVLTree {

public:
    AVLTree() : m_root() {}
    AVLTree(const AVLTree &a) = delete;
    AVLTree &operator=(const AVLTree &a) = delete;
    ~AVLTree() = default;

    t_Node<T>* treeObjectFind(T* obj);
    t_Node<T>* treeIdFind(int id);
    t_Node<T>* treeInsert(T* a);
    t_Node<T>* treeInsertById(T *a);
    bool treeDelete(t_Node<T>* nodeToDelete);

    void updateHeightAndBF(t_Node<T> *updateNode);
    int sideOfSon(t_Node<T> *son);
    void fatherAndSonSeparation(t_Node<T> *son);
    bool roll(t_Node<T>* nodeToRoll);
    void rollLL(t_Node<T> *notBalancedNode);
    void rollRR(t_Node<T> *notBalancedNode);
    void afterTreeInsert(t_Node<T>* newNode);
    void afterTreeDelete(t_Node<T>* fatherOfDeletedNode);
    void switchNodes(t_Node<T> *nodeToDelete, t_Node<T> *nodeToSwitch);
    void makeRightSonAndFather(t_Node<T>* intendedRightSon, t_Node<T>* intendedFather);
    void makeLeftSonAndFather(t_Node<T>* intendedLeftSon, t_Node<T>* intendedFather);
    void deletePostOrder (t_Node<T>* treeIterator, bool toDeleteDate);
    void updateRank(t_Node<T> *updateNode);
//    void printBT(const std::string& prefix, t_Node<T>* node, bool isLeft);
//    void printBT(t_Node<T>* node);

    t_Node<T> *m_root;
};

template<class T>
void AVLTree<T>::makeRightSonAndFather (t_Node<T>* intendedRightSon, t_Node<T>* intendedFather){
    if ((intendedRightSon == nullptr)&&(intendedFather == nullptr)){
        return;
    }
    if (intendedRightSon == nullptr){
        intendedFather->m_right_son = intendedRightSon;
        return;
    }
    if (intendedFather == nullptr){
        intendedRightSon->m_father = intendedFather;
        return;
    }
    intendedFather->m_right_son = intendedRightSon;
    intendedRightSon->m_father = intendedFather;
}

template<class T>
void AVLTree<T>::makeLeftSonAndFather (t_Node<T>* intendedLeftSon, t_Node<T>* intendedFather){
    if ((intendedLeftSon == nullptr)&&(intendedFather == nullptr)){
        return;
    }
    if (intendedLeftSon == nullptr){
        intendedFather->m_left_son = intendedLeftSon;
        return;
    }
    if (intendedFather == nullptr){
        intendedLeftSon->m_father = intendedFather;
        return;
    }
    intendedFather->m_left_son = intendedLeftSon;
    intendedLeftSon->m_father = intendedFather;
}

template<class T>
void AVLTree<T>::updateHeightAndBF(t_Node<T> *updateNode) {
    if ((updateNode->m_left_son == nullptr) && (updateNode->m_right_son == nullptr)){
        updateNode->m_balance_Factor = 0;
        updateNode->m_height = 0;
    }
    if ((updateNode->m_left_son != nullptr) && (updateNode->m_right_son == nullptr)){
        updateNode->m_balance_Factor= 1 + updateNode->m_left_son->m_height;
        updateNode->m_height= 1 + updateNode->m_left_son->m_height;
    }
    if ((updateNode->m_left_son == nullptr) && (updateNode->m_right_son != nullptr)){
        updateNode->m_balance_Factor = - (1 + updateNode->m_right_son->m_height);
        updateNode->m_height= 1 + updateNode->m_right_son->m_height;
    }
    if ((updateNode->m_left_son != nullptr) && (updateNode->m_right_son != nullptr)){
        updateNode->m_balance_Factor = (updateNode->m_left_son->m_height) - (updateNode->m_right_son->m_height);
        if (updateNode->m_balance_Factor > 0){
            updateNode->m_height= 1 + updateNode->m_left_son->m_height;
        }
        else {
            updateNode->m_height= 1 + updateNode->m_right_son->m_height;
        }
    }
}

template<class T>
t_Node<T>* AVLTree<T>::treeInsert(T* a) {
    t_Node<T>* b = new t_Node<T>(a);
    if (this->m_root == nullptr) {
        m_root = b;
        return b;
    }
    t_Node<T> *iterator = this->m_root;
    while (iterator != nullptr) {
        if (*(iterator->m_data) > *a) {
            if (iterator->m_left_son == nullptr) {
                makeLeftSonAndFather(b, iterator);
                afterTreeInsert(b);
                return b;
            }
            iterator = iterator->m_left_son;
        }
        else {
            if (iterator->m_right_son == nullptr) {
                makeRightSonAndFather(b, iterator);
                afterTreeInsert(b);
                return b;
            }
            iterator = iterator->m_right_son;
        }
    }
    return nullptr;
}

template<class T>
t_Node<T>* AVLTree<T>::treeInsertById(T *a) {
    t_Node<T> *b = new t_Node<T>(a);
    if (this->m_root == nullptr) {
        m_root = b;
        return b;
    }

    t_Node<T> *iterator = this->m_root;
    while (iterator != nullptr) {
        if ((int)(*iterator->m_data) > (int)(*a)) {
            if (iterator->m_left_son == nullptr) {
                makeLeftSonAndFather(b, iterator);
                afterTreeInsert(b);
                return b;
            }
            iterator = iterator->m_left_son;
        }
        else {
            if (iterator->m_right_son == nullptr) {
                makeRightSonAndFather(b, iterator);
                afterTreeInsert(b);
                return b;
            }
            iterator = iterator->m_right_son;
        }
    }
    return nullptr;
}

template <class T>
void AVLTree<T>::afterTreeInsert(t_Node<T>* newNode) {
    t_Node<T> *iterator = newNode;
    bool rollDone = false;
    while((iterator != nullptr) && !rollDone) {
        updateHeightAndBF(iterator);
        rollDone = roll(iterator);
        if (!rollDone) {
            updateRank(iterator);
        }
        iterator = iterator->m_father;
    }
    while (iterator!=nullptr){
        updateRank(iterator);
        iterator = iterator->m_father;
    }
}

template <class T>
t_Node<T>* AVLTree<T>::treeObjectFind(T* obj){
    if (this->m_root == nullptr) {
        return nullptr;
    }
    t_Node<T> *iterator = this->m_root;
    while (int(*(iterator->m_data)) != int(*obj)) {
        if ((*iterator->m_data) > *obj) {
            if (iterator->m_left_son == nullptr) {
                return nullptr;
            }
            iterator = iterator->m_left_son;
        }
        else {
            if (iterator->m_right_son == nullptr) {
                return nullptr;
            }
            iterator = iterator->m_right_son;
        }
    }
    return iterator;
}

template <class T>
t_Node<T>* AVLTree<T>::treeIdFind(int id){
    if (this->m_root == nullptr) {
        return nullptr;
    }
    t_Node<T> *iterator = this->m_root;
    while ((int)(*(iterator->m_data)) != id) {
        if ((int)(*iterator->m_data) > id) {
            if (iterator->m_left_son == nullptr) {
                return nullptr;
            }
            iterator = iterator->m_left_son;
        }
        else {
            if (iterator->m_right_son == nullptr) {
                return nullptr;
            }
            iterator = iterator->m_right_son;
        }
    }
    return iterator;
}

template<class T>
bool AVLTree<T>::treeDelete(t_Node<T>* nodeToDelete) {
    if (nodeToDelete == nullptr) {
        return false;
    }

    if ((nodeToDelete->m_left_son == nullptr) && (nodeToDelete->m_right_son == nullptr)) {
        if(nodeToDelete->m_father == nullptr) {
            this->m_root = nullptr;
            delete nodeToDelete;
            return true;
        }
        else {
            t_Node<T> *fatherOfNodeToDelete = nodeToDelete->m_father;
            fatherAndSonSeparation(nodeToDelete);
            delete nodeToDelete;
            afterTreeDelete(fatherOfNodeToDelete);
            return true;
        }
    }

    t_Node<T> *iterator = nullptr;
    int counter = 0;
    if (nodeToDelete->m_left_son != nullptr) {
        iterator = nodeToDelete->m_left_son;
        while (iterator->m_right_son != nullptr) {
            iterator = iterator->m_right_son;
            counter++;
        }
        if (iterator->m_left_son == nullptr) {
            switchNodes(nodeToDelete, iterator);
            if (m_root == nodeToDelete) {
                m_root = iterator;
            }
            t_Node<T>* fatherOfNodeToDelete = nodeToDelete->m_father;
            fatherAndSonSeparation(nodeToDelete);
            delete nodeToDelete;
            afterTreeDelete(fatherOfNodeToDelete);
            return true;
        }
        else {
            switchNodes(nodeToDelete, iterator);
            if (m_root == nodeToDelete) {
                m_root = iterator;
            }
            t_Node<T>* fatherOfNodeToDelete = nodeToDelete->m_father;
            fatherAndSonSeparation(nodeToDelete);
            if (counter == 0) {
                makeLeftSonAndFather(nodeToDelete->m_left_son, fatherOfNodeToDelete);
            }
            else {
                makeRightSonAndFather(nodeToDelete->m_left_son, fatherOfNodeToDelete);
            }
            nodeToDelete->m_left_son = nullptr;
            delete nodeToDelete;
            afterTreeDelete(fatherOfNodeToDelete);
            return true;
        }
    }
    else {
        iterator = nodeToDelete->m_right_son;
        while (iterator->m_left_son != nullptr) {
            iterator = iterator->m_left_son;
            counter++;
        }
        if (iterator->m_right_son == nullptr) {
            switchNodes(nodeToDelete, iterator);
            if (m_root == nodeToDelete) {
                m_root = iterator;
            }
            t_Node<T>* fatherOfNodeToDelete = nodeToDelete->m_father;
            fatherAndSonSeparation(nodeToDelete);
            delete nodeToDelete;
            afterTreeDelete(fatherOfNodeToDelete);
            return true;
        }
        else {
            switchNodes(nodeToDelete, iterator);
            if (m_root == nodeToDelete) {
                m_root = iterator;
            }
            t_Node<T>* fatherOfNodeToDelete = nodeToDelete->m_father;
            fatherAndSonSeparation(nodeToDelete);
            if (counter == 0) {
                makeRightSonAndFather(nodeToDelete->m_right_son, fatherOfNodeToDelete);
            }
            else {
                makeLeftSonAndFather(nodeToDelete->m_right_son, fatherOfNodeToDelete);
            }
            nodeToDelete->m_right_son = nullptr;
            delete nodeToDelete;
            afterTreeDelete(fatherOfNodeToDelete->m_left_son);
            return true;
        }
    }
}

template <class T>
void AVLTree<T>::afterTreeDelete(t_Node<T>* fatherOfDeletedNode) {
    t_Node<T> *iterator = fatherOfDeletedNode;
    while((iterator != nullptr)) {
        updateHeightAndBF(iterator);
        roll(iterator);
        updateRank(iterator);
        iterator = iterator->m_father;
    }
}

template <class T>
bool AVLTree<T>::roll(t_Node<T>* nodeToRoll) {

    if ((nodeToRoll->m_balance_Factor == 2) && ((nodeToRoll->m_left_son->m_balance_Factor) >= 0)) { ///////////////LL
        rollLL(nodeToRoll);
        return true;
    }
    else if ((nodeToRoll->m_balance_Factor == -2) && ((nodeToRoll->m_right_son->m_balance_Factor) <= 0)) {///////////////RR
        rollRR(nodeToRoll);
        return true;
    }
    else if ((nodeToRoll->m_balance_Factor == 2) && (nodeToRoll->m_left_son->m_balance_Factor == -1)) { ////////////////LR
        rollRR(nodeToRoll->m_left_son);
        rollLL(nodeToRoll);
        return true;
    }
    else if ((nodeToRoll->m_balance_Factor == -2) && (nodeToRoll->m_right_son->m_balance_Factor == 1)) { ////////////////RL
        rollLL(nodeToRoll->m_right_son);
        rollRR(nodeToRoll);
        return true;
    }
    return false;
}

template<class T>
void AVLTree<T>::switchNodes(t_Node<T> *nodeToDelete, t_Node<T> *nodeToSwitch) {
    int k1 = sideOfSon(nodeToDelete);
    int k2 = sideOfSon(nodeToSwitch);
    int sum=0;

    t_Node<T>* a = nodeToDelete->m_father;
    t_Node<T>* b = nodeToDelete->m_right_son;
    t_Node<T>* c = nodeToDelete->m_left_son;

    t_Node<T>* d = nodeToSwitch->m_father;
    t_Node<T>* e = nodeToSwitch->m_right_son;
    t_Node<T>* f = nodeToSwitch->m_left_son;

    fatherAndSonSeparation(nodeToDelete);
    fatherAndSonSeparation(b);
    fatherAndSonSeparation(c);

    fatherAndSonSeparation(nodeToSwitch);
    fatherAndSonSeparation(e);
    fatherAndSonSeparation(f);


    if ((nodeToDelete==d)&&(nodeToDelete!=nullptr)){
        if (k2==1){
            makeLeftSonAndFather(nodeToDelete,nodeToSwitch);
            makeRightSonAndFather(b,nodeToSwitch);
            sum++;
        }
        if (k2==2){
            makeRightSonAndFather(nodeToDelete,nodeToSwitch);
            makeLeftSonAndFather(c,nodeToSwitch);
            sum++;
        }
        if (k1==1){
            makeLeftSonAndFather(nodeToSwitch,a);
        }
        else{
            makeRightSonAndFather(nodeToSwitch,a);
        }
        makeRightSonAndFather(e,nodeToDelete);
        makeLeftSonAndFather(f,nodeToDelete);
    }

    if (sum==0) {

        makeRightSonAndFather(b, nodeToSwitch);

        makeLeftSonAndFather(c, nodeToSwitch);

        makeRightSonAndFather(e, nodeToDelete);

        makeLeftSonAndFather(f, nodeToDelete);


        switch (k1) {
            case (0):
                makeRightSonAndFather(nodeToSwitch, nullptr);
                break;
            case (1):
                makeLeftSonAndFather(nodeToSwitch, a);
                break;
            case (2):
                makeRightSonAndFather(nodeToSwitch, a);
                break;
        }

        switch (k2) {
            case (0):
                makeRightSonAndFather(nodeToDelete, nullptr);
                break;
            case (1):
                makeLeftSonAndFather(nodeToDelete, d);
                break;
            case (2):
                makeRightSonAndFather(nodeToDelete, d);
                break;
        }
    }


    int tempHeight = nodeToDelete->m_height;
    nodeToDelete->m_height = nodeToSwitch->m_height;
    nodeToSwitch->m_height = tempHeight;

    int tempBF = nodeToDelete->m_balance_Factor;
    nodeToDelete->m_balance_Factor=nodeToSwitch->m_balance_Factor;
    nodeToSwitch->m_balance_Factor=tempBF;

    int tempRank = nodeToDelete->m_rank;
    nodeToDelete->m_rank=nodeToSwitch->m_rank;
    nodeToSwitch->m_rank=tempRank;

    if (nodeToDelete == m_root){
        m_root = nodeToSwitch;
    }
    else if (nodeToSwitch == m_root){
        m_root=nodeToDelete;
    }
}

template<class T>
int AVLTree<T>::sideOfSon(t_Node<T> *son){
    if (son->m_father == nullptr){
        return 0;
    }
    else {
        if (son->m_father->m_left_son==son){
            return 1;
        }
        return 2;
    }
}

template<class T>
void AVLTree<T>::fatherAndSonSeparation(t_Node<T> *son) {
    if (son==nullptr){
        return;
    }
    if (sideOfSon(son) == 1) {
        son->m_father->m_left_son = nullptr;
    }
    else if (sideOfSon(son) == 2) {
        son->m_father->m_right_son = nullptr;
    }
    son->m_father = nullptr;
}

template<class T>
void AVLTree<T>::rollLL(t_Node<T> *notBalancedNode) {
    //enters here only if BF=2, and left son BF>=0
    t_Node<T>* B = notBalancedNode;
    t_Node<T>* A = B->m_left_son;
    t_Node<T>* A_r = A->m_right_son;
    int side = sideOfSon(B);

    if (side == 0){      // no father for B
        A->m_father = nullptr;
        this->m_root = A;
    }
    else if (side == 1) { // B is left son
        makeLeftSonAndFather(A, B->m_father);
    }
    else { // B is right son
        makeRightSonAndFather(A, B->m_father);
    }

    makeRightSonAndFather(B, A);

    if (A_r == nullptr){
        B->m_left_son = nullptr;
        if (B->m_right_son == nullptr){
            B->m_height = 0;
            B->m_balance_Factor = 0;
        }
        else {
            B->m_height = B->m_right_son->m_height + 1;
            B->m_balance_Factor = -(B->m_right_son->m_height + 1);
        }
    }
    else {
        makeLeftSonAndFather(A_r, B);
        if (B->m_right_son == nullptr) {
            B->m_height = B->m_left_son->m_height + 1;
            B->m_balance_Factor = B->m_left_son->m_height + 1;
        }
        else{
            if ((B->m_left_son->m_height) > (B->m_right_son->m_height)){
                B->m_height = B->m_left_son->m_height + 1;
            }
            else {
                B->m_height = B->m_right_son->m_height + 1;
            }
            B->m_balance_Factor = (B->m_left_son->m_height) - (B->m_right_son->m_height);
        }
    }
    if (A->m_left_son == nullptr){
        A->m_height = A->m_right_son->m_height + 1;
        A->m_balance_Factor = - (A->m_right_son->m_height + 1);
    }
    else {
        if ((A->m_left_son->m_height) > (A->m_right_son->m_height)){
            A->m_height = A->m_left_son->m_height + 1;
        }
        else {
            A->m_height = A->m_right_son->m_height + 1;
        }
        A->m_balance_Factor = (A->m_left_son->m_height) - (A->m_right_son->m_height);
    }
    updateRank(B);
    updateRank(A);
}

template<class T>
void AVLTree<T>::rollRR(t_Node<T> *notBalancedNode) {
    //enters here only if BF=-2, and right son BF<=0
    t_Node<T>* B = notBalancedNode;
    t_Node<T>* A = B->m_right_son;
    t_Node<T>* A_l = A->m_left_son;
    int side = sideOfSon(B);

    if (side == 0){      // no father for B
        A->m_father = nullptr;
        this->m_root = A;
    }
    else if (side == 1) { // B is left son
        makeLeftSonAndFather(A, B->m_father);
    }
    else { // B is right son
        makeRightSonAndFather(A, B->m_father);
    }

    makeLeftSonAndFather(B, A);

    if (A_l == nullptr){
        B->m_right_son = nullptr;
        if (B->m_left_son == nullptr){
            B->m_height = 0;
            B->m_balance_Factor = 0;
        }
        else {
            B->m_height = B->m_left_son->m_height + 1;
            B->m_balance_Factor = B->m_left_son->m_height + 1;
        }
    }
    else {
        makeRightSonAndFather(A_l, B);
        if (B->m_left_son == nullptr) {
            B->m_height = B->m_right_son->m_height + 1;
            B->m_balance_Factor = -(B->m_right_son->m_height + 1);
        }
        else{
            if ((B->m_right_son->m_height) > (B->m_left_son->m_height)){
                B->m_height = B->m_right_son->m_height + 1;
            }
            else {
                B->m_height = B->m_left_son->m_height + 1;
            }
            B->m_balance_Factor = (B->m_left_son->m_height) - (B->m_right_son->m_height);
        }
    }
    if (A->m_right_son == nullptr){
        A->m_height = A->m_left_son->m_height + 1;
        A->m_balance_Factor = A->m_left_son->m_height + 1;
    }
    else {
        if ((A->m_right_son->m_height) > (A->m_left_son->m_height)){
            A->m_height = A->m_right_son->m_height + 1;
        }
        else {
            A->m_height = A->m_left_son->m_height + 1;
        }
        A->m_balance_Factor = (A->m_left_son->m_height) - (A->m_right_son->m_height);
    }
    updateRank(B);
    updateRank(A);
}

template<class T>
void AVLTree<T>::deletePostOrder (t_Node<T>* treeIterator, bool toDeleteDate) {

    if (treeIterator==nullptr){
        return;
    }
    deletePostOrder(treeIterator->m_left_son,toDeleteDate);
    deletePostOrder(treeIterator->m_right_son,toDeleteDate);
    if (!toDeleteDate){
        treeIterator->m_data=nullptr;
        delete (treeIterator->m_data);
    }
    else {
        delete (treeIterator->m_data);
    }
    treeIterator->m_father=nullptr;
    treeIterator->m_right_son=nullptr;
    treeIterator->m_left_son=nullptr;
    delete treeIterator;
}

template<class T>
void AVLTree<T>::updateRank(t_Node<T> *updateNode){
    if (updateNode == nullptr ){
        return;
    }
    t_Node<T>* leftSon = updateNode->m_left_son;
    t_Node<T>* rightSon = updateNode->m_right_son;
    if ( (leftSon == nullptr) && (rightSon == nullptr) ){
        updateNode->m_rank = 1;
        return;
    }
    else {
        if (leftSon == nullptr){
            updateNode->m_rank = updateNode->m_right_son->m_rank+1;
            return;
        }
        if (rightSon == nullptr){
            updateNode->m_rank = updateNode->m_left_son->m_rank+1;
            return;
        }
        updateNode->m_rank=updateNode->m_right_son->m_rank+ updateNode->m_left_son->m_rank + 1;
        return;
    }
}

//template<class T>
//void AVLTree<T>::printBT(const std::string& prefix, t_Node<T>* node, bool isLeft)
//{
//    if( node != nullptr )
//    {
//        std::cout << prefix;
//
//        std::cout << (isLeft ? "|--" : "'--" );
//
//        // print the value of the node
//        std::cout << *(node->m_data) << std::endl;
//
//        // enter the next tree level - left and right branch
//        printBT( prefix + (isLeft ? "|   " : "    "), node->m_left_son, true);
//        printBT( prefix + (isLeft ? "|   " : "    "), node->m_right_son, false);
//    }
//}
//
//template<class T>
//void AVLTree<T>::printBT(t_Node<T>* node) {
//    printBT("", node, false);
//}



#endif //WET1_AVLTREE_H