#ifndef HW4_PLAYER_H
#define HW4_PLAYER_H

#include <string>
#include <iostream>
#include <queue>
#include <deque>
using namespace std;

template<typename T, typename Container=std::deque<T> >
class iterable_queue : public std::queue<T,Container>
{
public:
    typedef typename Container::iterator iterator;
    typedef typename Container::const_iterator const_iterator;

    iterator begin() { return this->c.begin(); }
    iterator end() { return this->c.end(); }
    const_iterator begin() const { return this->c.begin(); }
    const_iterator end() const { return this->c.end(); }
};


class Player {

    protected:

        std::string m_name;
        int m_level, m_force,m_HP, m_coins;
        std::string m_job;
    public:

    friend std::ostream& operator<<(std::ostream& os, const Player& player);

        Player(const std::string name);
        Player(const Player& )=default;
        Player & operator=(const Player& player)=default;
        virtual ~Player()=default;

    //from ex2 start

        void printInfo() const;
        void levelUp();
        int getLevel() const;
        void buff(int improveForce);
        virtual void heal(int improveHP);
        void damage(int damageHP);
        bool isKnockedOut() const;
        virtual void addCoins(int extraCoins);
        bool pay(int payment);
        virtual int getAttackStrength() const;

    //from ex2 end

        void print(std::ostream& os) const;
        std::string getName() const;
        std::string getJob() const;
        int getCoins() const;
};

#endif //HW4_PLAYER_H