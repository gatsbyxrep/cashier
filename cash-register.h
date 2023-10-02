#ifndef CASHREGISTER_H
#define CASHREGISTER_H

#include <iostream>
#include <unordered_map>
#include <string>
#include <optional>
#include <fstream>
#include <unordered_set>

#include "csvrow.h"

namespace Cashier {
    struct Product {
        struct Fields {
            std::wstring group;
            std::wstring underGroup;
            double price = 0.0;
            std::wstring vendor;
            int expirationDate = 0;
            std::wstring address;
        } fields;
        std::size_t getBarcode() const;
        std::wstring getName() const;
    public:
        Product(const Product& product);
        Product(Product&& product);
        Product(std::size_t barcode, std::wstring name);
    private:
        std::size_t barcode;
        std::wstring name;
    };
    std::wostream& operator << (std::wostream& os, const Product& product);

    struct ProductHashForName {
        using is_transparent = void;

        size_t operator()(const Product& p) const {
            return std::hash<std::wstring>()(p.getName());
        }

        size_t operator()(const std::wstring& name) const {
            return std::hash<std::wstring>()(name);
        }
    };

    struct ProductComparatorForName {
        using is_transparent = void;

        bool operator()(const Product& p1, const Product& p2) const {
            return p1.getBarcode() < p2.getBarcode();
        }

        bool operator()(const Product& p, const std::wstring& name) const {
            return p.getName() == name;
        }

        bool operator()(const std::wstring& name, const Product& p) const {
            return p.getName() == name;
        }
    };

    struct ProductHashForBarcode {
        using is_transparent = void;

        size_t operator()(std::unordered_set<Product, ProductHashForName, ProductComparatorForName>::iterator p) const {
            return std::hash<std::size_t>()(p->getBarcode());
        }

        size_t operator()(std::size_t barcode) const {
            return std::hash<std::size_t>()(barcode);
        }

    };
    using ProductMap = std::unordered_set<Product, ProductHashForName, ProductComparatorForName>;
    struct ProductComparatorForBarcode {
        using is_transparent = void;

        bool operator()(ProductMap::iterator p1, std::unordered_set<Product, ProductHashForName, ProductComparatorForName>::iterator p2) const {
            return p1->getBarcode() < p2->getBarcode();
        }

        bool operator()(ProductMap::iterator p, int barcode) const {
            std::cout << "Compare int" << std::endl;
            return p->getBarcode() == barcode;
        }

        bool operator()(int barcode, ProductMap::iterator p) const {
            return p->getBarcode() == barcode;
        }
    };

    class Cash {
    public:
        void addProduct(Product&& productInfo);
        ProductMap::iterator at(size_t barcode);
        ProductMap::iterator at(const std::wstring& name);
        Cash() = default;
        Cash(const Cash& other);
        ProductMap::iterator end();
    private:
        ProductMap nameVsProductMap;
        std::unordered_set<ProductMap::iterator, ProductHashForBarcode, ProductComparatorForBarcode> barcodeVsProductMap;
    };

    class ProductFactory {
    public:
        static Product createProduct(CSVRow csv);
        static std::optional<Cash> createCashFromFile(const std::string& filePath);
    };
}
#endif // CASHREGISTER_H
