#include <iostream>
#include <codecvt>
#include <locale>
#include <io.h>
#include <fcntl.h>

#include "cash-register.h"
#include "menu.h"

int main() {
    #ifdef WIN32
    _setmode(_fileno(stdout), _O_U16TEXT);
    _setmode(_fileno(stdin), _O_U16TEXT);
    #endif
    std::wcin.imbue(std::locale(std::locale(), new std::codecvt_utf16<wchar_t>));

    auto cashOpt = Cashier::ProductFactory::createCashFromFile("data/products.csv");
    if(!cashOpt.has_value()) {
        std::wcout << "Cant create cashier" << std::endl;
        return -1;
    }
    auto cash = cashOpt.value();
    Menu menu {&cash};
    menu.run();
    return 0;
}


