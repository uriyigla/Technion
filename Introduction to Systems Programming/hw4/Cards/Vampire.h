#ifndef MAIN_CPP_VAMPIRE_H
#define MAIN_CPP_VAMPIRE_H

#include "Card.h"

class  Vampire: public  Battle{

public:

    Vampire();
    Vampire(const Vampire&) = default;
    Vampire& operator=(const Vampire& other) = default;
    ~Vampire() = default;

};

#endif //MAIN_CPP_VAMPIRE_H