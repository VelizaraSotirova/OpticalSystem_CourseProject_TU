#include "OpticalMaterial.h"
#include <stdexcept>
#include <cstring>

OpticalMaterial::OpticalMaterial()
    : type(""), thickness(0.0), diopter(0.0), materialName(""), price(0.0) {
}

OpticalMaterial::OpticalMaterial(std::string t, double th, double d, std::string m, double p)
    : type(t), thickness(th), diopter(d), materialName(m), price(p) {
    if (thickness <= 0) throw std::invalid_argument("Thickness must be positive");
    if (price <= 0) throw std::invalid_argument("Price must be positive");
}

std::string OpticalMaterial::getType() const { return type; }
double OpticalMaterial::getThickness() const { return thickness; }
double OpticalMaterial::getDiopter() const { return diopter; }
std::string OpticalMaterial::getMaterialName() const { return materialName; }
double OpticalMaterial::getPrice() const { return price; }

void OpticalMaterial::setType(std::string t) { type = t; }
void OpticalMaterial::setThickness(double th) {
    if (th <= 0) throw std::invalid_argument("Thickness must be positive");
    thickness = th;
}
void OpticalMaterial::setDiopter(double d) { diopter = d; }
void OpticalMaterial::setMaterialName(std::string m) { materialName = m; }
void OpticalMaterial::setPrice(double p) {
    if (p <= 0) throw std::invalid_argument("Price must be positive");
    price = p;
}

bool OpticalMaterial::isValid() const {
    return (thickness > 0 && price > 0 && !materialName.empty());
}

bool OpticalMaterial::saveToBinary(std::ostream& out) const {
    try {
        size_t len = type.length();
        out.write(reinterpret_cast<const char*>(&len), sizeof(len));
        out.write(type.c_str(), len);

        out.write(reinterpret_cast<const char*>(&thickness), sizeof(thickness));
        out.write(reinterpret_cast<const char*>(&diopter), sizeof(diopter));

        len = materialName.length();
        out.write(reinterpret_cast<const char*>(&len), sizeof(len));
        out.write(materialName.c_str(), len);

        out.write(reinterpret_cast<const char*>(&price), sizeof(price));

        return out.good();
    }
    catch (...) {
        return false;
    }
}

bool OpticalMaterial::loadFromBinary(std::istream& in) {
    try {
        size_t len;
        if (!in.read(reinterpret_cast<char*>(&len), sizeof(len))) return false;

        char* buffer = new char[len + 1];
        if (!in.read(buffer, len)) {
            delete[] buffer;
            return false;
        }
        buffer[len] = '\0';
        type = buffer;
        delete[] buffer;

        if (!in.read(reinterpret_cast<char*>(&thickness), sizeof(thickness))) return false;
        if (!in.read(reinterpret_cast<char*>(&diopter), sizeof(diopter))) return false;

        if (!in.read(reinterpret_cast<char*>(&len), sizeof(len))) return false;

        buffer = new char[len + 1];
        if (!in.read(buffer, len)) {
            delete[] buffer;
            return false;
        }
        buffer[len] = '\0';
        materialName = buffer;
        delete[] buffer;

        if (!in.read(reinterpret_cast<char*>(&price), sizeof(price))) return false;

        if (!isValid()) return false;

        return true;
    }
    catch (...) {
        return false;
    }
}

bool OpticalMaterial::saveToText(std::ostream& out) const {
    try {
        out << type << "\n"
            << thickness << "\n"
            << diopter << "\n"
            << materialName << "\n"
            << price << "\n";
        return out.good();
    }
    catch (...) {
        return false;
    }
}

bool OpticalMaterial::loadFromText(std::istream& in) {
    try {
        std::getline(in, type);
        in >> thickness;
        in.ignore();
        in >> diopter;
        in.ignore();
        std::getline(in, materialName);
        in >> price;
        in.ignore();

        if (!isValid()) return false;

        return in.good();
    }
    catch (...) {
        return false;
    }
}

std::ostream& operator<<(std::ostream& os, const OpticalMaterial& om) {
    os << "Type: " << om.type
        << ", Thickness: " << om.thickness << " mm"
        << ", Diopter: " << om.diopter
        << ", Material: " << om.materialName
        << ", Price: " << om.price << " BGN";
    return os;
}

std::istream& operator>>(std::istream& is, OpticalMaterial& om) {
    std::getline(is, om.type);
    is >> om.thickness;
    is.ignore();
    is >> om.diopter;
    is.ignore();
    std::getline(is, om.materialName);
    is >> om.price;
    is.ignore();
    return is;
}

bool OpticalMaterial::operator==(const OpticalMaterial& other) const {
    return (type == other.type &&
        thickness == other.thickness &&
        diopter == other.diopter &&
        materialName == other.materialName &&
        price == other.price);
}