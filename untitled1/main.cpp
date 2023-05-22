#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <utility>
#include <random>
#include <stack>
#include <queue>

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
            std::cout << node->key << " : " << std::endl;
            traverseLeft(node->right);
        }
    }

    void inorderTraversal(BSTNode* root, std::vector<BSTNode*>& maxNodes, int& maxOccurrences) {
        if (root != nullptr) {
            inorderTraversal(root->left, maxNodes, maxOccurrences);

            // Check if the current node has more occurrences
            if (root->value.occurrences > maxOccurrences) {
                maxOccurrences = root->value.occurrences;
                maxNodes.clear(); // Clear previous max nodes
                maxNodes.push_back(root);
            } else if (root->value.occurrences == maxOccurrences) {
                maxNodes.push_back(root);
            }

            inorderTraversal(root->right, maxNodes, maxOccurrences);
        }
    }
    void inorderTraversalL(BSTNode* root, std::vector<BSTNode*>& maxNodes, int& maxLetters) {
        if (root != nullptr) {
            inorderTraversalL(root->left, maxNodes, maxLetters);

            // Check if the current node has more occurrences
            if (root->value.letters > maxLetters) {
                maxLetters = root->value.letters;
                maxNodes.clear(); // Clear previous max nodes
                maxNodes.push_back(root);
            } else if (root->value.letters == maxLetters) {
                maxNodes.push_back(root);
            }

            inorderTraversalL(root->right, maxNodes, maxLetters);
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

        inorderTraversal(root, maxNodes, maxOccurrences);

        if (maxNodes.empty()) {
            std::cout << "The binary search tree is empty." << std::endl;
        } else {
            std::cout << "Nodes with the most occurrences:" << std::endl;
            for (const auto& node : maxNodes) {
                std::cout << "Key: " << node->key << ", ";
                std::cout << "Occurrences: " << node->value.occurrences << std::endl;
            }
        }
    }
    void longer() {
        std::vector<BSTNode*> maxNodes;
        int maxLetters = 0;

        inorderTraversalL(root, maxNodes, maxLetters);

        if (maxNodes.empty()) {
            std::cout << "The binary search tree is empty." << std::endl;
        } else {
            std::cout << "Nodes with the most occurrences:" << std::endl;
            for (const auto& node : maxNodes) {
                std::cout << "Key: " << node->key << ", ";
                std::cout << "Letters: " << node->value.letters << std::endl;
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
            std::cout << "Word: " << entries[i].key << std:: endl <<
                      "Number of Occurrences: " << entries[i].value.occurrences << std::endl <<
                      "Number of Letters: " << entries[i].value.letters << std::endl <<
                      "Number of Vowels: " << entries[i].value.vowels << std::endl;
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

    void printTree() {
        traverseInOrder(root);
    }

    void print() {
        inorder(root);
    }

};

/*
struct Node23 {
    std::vector<std::string> keys;
    std::vector<Item> values;
    std::vector<Node23*> children;

    Node23() {}

    bool isLeaf() {
        return children.empty();
    }

    bool isFull() {
        return keys.size() == 2;
    }
};

class SymbolTable23 {
private:
    Node23* root;

    Item findValue(const std::string& key) {
        Item value{};
        value.occurrences = 1;
        value.letters = key.size();
        value.vowels = vowels(key);
        return value;
    }

    void insertNonFull(Node23* node, const std::string& key, Item value) {
        int i = node->keys.size() - 1;

        if (node->isLeaf()) {
            // Find the correct position to insert the key and value
            while (i >= 0 && key < node->keys[i]) {
                i--;
            }
            node->keys.insert(node->keys.begin() + i + 1, key);
            node->values.insert(node->values.begin() + i + 1, value);
        } else {
            // Find the child to descend into
            while (i >= 0 && key < node->keys[i]) {
                i--;
            }
            i++;

            if (node->children[i]->isFull()) {
                splitChild(node, i);
                if (key > node->keys[i]) {
                    i++;
                }
            }

            insertNonFull(node->children[i], key, value);
        }
    }

    void splitChild(Node23* parent, int index) {
        Node23* child = parent->children[index];
        Node23* newNode = new Node23();

        parent->keys.insert(parent->keys.begin() + index, child->keys[1]);
        parent->values.insert(parent->values.begin() + index, child->values[1]);

        parent->children.insert(parent->children.begin() + index + 1, newNode);

        newNode->keys.push_back(child->keys[2]);
        newNode->values.push_back(child->values[2]);

        child->keys.erase(child->keys.begin() + 1, child->keys.end());
        child->values.erase(child->values.begin() + 1, child->values.end());

        if (!child->isLeaf()) {
            newNode->children.push_back(child->children[3]);
            newNode->children.push_back(child->children[4]);

            child->children.erase(child->children.begin() + 3, child->children.end());
        }
    }


    Item getRecursive(Node23* node, const std::string& key) {
        int i = 0;
        while (i < node->keys.size() && key > node->keys[i]) {
            i++;
        }

        if (i < node->keys.size() && key == node->keys[i]) {
            return node->values[i];
        } else if (node->isLeaf()) {
            return Item::None();
        } else {
            return getRecursive(node->children[i], key);
        }
    }

    void traverseInOrder(Node23* node) {
        if (node != nullptr) {
            for (int i = 0; i < node->keys.size(); i++) {
                traverseInOrder(node->children[i]);
                std::cout << node->keys[i] << " : " << std::endl;
            }
            traverseInOrder(node->children[node->keys.size()]);
        }
    }
    void inorder(Node23* root) {
        if (root) {
            if (root->isLeaf()) {
                for (int i = 0; i < root->keys.size(); i++) {
                    std::cout << "key: " << root->keys[i] << std::endl;
                }
            } else {
                for (int i = 0; i < root->keys.size(); i++) {
                    inorder(root->children[i]);
                    std::cout << "key: " << root->keys[i] << std::endl;
                }
                inorder(root->children[root->keys.size()]);
            }
        }
    }


public:
    SymbolTable23() : root(nullptr) {}

    void put(const std::string& key) {
        Item value = findValue(key);
        if (root == nullptr) {
            root = new Node23();
            root->keys.push_back(key);
            root->values.push_back(value);
        } else {
            if (root->isFull()) {
                Node23* newRoot = new Node23();
                newRoot->children.push_back(root);
                splitChild(newRoot, 0);
                root = newRoot;
            }
            insertNonFull(root, key, value);
        }
    }

    Item get(const std::string& key) {
        if (root == nullptr) {
            return Item::None();
        } else {
            return getRecursive(root, key);
        }
    }

    void printTree() {
        traverseInOrder(root);
    }
    void print() {
        inorder(root);
    }
};

struct Node23{
    bool is2node;
    std::string lkey; //left key
    std::string rkey; //right key --> rkey > lkey
    Item lvalue; //left value
    Item rvalue; //right value
    Node23* left; // lkey
    Node23* mid; // lkey and rkey
    Node23* right; // rkey
};
*/
struct No23 {
    bool is2no;
    std::string ch1;
    Item val1;
    std::string ch2;
    Item val2;
    No23* p1;
    No23* p2;
    No23* p3;
    No23()
            : is2no(true), ch1(""), val1(Item().None()), ch2(""), val2(Item().None()), p1(nullptr), p2(nullptr), p3(nullptr) {}
};
bool increased = false;
class SymbolTable23 {
private:
    No23* root;

    No23* add23(No23* root, const std::string& key, const Item& val) {
        if (root == nullptr) {
            root = new No23();
            root->ch1 = key;
            root->val1 = val;
            increased = true;
            return root;
        }
        if (key == root->ch1) {
            root->val1.occurrences +=1;
            return root;
        }
        else if (key == root->ch2) {
            root->val2.occurrences +=1;
            return root;
        }
        if (root->p1 != nullptr) { // raiz is not a leaf
            if (root->ch1 > key) {
                No23* p = add23(root->p1, key, val);
                if (increased) {
                    if (root->is2no) {
                        root->ch2 = root->ch1;
                        root->val2 = root->val1;
                        root->p3 = root->p2;
                        root->ch1 = p->ch1;
                        root->p2 = p->p2;
                        root->p1 = p->p1;
                        root->val1 = p->val1;
                        increased = false;
                        root->is2no = false;
                        delete p;
                        return root;
                    } else {
                        No23* newNode = new No23();
                        newNode->ch1 = root->ch2;
                        newNode->val1 = root->val2;
                        newNode->p2 = root->p3;
                        newNode->p1 = root->p2;
                        No23* nroot = new No23();
                        nroot->ch1 = root->ch1;
                        nroot->val2 = root->val1;
                        nroot->p2 = newNode;
                        nroot->p1 = root;
                        root->ch1 = p->ch1;
                        root->val1 = p->val1;
                        root->ch2 = "";
                        root->val2 = val;
                        root->p3 = nullptr;
                        root->p2 = p->p2;
                        newNode->is2no = root->is2no = nroot->is2no = true;
                        increased = true;
                        delete p;
                        return nroot;
                    }
                }
                else{
                    root->p1 = p;
                    /*if (raiz->ch1 == "d" && raiz->p2->is2no == false && raiz->p2->p3->is2no == false){
                        std::cout << "raiz ch1 e: " << raiz->ch1 << std::endl;
                        std::cout << "p1 ch1 e: " << raiz->p1->ch1 << std::endl;
                        std::cout << "p2 ch1 e: " << raiz->p2->ch1 << std::endl;
                        std::cout << "p2 ch2 e: " << raiz->p2->ch2 << std::endl;
                        std::cout << "p1->p1 ch1 e: " << raiz->p1->p1->ch1 << std::endl;
                        std::cout << "p1->p1 ch2 e: " << raiz->p1->p1->ch2 << std::endl;
                        std::cout << "p1->p2 ch1 e: " << raiz->p1->p2->ch1 << std::endl;
                        std::cout << "p2->p1 ch1 e: " << raiz->p2->p1->ch1 << std::endl;
                        std::cout << "p2->p2 ch1 e: " << raiz->p2->p2->ch1 << std::endl;
                        std::cout << "p2->p3 ch1 e: " << raiz->p2->p3->ch1 << std::endl;
                        std::cout << "p2->p3 ch2 e: " << raiz->p2->p3->ch2 << std::endl;
                    }*///teste do 11º
                    /*if (raiz->ch1 == "d" && !raiz->p1->is2no){
                        std::cout << "raiz ch1 e: " << raiz->ch1 << std::endl;
                        std::cout << "p1 ch1 e: " << raiz->p1->ch1 << std::endl;
                        std::cout << "p1 ch2 e: " << raiz->p1->ch2 << std::endl;
                        std::cout << "p2 ch1 e: " << raiz->p2->ch1 << std::endl;
                        std::cout << "p2 ch2 e: " << raiz->p2->ch2 << std::endl;
                        std::cout << "p1->p1 ch1 e: " << raiz->p1->p1->ch1 << std::endl;
                        std::cout << "p1->p2 ch1 e: " << raiz->p1->p2->ch1 << std::endl;
                        std::cout << "p1->p3 ch1 e: " << raiz->p1->p3->ch1 << std::endl;
                        std::cout << "p1->p3 ch2 e: " << raiz->p1->p3->ch2 << std::endl;
                        std::cout << "p2->p1 ch1 e: " << raiz->p2->p1->ch1 << std::endl;
                        std::cout << "p2->p1 ch2 e: " << raiz->p2->p1->ch2 << std::endl;
                        std::cout << "p2->p2 ch1 e: " << raiz->p2->p2->ch1 << std::endl;
                        std::cout << "p2->p3 ch1 e: " << raiz->p2->p3->ch1 << std::endl;
                        std::cout << "p2->p3 ch2 e: " << raiz->p2->p3->ch2 << std::endl;
                    }*///teste do 14º
                    return root;
                }
            }
            else if (root->is2no || root->ch2 > key) {
                No23* p = add23(root->p2, key, val);
                if (increased) {
                    if (p->is2no) {
                        root->ch2 = p->ch1;
                        root->val2 = p->val1;
                        root->p3 = p->p2;
                        root->p2 = p->p1;
                        increased = false;
                        delete p;
                        root->is2no = false;
                       /* std::cout << " ch1 da raiz: " << raiz->ch1 << " ch2 da raiz: " << raiz->ch2 << std::endl;
                        std::cout << " ch1 p1: " << raiz->p1->ch1 << std::endl;
                        std::cout << " ch1 p2: " << raiz->p2->ch1 << std::endl;
                        std::cout << " ch1 p3: " << raiz->p3->ch1 << std::endl;*/
                        return root;
                    }
                    else {
                        No23* newNode = new No23();
                        newNode->ch1 = root->ch2;
                        newNode->val1 = root->val2;
                        newNode->p2 = root->p3;
                        newNode->p1 = root->p2;
                        root->ch2 = p->ch1;
                        root->val2 = p->val1;
                        root->p3 = p->p2;
                        root->p2 = newNode;
                        increased = true;
                        delete p;
                        return root;
                    }
                }
                else {
                    root->p2 = p;
                    /*if (raiz->ch1 == "b" && raiz->ch2 == "d"){
                        std::cout << "raiz ch1 e: " << raiz->ch1 << std::endl;
                        std::cout << "raiz ch2 e: " << raiz->ch2 << std::endl;
                        std::cout << "p1 ch1 e: " << raiz->p1->ch1 << std::endl;
                        std::cout << "p2 ch1 e: " << raiz->p2->ch1 << std::endl;
                        std::cout << "p2 ch2 e: " << raiz->p2->ch2 << std::endl;
                        std::cout << "p3 ch1 e: " << raiz->p3->ch1 << std::endl;
                    }//teste do 6º*/
                    /*if (raiz->ch1 == "d"){
                        std::cout << "raiz ch1 e: " << raiz->ch1 << std::endl;
                        std::cout << "p1 ch1 e: " << raiz->p1->ch1 << std::endl;
                        std::cout << "p2 ch1 e: " << raiz->p2->ch1 << std::endl;
                        std::cout << "p1->p1 ch1 e: " << raiz->p1->p1->ch1 << std::endl;
                        std::cout << "p1->p2 ch1 e: " << raiz->p1->p2->ch1 << std::endl;
                        std::cout << "p2->p1 ch1 e: " << raiz->p2->p1->ch1 << std::endl;
                        std::cout << "p2->p2 ch1 e: " << raiz->p2->p2->ch1 << std::endl;
                        std::cout << "p2->p2 ch2 e: " << raiz->p2->p2->ch2 << std::endl;
                    }*///teste do 8º
                    /*if (raiz->ch1 == "d" && raiz->p2->is2no == false){
                        std::cout << "raiz ch1 e: " << raiz->ch1 << std::endl;
                        std::cout << "p1 ch1 e: " << raiz->p1->ch1 << std::endl;
                        std::cout << "p2 ch1 e: " << raiz->p2->ch1 << std::endl;
                        std::cout << "p2 ch2 e: " << raiz->p2->ch2 << std::endl;
                        std::cout << "p1->p1 ch1 e: " << raiz->p1->p1->ch1 << std::endl;
                        std::cout << "p1->p2 ch1 e: " << raiz->p1->p2->ch1 << std::endl;
                        std::cout << "p2->p1 ch1 e: " << raiz->p2->p1->ch1 << std::endl;
                        std::cout << "p2->p2 ch1 e: " << raiz->p2->p2->ch1 << std::endl;
                        std::cout << "p2->p3 ch1 e: " << raiz->p2->p3->ch1 << std::endl;
                    }*///teste do 9º
                    /*if (raiz->ch1 == "d" && raiz->p2->is2no == false && raiz->p2->p3->is2no == false){
                        std::cout << "raiz ch1 e: " << raiz->ch1 << std::endl;
                        std::cout << "p1 ch1 e: " << raiz->p1->ch1 << std::endl;
                        std::cout << "p2 ch1 e: " << raiz->p2->ch1 << std::endl;
                        std::cout << "p2 ch2 e: " << raiz->p2->ch2 << std::endl;
                        std::cout << "p1->p1 ch1 e: " << raiz->p1->p1->ch1 << std::endl;
                        std::cout << "p1->p2 ch1 e: " << raiz->p1->p2->ch1 << std::endl;
                        std::cout << "p2->p1 ch1 e: " << raiz->p2->p1->ch1 << std::endl;
                        std::cout << "p2->p2 ch1 e: " << raiz->p2->p2->ch1 << std::endl;
                        std::cout << "p2->p3 ch1 e: " << raiz->p2->p3->ch1 << std::endl;
                        std::cout << "p2->p3 ch2 e: " << raiz->p2->p3->ch2 << std::endl;
                    }*///teste do 10º

                    return root;
                }
            }
            else {
                /* std::cout << "insere no p3 a palavra: " << raiz->p3 << std::endl;*/
                No23* p = add23(root->p3, key, val);
                if (increased) {
                    No23* nroot = new No23();
                    No23* newNode = new No23();
                    nroot->ch1 = root->ch2;
                    nroot->val1 = root->val2;
                    newNode->ch1 = p->ch1;
                    newNode->p1 = p->p1;
                    newNode->p2 = p->p2;
                    root->ch2 = "";
                    root->val2 = Item::None();
                    root->p3 = nullptr;
                    nroot->p1 = root;
                    nroot->p2 = newNode;
                    delete(p);
                    increased = true;
                    nroot->is2no = root->is2no = newNode->is2no;
                    /*if(nroot->ch1 == "d"){
                        std::cout << "raiz ch1 e: " << nroot->ch1 << std::endl;
                        std::cout << "p1 ch1 e: " << nroot->p1->ch1 << std::endl;
                        std::cout << "p2 ch1 e: " << nroot->p2->ch1 << std::endl;
                        std::cout << "p1->p1 ch1 e: " << nroot->p1->p1->ch1 << std::endl;
                        std::cout << "p1->p2 ch1 e: " << nroot->p1->p2->ch1 << std::endl;
                        std::cout << "p2->p1 ch1 e: " << nroot->p2->p1->ch1 << std::endl;
                        std::cout << "p2->p2 ch1 e: " << nroot->p2->p2->ch1 << std::endl;
                    }*/
                    return nroot;
                }
                else {
                    root->p3 = p;
                    /*std::cout << "raiz ch1 e: " << raiz->ch1 << std::endl;
                    std::cout << "raiz ch2 e: " << raiz->ch2 << std::endl;
                    std::cout << "p1 ch1 e: " << raiz->p1->ch1 << std::endl;
                    std::cout << "p2 ch1 e: " << raiz->p2->ch1 << std::endl;
                    std::cout << "p3 ch1 e: " << raiz->p3->ch1 << std::endl;
                    std::cout << "p3 ch2 e: " << raiz->p3->ch2 << std::endl;*///teste do 6º
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
                    /*std::cout << raiz->ch1 << "--e--" << raiz->ch2 << std::endl;*///teste 2º
                    return root;
                }
                else {
                    root->ch2 = key;
                    root->val2 = val;
                    root->is2no = false;
                    increased = false;
                    /*std::cout << "ch1: " << raiz->ch1 << std::endl;
                    std::cout << "ch2: " << raiz->ch2 << std::endl;*/
                    return root;
                }
            }
            else { // raiz is a 3 node
                if (key < root->ch1) {
                    No23* newNode = new No23();
                    No23* nroot = new No23();
                    newNode->ch1 = root->ch2;
                    newNode->val1 = root->val2;
                    newNode->p2 = root->p3;
                    nroot->ch1 = root->ch1;
                    nroot->val1 = root->val1;
                    root->ch1 = key;
                    root->val1 = val;
                    root->ch2 = "";
                    root->val2 = val;
                    nroot->p1 = root;
                    nroot->p2 = newNode;
                    increased = true;
                    root->is2no = nroot->is2no = newNode->is2no = true;
                    return nroot;
                }
                else if (key < root->ch2) {
                    No23* newNode = new No23();
                    No23* nroot = new No23();
                    newNode->ch1 = root->ch2;
                    newNode->val1 = root->val2;
                    nroot->ch1 = key;
                    nroot->val1 = val;
                    root->ch2 = "";
                    root->val2 = val.None();
                    nroot->p1 = root;
                    nroot->p2 = newNode;
                    nroot->p3 = nullptr;
                    increased = true;
                    root->is2no = nroot->is2no = newNode->is2no = true;
                   /* std::cout << nroot->ch1 << " essa e a raiz" << std::endl;
                    std::cout << nroot->p1->ch1 << " essa e o ch1 do p1" << std::endl;
                    std::cout << nroot->p2->ch1 << " essa e o ch1 do p2" << std::endl;*/ //teste do 3º
                    return nroot;
                }
                else {
                    No23* newNode = new No23();
                    No23* nroot = new No23();
                    newNode->ch1 = key;
                    newNode->val1 = val;
                    nroot->ch1 = root->ch2;
                    nroot->val1 = root->val2;
                    root->ch2 = "";
                    root->val2 = val;
                    nroot->p1 = root;
                    nroot->p2 = newNode;
                    increased = true;
                    root->is2no = nroot->is2no = newNode->is2no = true;
                    return nroot;
                }
            }
        }
    }

    Item get(No23* root, std::string key) {
        std::stack<No23*> stack;
        stack.push(root);

        while (!stack.empty()) {
            No23* current = stack.top();
            stack.pop();

            if (current != nullptr) {
                if (current->ch1 == key) {
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

    void inorder(No23* root) {
        if (root != nullptr) {
            if (root->p1 == nullptr) { // Node is a leaf
                std::cout << "key: " << root->ch1 << std::endl;
                if (!root->is2no) {
                    std::cout << "key: " << root->ch2 << std::endl;
                }
            } else { // Node is not a leaf
                inorder(root->p1);
                std::cout << "key: " << root->ch1 << std::endl;
                inorder(root->p2);
                if (!root->is2no) {
                    std::cout << "key: " << root->ch2 << std::endl;
                    inorder(root->p3);
                }
            }
        }
    }

    Item findValue(const std::string& key) {
        Item value{};
        value.occurrences = 1;
        value.letters = key.size();
        value.vowels = vowels(key);
        return value;
    }

public:
    SymbolTable23() : root(nullptr) {}
    void put(std::string key) {
        Item value = findValue(key);
        root = add23(root, key, value);
    }
    void print() {
        inorder(root);
    }
    Item find(std::string& key){
        return get(root, key);
    }

};

const bool RED = true;
const bool BLACK = false;

class RBNode {
public:
    std::string key; // key
    Item val; // associated data
    RBNode* left; // left subtree
    RBNode* right; // right subtree
    int N; // # nodes in this subtree
    bool color; // color of link from parent to this node

    RBNode(std::string& key, Item val, int N, bool color) {
        this->key = key;
        this->val = val;
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
            h->val.occurrences += 1; // Increment occurrences when keys are equal

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
        std::cout << "Key: " << node->key << ", Occurrences: " << node->val.occurrences << std::endl;
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
        for (char& c : choice) {
            c = tolower(c);
        }
        if (choice == "sair") break;
        else if (choice == "vo"){
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
            }
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
            for (string consult : consultas) {
                const char o = 'O';
                // Acha as palavras com maiores ocorrências no texto
                if (consult == "F"){
                    cout << endl;
                    cout << "Maior(es) ocorrencia(s) no texto: " << endl;
                    //table.mostOccurred();
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
                    //table.longer();
                    cout << endl;
                }
                else if (consult == "SR") {

                }
            }
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
            for (string consult : consultas) {
                const char o = 'O';
                // Acha as palavras com maiores ocorrências no texto
                if (consult == "F"){
                    cout << endl;
                    cout << "Maior(es) ocorrencia(s) no texto: " << endl;
                    //table.mostOccurred();
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
                    //table.longer();
                    cout << endl;
                }
                else if (consult == "SR") {

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
                if (consult == "F"){
                    cout << endl;
                    cout << "Maior(es) ocorrencia(s) no texto: " << endl;
                    //table.mostOccurred();
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
                    RBNode* node;
                    node = table.get(key);
                    if (node->val.occurrences < 1) cout << "A palavra " << key << " nao aparece no texto" << endl;
                    else cout << "A palavra " << key << " aparece " << node->val.occurrences << " vezes no texto" << endl;
                }
                else if (consult == "L") {
                    cout << endl;
                    cout << "Palavra mais longa: ";
                    //table.longer();
                    cout << endl;
                }
                else if (consult == "SR") {

                }
            }
        }
        else cout << "Valor invalido, insira um dentre as opcoes" << endl;

        cout << "Fim do teste" << endl;
        cout << endl;
    }


    return 0;
}
