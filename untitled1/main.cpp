#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <utility>
#include <random>

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
        size_t start = 0;
        size_t end = size;
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

    Item findValue(const std::string& key) {
        Item value{};
        value.occurrences = 1;
        value.letters = key.size();
        value.vowels = vowels(key);
        return value;
    }

    TreapNode* rotateLeft(TreapNode* node) {
        TreapNode* newRoot = node->right;
        node->right = newRoot->left;
        newRoot->left = node;
        return newRoot;
    }

    TreapNode* rotateRight(TreapNode* node) {
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
struct No {
    bool is2no;
    std::string ch1;
    Item val1;
    std::string ch2;
    Item val2;
    No* p1;
    No* p2;
    No* p3;
    No()
            : is2no(true), ch1(""), val1(Item().None()), ch2(""), val2(Item().None()), p1(nullptr), p2(nullptr), p3(nullptr) {}
};
bool cresceu = false;
class SymbolTable23 {
private:
    No* raiz;

    No* add23(No* raiz, const std::string& key, const Item& val) {
        if (raiz == nullptr) {
            raiz = new No();
            raiz->ch1 = key;
            raiz->val1 = val;
            cresceu = true;
            return raiz;
        }

        if (raiz->p1 != nullptr) { // raiz is not a leaf
            if (raiz->ch1 > key) {
                No* p = add23(raiz->p1, key, val);
                if (cresceu) {
                    if (raiz->is2no) {
                        raiz->ch2 = raiz->ch1;
                        raiz->val2 = raiz->val1;
                        raiz->p3 = raiz->p2;
                        raiz->ch1 = p->ch1;
                        raiz->p2 = p->p2;
                        raiz->p1 = p->p1;
                        raiz->val1 = p->val1;
                        cresceu = false;
                        raiz->is2no = false;
                        delete p;
                        return raiz;
                    } else {
                        No* novo = new No();
                        novo->ch1 = raiz->ch2;
                        novo->val1 = raiz->val2;
                        novo->p2 = raiz->p3;
                        novo->p1 = raiz->p2;
                        No* nraiz = new No();
                        nraiz->ch1 = raiz->ch1;
                        nraiz->val2 = raiz->val1;
                        nraiz->p2 = novo;
                        nraiz->p1 = raiz;
                        raiz->ch1 = p->ch1;
                        raiz->val1 = p->val1;
                        raiz->ch2 = "";
                        raiz->val2 = val;
                        raiz->p3 = nullptr;
                        raiz->p2 = p->p2;
                        novo->is2no = raiz->is2no = nraiz->is2no = true;
                        cresceu = true;
                        delete p;
                        return nraiz;
                    }
                }
                else{
                    raiz->p1 = p;
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
                    return raiz;
                }
            }
            else if (raiz->is2no || raiz->ch2 > key) {
                No* p = add23(raiz->p2, key, val);
                if (cresceu) {
                    if (p->is2no) {
                        raiz->ch2 = p->ch1;
                        raiz->val2 = p->val1;
                        raiz->p3 = p->p2;
                        raiz->p2 = p->p1;
                        cresceu = false;
                        delete p;
                        raiz->is2no = false;
                       /* std::cout << " ch1 da raiz: " << raiz->ch1 << " ch2 da raiz: " << raiz->ch2 << std::endl;
                        std::cout << " ch1 p1: " << raiz->p1->ch1 << std::endl;
                        std::cout << " ch1 p2: " << raiz->p2->ch1 << std::endl;
                        std::cout << " ch1 p3: " << raiz->p3->ch1 << std::endl;*/
                        return raiz;
                    }
                    else {
                        No* novo = new No();
                        novo->ch1 = raiz->ch2;
                        novo->val1 = raiz->val2;
                        novo->p2 = raiz->p3;
                        novo->p1 = raiz->p2;
                        raiz->ch2 = p->ch1;
                        raiz->val2 = p->val1;
                        raiz->p3 = p->p2;
                        raiz->p2 = novo;
                        cresceu = true;
                        delete p;
                        return raiz;
                    }
                }
                else {
                    raiz->p2 = p;
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

                    return raiz;
                }
            }
            else {
               /* std::cout << "insere no p3 a palavra: " << raiz->p3 << std::endl;*/
                No* p = add23(raiz->p3, key, val);
                if (cresceu) {
                    No* nraiz = new No();
                    No* novo = new No();
                    nraiz->ch1 = raiz->ch2;
                    nraiz->val1 = raiz->val2;
                    novo->ch1 = p->ch1;
                    novo->p1 = p->p1;
                    novo->p2 = p->p2;
                    raiz->ch2 = "";
                    raiz->val2 = Item::None();
                    raiz->p3 = nullptr;
                    nraiz->p1 = raiz;
                    nraiz->p2 = novo;
                    delete(p);
                    cresceu = true;
                    nraiz->is2no = raiz->is2no = novo->is2no;
                    /*if(nraiz->ch1 == "d"){
                        std::cout << "raiz ch1 e: " << nraiz->ch1 << std::endl;
                        std::cout << "p1 ch1 e: " << nraiz->p1->ch1 << std::endl;
                        std::cout << "p2 ch1 e: " << nraiz->p2->ch1 << std::endl;
                        std::cout << "p1->p1 ch1 e: " << nraiz->p1->p1->ch1 << std::endl;
                        std::cout << "p1->p2 ch1 e: " << nraiz->p1->p2->ch1 << std::endl;
                        std::cout << "p2->p1 ch1 e: " << nraiz->p2->p1->ch1 << std::endl;
                        std::cout << "p2->p2 ch1 e: " << nraiz->p2->p2->ch1 << std::endl;
                    }*/
                    return nraiz;
                }
                else {
                    raiz->p3 = p;
                    /*std::cout << "raiz ch1 e: " << raiz->ch1 << std::endl;
                    std::cout << "raiz ch2 e: " << raiz->ch2 << std::endl;
                    std::cout << "p1 ch1 e: " << raiz->p1->ch1 << std::endl;
                    std::cout << "p2 ch1 e: " << raiz->p2->ch1 << std::endl;
                    std::cout << "p3 ch1 e: " << raiz->p3->ch1 << std::endl;
                    std::cout << "p3 ch2 e: " << raiz->p3->ch2 << std::endl;*///teste do 6º
                    return raiz;

                }
            }
        }
        else { // raiz is a leaf
            if (raiz->is2no) {
                if (raiz->ch1 > key) {
                    raiz->ch2 = raiz->ch1;
                    raiz->val2 = raiz->val1;
                    raiz->ch1 = key;
                    raiz->val1 = val;
                    raiz->is2no = false;
                    cresceu = false;
                    /*std::cout << raiz->ch1 << "--e--" << raiz->ch2 << std::endl;*///teste 2º
                    return raiz;
                }
                else {
                    raiz->ch2 = key;
                    raiz->val2 = val;
                    raiz->is2no = false;
                    cresceu = false;
                    /*std::cout << "ch1: " << raiz->ch1 << std::endl;
                    std::cout << "ch2: " << raiz->ch2 << std::endl;*/
                    return raiz;
                }
            }
            else { // raiz is a 3 node
                if (key < raiz->ch1) {
                    No* novo = new No();
                    No* nraiz = new No();
                    novo->ch1 = raiz->ch2;
                    novo->val1 = raiz->val2;
                    novo->p2 = raiz->p3;
                    nraiz->ch1 = raiz->ch1;
                    nraiz->val1 = raiz->val1;
                    raiz->ch1 = key;
                    raiz->val1 = val;
                    raiz->ch2 = "";
                    raiz->val2 = val;
                    nraiz->p1 = raiz;
                    nraiz->p2 = novo;
                    cresceu = true;
                    raiz->is2no = nraiz->is2no = novo->is2no = true;
                    return nraiz;
                }
                else if (key < raiz->ch2) {
                    No* novo = new No();
                    No* nraiz = new No();
                    novo->ch1 = raiz->ch2;
                    novo->val1 = raiz->val2;
                    nraiz->ch1 = key;
                    nraiz->val1 = val;
                    raiz->ch2 = "";
                    raiz->val2 = val.None();
                    nraiz->p1 = raiz;
                    nraiz->p2 = novo;
                    nraiz->p3 = nullptr;
                    cresceu = true;
                    raiz->is2no = nraiz->is2no = novo->is2no = true;
                   /* std::cout << nraiz->ch1 << " essa e a raiz" << std::endl;
                    std::cout << nraiz->p1->ch1 << " essa e o ch1 do p1" << std::endl;
                    std::cout << nraiz->p2->ch1 << " essa e o ch1 do p2" << std::endl;*/ //teste do 3º
                    return nraiz;
                } else {
                    No* novo = new No();
                    No* nraiz = new No();
                    novo->ch1 = key;
                    novo->val1 = val;
                    nraiz->ch1 = raiz->ch2;
                    nraiz->val1 = raiz->val2;
                    raiz->ch2 = "";
                    raiz->val2 = val;
                    nraiz->p1 = raiz;
                    nraiz->p2 = novo;
                    cresceu = true;
                    raiz->is2no = nraiz->is2no = novo->is2no = true;
                    return nraiz;
                }
            }
        }
    }

    Item get(No* root, const std::string& key) {
        if (root == nullptr) {
            // Key not found, return some default value or handle the case accordingly
            return Item::None() ; // Assuming Item has a default constructor
        }

        if (root->p1 == nullptr) {
            // Node is a leaf, check if the key matches
            if (root->ch1 == key) {
                return root->val1;
            }
            if (!root->is2no && root->ch2 == key) {
                return root->val2;
            }
            // Key not found in the leaf node
            return Item::None(); // Assuming Item has a default constructor
        }

        // Node is not a leaf, perform recursive search
        if (key < root->ch1) {
            return get(root->p1, key);
        } else if (root->is2no || key < root->ch2) {
            return get(root->p2, key);
        } else {
            return get(root->p3, key);
        }
    }


    void inorder(No* root) {
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
    SymbolTable23() : raiz(nullptr) {}
    void put(std::string key) {
        Item value = findValue(key);
        raiz = add23(raiz, key, value);
    }
    void print() {
        inorder(raiz);
    }
    void printtest() {
        std::cout << raiz->p1->ch1 << std::endl;
    }
    Item find(std::string& key){
        return get(raiz, key);
    }

};

int main() {
    cresceu = false;
    SymbolTable23 table;
    std::string filePath = R"(C:\Users\Matheus\CLionProjects\EP02 de MAC0323\file.txt)";
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
    inputFile.close();
    table.print();


    return 0;
}
