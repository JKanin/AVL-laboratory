#include <iostream>
#include <string>
#include <fstream>
#include <vector>
using namespace std;

struct number { // структура госномера
    char series1;
    int num;
    string series2;

    bool operator==(const number& other) const { // Оператор ==
        return (series1 == other.series1 && num == other.num && series2 == other.series2);
    }

    bool operator<(const number& other) const { // Оператор <
        if (series1 != other.series1) {
            return series1 < other.series1;
        }
        else if (num != other.num) {
            return num < other.num;
        }
        else {
            return series2 < other.series2;
        }
    }

    bool operator>(const number& other) const { // Оператор >
        if (series1 != other.series1) {
            return series1 > other.series1;
        }
        else if (num != other.num) {
            return num > other.num;
        }
        else {
            return series2 > other.series2;
        }
    }
};

struct Node { //структура узла дерева
    number key;
    unsigned char height;
    Node* left;
    Node* right;
    Node(number k) { key = k; left = right = nullptr; height = 1; }
};

void readNumbers(vector<number>& numbers) {//считывание узлов дерева из файла
    ifstream fin("TXT.txt", ios::in);

    char series1;
    int num;
    string series2;

    while (fin >> series1 >> num >> series2) {
        number numStruct = { series1, num, series2 };
        numbers.push_back(numStruct);
    }

    fin.close();
}

unsigned char balanceFactor(Node* p) { // баланс между левым и правым потомками
    unsigned char hl = p->left ? p->left->height : 0;
    unsigned char hr = p->right ? p->right->height : 0;
    return hr - hl;
}

void fixheight(Node* p) { // корректировка высоты выбранного узла
    unsigned char hl = p->left ? p->left->height : 0;
    unsigned char hr = p->right ? p->right->height : 0;
    p->height = (hl > hr ? hl : hr) + 1;
}

Node* turn_right(Node* p) { // правый поворот
    Node* q = p->left;
    if (!q) return p; // Если q == nullptr, ничего не делаем
    p->left = q->right;
    q->right = p;
    fixheight(p);
    fixheight(q);
    return q;
}

Node* turn_left(Node* q) { // левый поворот
    Node* p = q->right;
    if (!p) return q; // Если p == nullptr, ничего не делаем
    q->right = p->left;
    p->left = q;
    fixheight(q);
    fixheight(p);
    return p;
}

Node* balance_node(Node* p) {
    fixheight(p);
    if (balanceFactor(p) == 2) {
        // Большой левый поворот
        if (p->right && balanceFactor(p->right) < 0) {
            p->right = turn_right(p->right);
        }
        return turn_left(p);
    }
    if (balanceFactor(p) == -2) {
        // Большой правый поворот
        if (p->left && balanceFactor(p->left) > 0) {
            p->left = turn_left(p->left);
        }
        return turn_right(p);
    }
    return p;
}

Node* Insert(Node* p, number k) { // добавление нового узла
    if (!p) {
        return new Node(k);
    }
    if (k < p->key) {
        p->left = Insert(p->left, k);
    }
    if (k > p->key) {
        p->right = Insert(p->right, k);
    }
    return balance_node(p);
}

Node* findmin(Node* p) { // поиск минимального элемента среди предков p (для удаления элемента p)
    return p->left ? findmin(p->left) : p;
}

Node* deletemin(Node* p) { // удаление минимального элемента среди предков (для удаления элемента p)
    if (!p->left) {
        return p->right;
    }
    p->left = deletemin(p->left);
    return balance_node(p);
}

Node* DeleteEl(Node* p, number x) { // удаление элемента
    if (!p) {
        cout << "Element not found" << endl;
        return nullptr;
    }
    if (x < p->key) {
        p->left = DeleteEl(p->left, x);
    }
    else if (x > p->key) {
        p->right = DeleteEl(p->right, x);
    }
    else {
        Node* q = p->left;
        Node* g = p->right;
        delete p;
        if (!g) return q;
        Node* min = findmin(g);
        min->right = deletemin(g);
        min->left = q;
        return balance_node(min);
    }
    return balance_node(p);
}

void FindElemNumber(Node* p, int x) { // поиск элемента дерева по номеру
    if (!p) {
        return;
    }
    if (p->key.num == x) {
        cout << "Element found - " << p->key.series1 << ' ' << p->key.num << ' ' << p->key.series2 << endl;
    }
    FindElemNumber(p->left, x);
    FindElemNumber(p->right, x);
}

void PrintTree(Node* p, int space = 0, int level = 12) { // печать дерева
    if (!p) return;
    space += level;
    PrintTree(p->right, space);
    cout << endl;
    for (int i = level; i < space; i++)
        cout << " ";
    cout << p->key.series1 << ' ' << p->key.num << " " << p->key.series2 << endl;
    PrintTree(p->left, space);
}

int main() {
    Node* root = nullptr;
    vector<number> numbers;
    readNumbers(numbers);

    for (const auto& num : numbers) {
        root = Insert(root, num);
    }
    number num1 = { 'N',326, "BO" };
    
    PrintTree(root);
    ofstream out("TXT_1.txt");
    return 0;
}
