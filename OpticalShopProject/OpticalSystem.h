#ifndef OPTICALSYSTEM_H
#define OPTICALSYSTEM_H

#include <vector>
#include "Supplier.h"
#include "Order.h"
#include "InputHelper.h"
#include "FileManager.h"

class OpticalSystem {
private:
    std::vector<Supplier> suppliers;
    Order currentOrder;
    FileManager fileManager;

    void displayFileMenu() const;
    bool handleFileMenu(int choice);

    void saveData();
    void loadData();
    void previewFileContent(const std::string& filename);
    void displayLoadedData(const std::vector<Supplier>& suppliers, const std::string& filename);
    void checkFileInfo();


public:
    OpticalSystem();

    void displayMenu() const;
    void addSupplier();
    void displayAllSuppliers() const;
    void addMaterialToSupplier();
    void createOrder();
    void displayCurrentOrder() const;
    void calculateOrderPrice() const;

    void run();
};

#endif