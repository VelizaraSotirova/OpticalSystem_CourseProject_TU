#include "Order.h"
#include <stdexcept>

Order::Order() : supplier(nullptr) {}
Order::Order(Supplier* s) : supplier(s) {}

Supplier* Order::getSupplier() const { return supplier; }
const std::vector<OpticalMaterial*>& Order::getMaterials() const { return materials; }
const std::vector<int>& Order::getQuantities() const { return quantities; }

void Order::setSupplier(Supplier* s) {
    supplier = s;
    clearOrder();
}

void Order::addMaterial(OpticalMaterial* material, int quantity) {
    if (quantity <= 0) throw std::invalid_argument("Quantity must be positive");
    materials.push_back(material);
    quantities.push_back(quantity);
}

bool Order::removeMaterial(size_t index) {
    if (index >= materials.size()) return false;
    materials.erase(materials.begin() + static_cast<std::ptrdiff_t>(index));
    quantities.erase(quantities.begin() + static_cast<std::ptrdiff_t>(index));
    return true;
}

double Order::calculateTotalPrice() const {
    double total = 0.0;
    for (size_t i = 0; i < materials.size(); i++) {
        total += materials[i]->getPrice() * quantities[i];
    }
    return total;
}

void Order::clearOrder() {
    materials.clear();
    quantities.clear();
}

std::ostream& operator<<(std::ostream& os, const Order& o) {
    if (o.supplier == nullptr) {
        os << "No supplier selected for the order!";
        return os;
    }

    os << "Order to: " << o.supplier->getName() << "\n";
    os << "Order items:\n";

    double total = 0.0;
    for (size_t i = 0; i < o.materials.size(); i++) {
        double itemTotal = o.materials[i]->getPrice() * o.quantities[i];
        os << "  " << i + 1 << ". " << *(o.materials[i])
            << " x " << o.quantities[i] << " = " << itemTotal << " BGN\n";
        total += itemTotal;
    }

    os << "Total amount: " << total << " BGN\n";
    return os;
}