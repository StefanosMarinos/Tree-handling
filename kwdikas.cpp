#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <random>
#include <climits>
#include <queue>
using namespace std;

// Κόμβος του Treap
class Node {
public:
    string name;       // Όνομα παίκτη
    string country;    // Χώρα παίκτη
    int ID;            // Μοναδικό αναγνωριστικό
    int rank;          // Κατάταξη (μικρότερο = καλύτερος)
    int signupTime;    // Χρόνος εγγραφής
    int priority;      // Τυχαία τιμή για ισορροπία Treap (Min-Heap)
    Node* left;
    Node* right;

    Node(string name, int ID, int rank, string country, int signupTime)
        : name(name), ID(ID), rank(rank), country(country), signupTime(signupTime),
        left(nullptr), right(nullptr) {
        priority = rand();  // Δίνουμε τυχαία προτεραιότητα στον κόμβο
    }
};

// Διαχωρισμός του δέντρου με βάση το signupTime
void split(Node* current, int key, Node*& left, Node*& right) {
    if (!current) {
        left = right = nullptr;
    }
    else if (key < current->signupTime) {
        split(current->left, key, left, current->left);
        right = current;
    }
    else {
        split(current->right, key, current->right, right);
        left = current;
    }
}

// Συνένωση δύο υποδέντρων
Node* merge(Node* left, Node* right) {
    if (!left || !right)
        return left ? left : right;

    if (left->priority > right->priority) {
        left->right = merge(left->right, right);
        return left;
    }
    else {
        right->left = merge(left, right->left);
        return right;
    }
}

// Εισαγωγή νέου κόμβου στο Treap
Node* insert(Node* root, Node* newNode) {
    if (!root) return newNode;

    if (newNode->priority > root->priority) {
        split(root, newNode->signupTime, newNode->left, newNode->right);
        return newNode;
    }
    else if (newNode->signupTime < root->signupTime) {
        root->left = insert(root->left, newNode);
    }
    else {
        root->right = insert(root->right, newNode);
    }
    return root;
}

// Διαγραφή παίκτη με βάση το ID
Node* deleteByID(Node* root, int ID, bool& found) {
    if (!root) return nullptr;

    if (root->ID == ID) {
        found = true;
        Node* temp = merge(root->left, root->right);
        delete root;
        return temp;
    }
    root->left = deleteByID(root->left, ID, found);
    root->right = deleteByID(root->right, ID, found);
    return root;
}

// Διαγραφή ολόκληρου υποδέντρου
void deleteSubtree(Node* node) {
    if (!node) return;
    deleteSubtree(node->left);
    deleteSubtree(node->right);
    delete node;
}

// Κλάση Treap που περιέχει τη ρίζα και λειτουργίες πάνω στο δέντρο
class Treap {
public:
    Node* root;

    Treap() : root(nullptr) {
        srand(time(0));  // Για τυχαία προτεραιότητα
    }

    // Εισαγωγή παίκτη
    void insert(string name, int id, int rank, string country, int signupTime) {
        Node* newNode = new Node(name, id, rank, country, signupTime);
        root = insert(root, newNode);
    }

    // Διαγραφή με βάση ID
    void deleteByID(int ID) {
        bool found = false;
        root = deleteByID(root, ID, found);
        if (found)
            cout << "Διαγράφηκε ο παίκτης με ID: " << ID << endl;
        else
            cout << "Δεν βρέθηκε παίκτης με ID: " << ID << endl;
    }

    // Διαγραφή παικτών με συγκεκριμένη ημερομηνία εγγραφής
    void deleteBySignupDate(int date) {
        Node* L, * R, * M, * R2;
        split(root, date - 1, L, R);  // R έχει nodes με signupTime >= date
        split(R, date, M, R2);        // M έχει signupTime == date
        int deletedCount = countNodes(M);
        deleteSubtree(M);            // Διαγράφουμε M
        root = merge(L, R2);         // Ξανασυνδέουμε το δέντρο
        cout << "Διαγράφηκαν " << deletedCount << " παίκτες με ημερομηνία εγγραφής: " << date << endl;
    }

    // Εκτύπωση ID παικτών με signupTime στο [t1, t2]
    void printIDsInRange(int t1, int t2) {
        cout << "ID παικτών μεταξύ " << t1 << " και " << t2 << ":\n";
        printIDsInRange(root, t1, t2);
    }

    void printIDsInRange(Node* node, int t1, int t2) {
        if (!node) return;
        if (node->signupTime > t1) printIDsInRange(node->left, t1, t2);
        if (node->signupTime >= t1 && node->signupTime <= t2)
            cout << node->ID << " (" << node->signupTime << ")\n";
        if (node->signupTime < t2) printIDsInRange(node->right, t1, t2);
    }

    // Εύρεση παίκτη με την καλύτερη κατάταξη (δηλ. μικρότερο rank) πριν από μια ημερομηνία
    int findBestRankBeforeDate(int date) {
        int bestRank = INT_MAX, bestID = -1;
        findBestRankBeforeDate(root, date, bestRank, bestID);
        return bestID;
    }

    void findBestRankBeforeDate(Node* node, int date, int& bestRank, int& bestID) {
        if (!node) return;
        if (node->signupTime < date && node->rank < bestRank) {
            bestRank = node->rank;
            bestID = node->ID;
        }
        findBestRankBeforeDate(node->left, date, bestRank, bestID);
        findBestRankBeforeDate(node->right, date, bestRank, bestID);
    }

    // Εύρεση του Κ-οστού καλύτερου παίκτη με βάση rank
    int findKthBestRank(int k) {
        priority_queue<pair<int, int>> pq;  // Max-heap
        findK(root, pq, k);
        if (pq.size() < k) return -1;
        return pq.top().second;
    }

    void findK(Node* node, priority_queue<pair<int, int>>& pq, int k) {
        if (!node) return;
        pq.push({ node->rank, node->ID });
        if (pq.size() > k) pq.pop();
        findK(node->left, pq, k);
        findK(node->right, pq, k);
    }

    // Μετράει πόσοι κόμβοι υπάρχουν σε ένα υποδέντρο
    int countNodes(Node* node) {
        if (!node) return 0;
        return 1 + countNodes(node->left) + countNodes(node->right);
    }
};

// Ελέγχει αν μια συμβολοσειρά είναι ακέραιος αριθμός
bool isInteger(const string& s) {
    if (s.empty()) return false;
    try {
        size_t pos;
        stoi(s, &pos);
        return pos == s.length();
    }
    catch (...) {
        return false;
    }
}

// Παίρνει έγκυρη ακέραια είσοδο από τον χρήστη σε εύρος
int getValidChoice(int min, int max) {
    string input;
    int num;
    while (true) {
        cout << "Δώσε αριθμό από " << min << " έως " << max << ": ";
        cin >> input;
        if (isInteger(input)) {
            num = stoi(input);
            if (num >= min && num <= max) return num;
            else cout << "Εκτός εύρους.\n";
        }
        else {
            cout << "Μη έγκυρη τιμή.\n";
        }
    }
}

// Κύριο πρόγραμμα
int main() {
    Treap tree;
    bool loaded = false;
    int choice = 0;

    while (choice != 7) {
        // Μενού επιλογών
        cout << "\n1. Φόρτωμα από αρχείο\n2. Διαγραφή παίκτη\n3. Κ-οστή καλύτερη κατάταξη\n4. Εκτύπωση ID σε [t1, t2]\n5. Διαγραφή ανά ημερομηνία\n6. Καλύτερος πριν από ημερομηνία\n7. Έξοδος\n";
        choice = getValidChoice(1, 7);

        switch (choice) {
        case 1: {
            if (!loaded) {
                ifstream file("contest_participants.txt");
                if (!file) {
                    cerr << "Αποτυχία ανοίγματος αρχείου.\n";
                    break;
                }
                string line;
                while (getline(file, line)) {
                    stringstream ss(line);
                    string name, country, tmp;
                    int ID, rank, signupTime;

                    getline(ss, name, ',');
                    getline(ss, tmp, ','); ID = stoi(tmp);
                    getline(ss, tmp, ','); rank = stoi(tmp);
                    getline(ss, country, ',');
                    getline(ss, tmp, ','); signupTime = stoi(tmp);

                    tree.insert(name, ID, rank, country, signupTime);
                }
                loaded = true;
                cout << "Επιτυχής φόρτωση.\n";
            }
            else {
                cout << "Έχει ήδη φορτωθεί.\n";
            }
            break;
        }
        case 2: {
            cout << "Δώσε ID: ";
            int id = getValidChoice(1, INT_MAX);
            tree.deleteByID(id);
            break;
        }
        case 3: {
            cout << "Δώσε k: ";
            int k = getValidChoice(1, INT_MAX);
            int id = tree.findKthBestRank(k);
            if (id != -1)
                cout << "ID του " << k << "-οστού καλύτερου: " << id << endl;
            else
                cout << "Λιγότεροι από " << k << " παίκτες.\n";
            break;
        }
        case 4: {
            int t1, t2;
            cout << "t1: "; t1 = getValidChoice(1, INT_MAX);
            cout << "t2: "; t2 = getValidChoice(t1, INT_MAX);
            tree.printIDsInRange(t1, t2);
            break;
        }
        case 5: {
            int date;
            cout << "Δώσε ημερομηνία: ";
            date = getValidChoice(1, INT_MAX);
            tree.deleteBySignupDate(date);
            break;
        }
        case 6: {
            int date;
            cout << "Δώσε όριο ημερομηνίας: ";
            date = getValidChoice(1, INT_MAX);
            int id = tree.findBestRankBeforeDate(date);
            if (id == -1)
                cout << "Δε βρέθηκαν παίκτες πριν από την ημερομηνία.\n";
            else
                cout << "ID καλύτερου παίκτη πριν την ημερομηνία: " << id << endl;
            break;
        }
        case 7:
            cout << "Έξοδος...\n";
            break;
        }
    }

    return 0;
}