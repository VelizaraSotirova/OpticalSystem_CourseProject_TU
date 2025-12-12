#ifndef SUPPLIER_H
#define SUPPLIER_H

#include <iostream>
#include <string>
#include <vector>
#include "OpticalMaterial.h"

class Supplier {
private:
    std::string bulstat;
    std::string name;
    std::string city;
    std::string phone;
    std::vector<OpticalMaterial> materials;

public:
    Supplier();
    Supplier(std::string b, std::string n, std::string c, std::string p);

    std::string getBulstat() const;
    std::string getName() const;
    std::string getCity() const;
    std::string getPhone() const;
    const std::vector<OpticalMaterial>& getMaterials() const;

    void setBulstat(std::string b);
    void setName(std::string n);
    void setCity(std::string c);
    void setPhone(std::string p);

    void addMaterial(const OpticalMaterial& material);
    bool removeMaterial(size_t index);
    OpticalMaterial* getMaterial(size_t index);
    int getMaterialCount() const;

    bool saveToBinary(std::ostream& out) const;
    bool loadFromBinary(std::istream& in);
    bool saveToText(std::ostream& out) const;
    bool loadFromText(std::istream& in);

    bool isValid() const;

    int getMaterialsCount() const;
    double getTotalMaterialValue() const;

    static bool isValidPhone(const std::string& phone);
    static bool isValidBulstat(const std::string& bulstat);
    static bool isBinaryFile(const std::string& filename);
    static bool isTextFile(const std::string& filename);

    friend std::ostream& operator<<(std::ostream& os, const Supplier& s);
};

#endif