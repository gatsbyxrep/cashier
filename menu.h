#ifndef MENU_H
#define MENU_H

#include "cash-register.h"

class DefaultMenu {
public:
    virtual void run() = 0;
};

class Menu : public DefaultMenu {
public:
    using CashierFunc = Cashier::ProductMap::iterator (*)(const std::wstring&, Cashier::Cash*);
    Menu(Cashier::Cash* cash);
    void run() override;
private:
    Cashier::Cash* cash;
    std::unordered_map<std::wstring,  CashierFunc> commands;
    void initMenuCommands();
};

#endif // MENU_H
