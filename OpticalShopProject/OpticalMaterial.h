#ifndef OPTICALMATERIAL_H
#define OPTICALMATERIAL_H

#include <iostream>
#include <string>

class OpticalMaterial {
private:
    std::string type;
    double thickness;
    double diopter;
    std::string materialName;
    double price;

public:
    OpticalMaterial();
    OpticalMaterial(std::string t, double th, double d, std::string m, double p);

    std::string getType() const;
    double getThickness() const;
    double getDiopter() const;
    std::string getMaterialName() const;
    double getPrice() const;

    void setType(std::string t);
    void setThickness(double th);
    void setDiopter(double d);
    void setMaterialName(std::string m);
    void setPrice(double p);

    bool isValid() const;

    bool saveToBinary(std::ostream& out) const;
    bool loadFromBinary(std::istream& in);
    bool saveToText(std::ostream& out) const;
    bool loadFromText(std::istream& in);

    friend std::ostream& operator<<(std::ostream& os, const OpticalMaterial& om);
    friend std::istream& operator>>(std::istream& is, OpticalMaterial& om);

    bool operator==(const OpticalMaterial& other) const;
};

#endif
