#include "Supplier.h"
#include <fstream>
#include <stdexcept>
#include <algorithm>
#include <cctype>
#include <cstring>

Supplier::Supplier() : bulstat(""), name(""), city(""), phone("") {}

Supplier::Supplier(std::string b, std::string n, std::string c, std::string p)
    : bulstat(b), name(n), city(c), phone(p) {
    if (!isValidBulstat(b)) throw std::invalid_argument("Invalid VAT number");
    if (!isValidPhone(p)) throw std::invalid_argument("Invalid phone number");
}

std::string Supplier::getBulstat() const { return bulstat; }
std::string Supplier::getName() const { return name; }
std::string Supplier::getCity() const { return city; }
std::string Supplier::getPhone() const { return phone; }
const std::vector<OpticalMaterial>& Supplier::getMaterials() const { return materials; }

void Supplier::setBulstat(std::string b) {
    if (!isValidBulstat(b)) throw std::invalid_argument("Invalid VAT number");
    bulstat = b;
}
void Supplier::setName(std::string n) { name = n; }
void Supplier::setCity(std::string c) { city = c; }
void Supplier::setPhone(std::string p) {
    if (!isValidPhone(p)) throw std::invalid_argument("Invalid phone number");
    phone = p;
}

void Supplier::addMaterial(const OpticalMaterial& material) {
    materials.push_back(material);
}

bool Supplier::removeMaterial(size_t index) {
    if (index >= materials.size()) return false;
    materials.erase(materials.begin() + static_cast<std::ptrdiff_t>(index));
    return true;
}

OpticalMaterial* Supplier::getMaterial(size_t index) {
    if (index >= materials.size()) return nullptr;
    return &materials[index];
}

int Supplier::getMaterialCount() const {
    return static_cast<int>(materials.size());
}

bool Supplier::saveToBinary(std::ostream& out) const {
    try {
        size_t len = bulstat.length();
        out.write(reinterpret_cast<const char*>(&len), sizeof(len));
        out.write(bulstat.c_str(), len);

        len = name.length();
        out.write(reinterpret_cast<const char*>(&len), sizeof(len));
        out.write(name.c_str(), len);

        len = city.length();
        out.write(reinterpret_cast<const char*>(&len), sizeof(len));
        out.write(city.c_str(), len);

        len = phone.length();
        out.write(reinterpret_cast<const char*>(&len), sizeof(len));
        out.write(phone.c_str(), len);

        size_t count = materials.size();
        out.write(reinterpret_cast<const char*>(&count), sizeof(count));

        for (const auto& material : materials) {
            if (!material.saveToBinary(out)) return false;
        }

        return out.good();
    }
    catch (...) {
        return false;
    }
}

bool Supplier::loadFromBinary(std::istream& in) {
    try {
        size_t len;

        if (!in.read(reinterpret_cast<char*>(&len), sizeof(len))) return false;
        char* buffer = new char[len + 1];
        if (!in.read(buffer, len)) {
            delete[] buffer;
            return false;
        }
        buffer[len] = '\0';
        bulstat = buffer;
        delete[] buffer;

        if (!in.read(reinterpret_cast<char*>(&len), sizeof(len))) return false;
        buffer = new char[len + 1];
        if (!in.read(buffer, len)) {
            delete[] buffer;
            return false;
        }
        buffer[len] = '\0';
        name = buffer;
        delete[] buffer;

        if (!in.read(reinterpret_cast<char*>(&len), sizeof(len))) return false;
        buffer = new char[len + 1];
        if (!in.read(buffer, len)) {
            delete[] buffer;
            return false;
        }
        buffer[len] = '\0';
        city = buffer;
        delete[] buffer;

        if (!in.read(reinterpret_cast<char*>(&len), sizeof(len))) return false;
        buffer = new char[len + 1];
        if (!in.read(buffer, len)) {
            delete[] buffer;
            return false;
        }
        buffer[len] = '\0';
        phone = buffer;
        delete[] buffer;

        size_t count;
        if (!in.read(reinterpret_cast<char*>(&count), sizeof(count))) return false;

        materials.clear();
        for (size_t i = 0; i < count; i++) {
            OpticalMaterial material;
            if (!material.loadFromBinary(in)) return false;
            materials.push_back(material);
        }

        if (!isValid()) {
            materials.clear();
            return false;
        }

        return true;
    }
    catch (...) {
        return false;
    }
}

bool Supplier::saveToText(std::ostream& out) const {
    try {
        out << "=== SUPPLIER ===\n";
        out << bulstat << "\n"
            << name << "\n"
            << city << "\n"
            << phone << "\n"
            << materials.size() << "\n";

        for (const auto& material : materials) {
            if (!material.saveToText(out)) return false;
        }

        return out.good();
    }
    catch (...) {
        return false;
    }
}

bool Supplier::loadFromText(std::istream& in) {
    try {
        std::getline(in, bulstat);
        std::getline(in, name);
        std::getline(in, city);
        std::getline(in, phone);

        size_t count;
        in >> count;
        in.ignore();

        materials.clear();
        for (size_t i = 0; i < count; i++) {
            OpticalMaterial material;
            if (!material.loadFromText(in)) return false;
            materials.push_back(material);
        }

        if (!isValid()) {
            materials.clear();
            return false;
        }

        return in.good();
    }
    catch (...) {
        return false;
    }
}

bool Supplier::isValid() const {
    if (!isValidBulstat(bulstat)) return false;
    if (!isValidPhone(phone)) return false;
    if (name.empty() || city.empty()) return false;

    for (const auto& material : materials) {
        if (!material.isValid()) return false;
    }

    return true;
}

int Supplier::getMaterialsCount() const {
    return static_cast<int>(materials.size());
}

double Supplier::getTotalMaterialValue() const {
    double total = 0.0;
    for (const auto& material : materials) {
        total += material.getPrice();
    }
    return total;
}

bool Supplier::isValidPhone(const std::string& phone) {
    if (phone.empty()) return false;

    for (char c : phone) {
        if (!std::isdigit(c) && c != ' ' && c != '+' && c != '-') return false;
    }

    int digitCount = 0;
    for (char c : phone) {
        if (std::isdigit(c)) digitCount++;
    }

    return digitCount >= 5;
}

bool Supplier::isValidBulstat(const std::string& bulstat) {
    if (bulstat.length() < 9 || bulstat.length() > 13) return false;

    for (char c : bulstat) {
        if (!std::isdigit(c)) return false;
    }

    return true;
}

bool Supplier::isBinaryFile(const std::string& filename) {
    std::ifstream file(filename, std::ios::binary);
    if (!file) return false;

    size_t len;
    if (!file.read(reinterpret_cast<char*>(&len), sizeof(len))) return false;

    if (len > 1000) return false;

    return true;
}

bool Supplier::isTextFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file) return false;

    std::string line;
    std::getline(file, line);

    for (char c : line) {
        if (!std::isprint(c) && c != '\n' && c != '\r' && c != '\t') return false;
    }

    return true;
}

std::ostream& operator<<(std::ostream& os, const Supplier& s) {
    os << "Supplier: " << s.name << "\n"
        << "VAT: " << s.bulstat << "\n"
        << "City: " << s.city << "\n"
        << "Phone: " << s.phone << "\n"
        << "Materials (" << s.materials.size() << "):\n";

    for (size_t i = 0; i < s.materials.size(); i++) {
        os << "  " << i + 1 << ". " << s.materials[i] << "\n";
    }
    return os;
}