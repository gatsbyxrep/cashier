#ifndef CSVROW_H
#define CSVROW_H
#include <vector>
#include <fstream>

class CSVRow {
public:
    bool read(std::wifstream& streamedStr, std::size_t size = 0);
    std::wstring getElement(size_t elIndex) const;
private:
    std::wstring buffer;
    std::vector<std::size_t> elIndexes;
};

#endif // CSVROW_H
