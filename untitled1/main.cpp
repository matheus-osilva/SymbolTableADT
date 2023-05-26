#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <utility>
#include <random>
#include <stack>
#include <queue>
#include <unordered_set>

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

bool hasRepeatedLetters(const std::string& str) {
    std::unordered_set<char> uniqueLetters;
    for (char c : str) {
        if (uniqueLetters.find(c) != uniqueLetters.end()) {
            return true;  // Found a repeated letter
        }
        uniqueLetters.insert(c);
    }
    return false;  // No repeated letters found
}

//return the number of vowels in given string
int vowels(const std::string& key){
    int numVowels = 0;
    for (char c : key){
        c = tolower(c);
        if (c == 'a' | c == 'e' | c == 'i' | c == 'o' | c == 'u') numVowels += 1;
    }
    return numVowels;
}
static Item findValue(const std::string& key){
    Item value{};
    value.occurrences = 1;
    value.letters = key.size();
    value.vowels = vowels(key);
    return value;
}

template<typename NodeType>
void greatOccurrences(NodeType* root, std::vector<NodeType*>& maxNodes, int& maxOccurrences) {
    if (root != nullptr) {
        greatOccurrences(root->left, maxNodes, maxOccurrences);

        // Check if the current node has more occurrences
        if (root->value.occurrences > maxOccurrences) {
            maxOccurrences = root->value.occurrences;
            maxNodes.clear(); // Clear previous max nodes
            maxNodes.push_back(root);
        } else if (root->value.occurrences == maxOccurrences) {
            maxNodes.push_back(root);
        }

        greatOccurrences(root->right, maxNodes, maxOccurrences);
    }
}

template<typename NodeType>
void greatLetters(NodeType *root, std::vector<NodeType *> &maxNodes, int &maxLetters) {
    if (root != nullptr) {
        greatLetters(root->left, maxNodes, maxLetters);

        // Check if the current node has more occurrences
        if (root->value.letters > maxLetters) {
            maxLetters = root->value.letters;
            maxNodes.clear(); // Clear previous max nodes
            maxNodes.push_back(root);
        } else if (root->value.letters == maxLetters) {
            maxNodes.push_back(root);
        }

        greatLetters(root->right, maxNodes, maxLetters);
    }
}

template<typename NodeType>
void sr(NodeType *root, std::vector<NodeType *> &maxNodes, int &maxLetters) {
    if (root != nullptr) {
        sr(root->left, maxNodes, maxLetters);

        // Check if the current node has more occurrences
        if (root->value.letters > maxLetters) {
            if(!hasRepeatedLetters(root->key)){
                maxLetters = root->value.letters;
                maxNodes.clear(); // Clear previous max nodes
                maxNodes.push_back(root);
            }

        } else if (root->value.letters == maxLetters) {
            maxNodes.push_back(root);
        }

        sr(root->right, maxNodes, maxLetters);
    }
}

template<typename NodeType>
void vd(std::vector<NodeType*>& minNodes) {
    int lowestLetters = INT_MAX;
    // Find the lowest value of letters
    for (const auto& node : minNodes) {
        if (node->value.letters < lowestLetters) {
            lowestLetters = node->value.letters;
        }
    }
    // Erase elements with letters greater than the lowest value
    auto it = minNodes.begin();
    while (it != minNodes.end()) {
        if ((*it)->value.letters > lowestLetters) {
            it = minNodes.erase(it);
        } else {
            ++it;
        }
    }
}



template<typename NodeType>
void mostVowels(NodeType *root, std::vector<NodeType *> &maxNodes, int &maxVowels) {
    if (root != nullptr) {
        mostVowels(root->left, maxNodes, maxVowels);

        // Check if the current node has more vowels
        if (root->value.vowels > maxVowels && !hasRepeatedLetters(root->key)) {
            maxVowels = root->value.vowels;
            maxNodes.clear(); // Clear previous max nodes
            maxNodes.push_back(root);
        } else if (root->value.vowels == maxVowels && !hasRepeatedLetters(root->key)) {
            maxNodes.push_back(root);
        }
        mostVowels(root->right, maxNodes, maxVowels);
    }
}

template<typename TableType>
void consultas_gerais(std::vector<std::string> consultas, TableType& table){
    using namespace std;
    for (string consult : consultas) {
        const char o = 'O';
        // Acha as palavras com maiores ocorrências no texto
        if (consult == "F") {
            cout << endl;
            cout << "Maior(es) ocorrencia(s) no texto: " << endl;
            table.mostOccurred();
            cout << endl;
        }
        else if (consult[0] == o) {
            cout << endl;
            string key;
            size_t spacePos = consult.find(' ');
            if (spacePos != std::string::npos) {
                // Extract the substring starting from the character after the space
                key = consult.substr(spacePos + 1);
            }
            Item value;
            value = table.get(key);
            if (value.occurrences < 1) cout << "A palavra " << key << " nao aparece no texto" << endl;
            else cout << "A palavra " << key << " aparece " << value.occurrences << " vezes no texto" << endl;
        }
        else if (consult == "L") {
            cout << endl;
            cout << "Palavra mais longa: ";
            table.longer();
            cout << endl;
        }
        else if (consult == "SR") {
            cout << endl;
            cout << "Palavra mais longa sem letras repetidas: ";
            table.longerwithnorepeatedletters();
            cout << endl;
        }
        else if (consult == "VD") {
            cout << endl;
            cout << "Palavra mais curta sem repeticao e com mais vogais: ";
            table.shorterwithmostvowel();
            cout << endl;
        }
        else break;
    }
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
            std::cout << node->key << " : " << node->value.vowels << std::endl;
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
    void print(){
        traverseLeft(root);
    }

    void mostOccurred() {
        std::vector<BSTNode*> maxNodes;
        int maxOccurrences = 0;
        greatOccurrences(root, maxNodes, maxOccurrences);
        if (maxNodes.empty()) {
            std::cout << "A arvore esta vazia" << std::endl;
        } else {
            for (const auto& node : maxNodes) {
                std::cout << "Palavra: " << node->key << ", ";
                std::cout << "Numero de ocorrencias: " << node->value.occurrences << std::endl;
            }
        }
    }
    void longer() {
        std::vector<BSTNode*> maxNodes;
        int maxLetters = 0;

        greatLetters(root, maxNodes, maxLetters);

        if (maxNodes.empty()) {
            std::cout << "A arvore esta vazia" << std::endl;
        } else {
            for (const auto& node : maxNodes) {
                std::cout << "Palavra: " << node->key << ", ";
                std::cout << "Numero de letras: " << node->value.letters << std::endl;
            }
        }
    }

    void longerwithnorepeatedletters() {
        std::vector<BSTNode*> maxNodes;
        int maxLetters = 0;

        sr(root, maxNodes, maxLetters);

        if (maxNodes.empty()) {
            std::cout << "A arvore esta vazia" << std::endl;
        } else {
            for (const auto& node : maxNodes) {
                std::cout << "Palavra: " << node->key << ", ";
                std::cout << "Numero de letras: " << node->value.letters << std::endl;
            }
        }
    }

    void shorterwithmostvowel() {
        std::vector<BSTNode*> minNodes;
        int lowVowels = root->value.vowels;
        mostVowels(root, minNodes, lowVowels);
        vd(minNodes);
        if (minNodes.empty()) {
            std::cout << "A arvore esta vazia" << std::endl;
        } else {
            for (const auto& node : minNodes) {
                std::cout << "Palavra: " << node->key << ", ";
                std::cout << "Numero de letras: " << node->value.letters << std::endl;
            }
        }
    }

};

struct VOEntry {
    std::string key;
    Item value{};
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
        if (size == 0) return 0;
        if (size == 1) {
            if (entries[0].key > key) return 0;
            else return 1;
        }
        int start = 0;
        int end = size - 1;
        while(start <= end){
            int mid = (start + end)/2;
            if(entries[mid].key == key) return mid;
            if(entries[mid].key < key) start = mid + 1;
            else end = mid - 1;
        }
        return start;
    }

    void put(const std::string& key) {
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
        value = findValue(key);
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
    Item get(const std::string& key) const {
        size_t start = 0;
        size_t end = size - 1;

        while (start <= end) {
            size_t mid = (start + end) / 2;
            if (entries[mid].key == key) {
                return entries[mid].value;
            } else if (entries[mid].key < key) {
                start = mid + 1;
            } else {
                end = mid - 1;
            }
        }
        // Key not found
        return Item::None(); // Return a default-constructed value
    }


    void print() {
        for (size_t i = 0; i < size; ++i) {
            std::cout << "Word: " << entries[i].key << std:: endl ;/*<<
                      "Number of Occurrences: " << entries[i].value.occurrences << std::endl <<
                      "Number of Letters: " << entries[i].value.letters << std::endl <<
                      "Number of Vowels: " << entries[i].value.vowels << std::endl;*/
        }
    }

    void mostOccurred() {
        if (size == 0) {
            // Handle the case when the symbol table is empty
            // Return an empty vector or indicate an error
            std::cout << "Tabela vazia" << std::endl;
        }

        std::vector<VOEntry> maxEntries;
        int maxOccurrences = entries[0].value.occurrences;

        for (size_t i = 1; i < size; ++i) {
            if (entries[i].value.occurrences > maxOccurrences) {
                maxEntries.clear();  // Clear the previous entries with the maximum occurrences
                maxOccurrences = entries[i].value.occurrences;
            }
            if (entries[i].value.occurrences == maxOccurrences) {
                maxEntries.push_back(entries[i]);  // Add the current entry to the vector
            }
        }

        for (const auto& entry : maxEntries) {
            // Access the elements of entry
            std::cout << "Palavra: " << entry.key << ", Ocorrencias: " << entry.value.occurrences << std::endl;
        }
    }
    void longer() {
        if (size == 0) {
            // Handle the case when the symbol table is empty
            // Return an empty vector or indicate an error
            std::cout << "Tabela vazia" << std::endl;
        }

        std::vector<VOEntry> maxEntries;
        int longer = entries[0].value.letters;

        for (size_t i = 1; i < size; ++i) {
            if (entries[i].value.letters > longer) {
                maxEntries.clear();  // Clear the previous entries with the maximum occurrences
                longer = entries[i].value.letters;
            }
            if (entries[i].value.letters == longer) {
                maxEntries.push_back(entries[i]);  // Add the current entry to the vector
            }
        }

        for (const auto& entry : maxEntries) {
            // Access the elements of entry
            std::cout << "Palavra: " << entry.key << ", Numero de letras: " << entry.value.letters << std::endl;
        }
    }
    void sr(){
        if (size == 0) {
            // Handle the case when the symbol table is empty
            // Return an empty vector or indicate an error
            std::cout << "Tabela vazia" << std::endl;
        }

        std::vector<VOEntry> maxEntries;
        int longer = entries[0].value.letters;

        for (size_t i = 1; i < size; ++i) {
            if (entries[i].value.letters > longer && !hasRepeatedLetters(entries[i].key)) {
                maxEntries.clear();  // Clear the previous entries with the maximum occurrences
                longer = entries[i].value.letters;
            }
            if (entries[i].value.letters == longer && !hasRepeatedLetters(entries[i].key)) {
                maxEntries.push_back(entries[i]);  // Add the current entry to the vector
            }
        }

        for (const auto& entry : maxEntries) {
            // Access the elements of entry
            std::cout << "Palavra: " << entry.key << ", Numero de letras: " << entry.value.letters << std::endl;
        }
    }
    void vd(){
        if (size == 0) {
            // Handle the case when the symbol table is empty
            // Return an empty vector or indicate an error
            std::cout << "Tabela vazia" << std::endl;
        }

        std::vector<VOEntry> maxVowels;
        int longer = entries[0].value.vowels;

        for (size_t i = 1; i < size; ++i) {
            if (entries[i].value.vowels > longer && !hasRepeatedLetters(entries[i].key)) {
                maxVowels.clear();  // Clear the previous entries with the maximum occurrences
                longer = entries[i].value.vowels;
            }
            if (entries[i].value.vowels == longer) {
                maxVowels.push_back(entries[i]);  // Add the current entry to the vector
            }
        }
        int lowestLetters = INT_MAX;
        // Find the lowest value of letters
        for (const auto& entry : maxVowels) {
            if (entry.value.letters < lowestLetters) {
                lowestLetters = entry.value.letters;
            }
        }
        // Erase elements with letters greater than the lowest value
        auto it = maxVowels.begin();
        while (it != maxVowels.end()) {
            if ((*it).value.letters > lowestLetters) {
                it = maxVowels.erase(it);
            } else {
                ++it;
            }
        }
        auto it2 = maxVowels.begin();
        while (it2 != maxVowels.end()){
            std::cout << "Palavra: " << it2->key << ", ";
            std::cout << "Numero de letras: " << it2->value.letters << std::endl;
            ++it2;
        }
    }
};

struct TreapNode {
    std::string key;
    Item value;
    int priority;  // Priority for the treap property
    TreapNode* left;
    TreapNode* right;

    TreapNode(std::string k, Item v, int p) : key(std::move(k)), value(v), priority(p), left(nullptr), right(nullptr) {}
};
class SymbolTableTreap {
private:
    TreapNode* root;
    std::random_device rd;
    std::mt19937 gen;

    static TreapNode* rotateLeft(TreapNode* node) {
        TreapNode* newRoot = node->right;
        node->right = newRoot->left;
        newRoot->left = node;
        return newRoot;
    }

    static TreapNode* rotateRight(TreapNode* node) {
        TreapNode* newRoot = node->left;
        node->left = newRoot->right;
        newRoot->right = node;
        return newRoot;
    }

    TreapNode* addRecursive(TreapNode* node, const std::string& key, Item value) {
        if (node == nullptr) {
            return new TreapNode(key, value, gen());
        }

        if (key < node->key) {
            node->left = addRecursive(node->left, key, value);
            if (node->left->priority > node->priority) {
                node = rotateRight(node);
            }
        } else if (key > node->key) {
            node->right = addRecursive(node->right, key, value);
            if (node->right->priority > node->priority) {
                node = rotateLeft(node);
            }
        } else if (key == node->key) {
            node->value.occurrences += 1;
        } else {
            node->value = value;
        }

        return node;
    }

    TreapNode* getRecursive(TreapNode* node, const std::string& key) {
        if (node == nullptr || node->key == key) {
            return node;
        }

        if (key < node->key) {
            return getRecursive(node->left, key);
        } else {
            return getRecursive(node->right, key);
        }
    }

    void traverseInOrder(TreapNode* node) {
        if (node != nullptr) {
            traverseInOrder(node->left);
            std::cout << node->key << " : " << std::endl;
            traverseInOrder(node->right);
        }
    }

    void inorder(TreapNode* root)
    {
        if (root)
        {
            inorder(root->left);
            std::cout << "key: "<< root->key << " | priority: "
                      << root->priority;
            if (root->left)
                std::cout << " | left child: " << root->left->key;
            if (root->right)
                std::cout << " | right child: " << root->right->key;
            std::cout << std::endl;
            inorder(root->right);
        }
    }
public:
    SymbolTableTreap() : root(nullptr), gen(rd()) {}

    void put(const std::string& key) {
        Item value = findValue(key);
        root = addRecursive(root, key, value);
    }

    Item get(const std::string& key) {
        TreapNode* node = getRecursive(root, key);
        if (node != nullptr) {
            return node->value;
        } else {
            return Item::None();  // Return a default value indicating key not found
        }
    }

    void mostOccurred() {
        std::vector<TreapNode*> maxNodes;
        int maxOccurrences = 0;
        greatOccurrences(root, maxNodes, maxOccurrences);
        if (maxNodes.empty()) {
            std::cout << "A arvore esta vazia" << std::endl;
        } else {
            for (const auto& node : maxNodes) {
                std::cout << "Palavra: " << node->key << ", ";
                std::cout << "Numero de ocorrencias: " << node->value.occurrences << std::endl;
            }
        }
    }
    void longer() {
        std::vector<TreapNode*> maxNodes;
        int maxLetters = 0;

        greatLetters(root, maxNodes, maxLetters);

        if (maxNodes.empty()) {
            std::cout << "A arvore esta vazia" << std::endl;
        } else {
            for (const auto& node : maxNodes) {
                std::cout << "Palavra: " << node->key << ", ";
                std::cout << "Numero de letras: " << node->value.letters << std::endl;
            }
        }
    }
    void longerwithnorepeatedletters() {
        std::vector<TreapNode*> maxNodes;
        int maxLetters = 0;

        sr(root, maxNodes, maxLetters);

        if (maxNodes.empty()) {
            std::cout << "A arvore esta vazia" << std::endl;
        } else {
            for (const auto& node : maxNodes) {
                std::cout << "Palavra: " << node->key << ", ";
                std::cout << "Numero de letras: " << node->value.letters << std::endl;
            }
        }
    }
    void shorterwithmostvowel() {
        std::vector<TreapNode*> minNodes;
        int lowVowels = root->value.vowels;
        mostVowels(root, minNodes, lowVowels);
        vd(minNodes);
        if (minNodes.empty()) {
            std::cout << "A arvore esta vazia" << std::endl;
        } else {
            for (const auto& node : minNodes) {
                std::cout << "Palavra: " << node->key << ", ";
                std::cout << "Numero de letras: " << node->value.letters << std::endl;
            }
        }
    }
};

struct Node23 {
    bool is2no;
    std::string ch1;
    Item val1;
    std::string ch2;
    Item val2;
    Node23* p1;
    Node23* p2;
    Node23* p3;
    Node23()
            : is2no(true), ch1(""), val1(Item().None()), ch2(""), val2(Item().None()), p1(nullptr), p2(nullptr), p3(nullptr) {}
};
bool increased = false;
class SymbolTable23 {
private:
    Node23* root;

    Node23* add23(Node23* root, const std::string& key, const Item& val) {
        if (root == nullptr) {
            root = new Node23();
            root->ch1 = key;
            root->val1 = val;
            increased = false;
            return root;
        }
        else if (key == root->ch1) {
            root->val1.occurrences +=1;
            /*std::cout << "palavra: " << key << "  ch1: " << root->ch1 << "  cresceu: " << increased << std::endl;
            print();*/
            return root;
        }
        else if (!root->is2no && key == root->ch2) {
            root->val2.occurrences +=1;
            return root;
        }
        if (root->p1 != nullptr) { // raiz is not a leaf
            if (root->ch1 > key) {
                Node23* p = add23(root->p1, key, val);
                if (increased) {
                    if (root->is2no) {
                        root->ch2 = root->ch1;
                        root->val2 = root->val1;
                        root->p3 = root->p2;
                        root->ch1 = p->ch1;
                        root->val1 = p->val1;
                        root->p1 = p->p1;
                        root->p2 = p->p2;
                        increased = false;
                        root->is2no = false;
                        delete p;
                        return root;
                    }
                    else {
                        Node23* newNode = new Node23();
                        newNode->ch1 = root->ch2;
                        newNode->val1 = root->val2;
                        newNode->p1 = root->p2;
                        newNode->p2 = root->p3;
                        Node23* nroot = new Node23();
                        nroot->ch1 = root->ch1;
                        nroot->val1 = root->val1;
                        nroot->p2 = newNode;
                        nroot->p1 = root;
                        root->ch1 = p->ch1;
                        root->val1 = p->val1;
                        root->ch2 = "";
                        root->val2 = Item::None();
                        root->p3 = nullptr;
                        root->p2 = p->p2;
                        root->p1 = p->p1;
                        newNode->is2no = root->is2no = nroot->is2no = true;
                        increased = true;
                        delete p;
                        /*if (nroot->ch1 == "Maria"){
                            using namespace std;
                            cout << "raiz ch1: " << nroot->ch1 << "  occ:" << nroot->val1.occurrences << "  lett:" << nroot->val1.letters << "  vow:" << nroot->val1.vowels << std::endl;
                            cout << "p1 ch1: " << nroot->p1->ch1 << "  occ:" << nroot->p1->val1.occurrences << "  lett:" << nroot->p1->val1.letters << "  vow:" << nroot->p1->val1.vowels << std::endl;
                            cout << "p1 p1 ch1: " << nroot->p1->p1->ch1 << "  occ:" << nroot->p1->p1->val1.occurrences << "  lett:" << nroot->p1->p1->val1.letters << "  vow:" << nroot->p1->p1->val1.vowels << std::endl;
                            cout << "p1 p2 ch1: " << nroot->p1->p2->ch1 << "  occ:" << nroot->p1->p2->val1.occurrences << "  lett:" << nroot->p1->p1->val1.letters << "  vow:" << nroot->p1->p2->val1.vowels << std::endl;
                            cout << "p2 ch1: " << nroot->p2->ch1 << "  occ:" << nroot->p2->val1.occurrences << "  lett:" << nroot->p2->val1.letters << "  vow:" << nroot->p2->val1.vowels << std::endl;
                            cout << "p2 p1 ch1: " << nroot->p2->p1->ch1 << "  occ:" << nroot->p2->p1->val1.occurrences << "  lett:" << nroot->p2->p1->val1.letters << "  vow:" << nroot->p2->p1->val1.vowels << std::endl;
                            cout << "p2 p2 ch1: " << nroot->p2->p2->ch1 << "  occ:" << nroot->p2->p2->val1.occurrences << "  lett:" << nroot->p2->p2->val1.letters << "  vow:" << nroot->p2->p2->val1.vowels << std::endl;
                            cout << "p2 p2 ch2: " << nroot->p2->p2->ch2 << "  occ:" << nroot->p2->p2->val2.occurrences << "  lett:" << nroot->p2->p2->val2.letters << "  vow:" << nroot->p2->p2->val2.vowels << std::endl;
                        }*/
                        return nroot;
                    }
                }
                else{
                    root->p1 = p;
                    /*if (root->ch1 == "Maria"){
                        std::cout << "raiz ch1 e: " << root->ch1 << "  occ:" << root->val1.occurrences << "  lett:" << root->val1.letters << "  vow:" << root->val1.vowels << std::endl;
                        std::cout << "raiz ch2 e: " << root->ch2 << "  occ:" << root->val2.occurrences << "  lett:" << root->val2.letters << "  vow:" << root->val2.vowels << std::endl;
                        std::cout << "p1 ch1 e: " << root->p1->ch1 << "  occ:" << root->p1->val1.occurrences << "  lett:" << root->p1->val1.letters << "  vow:" << root->p1->val1.vowels << std::endl;
                        std::cout << "p1 ch2 e: " << root->p1->ch2 << "  occ:" << root->p1->val2.occurrences << "  lett:" << root->p1->val2.letters << "  vow:" << root->p1->val2.vowels << std::endl;
                        std::cout << "p2 ch1 e: " << root->p2->ch1 << "  occ:" << root->p2->val1.occurrences << "  lett:" << root->p2->val1.letters << "  vow:" << root->p2->val1.vowels << std::endl;
                        std::cout << "p3 ch1 e: " << root->p3->ch1 << "  occ:" << root->p3->val1.occurrences << "  lett:" << root->p3->val1.letters << "  vow:" << root->p3->val1.vowels << std::endl;
                        std::cout << "p3 ch2 e: " << root->p3->ch2 << "  occ:" << root->p3->val2.occurrences << "  lett:" << root->p3->val2.letters << "  vow:" << root->p3->val2.vowels << std::endl;
                    }*/
                    return root;
                }
            }
            else if (root->is2no || root->ch2 > key) {
                Node23* p = add23(root->p2, key, val);
                if (increased) {
                    if (root->is2no) {
                        root->ch2 = p->ch1;
                        root->val2 = p->val1;
                        root->p3 = p->p2;
                        root->p2 = p->p1;
                        increased = false;
                        delete p;
                        root->is2no = false;
                        return root;
                    }
                    else {
                        Node23* nroot = new Node23();
                        nroot->ch1 = p->ch1;
                        nroot->val1 = p->val1;
                        Node23* newNode = new Node23();
                        newNode->ch1 = root->ch2;
                        newNode->val1 = root->val2;
                        newNode->p2 = root->p3;
                        newNode->p1 = p->p2;
                        root->ch2 = "";
                        root->val2 = Item::None();
                        root->p3 = nullptr;
                        root->p2 = p->p1;
                        nroot->p1 = root;
                        nroot->p2 = newNode;
                        increased = true;
                        nroot->is2no = root->is2no = newNode->is2no = true;
                        delete p;
                        return nroot;
                    }
                }
                else {
                    root->p2 = p;
                    /*if (root->ch1 == "Maria" && !root->p2->is2no){
                        using namespace std;
                        cout << "raiz ch1: " << root->ch1 << "  occ:" << root->val1.occurrences << "  lett:" << root->val1.letters << "  vow:" << root->val1.vowels << std::endl;
                        cout << "p1 ch1: " << root->p1->ch1 << "  occ:" << root->p1->val1.occurrences << "  lett:" << root->p1->val1.letters << "  vow:" << root->p1->val1.vowels << std::endl;
                        cout << "p1 p1 ch1: " << root->p1->p1->ch1 << "  occ:" << root->p1->p1->val1.occurrences << "  lett:" << root->p1->p1->val1.letters << "  vow:" << root->p1->p1->val1.vowels << std::endl;
                        cout << "p1 p2 ch1: " << root->p1->p2->ch1 << "  occ:" << root->p1->p2->val1.occurrences << "  lett:" << root->p1->p1->val1.letters << "  vow:" << root->p1->p2->val1.vowels << std::endl;
                        cout << "p2 ch1: " << root->p2->ch1 << "  occ:" << root->p2->val1.occurrences << "  lett:" << root->p2->val1.letters << "  vow:" << root->p2->val1.vowels << std::endl;
                        cout << "p2 ch2: " << root->p2->ch2 << "  occ:" << root->p2->val2.occurrences << "  lett:" << root->p2->val2.letters << "  vow:" << root->p2->val2.vowels << std::endl;
                        cout << "p2 p1 ch1: " << root->p2->p1->ch1 << "  occ:" << root->p2->p1->val1.occurrences << "  lett:" << root->p2->p1->val1.letters << "  vow:" << root->p2->p1->val1.vowels << std::endl;
                        cout << "p2 p2 ch1: " << root->p2->p2->ch1 << "  occ:" << root->p2->p2->val1.occurrences << "  lett:" << root->p2->p2->val1.letters << "  vow:" << root->p2->p2->val1.vowels << std::endl;
                        cout << "p2 p2 ch2: " << root->p2->p3->ch1 << "  occ:" << root->p2->p3->val1.occurrences << "  lett:" << root->p2->p3->val1.letters << "  vow:" << root->p2->p3->val1.vowels << std::endl;
                    }*/
                    /*if (root->ch1 == "Maria" && !root->is2no && !root->p2->is2no && root->p2->p3->ch1 == "foi"){
                        print();
                    }*/
                    return root;
                }
            }
            else {
                Node23* p = add23(root->p3, key, val);
                if (increased) {
                    Node23* nroot = new Node23();
                    Node23* newNode = new Node23();
                    nroot->ch1 = root->ch2;
                    nroot->val1 = root->val2;
                    newNode->ch1 = p->ch1;
                    newNode->val1 = p->val1;
                    newNode->p1 = p->p1;
                    newNode->p2 = p->p2;
                    root->ch2 = "";
                    root->val2 = Item::None();
                    root->p3 = nullptr;
                    nroot->p1 = root;
                    nroot->p2 = newNode;
                    delete(p);
                    increased = true;
                    nroot->is2no = root->is2no = newNode->is2no = true;
                    return nroot;
                }
                else {
                    root->p3 = p;
                    return root;
                }
            }
        }
        else { // raiz is a leaf
            if (root->is2no) {
                if (root->ch1 > key) {
                    root->ch2 = root->ch1;
                    root->val2 = root->val1;
                    root->ch1 = key;
                    root->val1 = val;
                    root->is2no = false;
                    increased = false;
                    return root;
                }
                else {
                    root->ch2 = key;
                    root->val2 = val;
                    root->is2no = false;
                    increased = false;
                    return root;
                }
            }
            else { // raiz is a 3 node
                if (key < root->ch1) {
                    Node23* newNode = new Node23();
                    Node23* nroot = new Node23();
                    newNode->ch1 = root->ch2;
                    newNode->val1 = root->val2;
                    nroot->ch1 = root->ch1;
                    nroot->val1 = root->val1;
                    root->ch1 = key;
                    root->val1 = val;
                    root->ch2 = "";
                    root->val2 = Item::None();
                    nroot->p1 = root;
                    nroot->p2 = newNode;
                    increased = true;
                    root->is2no = nroot->is2no = newNode->is2no = true;
                    return nroot;
                }
                else if (key < root->ch2) {
                    Node23* newNode = new Node23();
                    Node23* nroot = new Node23();
                    newNode->ch1 = root->ch2;
                    newNode->val1 = root->val2;
                    nroot->ch1 = key;
                    nroot->val1 = val;
                    root->ch2 = "";
                    root->val2 = Item::None();
                    nroot->p1 = root;
                    nroot->p2 = newNode;
                    increased = true;
                    root->is2no = nroot->is2no = newNode->is2no = true;
                    return nroot;
                }
                else {
                    Node23* newNode = new Node23();
                    Node23* nroot = new Node23();
                    newNode->ch1 = key;
                    newNode->val1 = val;
                    nroot->ch1 = root->ch2;
                    nroot->val1 = root->val2;
                    root->ch2 = "";
                    root->val2 = Item::None();
                    nroot->p1 = root;
                    nroot->p2 = newNode;
                    increased = true;
                    root->is2no = nroot->is2no = newNode->is2no = true;
                    return nroot;
                }
            }
        }
    }

    Item get(Node23* root, std::string key) {
        std::stack<Node23*> stack;
        stack.push(root);

        while (!stack.empty()) {
            Node23* current = stack.top();
            stack.pop();
            if (current != nullptr) {
                if (current->ch1 == key) {
                    std::cout << current->val1.occurrences << std::endl;
                    return current->val1;
                }

                if (!current->is2no && current->ch2 == key) {
                    return current->val2;
                }

                stack.push(current->p1);
                stack.push(current->p2);
                stack.push(current->p3);
            }
        }

        return Item::None();
    }

    void inorder(Node23* root) {
        if (root != nullptr) {
            if (root->p1 == nullptr) { // Node is a leaf
                std::cout << "key folha:" << root->ch1 << "  occ:" << root->val1.occurrences << "  lett:" << root->val1.letters << "  vow:" << root->val1.vowels << std::endl;
                if (!root->is2no) {
                    std::cout << "key2 folha:" << root->ch2 << "  occ:" << root->val2.occurrences << "  lett:" << root->val2.letters << "  vow:" << root->val2.vowels << std::endl;
                }
            } else { // Node is not a leaf
                inorder(root->p1);
                std::cout << "key:" << root->ch1 << "  occ:" << root->val1.occurrences << "  lett:" << root->val1.letters << "  vow:" << root->val1.vowels << std::endl;
                inorder(root->p2);
                if (!root->is2no) {
                    std::cout << "key2:" << root->ch2 << "  occ:" << root->val2.occurrences << "  lett:" << root->val2.letters << "  vow:" << root->val2.vowels << std::endl;
                    inorder(root->p3);
                }
            }
        }
    }
    Node23* twothreetotwonode(Node23* node){
        Node23* novo = new Node23();
        novo->ch1 = node->ch2;
        novo->val1 = node->val2;
        novo->p1 = node->p2;
        novo->p2 = node->p3;
        return novo;
    }
    void greatOccurrences23(Node23* root, std::vector<Node23*>& maxNodes, int& maxOccurrences) {
        if (root != nullptr) {
            if (root != nullptr) {
                if (root->p1 == nullptr) {
                    if (root->val1.occurrences > maxOccurrences) {
                        maxOccurrences = root->val1.occurrences;
                        maxNodes.clear(); // Clear previous max nodes
                        maxNodes.push_back(root);
                    } else if (root->val1.occurrences == maxOccurrences) {
                        maxNodes.push_back(root);
                    }
                    if (!root->is2no) {
                        if (root->val2.occurrences > maxOccurrences) {
                            maxOccurrences = root->val2.occurrences;
                            maxNodes.clear(); // Clear previous max nodes
                            maxNodes.push_back(twothreetotwonode(root));
                        } else if (root->val2.occurrences == maxOccurrences) {
                            maxNodes.push_back(twothreetotwonode(root));
                        }
                    }
                } else {
                    greatOccurrences23(root->p1, maxNodes, maxOccurrences);
                    if (root->val1.occurrences > maxOccurrences) {
                        maxOccurrences = root->val1.occurrences;
                        maxNodes.clear(); // Clear previous max nodes
                        maxNodes.push_back(root);
                    } else if (root->val1.occurrences == maxOccurrences) {
                        maxNodes.push_back(root);
                    }
                    greatOccurrences23(root->p2, maxNodes, maxOccurrences);
                    if (!root->is2no) {
                        if (root->val2.occurrences > maxOccurrences) {
                            maxOccurrences = root->val2.occurrences;
                            maxNodes.clear(); // Clear previous max nodes
                            maxNodes.push_back(twothreetotwonode(root));
                        } else if (root->val2.occurrences == maxOccurrences) {
                            maxNodes.push_back(twothreetotwonode(root));
                        }
                        greatOccurrences23(root->p3, maxNodes, maxOccurrences);
                    }
                }
            }
        }
    }
    void greatLetters23(Node23 *root, std::vector<Node23 *> &maxNodes, int &maxLetters) {
        if (root != nullptr) {
            if (root->p1 == nullptr) {
                if (root->val1.letters > maxLetters) {
                    maxLetters = root->val1.letters;
                    maxNodes.clear(); // Clear previous max nodes
                    maxNodes.push_back(root);
                } else if (root->val1.letters == maxLetters) {
                    maxNodes.push_back(root);
                }
                if (!root->is2no) {
                    if (root->val2.letters > maxLetters) {
                        maxLetters = root->val2.letters;
                        maxNodes.clear(); // Clear previous max nodes
                        maxNodes.push_back(twothreetotwonode(root));
                    } else if (root->val2.letters == maxLetters) {
                        maxNodes.push_back(twothreetotwonode(root));
                    }
                }
            }
            else {
                greatLetters23(root->p1, maxNodes, maxLetters);
                if (root->val1.letters > maxLetters) {
                    maxLetters = root->val1.letters;
                    maxNodes.clear(); // Clear previous max nodes
                    maxNodes.push_back(root);
                } else if (root->val1.letters == maxLetters) {
                    maxNodes.push_back(root);
                }
                greatLetters23(root->p2, maxNodes, maxLetters);
                if (!root->is2no) {
                    if (root->val2.letters > maxLetters) {
                        maxLetters = root->val2.letters;
                        maxNodes.clear(); // Clear previous max nodes
                        maxNodes.push_back(twothreetotwonode(root));
                    } else if (root->val2.letters == maxLetters) {
                        maxNodes.push_back(twothreetotwonode(root));
                    }
                    greatLetters23(root->p3, maxNodes, maxLetters);
                }
            }
        }
    }
    void sr23(Node23 *root, std::vector<Node23 *> &maxNodes, int &maxLetters) {
        if (root != nullptr) {
            if (root->p1 == nullptr) {
                if (root->val1.letters > maxLetters && !hasRepeatedLetters(root->ch1)) {
                    maxLetters = root->val1.letters;
                    maxNodes.clear(); // Clear previous max nodes
                    maxNodes.push_back(root);
                } else if (root->val1.letters == maxLetters && !hasRepeatedLetters(root->ch1)) {
                    maxNodes.push_back(root);
                }
                if (!root->is2no) {
                    if (root->val2.letters > maxLetters && !hasRepeatedLetters(root->ch1)) {
                        maxLetters = root->val2.letters;
                        maxNodes.clear(); // Clear previous max nodes
                        maxNodes.push_back(twothreetotwonode(root));
                    } else if (root->val2.letters == maxLetters && !hasRepeatedLetters(root->ch1)) {
                        maxNodes.push_back(twothreetotwonode(root));
                    }
                }
            }
            else {
                sr23(root->p1, maxNodes, maxLetters);
                if (root->val1.letters > maxLetters && !hasRepeatedLetters(root->ch1)) {
                    maxLetters = root->val1.letters;
                    maxNodes.clear(); // Clear previous max nodes
                    maxNodes.push_back(root);
                } else if (root->val1.letters == maxLetters && !hasRepeatedLetters(root->ch1)) {
                    maxNodes.push_back(root);
                }
                sr23(root->p2, maxNodes, maxLetters);
                if (!root->is2no) {
                    if (root->val2.letters > maxLetters && !hasRepeatedLetters(root->ch1)) {
                        maxLetters = root->val2.letters;
                        maxNodes.clear(); // Clear previous max nodes
                        maxNodes.push_back(twothreetotwonode(root));
                    } else if (root->val2.letters == maxLetters && !hasRepeatedLetters(root->ch1)) {
                        maxNodes.push_back(twothreetotwonode(root));
                    }
                    sr23(root->p3, maxNodes, maxLetters);
                }
            }
        }
    }
    void mostVowels23(Node23 *root, std::vector<Node23 *> &maxNodes, int &maxVowels) {
        if (root != nullptr) {
            if (root->p1 == nullptr) {
                if (root->val1.vowels > maxVowels && !hasRepeatedLetters(root->ch1)) {
                    maxVowels = root->val1.vowels;
                    maxNodes.clear(); // Clear previous max nodes
                    maxNodes.push_back(root);
                } else if (root->val1.vowels == maxVowels && !hasRepeatedLetters(root->ch1)) {
                    maxNodes.push_back(root);
                }
                if (!root->is2no) {
                    if (root->val2.vowels > maxVowels && !hasRepeatedLetters(root->ch1)) {
                        maxVowels = root->val2.vowels;
                        maxNodes.clear(); // Clear previous max nodes
                        maxNodes.push_back(twothreetotwonode(root));
                    } else if (root->val2.vowels == maxVowels && !hasRepeatedLetters(root->ch1)) {
                        maxNodes.push_back(twothreetotwonode(root));
                    }
                }
            }
            else {
                mostVowels23(root->p1, maxNodes, maxVowels);
                if (root->val1.vowels > maxVowels && !hasRepeatedLetters(root->ch1)) {
                    maxVowels = root->val1.vowels;
                    maxNodes.clear(); // Clear previous max nodes
                    maxNodes.push_back(root);
                } else if (root->val1.vowels == maxVowels && !hasRepeatedLetters(root->ch1)) {
                    maxNodes.push_back(root);
                }
                mostVowels23(root->p2, maxNodes, maxVowels);
                if (!root->is2no) {
                    if (root->val2.vowels > maxVowels && !hasRepeatedLetters(root->ch1)) {
                        maxVowels = root->val2.vowels;
                        maxNodes.clear(); // Clear previous max nodes
                        maxNodes.push_back(twothreetotwonode(root));
                    } else if (root->val2.vowels == maxVowels && !hasRepeatedLetters(root->ch1)) {
                        maxNodes.push_back(twothreetotwonode(root));
                    }
                    mostVowels23(root->p3, maxNodes, maxVowels);
                }
            }
        }
    }
    void vd23(std::vector<Node23*>& minNodes) {
        int lowestLetters = INT_MAX;
        // Find the lowest value of letters
        for (const auto& node : minNodes) {
            if (node->val1.letters < lowestLetters) {
                lowestLetters = node->val1.letters;
            }
        }
        // Erase elements with letters greater than the lowest value
        auto it = minNodes.begin();
        while (it != minNodes.end()) {
            if ((*it)->val1.letters > lowestLetters) {
                it = minNodes.erase(it);
            } else {
                ++it;
            }
        }
    }

public:
    SymbolTable23() : root(nullptr) {}
    void put(std::string key) {
        Item value = findValue(key);
        increased = false;
        root = add23(root, key, value);
    }
    void print() {
        inorder(root);
    }
    Item find(std::string& key){
        return get(root, key);
    }
    void mostOccurred() {
        std::vector<Node23*> maxNodes;
        int maxOccurrences = 0;
        greatOccurrences23(root, maxNodes, maxOccurrences);
        if (maxNodes.empty()) {
            std::cout << "A arvore esta vazia" << std::endl;
        } else {
            for (const auto& node : maxNodes) {
                std::cout << "Palavra: " << node->ch1 << ", ";
                std::cout << "Numero de ocorrencias: " << node->val1.occurrences << std::endl;
            }
        }
    }
    void longer() {
        std::vector<Node23*> maxNodes;
        int maxLetters = 0;
        greatLetters23(root, maxNodes, maxLetters);
        if (maxNodes.empty()) {
            std::cout << "A arvore esta vazia" << std::endl;
        } else {
            for (const auto& node : maxNodes) {
                std::cout << "Palavra: " << node->ch1 << ", ";
                std::cout << "Numero de letras: " << node->val1.letters << std::endl;
            }
        }
    }

    void longerwithnorepeatedletters() {
        std::vector<Node23*> maxNodes;
        int maxLetters = 0;

        sr23(root, maxNodes, maxLetters);

        if (maxNodes.empty()) {
            std::cout << "A arvore esta vazia" << std::endl;
        } else {
            for (const auto& node : maxNodes) {
                std::cout << "Palavra: " << node->ch1 << ", ";
                std::cout << "Numero de letras: " << node->val1.letters << std::endl;
            }
        }
    }

    void shorterwithmostvowel() {
        std::vector<Node23*> minNodes;
        int lowVowels = root->val1.vowels;
        mostVowels23(root, minNodes, lowVowels);
        vd23(minNodes);
        if (minNodes.empty()) {
            std::cout << "A arvore esta vazia" << std::endl;
        } else {
            for (const auto& node : minNodes) {
                std::cout << "Palavra: " << node->ch1 << ", ";
                std::cout << "Numero de letras: " << node->val1.letters << std::endl;
            }
        }
    }
};

const bool RED = true;
const bool BLACK = false;

class RBNode {
public:
    std::string key; // key
    Item value; // associated data
    RBNode* left; // left subtree
    RBNode* right; // right subtree
    int N; // # nodes in this subtree
    bool color; // color of link from parent to this node

    RBNode(std::string& key, Item val, int N, bool color) {
        this->key = key;
        this->value = val;
        this->N = N;
        this->color = color;
        this->left = nullptr;
        this->right = nullptr;
    }
};

bool isRed(RBNode* x) {
    if (x == nullptr)
        return false;
    return x->color == RED;
}

class SymbolTableRedBlack {
private:
    RBNode* root;

    Item findValue(const std::string& key) {
        Item value{};
        value.occurrences = 1;
        value.letters = key.size();
        value.vowels = vowels(key);
        return value;
    }

    RBNode* rotateLeft(RBNode* h) {
        RBNode* x = h->right;
        h->right = x->left;
        x->left = h;
        x->color = h->color;
        h->color = RED;
        x->N = h->N;
        h->N = 1 + size(h->left) + size(h->right);
        return x;
    }

    RBNode* rotateRight(RBNode* h) {
        RBNode* x = h->left;
        h->left = x->right;
        x->right = h;
        x->color = h->color;
        h->color = RED;
        x->N = h->N;
        h->N = 1 + size(h->left) + size(h->right);
        return x;
    }

    void flipColors(RBNode* h) {
        h->color = !h->color;
        h->left->color = !h->left->color;
        h->right->color = !h->right->color;
    }

    int size(RBNode* x) {
        if (x == nullptr)
            return 0;
        return x->N;
    }

    RBNode* put(RBNode* h, std::string key, Item val) {
        if (h == nullptr)
            return new RBNode(key, val, 1, RED);

        int cmp = key.compare(h->key);
        if (cmp < 0)
            h->left = put(h->left, key, val);
        else if (cmp > 0)
            h->right = put(h->right, key, val);
        else
            h->value.occurrences += 1; // Increment occurrences when keys are equal

        if (isRed(h->right) && !isRed(h->left))
            h = rotateLeft(h);
        if (isRed(h->left) && isRed(h->left->left))
            h = rotateRight(h);
        if (isRed(h->left) && isRed(h->right))
            flipColors(h);

        h->N = size(h->left) + size(h->right) + 1;
        return h;
    }

    void print(RBNode* node) {
        if (node == nullptr)
            return;

        print(node->left);
        std::cout << "Key: " << node->key << ", Occurrences: " << node->value.occurrences << std::endl;
        print(node->right);
    }
    void printByFloor(RBNode* root) {
        if (root == nullptr) {
            return;
        }

        std::queue<std::pair<RBNode*, int>> queue;
        queue.push({root, 0});

        int currentLevel = 0;

        while (!queue.empty()) {
            RBNode* current = queue.front().first;
            int level = queue.front().second;
            queue.pop();

            if (level > currentLevel) {
                std::cout << std::endl;
                currentLevel = level;
            }

            // Print the current node with its level
            std::cout << current->key << " (" << (current->color ? "R" : "B") << ") " << "Level: " << level << " ";

            // Enqueue the left and right children with their levels
            if (current->left != nullptr) {
                queue.push({current->left, level + 1});
            }
            if (current->right != nullptr) {
                queue.push({current->right, level + 1});
            }
        }
    }


    RBNode* getRecursive(RBNode* node, const std::string& key) {
        if (node == nullptr || node->key == key)
            return node;
        if (key < node->key)
            return getRecursive(node->left, key);
        else
            return getRecursive(node->right, key);
    }

public:
    SymbolTableRedBlack() {
        root = nullptr;
    }

    void put(const std::string& key) {
        Item val;
        val = findValue(key);
        root = put(root, key, val);
        root->color = BLACK;
    }
    RBNode* get(const std::string& key) {
        return getRecursive(root, key);
    }

    void display() {
        print(root);
    }
    void mostOccurred() {
        std::vector<RBNode*> maxNodes;
        int maxOccurrences = 0;
        greatOccurrences(root, maxNodes, maxOccurrences);
        if (maxNodes.empty()) {
            std::cout << "A arvore esta vazia" << std::endl;
        } else {
            for (const auto& node : maxNodes) {
                std::cout << "Palavra: " << node->key << ", ";
                std::cout << "Numero de ocorrencias: " << node->value.occurrences << std::endl;
            }
        }
    }
    void longer() {
        std::vector<RBNode*> maxNodes;
        int maxLetters = 0;

        greatLetters(root, maxNodes, maxLetters);

        if (maxNodes.empty()) {
            std::cout << "A arvore esta vazia" << std::endl;
        } else {
            for (const auto& node : maxNodes) {
                std::cout << "Palavra: " << node->key << ", ";
                std::cout << "Numero de letras: " << node->value.letters << std::endl;
            }
        }
    }
    void longerwithnorepeatedletters() {
        std::vector<RBNode*> maxNodes;
        int maxLetters = 0;

        sr(root, maxNodes, maxLetters);

        if (maxNodes.empty()) {
            std::cout << "A arvore esta vazia" << std::endl;
        } else {
            for (const auto& node : maxNodes) {
                std::cout << "Palavra: " << node->key << ", ";
                std::cout << "Numero de letras: " << node->value.letters << std::endl;
            }
        }
    }
    void shorterwithmostvowel() {
        std::vector<RBNode*> minNodes;
        int lowVowels = root->value.vowels;
        mostVowels(root, minNodes, lowVowels);
        vd(minNodes);
        if (minNodes.empty()) {
            std::cout << "A arvore esta vazia" << std::endl;
        } else {
            for (const auto& node : minNodes) {
                std::cout << "Palavra: " << node->key << ", ";
                std::cout << "Numero de letras: " << node->value.letters << std::endl;
            }
        }
    }
};




int main() {
    using namespace std;
    string choice;
    cout << "Bem vindo ao segundo exercicio programa de MAC0323" << endl;
    cout << "Escolha sua estrutura de dados dentre as seguintes opcoes: " << endl;
    while (true){
        int numconsultas;
        cout << "Digite 'VO' para um vetor ordenado" << endl;
        cout << "Digite 'ABB' para uma arvore de busca binaria" << endl;
        cout << "Digite 'TR' para uma Treap" << endl;
        cout << "Digite 'A23' para uma arvore balanceada 2-3" << endl;
        cout << "Digite 'ARN' para uma arvore balanceada rubro-negro" << endl;
        cout << "Digite 'sair' para finalizar o programa" << endl;
        cout << "Nao diferenciamos maiusculas e minusculas" << endl;
        cin >> choice;
        for (char& c : choice) {
            c = tolower(c);
        }
        if (choice == "sair") break;
        cout << "Digite a quantidade de consultas a serem feitas (1-5)" << endl;
        cin >> numconsultas;
        cin.ignore();
        vector<string> consultas(5);
        for (int i = 0; i < numconsultas; ++i) {
            cout << "Consulta [" << i + 1 << "]: ";
            getline(cin, consultas[i]);
            cout << endl;
        }
        if (choice == "vo"){
            std::string filePath = R"(C:\Users\Matheus\CLionProjects\EP02 de MAC0323\file.txt)";
            std::ifstream inputFile(filePath);
            if (!inputFile.is_open()) {
                std::cout << "Falha ao abrir o arquivo de texto" << std::endl;
                return 1;
            }
            SymbolTableOV table;
            std::string line;
            while (std::getline(inputFile, line)) {
                std::stringstream ss(line);
                std::string word;
                while (ss >> word) {
                    table.put(word);
                }
            }
            table.print();
            for (string consult : consultas) {
                const char o = 'O';
                // Acha as palavras com maiores ocorrências no texto
                if (consult == "F"){
                    cout << endl;
                    cout << "Maior(es) ocorrencia(s) no texto: " << endl;
                    table.mostOccurred();
                    cout << endl;
                }
                    // Verifica quantas vezes a palavra dada está no texto
                else if (consult[0] == o) {
                    cout << endl;
                    string key;
                    size_t spacePos = consult.find(' ');
                    if (spacePos != std::string::npos) {
                        // Extract the substring starting from the character after the space
                        key = consult.substr(spacePos + 1);
                    }
                    Item value;
                    value = table.get(key);
                    if (value.occurrences < 1) cout << "A palavra " << key << " nao aparece no texto" << endl;
                    else cout << "A palavra " << key << " aparece " << value.occurrences << " vezes no texto" << endl;
                }
                else if (consult == "L") {
                    cout << endl;
                    cout << "Palavra mais longa: ";
                    table.longer();
                    cout << endl;
                }
                else if (consult == "SR") {
                    cout << endl;
                    cout << "Palavra mais longa sem repeticoes: ";
                    table.sr();
                    cout << endl;
                }
                else if (consult == "VD") {
                    cout << endl;
                    cout << "Palavra mais longa sem letras repetidas: ";
                    table.vd();
                    cout << endl;
                }

            }
        }
        else if (choice == "abb") {
            std::string filePath = R"(C:\Users\Matheus\CLionProjects\EP02 de MAC0323\file.txt)";
            std::ifstream inputFile(filePath);
            if (!inputFile.is_open()) {
                std::cout << "Falha ao abrir o arquivo de texto" << std::endl;
                return 1;
            }
            SymbolTableBST table;
            std::string line;
            while (std::getline(inputFile, line)) {
                std::stringstream ss(line);
                std::string word;
                while (ss >> word) {
                    table.put(word);
                }
            }
            consultas_gerais(consultas, table);
        }
        else if (choice == "tr") {
            std::string filePath = R"(C:\Users\Matheus\CLionProjects\EP02 de MAC0323\file.txt)";
            std::ifstream inputFile(filePath);
            if (!inputFile.is_open()) {
                std::cout << "Falha ao abrir o arquivo de texto" << std::endl;
                return 1;
            }
            SymbolTableTreap table;
            std::string line;
            while (std::getline(inputFile, line)) {
                std::stringstream ss(line);
                std::string word;
                while (ss >> word) {
                    table.put(word);
                }
            }
            consultas_gerais(consultas, table);
        }
        else if (choice == "a23") {
            std::string filePath = R"(C:\Users\Matheus\CLionProjects\EP02 de MAC0323\file.txt)";
            std::ifstream inputFile(filePath);
            if (!inputFile.is_open()) {
                std::cout << "Falha ao abrir o arquivo de texto" << std::endl;
                return 1;
            }
            SymbolTable23 table;
            std::string line;
            while (std::getline(inputFile, line)) {
                std::stringstream ss(line);
                std::string word;
                while (ss >> word) {
                    table.put(word);
                }
            }
            table.print();
            for (string consult : consultas) {
                const char o = 'O';
                // Acha as palavras com maiores ocorrências no texto
                if (consult == "F"){
                    cout << endl;
                    cout << "Maior(es) ocorrencia(s) no texto: " << endl;
                    table.mostOccurred();
                    cout << endl;
                }
                    // Verifica quantas vezes a palavra dada está no texto
                else if (consult[0] == o) {
                    cout << endl;
                    string key;
                    size_t spacePos = consult.find(' ');
                    if (spacePos != std::string::npos) {
                        // Extract the substring starting from the character after the space
                        key = consult.substr(spacePos + 1);
                    }
                    Item value;
                    value = table.find(key);
                    if (value.occurrences < 1) cout << "A palavra " << key << " nao aparece no texto" << endl;
                    else cout << "A palavra " << key << " aparece " << value.occurrences << " vezes no texto" << endl;
                }
                else if (consult == "L") {
                    cout << endl;
                    cout << "Palavra mais longa: ";
                    table.longer();
                    cout << endl;
                }
                else if (consult == "SR") {
                    cout << endl;
                    cout << "Palavra mais longa sem letras repetidas: ";
                    table.longerwithnorepeatedletters();
                    cout << endl;
                }
                else if (consult == "VD") {
                    cout << endl;
                    cout << "Palavra mais curta sem repeticao e com mais vogais: ";
                    table.shorterwithmostvowel();
                    cout << endl;
                }
            }
        }
        else if (choice == "arn") {
            increased = false;
            std::string filePath = R"(C:\Users\Matheus\CLionProjects\EP02 de MAC0323\file.txt)";
            std::ifstream inputFile(filePath);
            if (!inputFile.is_open()) {
                std::cout << "Falha ao abrir o arquivo de texto" << std::endl;
                return 1;
            }
            SymbolTableRedBlack table;
            std::string line;
            while (std::getline(inputFile, line)) {
                std::stringstream ss(line);
                std::string word;
                while (ss >> word) {
                    table.put(word);
                }
            }
            for (string consult : consultas) {
                const char o = 'O';
                // Acha as palavras com maiores ocorrências no texto
                if (consult == "F") {
                    cout << endl;
                    cout << "Maior(es) ocorrencia(s) no texto: " << endl;
                    table.mostOccurred();
                    cout << endl;
                }
                else if (consult[0] == o) {
                    cout << endl;
                    string key;
                    size_t spacePos = consult.find(' ');
                    if (spacePos != std::string::npos) {
                        // Extract the substring starting from the character after the space
                        key = consult.substr(spacePos + 1);
                    }
                    RBNode* node;
                    node = table.get(key);
                    if (node->value.occurrences < 1) cout << "A palavra " << key << " nao aparece no texto" << endl;
                    else cout << "A palavra " << key << " aparece " << node->value.occurrences << " vezes no texto" << endl;
                }
                else if (consult == "L") {
                    cout << endl;
                    cout << "Palavra mais longa: ";
                    table.longer();
                    cout << endl;
                }
                else if (consult == "SR") {
                    cout << endl;
                    cout << "Palavra mais longa sem letras repetidas: ";
                    table.longerwithnorepeatedletters();
                    cout << endl;
                }
                else if (consult == "VD") {
                    cout << endl;
                    cout << "Palavra mais curta sem repeticao e com mais vogais: ";
                    table.shorterwithmostvowel();
                    cout << endl;
                }
                else break;
            }
        }
        else cout << "Valor invalido, insira um dentre as opcoes" << endl;
        cout << "Fim do teste" << endl;
        cout << endl;
    }
    return 0;
}
