#ifndef EX3_HEALTHPOINTS_H
#define EX3_HEALTHPOINTS_H
#include <iostream>

class HealthPoints
{
    int m_hp,m_maxHp;

    public:

        class InvalidArgument{};
        HealthPoints(int maxHp=100):m_hp(maxHp),m_maxHp(maxHp)
        {
            if (maxHp<=0){
                throw InvalidArgument();
            }
        }
        HealthPoints(const HealthPoints &)=default;
        HealthPoints & operator=(const HealthPoints & hp)=default;
        ~ HealthPoints()=default;
        explicit operator int() const { return m_hp; }

        friend void operator+= (HealthPoints& hp,const int num);
        friend HealthPoints operator+ ( HealthPoints& hp ,const int num);
        friend bool operator== (const HealthPoints& hp1,const HealthPoints& hp2);
        friend bool operator> (const HealthPoints& hp1,const HealthPoints& hp2);
        friend std::ostream& operator<<(std::ostream& os,const HealthPoints hp);

};

HealthPoints operator+ ( HealthPoints& hp ,const int num);
void operator-= (HealthPoints& hp,const int num);
HealthPoints operator + ( int num, HealthPoints& hp);
HealthPoints operator - ( HealthPoints& hp,const int num);
bool operator!= (const HealthPoints& hp1,const HealthPoints& hp2);
bool operator< (const HealthPoints& hp1,const HealthPoints& hp2);
bool operator>= (const HealthPoints& hp1,const HealthPoints& hp2);
bool operator<= (const HealthPoints& hp1,const HealthPoints& hp2);

#endif //EX3_HEALTHPOINTS_H