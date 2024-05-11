#include <iostream>
#include <fstream>
#include <string>
#include <limits>

using namespace std;

struct TreeNode {
    string englishWord;
    string ukrainianWord;
    int accessCount;
    TreeNode* left;
    TreeNode* right;

    TreeNode(const string& eng, const string& ukr, int count)
        : englishWord(eng), ukrainianWord(ukr), accessCount(count), left(nullptr), right(nullptr) {}
};

void deleteTree(TreeNode*& node) {
    if (node) {
        deleteTree(node->left);
        deleteTree(node->right);
        delete node;
        node = nullptr;
    }
}

void insertOrUpdate(TreeNode*& node, const string& eng, const string& ukr, int count = 0) {
    if (!node) {
        node = new TreeNode(eng, ukr, count);
    }
    else if (node->englishWord == eng) {
        node->ukrainianWord = ukr;
        node->accessCount += count;
    }
    else if (eng < node->englishWord) {
        insertOrUpdate(node->left, eng, ukr, count);
    }
    else {
        insertOrUpdate(node->right, eng, ukr, count);
    }
}

bool remove(TreeNode*& node, const string& eng) {
    if (!node) return false;

    // Search for the node to be removed
    if (eng < node->englishWord) {
        return remove(node->left, eng);
    }
    else if (eng > node->englishWord) {
        return remove(node->right, eng);
    }
    else {  // Node found
        // Node with only one child or no child
        if (!node->left || !node->right) {
            TreeNode* temp = node->left ? node->left : node->right;
            delete node;
            node = temp;
        }
        else {  // Node with two children
            // Find the inorder successor
            TreeNode* succParent = node;
            TreeNode* succ = node->right;
            while (succ->left) {
                succParent = succ;
                succ = succ->left;
            }

            // Copy the inorder successor's content to this node
            node->englishWord = succ->englishWord;
            node->ukrainianWord = succ->ukrainianWord;
            node->accessCount = succ->accessCount;

            // Delete the inorder successor
            if (succParent != node)
                succParent->left = succ->right;
            else
                succParent->right = succ->right;

            delete succ;
        }
        return true;
    }
}

void printTree(const TreeNode* node) {
    if (!node) return;
    printTree(node->left);
    cout << "English: " << node->englishWord << " - Ukrainian: " << node->ukrainianWord << " (Accesses: " << node->accessCount << ")\n";
    printTree(node->right);
}

void saveTreeRecursive(const TreeNode* node, ofstream& outFile) {
    if (!node) return;
    saveTreeRecursive(node->left, outFile);
    outFile << node->englishWord << ":" << node->ukrainianWord << ":" << node->accessCount << "\n";
    saveTreeRecursive(node->right, outFile);
}

void saveToFile(const TreeNode* node, const string& filename) {
    ofstream outFile(filename);
    if (outFile.is_open()) {
        saveTreeRecursive(node, outFile);
        outFile.close();
        cout << "Dictionary saved to file successfully." << endl;
    }
    else {
        cout << "Unable to open file for writing." << endl;
    }
}

void loadFromFile(TreeNode*& node, const string& filename) {
    ifstream inFile(filename);
    if (inFile.is_open()) {
        deleteTree(node);  // Clear current dictionary before loading new one
        string line;
        while (getline(inFile, line)) {
            size_t first_colon = line.find(':');
            size_t second_colon = line.find(':', first_colon + 1);
            string eng = line.substr(0, first_colon);
            string ukr = line.substr(first_colon + 1, second_colon - first_colon - 1);
            int count = stoi(line.substr(second_colon + 1));
            insertOrUpdate(node, eng, ukr, count);
        }
        inFile.close();
        cout << "Dictionary loaded from file successfully." << endl;
    }
    else {
        cout << "Unable to open file for reading." << endl;
    }
}

bool readLine(string& line) {
    getline(cin, line);
    return !line.empty();
}

void findMaxAccess(TreeNode* node, TreeNode*& maxNode, TreeNode*& maxParent, int& maxAccess) {
    if (!node) return;

    // Check current node
    if (node->accessCount > maxAccess) {
        maxAccess = node->accessCount;
        maxNode = node;
    }

    // Traverse left subtree
    if (node->left) {
        findMaxAccess(node->left, maxNode, node, maxAccess);
    }

    // Traverse right subtree
    if (node->right) {
        findMaxAccess(node->right, maxNode, node, maxAccess);
    }
}

// Utility wrapper function to simplify external calls
TreeNode* findMaxAccess(TreeNode*& root, TreeNode*& parent) {
    TreeNode* maxNode = nullptr;
    parent = nullptr;
    int maxAccess = -1;
    findMaxAccess(root, maxNode, parent, maxAccess);
    return maxNode;
}

void printAndTransfer(TreeNode*& oldRoot, TreeNode*& newRoot, const string& filename) {
    TreeNode* parent = nullptr;
    TreeNode* maxNode = findMaxAccess(oldRoot, parent);
    if (maxNode) {
        cout << "Transferring: " << maxNode->englishWord << " - " << maxNode->ukrainianWord << " (Accesses: " << maxNode->accessCount << ")" << endl;
        insertOrUpdate(newRoot, maxNode->englishWord, maxNode->ukrainianWord, maxNode->accessCount);
        remove(oldRoot, maxNode->englishWord);  // Remove from old tree after transferring

        ofstream outFile(filename, ios::app);
        if (outFile.is_open()) {
            outFile << maxNode->englishWord << ":" << maxNode->ukrainianWord << ":" << maxNode->accessCount << "\n";
            outFile.close();
            cout << "Entry saved to file: " << filename << endl;
        }
        else {
            cout << "Unable to open file for writing." << endl;
        }
    }
    else {
        cout << "No entries left to transfer or tree is empty.\n";
    }
    cout << "New Dictionary After Transfer:\n";
    printTree(newRoot);
}

int main() {
    TreeNode* root = nullptr;
    TreeNode* newRoot = nullptr;
    string filename, eng, ukr;
    int choice;

    while (true) {
        cout << "1. Add or Update Entry\n";
        cout << "2. Remove Entry\n";
        cout << "3. Print Initial Dictionary\n";
        cout << "4. Load from file\n";
        cout << "5. Save to file\n";
        cout << "6. Transfer entries based on max access to new tree and print\n";
        cout << "8. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');  // Clear input buffer

        switch (choice) {
        case 1:
            cout << "Enter English word: ";
            if (!readLine(eng)) continue;
            cout << "Enter Ukrainian word: ";
            if (!readLine(ukr)) continue;
            insertOrUpdate(root, eng, ukr, 1);  // Assuming an access counts as an addition
            break;
        case 2:
            cout << "Enter English word to remove: ";
            if (!readLine(eng)) continue;
            if (!remove(root, eng)) {
                cout << "Word not found!" << endl;
            }
            break;
        case 3:
            cout << "Initial Dictionary (Tree):\n";
            printTree(root);
            break;
        case 4:
            cout << "Enter filename to load from: ";
            if (!readLine(filename)) continue;
            loadFromFile(root, filename);
            break;
        case 5:
            cout << "Enter filename to save to: ";
            if (!readLine(filename)) continue;
            saveToFile(root, filename);
            break;
        case 6:
            cout << "Enter filename to save transferred entry: ";
            if (!readLine(filename)) continue;
            printAndTransfer(root, newRoot, filename);
            break;
        case 8:
            cout << "Exiting program.\n";
            deleteTree(root);
            deleteTree(newRoot);
            return 0;
        default:
            cout << "Invalid choice. Please try again.\n";
            break;
        }
    }

    return 0;
}