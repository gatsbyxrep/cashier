#include "cash-register.h"
#include <codecvt>

std::wostream& Cashier::operator << (std::wostream& os, const Cashier::Product& product) {
    os << std::endl
       << "Bar code:" << product.getBarcode() << std::endl
       << "Name:" <<  product.getName() << std::endl
       << "group: " << product.fields.group << std::endl
       << "underGrop: " << product.fields.underGroup << std::endl
       << "price: " << product.fields.price << std::endl
       << "vendor:" << product.fields.vendor << std::endl
       << "expiration date:" << product.fields.expirationDate << std::endl
       << "address:" << product.fields.address << std::endl;
    return os;
}

std::size_t Cashier::Product::getBarcode() const {
    return barcode;
}

std::wstring Cashier::Product::getName() const {
    return name;
}

Cashier::Product::Product(std::size_t barcode, std::wstring name) : barcode(barcode), name(name) {}

Cashier::Product::Product(const Product &product) {
    barcode = product.barcode;
    name = product.name;
    fields = product.fields;
}

Cashier::Product::Product(Product&& product) {
    barcode = std::move(product.barcode);
    name = std::move(product.name);
    fields = std::move(product.fields);
}

void Cashier::Cash::addProduct(Product&& product) {
    nameVsProductMap.insert(product);
    barcodeVsProductMap.insert(nameVsProductMap.find(product.getName()));
}

Cashier::ProductMap::iterator Cashier::Cash::end() {
    return nameVsProductMap.end();
}

Cashier::ProductMap::iterator Cashier::Cash::at(std::size_t barcode) {
    auto productIt = barcodeVsProductMap.find(barcode);
    return *productIt;
}

Cashier::ProductMap::iterator Cashier::Cash::at(const std::wstring& name) {
    return nameVsProductMap.find(name);
}

Cashier::Cash::Cash(const Cash& other) {
    for(auto& it : other.nameVsProductMap) {
        nameVsProductMap.emplace(it);
        barcodeVsProductMap.emplace(nameVsProductMap.find(it.getName()));
    }
}

Cashier::Product Cashier::ProductFactory::createProduct(CSVRow csv) {
    auto localCsv = std::move(csv);
    Product product {static_cast<size_t>(std::stoi(localCsv.getElement(0))), localCsv.getElement(1)};
    product.fields.group = localCsv.getElement(2);
    product.fields.underGroup = localCsv.getElement(3);
    product.fields.price = std::stod(localCsv.getElement(4));
    product.fields.vendor = localCsv.getElement(5);
    product.fields.expirationDate = std::stoi(localCsv.getElement(6));
    product.fields.address = localCsv.getElement(7);
    return product;
}

std::optional<Cashier::Cash> Cashier::ProductFactory::createCashFromFile(const std::string& filePath) {
        Cash cash;
        std::wifstream file {filePath};
        file.imbue(std::locale(std::locale(), new std::codecvt_utf8<wchar_t>));
        if(!file.is_open()) {
            std::wcout << "Cant open file: " << std::wstring{filePath.begin(), filePath.end()} << std::endl;
            return std::nullopt;
        }
        while(!file.eof()) {
            CSVRow row;
            if(!row.read(file, 8)) {
                std::wcout << "Invalid CSV file" << std::endl;
                return std::nullopt;
            }

            cash.addProduct(createProduct(std::move(row)));
        }
        return cash;
}
