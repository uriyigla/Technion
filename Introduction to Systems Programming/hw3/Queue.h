#ifndef EX3_QUEUE_H
#define EX3_QUEUE_H

#include <exception>

class bad_alloc {};

template <class T>
class Queue {
    class QNode {
    public:
        T m_data;
        QNode* m_next;

        QNode() = default;
        QNode(const T& data) : m_data(data), m_next(nullptr) {}
        QNode(const QNode&) = default;
        QNode& operator=(const QNode&) = default;
        ~QNode() = default;
    };

    int m_size;
    QNode* m_head;
    QNode* m_rear;

public:
    class EmptyQueue {};

    Queue<T>() : m_size(0), m_head(nullptr), m_rear(nullptr) {}

    Queue<T>(const Queue<T>& q) : m_size(0), m_head(nullptr), m_rear(nullptr) {
        const QNode* temp1 = q.m_head;
        while (temp1 != nullptr) {
            this->pushBack(temp1->m_data);
            temp1 = temp1->m_next;
        }
    }

    Queue<T>& operator=(const Queue<T>& q) {
        Queue<T>* keep = this;
        try {
            if (this == &q) {
                return *this;
            }
            QNode* temp1 = this->m_head;
            QNode* temp2 = this->m_head;
            while (temp1 != nullptr) {
                temp2 = temp1;
                temp1 = temp1->m_next;
                m_head = m_head->m_next;
                delete temp2;
            }
            this->m_size = 0;
            QNode* temp3 = q.m_head;
            while (temp3 != nullptr) {
                this->pushBack(temp3->m_data);
                temp3 = temp3->m_next;
            }
        } catch (bad_alloc& e) {
            Queue<T>* a = this;
            this->m_head = keep->m_head;
            this->m_size = keep->m_size;
            this->m_rear = keep->m_rear;
            delete a;
            throw e;
        }
        return *this;
    }

    ~Queue<T>() {
        QNode* temp1 = this->m_head;
        while (this->m_head != nullptr) {
            temp1 = m_head;
            m_head = m_head->m_next;
            delete temp1;
            this->m_size--;
        }
    }

    class Iterator {
        QNode* m_node;

    public:
        class InvalidOperation {};

        Iterator() : m_node(nullptr) {}
        Iterator(QNode* q_node) : m_node(q_node) {}
        Iterator(const Iterator& other) = default;
        Iterator& operator=(const Iterator& iterator) = default;
        ~Iterator() = default;

        Iterator& operator++() {
            if (m_node == nullptr) {
                throw Queue<T>::Iterator::InvalidOperation();
            }
            m_node = m_node->m_next;
            return *this;
        }
        Iterator operator++(int) {
            if (m_node == nullptr) {
                throw Queue<T>::Iterator::InvalidOperation();
            }
            Iterator result = *this;
            ++*this;
            return result;
        }
        bool operator!=(const Iterator& iterator) const {
            return (m_node != iterator.m_node);
        }
        T& operator*() const {
            return m_node->m_data;
        }
    };

    class ConstIterator {
        const QNode* m_node;

    public:
        class InvalidOperation {};

        ConstIterator() : m_node(nullptr) {}
        ConstIterator(const QNode* q_node) : m_node(q_node) {}
        ConstIterator(const ConstIterator& other) = default;
        ConstIterator& operator=(const ConstIterator& constiterator) = default;
        ~ConstIterator() = default;

        ConstIterator& operator++() {
            if (m_node == nullptr) {
                throw Queue<T>::ConstIterator::InvalidOperation();
            }
            m_node = m_node->m_next;
            return *this;
        }
        ConstIterator operator++(int) {
            if (m_node == nullptr) {
                throw Queue<T>::ConstIterator::InvalidOperation();
            }
            ConstIterator result = *this;
            ++*this;
            return result;
        }
        bool operator!=(const ConstIterator& constiterator) const {
            return (m_node != constiterator.m_node);
        }
        const T& operator*() const {
            return m_node->m_data;
        }
    };

    void pushBack(const T& t) {
        QNode* temp1;
        try {
            temp1 = (new QNode(t));
        } catch (bad_alloc&) {
            throw;
        }
        if (m_size == 0) {
            m_head = temp1;
            m_head->m_next = m_rear;
            m_size++;
            return;
        }
        QNode* temp2 = m_head;
        while (temp2->m_next != nullptr) {
            temp2 = temp2->m_next;
        }
        temp2->m_next = temp1;
        temp1->m_next = m_rear;
        m_size++;
    }

    const T& front() const {
        if (this->m_size == 0) {
            throw EmptyQueue();
        }
        return m_head->m_data;
    }

    T& front() {
        if (this->m_size == 0) {
            throw EmptyQueue();
        }
        return m_head->m_data;
    }

    void popFront() {
        if (m_size == 0) {
            throw EmptyQueue();
        }
        if (m_size == 1) {
            delete m_head;
            m_head = nullptr;
            m_rear = nullptr;
            m_size = 0;
            return;
        }
        QNode* temp = m_head;
        m_head = m_head->m_next;
        delete temp;
        this->m_size--;
    }

    int size() const {
        return this->m_size;
    }

    Iterator begin() {
        return Iterator(m_head);
    }

    Iterator end() {
        return Iterator(m_rear);
    }

    ConstIterator begin() const {
        return ConstIterator(m_head);
    }

    ConstIterator end() const {
        return ConstIterator(m_rear);
    }
};

template <class T, class Condition>
Queue<T> filter(Queue<T>& q, Condition myCondition) {
    Queue<T> boolQueue{};
    if (q.size() == 0) {
        return boolQueue;
    }
    for (typename Queue<T>::Iterator it = q.begin(); it != q.end(); ++it) {
        T val = *it;
        if (myCondition(val)) {
            boolQueue.pushBack(val);
        }
    }
    return boolQueue;
}

template <class T, class Function>
void transform(Queue<T>& q, Function myFunction) {
    for (typename Queue<T>::Iterator it = q.begin(); it != q.end(); ++it) {
        T& val = *it;
        myFunction(val);
    }
}

#endif //EX3_QUEUE_H
