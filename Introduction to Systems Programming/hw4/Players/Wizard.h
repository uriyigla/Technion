#ifndef MAIN_CPP_WIZARD_H
#define MAIN_CPP_WIZARD_H
#include "Player.h"

class Wizard : public Player {

public:

    Wizard(const std::string name);
    Wizard(const Wizard& )=default;
    Wizard & operator=(const Wizard& wizard)=default;
    ~Wizard()=default;

    void heal(int improveHP) override;

};

#endif //MAIN_CPP_WIZARD_H