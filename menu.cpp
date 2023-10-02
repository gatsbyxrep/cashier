#include "menu.h"

Menu::Menu(Cashier::Cash* cash) : cash(cash) {
    initMenuCommands();
}

void Menu::run() {
    bool ignore = false;
    for(;;) {
        if(!ignore) {
            std::wcout << "(cashier) >>> ";
        }
        std::wstring str;
        std::getline(std::wcin, str);
        if(str == L"") {
            ignore = true;
            continue;
        }
        ignore = false;
        if(str == L"quit") {
            break;
        }
        size_t space = str.find_first_of(L" ");
        auto command = str.substr(0, space);
        auto arg = str.substr(space + 1, command.size() - 1 - space);
        if(commands.find(command) == commands.end()) {
            std::wcout << "Invalid command" << std::endl;
            continue;
        }
        Cashier::ProductMap::iterator product = commands.at(command)(arg, cash);
        if(product != cash->end()) {
            std::wcout << *product << std::endl;
        } else {
            std::wcout << "Cant find product" << std::endl;
        }
    }
}

void Menu::initMenuCommands() {
    commands.emplace(L"find-name", [](const std::wstring& product, Cashier::Cash* cash) {
        return cash->at(product);
    });
    commands.emplace(L"find-barcode", [](const std::wstring& product, Cashier::Cash* cash) {
        return cash->at(std::stoi(product));
    });
}
