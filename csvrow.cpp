#include <iostream>

#include "csvrow.h"

bool CSVRow::read(std::wifstream &streamedStr, std::size_t size) {
    buffer.clear();
    std::getline(streamedStr, buffer);
    std::wstring::size_type tokenPos = 0;
    elIndexes.push_back(0);
    while((tokenPos = buffer.find(';', tokenPos)) != std::string::npos) {
        elIndexes.push_back(tokenPos);
        ++tokenPos;
    }
    tokenPos = elIndexes.size();
    buffer.push_back(tokenPos);
    return size == elIndexes.size();
}

std::wstring CSVRow::getElement(size_t elIndex) const {
    const size_t from = elIndex == 0 ? 0 : elIndexes[elIndex] + 1;
    const size_t to = elIndex == elIndexes.size() - 1 ? buffer.size() - 1 : elIndexes[elIndex + 1];
    return buffer.substr(from, to - from);
}
