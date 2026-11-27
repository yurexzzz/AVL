// AVL: отдельная программа для семинара.
// Глубина корня 0, пустого дерева -1. Повороты считаются с начала запуска.
#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

struct Node {
    int key;
    Node* left;
    Node* right;
    int height; // Для AVL: число уровней в поддереве.
    static long long rotations;
    Node(int value) : key(value), left(nullptr), right(nullptr), height(1) {}
};
long long Node::rotations = 0;

// Поиск возвращает узел или nullptr. Повторные ключи игнорируем.
Node* find(Node* root, int key) {
    while (root != nullptr && root->key != key) {
        if (key < root->key) root = root->left;
        else root = root->right;
    }
    return root;
}

int height(Node* node) {
    if (node == nullptr) return 0;
    return node->height;
}

void update(Node* node) {
    node->height = 1 + max(height(node->left), height(node->right));
}

Node* rotateLeft(Node* a) {
    Node* b = a->right;
    a->right = b->left;
    b->left = a;
    update(a);
    update(b);
    Node::rotations++;
    return b;
}

Node* rotateRight(Node* a) {
    Node* b = a->left;
    a->left = b->right;
    b->right = a;
    update(a);
    update(b);
    Node::rotations++;
    return b;
}

Node* balance(Node* root) {
    update(root);
    int difference = height(root->left) - height(root->right);
    if (difference > 1) {
        if (height(root->left->left) < height(root->left->right))
            root->left = rotateLeft(root->left);
        return rotateRight(root);
    }
    if (difference < -1) {
        if (height(root->right->right) < height(root->right->left))
            root->right = rotateRight(root->right);
        return rotateLeft(root);
    }
    return root;
}

Node* insert(Node* root, int key) {
    if (root == nullptr) return new Node(key);
    if (key < root->key) root->left = insert(root->left, key);
    else if (key > root->key) root->right = insert(root->right, key);
    else return root;
    return balance(root);
}

Node* remove(Node* root, int key) {
    if (root == nullptr) return nullptr;
    if (key < root->key) root->left = remove(root->left, key);
    else if (key > root->key) root->right = remove(root->right, key);
    else {
        if (root->left == nullptr || root->right == nullptr) {
            Node* child = root->left;
            if (child == nullptr) child = root->right;
            delete root;
            return child;
        }
        Node* next = root->right;
        while (next->left != nullptr) next = next->left;
        root->key = next->key;
        root->right = remove(root->right, next->key);
    }
    return balance(root);
}

// Обход по уровням не переполняет стек вызовов на цепочке BST.
int depth(Node* root) {
    if (root == nullptr) return -1;
    vector<Node*> level = {root};
    int result = -1;
    while (!level.empty()) {
        vector<Node*> next;
        for (Node* node : level) {
            if (node->left != nullptr) next.push_back(node->left);
            if (node->right != nullptr) next.push_back(node->right);
        }
        level = next;
        result++;
    }
    return result; // Корень имеет глубину 0.
}

void clear(Node* root) {
    vector<Node*> nodes;
    if (root != nullptr) nodes.push_back(root);
    while (!nodes.empty()) {
        Node* node = nodes.back();
        nodes.pop_back();
        if (node->left != nullptr) nodes.push_back(node->left);
        if (node->right != nullptr) nodes.push_back(node->right);
        delete node;
    }
}

Node* treeRoot = nullptr;

void printStats() {
    cout << "Глубина: " << depth(treeRoot) << '\n';
    cout << "Повороты: " << Node::rotations << '\n';
}

int main() {
    cout << "AVL: дерево поиска\n";
    cout << "Повторные ключи не добавляются.\n";
    cout << "1 число: вставить\n2 число: удалить\n";
    cout << "3 число: найти\n4: глубина и повороты\n0: выход\n";
    int command, key;
    while (true) {
        cout << "> ";
        if (!(cin >> command) || command == 0) break;
        if (command >= 1 && command <= 3) {
            if (!(cin >> key)) break;
            if (command == 1) {
                treeRoot = insert(treeRoot, key);
                cout << "Операция завершена.\n";
            } else if (command == 2) {
                treeRoot = remove(treeRoot, key);
                cout << "Операция завершена.\n";
            } else {
                if (find(treeRoot, key) != nullptr) cout << "Найден\n";
                else cout << "Не найден\n";
            }
        } else if (command == 4) {
            printStats();
        } else {
            cout << "Неизвестная команда.\n";
        }
    }
    clear(treeRoot);
    return 0;
}
