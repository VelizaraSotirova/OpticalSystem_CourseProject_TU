#ifndef ORDER_H
#define ORDER_H

#include <iostream>
#include <vector>
#include "Supplier.h"

class Order {
private:
    Supplier* supplier;
    std::vector<OpticalMaterial*> materials;
    std::vector<int> quantities;

public:
    Order();
    Order(Supplier* s);

    Supplier* getSupplier() const;
    const std::vector<OpticalMaterial*>& getMaterials() const;
    const std::vector<int>& getQuantities() const;

    void setSupplier(Supplier* s);

    void addMaterial(OpticalMaterial* material, int quantity);
    bool removeMaterial(size_t index);
    double calculateTotalPrice() const;
    void clearOrder();

    friend std::ostream& operator<<(std::ostream& os, const Order& o);
};

#endif