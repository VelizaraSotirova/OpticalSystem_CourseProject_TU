#include "OpticalSystem.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <map>
#include <cctype>   

using namespace std;

OpticalSystem::OpticalSystem() : fileManager() {}

void OpticalSystem::displayMenu() const {
    cout << "\n=== OPTICAL MATERIALS MANAGEMENT SYSTEM ===\n";
    cout << "1. Add new supplier\n";
    cout << "2. Display all suppliers\n";
    cout << "3. Add material to supplier\n";
    cout << "4. Create order\n";
    cout << "5. Display current order\n";
    cout << "6. Calculate order price\n";
    cout << "7. File operations\n";
    cout << "8. Exit\n";
    cout << "Select option: ";
}

void OpticalSystem::displayFileMenu() const {
    cout << "\n=== FILE OPERATIONS ===\n";
    cout << "1. Save data to file\n";
    cout << "2. Load data from file\n";
    cout << "3. Check file information\n";
    cout << "4. Return to main menu\n";
    cout << "Select option: ";
}

bool OpticalSystem::handleFileMenu(int choice) {
    switch (choice) {
    case 1: saveData(); break;
    case 2: loadData(); break;
    case 3: checkFileInfo(); break;
    case 4: 
        cout << "Returning to main menu..." << endl; 
        return false;
    default: 
        cout << "Invalid choice!" << endl;
    }

    return true;
}

void OpticalSystem::saveData() {
    cout << "\n=== SAVE DATA ===\n";
    cout << "1. Save as text file (readable)\n";
    cout << "2. Save as binary file (faster)\n";
    cout << "Select format: ";

    int choice = InputHelper::getValidatedInt("");

    FileManager::FileFormat format = (choice == 2) ?
        FileManager::BINARY :
        FileManager::TEXT;

    string filename = InputHelper::getStringInput("Enter filename: ");
    fileManager.setFileName(filename);

    if (fileManager.saveToFile(suppliers, format)) {
        cout << "Data saved successfully to: " << filename << endl;
    }
    else {
        cout << "Error saving data!" << endl;
    }
}

void OpticalSystem::loadData() {
    cout << "\n=== LOAD DATA FROM FILE ===\n";
    string filename = InputHelper::getStringInput("Enter filename to load: ");

    if (!FileManager::fileExists(filename)) {
        cout << "Error: File '" << filename << "' does not exist!\n";
        return;
    }

    cout << "File found. Size: " << FileManager::getFileSize(filename) << " bytes\n";

    cout << "\n1. Load as text file\n";
    cout << "2. Load as binary file\n";
    cout << "3. Preview file content (text only)\n";
    cout << "Select option: ";

    int choice = InputHelper::getValidatedInt("");

    if (choice == 3) {
        previewFileContent(filename);
        return;
    }

    FileManager::FileFormat format = (choice == 2) ?
        FileManager::BINARY :
        FileManager::TEXT;

    vector<Supplier> loadedSuppliers;
    fileManager.setFileName(filename);

    if (fileManager.loadFromFile(loadedSuppliers, format)) {
        displayLoadedData(loadedSuppliers, filename);

        // Asking whether to load into the system
        char confirm;
        cout << "\nDo you want to load this data into the system? (y/n): ";
        cin >> confirm;
        InputHelper::clearInputBuffer();

        if (tolower(confirm) == 'y') {
            suppliers = loadedSuppliers;
            cout << "Data loaded successfully into the system!\n";
        }
        else {
            cout << "Data preview only. Not loaded into system.\n";
        }
    }
    else {
        cout << "Error loading data from file!\n";
    }
}


void OpticalSystem::previewFileContent(const string& filename) {
    cout << "\n=== FILE CONTENT PREVIEW ===\n";
    cout << "File: " << filename << "\n";
    cout << "================================\n";

    ifstream file(filename);
    if (!file) {
        cout << "Cannot open file for reading!\n";
        return;
    }

    string line;
    int lineCount = 0;
    const int MAX_PREVIEW_LINES = 50;

    while (getline(file, line) && lineCount < MAX_PREVIEW_LINES) {
        lineCount++;
        cout << lineCount << ": " << line << "\n";
    }

    file.close();

    if (lineCount >= MAX_PREVIEW_LINES) {
        cout << "... (showing first " << MAX_PREVIEW_LINES << " lines)\n";
    }

    cout << "================================\n";
    cout << "Total lines shown: " << lineCount << "\n";
}


void OpticalSystem::displayLoadedData(const vector<Supplier>& suppliers, const string& filename) {
    cout << "\n=== LOADED DATA FROM '" << filename << "' ===\n";

    if (suppliers.empty()) {
        cout << "File is empty.\n";
        return;
    }

    cout << "Number of suppliers: " << suppliers.size() << "\n";
    cout << "===========================================\n";

    int totalMaterials = 0;
    double totalValue = 0.0;

    for (size_t i = 0; i < suppliers.size(); i++) {
        cout << "\n[" << i + 1 << "] " << suppliers[i].getName() << "\n";
        cout << "   VAT: " << suppliers[i].getBulstat() << "\n";
        cout << "   City: " << suppliers[i].getCity() << "\n";
        cout << "   Phone: " << suppliers[i].getPhone() << "\n";

        int matCount = suppliers[i].getMaterialsCount();
        totalMaterials += matCount;
        cout << "   Materials: " << matCount << "\n";

        double supplierValue = suppliers[i].getTotalMaterialValue();
        totalValue += supplierValue;

        // Show materials
        if (matCount > 0) {
            const auto& materials = suppliers[i].getMaterials();
            for (size_t j = 0; j < materials.size() && j < 3; j++) {
                cout << "     " << j + 1 << ". " << materials[j].getMaterialName()
                    << " (" << materials[j].getType() << ") - "
                    << materials[j].getPrice() << " BGN\n";
            }
            if (matCount > 3) {
                cout << "     ... and " << (matCount - 3) << " more materials\n";
            }
        }

        cout << "   Total value: " << fixed << setprecision(2) << supplierValue << " BGN\n";
    }

    cout << "\n===========================================\n";
    cout << "SUMMARY:\n";
    cout << "Total suppliers: " << suppliers.size() << "\n";
    cout << "Total materials: " << totalMaterials << "\n";
    cout << "Total inventory value: " << fixed << setprecision(2) << totalValue << " BGN\n";
    cout << "===========================================\n";
}



void OpticalSystem::checkFileInfo() {
    cout << "\n=== FILE INFORMATION ===\n";
    string filename = InputHelper::getStringInput("Enter filename: ");

    if (FileManager::fileExists(filename)) {
        cout << "File exists.\n";
        cout << "File name: " << FileManager::getFileName(filename) << endl;
        cout << "Extension: " << FileManager::getFileExtension(filename) << endl;
        cout << "Size: " << FileManager::getFileSize(filename) << " bytes" << endl;
    }
    else {
        cout << "File does not exist." << endl;
    }
}


void OpticalSystem::addSupplier() {
    cout << "\n=== ADD NEW SUPPLIER ===\n";

    string bulstat, name, city, phone;

    // VAT validation
    while (true) {
        bulstat = InputHelper::getStringInput("Enter VAT number: ");

        // Check for empty input
        if (bulstat.empty()) {
            cout << "VAT number cannot be empty! Please try again.\n";
            continue;
        }

        // Length check
        if (bulstat.length() < 9 || bulstat.length() > 13) {
            cout << "Invalid VAT number! Please enter a number with length 9-13 symbols.\n";
            continue;
        }

		// Digit check
        bool allDigits = true;
        for (char c : bulstat) {
            if (!isdigit(static_cast<unsigned char>(c))) {
                allDigits = false;
                break;
            }
        }

        if (!allDigits) {
            cout << "Invalid VAT number! VAT must contain only digits (0-9).\n";
            continue;
        }

		break; // Successful validation
    }

	// Name input validation
    while (true) {
        name = InputHelper::getStringInput("Enter company name: ");

		// Remove leading and trailing spaces
        size_t start = name.find_first_not_of(" \t\n\r\f\v");
        size_t end = name.find_last_not_of(" \t\n\r\f\v");

        if (start == std::string::npos) {
            cout << "Company name cannot be empty or contain only spaces! Please try again.\n";
            continue;
        }

        name = name.substr(start, end - start + 1);

		// Min length check
        if (name.length() < 3) {
            cout << "Company name must be at least 3 characters long! Please try again.\n";
            continue;
        }

        // Check for allowed characters: letters, numbers, spaces, and dashes
        bool validName = true;
        bool hasLetterOrDigit = false;

        for (size_t i = 0; i < name.length(); i++) {
            char c = name[i];

            if (std::isalnum(static_cast<unsigned char>(c))) {
                hasLetterOrDigit = true;
            }
            else if (c == ' ') {
                // Intervals are allowed, but not consecutive intervals
                if (i > 0 && name[i - 1] == ' ') {
                    cout << "Company name cannot have consecutive spaces! Please try again.\n";
                    validName = false;
                    break;
                }
            }
            else if (c == '-') {
				// Dashes are allowed with restrictions:
				// Not in the beginning or end, not consecutive, not next to spaces
                if (i == 0 || i == name.length() - 1) {
                    cout << "Company name cannot start or end with a dash (-)! Please try again.\n";
                    validName = false;
                    break;
                }
                if (i > 0 && name[i - 1] == '-') {
                    cout << "Company name cannot have consecutive dashes (--)! Please try again.\n";
                    validName = false;
                    break;
                }
                if (i > 0 && name[i - 1] == ' ') {
                    cout << "Company name cannot have space before dash! Please try again.\n";
                    validName = false;
                    break;
                }
                if (i < name.length() - 1 && name[i + 1] == ' ') {
                    cout << "Company name cannot have dash before space! Please try again.\n";
                    validName = false;
                    break;
                }
            }
            else if (c == '&') {
                if (i == 0 || i == name.length() - 1) {
                    cout << "Company name cannot start or end with an ampersand (&)! Please try again.\n";
                    validName = false;
                    break;
                }
                if (i > 0 && name[i - 1] == '&') {
                    cout << "Company name cannot have consecutive ampersands (&&)! Please try again.\n";
                    validName = false;
                    break;
                }
                if (i > 0 && name[i - 1] == ' ') {
                    cout << "Company name cannot have space before ampersand! Please try again.\n";
                    validName = false;
                    break;
                }
                if (i < name.length() - 1 && name[i + 1] == ' ') {
                    cout << "Company name cannot have ampersand before space! Please try again.\n";
                    validName = false;
                    break;
                }
            }
            else {
				// Anything else is invalid
                validName = false;
                break;
            }
        }

        if (!validName) {
			// The message is in the specific checks above
            continue;
        }

        if (!hasLetterOrDigit) {
            cout << "Company name must contain at least one letter or digit! Please try again.\n";
            continue;
        }

        if (name.front() == ' ' || name.back() == ' ') {
            cout << "Company name cannot start or end with a space! Please try again.\n";
            continue;
        }

		break; // Successful validation
    }

	// City validation
    while (true) {
        city = InputHelper::getStringInput("Enter city: ");

        
        if (city.empty()) {
            cout << "City cannot be empty! Please try again.\n";
            continue;
        }

        
        if (city.length() < 3) {
            cout << "City name must be at least 3 characters long! Please try again.\n";
            continue;
        }

		// Only letters and spaces allowed
        bool validCity = true;
        bool hasLetter = false;

        for (char c : city) {
            if (isalpha(static_cast<unsigned char>(c))) {
                hasLetter = true;
            }
            else if (c != ' ') {
                validCity = false;
                break;
            }
        }

        if (!validCity) {
            cout << "Invalid city name! Use only letters and spaces.\n";
            continue;
        }

        if (!hasLetter) {
            cout << "City name must contain at least one letter! Please try again.\n";
            continue;
        }

        
        if (!isupper(static_cast<unsigned char>(city[0]))) {
            cout << "Note: City names usually start with a capital letter.\n";
			// Warning only, not an error
        }

		break; // Successful validation
    }

	// Phone number validation
    while (true) {
        phone = InputHelper::getStringInput("Enter phone number: ");

        // Проверка за празен вход
        if (phone.empty()) {
            cout << "Phone number cannot be empty! Please try again.\n";
            continue;
        }

		// Check for valid characters and count digits
        bool validChars = true;
        int digitCount = 0;

        for (char c : phone) {
            if (isdigit(static_cast<unsigned char>(c))) {
                digitCount++;
            }
            else if (c != ' ' && c != '+' && c != '-') {
                validChars = false;
                break;
            }
        }

        if (!validChars) {
            cout << "Invalid phone number! Only digits, spaces, + and - are allowed.\n";
            continue;
        }

        if (digitCount < 5 || digitCount > 15) {
            cout << "Invalid phone number! Must be between 5-15 digits.\n";
            continue;
        }

		break; // Successful validation
    }

    try {
        Supplier newSupplier(bulstat, name, city, phone);
        suppliers.push_back(newSupplier);

        cout << "\nSupplier added successfully!\n";
        cout << "VAT: " << bulstat << "\n";
        cout << "Name: " << name << "\n";
        cout << "City: " << city << "\n";
        cout << "Phone: " << phone << "\n";

    }
    catch (const invalid_argument& e) {
        cout << "Error creating supplier: " << e.what() << "\n";
    }
}

void OpticalSystem::displayAllSuppliers() const {
    std::cout << "\n=== ALL SUPPLIERS ===\n";
    if (suppliers.empty()) {
        std::cout << "No registered suppliers.\n";
        return;
    }

    for (size_t i = 0; i < suppliers.size(); i++) {
        std::cout << "\n[" << i + 1 << "] " << suppliers[i] << "\n";
    }
}

void OpticalSystem::addMaterialToSupplier() {
    if (suppliers.empty()) {
        std::cout << "No registered suppliers!\n";
        return;
    }

    displayAllSuppliers();
    int supplierIndex = InputHelper::getValidatedInt("Select supplier number: ") - 1;

    if (supplierIndex < 0 || static_cast<size_t>(supplierIndex) >= suppliers.size()) {
        std::cout << "Invalid supplier number!\n";
        return;
    }

    std::cout << "\n=== ADD MATERIAL ===\n";

	// Type validation
    std::string type;
    while (true) {
        type = InputHelper::getStringInput("Enter material type: ");

		// Removing leading and trailing spaces
        size_t start = type.find_first_not_of(" \t\n\r");
        size_t end = type.find_last_not_of(" \t\n\r");

        if (start != std::string::npos) {
            type = type.substr(start, end - start + 1);
        }

        
        if (type.empty()) {
            std::cout << "Material type cannot be empty! Please try again.\n";
            continue;
        }

		// Minimum length check
        if (type.length() < 3) {
            std::cout << "Material type must be at least 3 characters long! Please try again.\n";
            continue;
        }

		// All letters check
        bool allLetters = true;
        for (char c : type) {
            if (!std::isalpha(static_cast<unsigned char>(c))) {
                allLetters = false;
                break;
            }
        }

        if (!allLetters) {
            std::cout << "Material type can only contain letters! Please try again.\n";
            continue;
        }

		break; // Successful validation
    }

	// Thickness validation (positive number)
    double thickness = 0.0;
    while (true) {
        try {
            thickness = InputHelper::getValidatedDouble("Enter thickness (mm): ");
            
            if (thickness <= 0) {
                std::cout << "Thickness must be a positive number! Please try again.\n";
                continue;
            }
            break;
        }
        catch (const std::exception&) {
            std::cout << "Invalid input for thickness! Please enter a valid number.\n";
            continue;
        }
    }

	// Diopter validation (any double)
    double diopter = 0.0;
    while (true) {
        try {
            diopter = InputHelper::getValidatedDouble("Enter diopter: ");
            break;
        }
        catch (const std::exception&) {
            std::cout << "Invalid input for diopter! Please enter a valid number.\n";
            continue;
        }
    }

	// Material name validation 
    std::string materialName;
    while (true) {
        materialName = InputHelper::getStringInput("Enter material name: ");

		// Removing leading and trailing spaces
        size_t start = materialName.find_first_not_of(" \t\n\r");
        size_t end = materialName.find_last_not_of(" \t\n\r");

        if (start != std::string::npos) {
            materialName = materialName.substr(start, end - start + 1);
        }

        
        if (materialName.empty()) {
            std::cout << "Material name cannot be empty! Please try again.\n";
            continue;
        }

		// Minimum length check
        if (materialName.length() < 3) {
            std::cout << "Material name must be at least 3 characters long! Please try again.\n";
            continue;
        }

		// All letters check
        bool allLetters = true;
        for (char c : materialName) {
            if (!std::isalpha(static_cast<unsigned char>(c))) {
                allLetters = false;
                break;
            }
        }

        if (!allLetters) {
            std::cout << "Material name can only contain letters! Please try again.\n";
            continue;
        }

		break; // Successful validation
    }

	// Price validation (positive number)
    double price = 0.0;
    while (true) {
        try {
            price = InputHelper::getValidatedDouble("Enter price: ");

            if (price <= 0) {
                std::cout << "Price must be a positive number! Please try again.\n";
                continue;
            }
            break;
        }
        catch (const std::exception&) {
            std::cout << "Invalid input for price! Please enter a valid number.\n";
            continue;
        }
    }

    try {
		// Material creation - the constructor will do final validation
        OpticalMaterial newMaterial(type, thickness, diopter, materialName, price);
        suppliers[supplierIndex].addMaterial(newMaterial);

        std::cout << "\nMaterial added successfully!\n";
        std::cout << "Type: " << type << "\n";
        std::cout << "Thickness: " << thickness << " mm\n";
        std::cout << "Diopter: " << diopter << "\n";
        std::cout << "Name: " << materialName << "\n";
        std::cout << "Price: " << price << " BGN\n";
    }
    catch (const std::invalid_argument& e) {
        std::cout << "Error creating material: " << e.what() << "\n";
        std::cout << "Please try again with valid values.\n";
    }
}

void OpticalSystem::createOrder() {
    if (suppliers.empty()) {
        std::cout << "No registered suppliers!\n";
        return;
    }

    displayAllSuppliers();
    int supplierIndex = InputHelper::getValidatedInt("Select supplier number: ") - 1;

    if (supplierIndex < 0 || static_cast<size_t>(supplierIndex) >= suppliers.size()) {
        std::cout << "Invalid supplier number!\n";
        return;
    }

    Supplier* selectedSupplier = &suppliers[supplierIndex];

    if (selectedSupplier->getMaterialsCount() == 0) {
        std::cout << "This supplier has no materials!\n";
        return;
    }

    currentOrder.setSupplier(selectedSupplier);

    std::cout << "\nSupplier's materials:\n";
    for (int i = 0; i < selectedSupplier->getMaterialsCount(); i++) {
        OpticalMaterial* material = selectedSupplier->getMaterial(i);
        std::cout << i + 1 << ". " << *material << "\n";
    }

    while (true) {
        std::cout << "\nAdd material to order (0 to finish): ";
        int choice = InputHelper::getValidatedInt("Select material number: ");

        if (choice == 0) break;

        if (choice < 1 || choice > selectedSupplier->getMaterialsCount()) {
            std::cout << "Invalid material number!\n";
            continue;
        }

        int quantity = InputHelper::getValidatedInt("Enter quantity: ");

        OpticalMaterial* selectedMaterial = selectedSupplier->getMaterial(choice - 1);
        currentOrder.addMaterial(selectedMaterial, quantity);

        std::cout << "Material added to order!\n";
    }

    std::cout << "\nOrder created successfully!\n";
}

void OpticalSystem::displayCurrentOrder() const {
    std::cout << "\n=== CURRENT ORDER ===\n";
    std::cout << currentOrder << "\n";
}

void OpticalSystem::calculateOrderPrice() const {
    if (currentOrder.getSupplier() == nullptr) {
        std::cout << "No order created!\n";
        return;
    }

    double total = currentOrder.calculateTotalPrice();
    std::cout << "\nTotal order price: " << total << " BGN\n";
}



void OpticalSystem::run() {
    int choice;

    do {
        displayMenu();

        if (!(std::cin >> choice)) {
            InputHelper::clearInputBuffer();
            choice = 0;
        }
        InputHelper::clearInputBuffer();

        switch (choice) {
        case 1: addSupplier(); break;
        case 2: displayAllSuppliers(); break;
        case 3: addMaterialToSupplier(); break;
        case 4: createOrder(); break;
        case 5: displayCurrentOrder(); break;
        case 6: calculateOrderPrice(); break;
        case 7: {
            int fileChoice;
			bool continueFileMenu = true;
            do {
                displayFileMenu();
                fileChoice = InputHelper::getValidatedInt("");
                continueFileMenu = handleFileMenu(fileChoice);
            } while (continueFileMenu);
            break;
        }
        case 8: std::cout << "Goodbye!\n"; break;
        default: std::cout << "Invalid choice! Please try again.\n";
        }
    } while (choice != 8);
}