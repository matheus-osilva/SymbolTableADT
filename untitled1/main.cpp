#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <utility>

struct Item {
    int occurrences;
    int letters;
    int vowels;
    static Item None(){
        Item value{};
        value.occurrences = -1;
        value.letters = -1;
        value.vowels = -1;
        return value;
    }
};

struct VOEntry {
    std::string key;
    Item value{};
};

//return the number of vowels in given string
int vowels(const std::string& key){
    int numVowels = 0;
    for (char c : key){
        c = tolower(c);
        if (c == 'a' | c == 'e' | c == 'i' | c == 'o' | c == 'u') numVowels += 1;
    }
    return numVowels;
}

struct BSTNode {
    std::string key;
    Item value;
    BSTNode* left;
    BSTNode* right;

    BSTNode(std::string  k, Item v) : key(std::move(k)), value(v) , left(nullptr), right(nullptr) {}
};

class SymbolTableBST {
private:
    BSTNode* root;

    Item findValue(const std::string& key){
        Item value{};
        value.occurrences = 1;
        value.letters = key.size();
        value.vowels = vowels(key);
        return value;
    }

    BSTNode* addRecursive(BSTNode* node, const std::string& key, Item value){

        if (node == nullptr) {
            return new BSTNode(key, value);
        }
        if (key < node->key){
            node->left = addRecursive(node->left, key, value);
        }
        else if (key > node->key){
            node->right = addRecursive(node->right, key, value);
        }
        else if (key == node->key) {
            node->value.occurrences += 1;
        } else node->value = value;
        return node;
    }
    BSTNode* getRecursive(BSTNode* node, const std::string& key) {
        if (node == nullptr || node->key == key)
            return node;
        if (key < node->key)
            return getRecursive(node->left, key);
        else
            return getRecursive(node->right, key);
    }
    void traverseLeft(BSTNode* node) {
        if (node != nullptr) {
            traverseLeft(node->left);
            std::cout << node->key << " : " << std::endl;
            traverseLeft(node->right);
        }
    }

public:
    SymbolTableBST() : root(nullptr) {}

    void put(const std::string& key) {
        Item value = findValue(key);
        root = addRecursive(root, key, value);
    }

    Item get(const std::string& key) {
        BSTNode* node = getRecursive(root, key);
        if (node != nullptr)
            return node->value;
        else
            return Item::None();  // Return a default value indicating key not found
    }
    void printtree(){
        traverseLeft(root);
    }
};








class SymbolTableOV {
private:
    VOEntry* entries;
    size_t capacity;
    size_t size;

public:
    SymbolTableOV() : entries(nullptr), capacity(0), size(0) {}

    ~SymbolTableOV() {
        delete[] entries;
    }
    size_t rank(const std::string& key){
        size_t start = 0;
        size_t end = size - 1;
        while(start <= end){
            size_t mid = (start + end)/2;
            if(entries[mid].key == key) return mid;
            if(entries[mid].key < key) start = mid + 1;
            else end = mid - 1;
        }
        return start;
    }

    void add(const std::string& key) {
        size_t index = rank(key);
        for (size_t i = index; i < size; ++i) {
            if (entries[i].key == key) {
                // Key already exists, update the value
                entries[i].value.occurrences += 1;
                return;
            }
        }
        if (size == capacity) {
            // Array is full, need to resize
            size_t newCapacity = (capacity == 0) ? 1 : capacity * 2;
            auto* newArray = new VOEntry[newCapacity];

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
        Item value{};
        value.letters = key.size();
        value.vowels = vowels(key);
        value.occurrences = 1;
        // Add the new entry
        auto *aux = new VOEntry[size - index];
        for (size_t i = index; i < size; ++i) {
            aux[i - index] = entries[i];
        }
        entries[index] = {key, value};
        for (size_t i = index + 1; i < size + 1; ++i) {
            entries[i] = aux[i - index - 1];
        }
        ++size;
    }
    void display() {
        for (size_t i = 0; i < size; ++i) {
            std::cout << "Word: " << entries[i].key << std:: endl <<
                      "Number of Occurrences: " << entries[i].value.occurrences << std::endl <<
                      "Number of Letters: " << entries[i].value.letters << std::endl <<
                      "Number of Vowels: " << entries[i].value.vowels << std::endl;
        }
    }
};


int main() {
    SymbolTableBST table;

    std::string filePath = R"(C:\Users\Matheus\CLionProjects\untitled1\file.txt)";

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
            table.put(word);
        }
    }
    Item value;
    value = table.get("baixo");
    std::cout << value.occurrences << std::endl;
    inputFile.close();
    table.printtree();
    return 0;
}