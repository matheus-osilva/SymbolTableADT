#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

struct Item {
    int occurrences;
    int letters;
    int vowels;
};

struct SymbolEntry {
    std::string name;
    Item value;
};

class SymbolTable {
private:
    SymbolEntry* entries;
    size_t capacity;
    size_t size;

public:
    SymbolTable() : entries(nullptr), capacity(0), size(0) {}

    ~SymbolTable() {
        delete[] entries;
    }

    void add(const std::string& word) {
        for (size_t i = 0; i < size; ++i) {
            if (entries[i].name == word) {
                // Key already exists, update the value
                entries[i].value.occurrences += 1;
                return;
            }
        }
        if (size == capacity) {
            // Array is full, need to resize
            size_t newCapacity = (capacity == 0) ? 1 : capacity * 2;
            SymbolEntry* newArray = new SymbolEntry[newCapacity];

            // Copy existing elements to the new array
            for (size_t i = 0; i < size; ++i) {
                newArray[i] = entries[i];
            }

            // Deallocate the old array
            delete[] entries;

            // Update capacity and entries pointer
            entries = newArray;
            capacity = newCapacity;
        }
        Item value;
        value.letters = word.size();
        value.vowels = 0;
        for (char c : word){
            c = tolower(c);
            if (c == 'a' | c == 'e' | c == 'i' | c == 'o' | c == 'u') value.vowels += 1;
        }
        value.occurrences = 1;
        // Add the new entry
        entries[size] = { word, value };
        ++size;
    }
    void addWithOccurrences(const std::string& word, int occurrences) {
        for (size_t i = 0; i < size; ++i) {
            if (entries[i].name == word) return;
        }
        if (size == capacity) {
            // Array is full, need to resize
            size_t newCapacity = (capacity == 0) ? 1 : capacity * 2;
            SymbolEntry* newArray = new SymbolEntry[newCapacity];

            // Copy existing elements to the new array
            for (size_t i = 0; i < size; ++i) {
                newArray[i] = entries[i];
            }

            // Deallocate the old array
            delete[] entries;

            // Update capacity and entries pointer
            entries = newArray;
            capacity = newCapacity;
        }
        Item value;
        value.letters = word.size();
        value.vowels = 0;
        for (char c : word){
            c = tolower(c);
            if (c == 'a' | c == 'e' | c == 'i' | c == 'o' | c == 'u') value.vowels += 1;
        }
        value.occurrences = occurrences;
        // Add the new entry
        entries[size] = { word, value };
        ++size;
    }
    void mostOccurred(){
        SymbolTable aux;
        int greater = 0;
        int j = 0;
        for (size_t i = 0; i < size; ++i){
            if (entries[i].value.occurrences > greater) greater = entries[i].value.occurrences;
        }
        for (size_t i = 0; i < size; ++i){
            if (entries[i].value.occurrences >= greater){
                aux.addWithOccurrences(entries[i].name, greater);
                ++j;
            }
        }
        aux.display();
    }

    Item value(const std::string& name) const {
        for (size_t i = 0; i < size; ++i) {
            if (entries[i].name == name) {
                return entries[i].value;
            }
        }
        Item null;
        null.letters = -1;
        null.occurrences = -1;
        null.vowels = -1;
        return null; // Symbol not found
    }

    void display() const {
        for (size_t i = 0; i < size; ++i) {
            std::cout << "Word: " << entries[i].name << std:: endl <<
            "Number of Occurrences: " << entries[i].value.occurrences << std::endl <<
            "Number of Letters: " << entries[i].value.letters << std::endl <<
            "Number of Vowels: " << entries[i].value.vowels << std::endl;
        }
    }
};

int main() {
    SymbolTable table;

    std::string filePath = R"(/home/bmac/matheusnz/Documentos/ep2mac0323/file.txt)";

    std::ifstream inputFile(filePath);

    if (!inputFile.is_open()) {
        std::cout << "Failed to open the file." << std::endl;
        return 1;
    }

    std::string line;

    while (std::getline(inputFile, line)) {
        std::stringstream ss(line);
        std::string word;

        while (ss >> word) {
            table.add(word);
        }
    }
    inputFile.close();
    table.mostOccurred();
    return 0;
}