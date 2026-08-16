/**
 * Name        : ProjectTwo_AVL.cpp
 * Author      : Jean Lukenson Collin
 * Course      : CS 300 - Data Structures and Algorithms
 *
 * ENHANCEMENT: AVL Tree (Self-Balancing Binary Search Tree)
 *
 * ORIGINAL LIMITATION:
 * - BST degenerates to O(n) with sorted input
 *
 * ENHANCEMENT:
 * - AVL Tree guarantees O(log n) performance for insert/search
 * - Self-balancing with rotations (LL, RR, LR, RL)
 * - Maintains balance factor of -1, 0, or 1 for all nodes
 *
 * COMPLEXITY (AVL Tree):
 * - Insert: O(log n) guaranteed
 * - Search: O(log n) guaranteed
 * - Inorder traversal: O(n) always
 */

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <cctype>
#include <iomanip>

using namespace std;

//============================================================================
// Course Structure (UNCHANGED)
//============================================================================

/**
 * Structure to hold course information
 */
struct Course {
    string courseNumber;
    string title;
    vector<string> prerequisites;

    Course() {}
    Course(string number, string title) {
        this->courseNumber = number;
        this->title = title;
    }
};

//============================================================================
// AVL Tree Class (SELF-BALANCING)
//============================================================================

/**
 * AVL Tree implementation for storing Course objects.
 *
 * KEY IMPROVEMENT OVER BST:
 * - Maintains balance factor (-1, 0, 1) for all nodes
 * - Guarantees O(log n) performance even with sorted input
 * - Uses four rotation cases: LL, RR, LR, RL
 *
 * COMPLEXITY:
 * - Insert: O(log n) guaranteed
 * - Search: O(log n) guaranteed
 * - Inorder traversal: O(n) always
 */
class AVLTree {

private:

    /**
     * Node structure for AVL Tree
     * - course: Course data
     * - left: pointer to left child
     * - right: pointer to right child
     * - height: height of the node (for balance calculation)  // [AVL] NOUVEAU
     */
    struct Node {
        Course course;
        Node* left;
        Node* right;
        int height;  // [AVL] NOUVEAU : hauteur du nœud

        Node() {
            left = nullptr;
            right = nullptr;
            height = 1;  // [AVL] Nouveau nœud commence à hauteur 1
        }

        Node(Course aCourse) : Node() {
            course = aCourse;
        }
    };

    Node* root;
    int nodeCount;

    //========================================================================
    // AVL Helper Functions  // [AVL] NOUVELLES FONCTIONS
    //========================================================================

    /**
     * Get the height of a node
     * @param node Pointer to the node
     * @return Height of the node (0 if null)
     * Complexity: O(1)
     */
    int getHeight(Node* node) {
        if (node == nullptr) {
            return 0;
        }
        return node->height;
    }

    /**
     * Get the balance factor of a node
     * Balance = height(left) - height(right)
     * @param node Pointer to the node
     * @return Balance factor (positive = left-heavy, negative = right-heavy)
     * Complexity: O(1)
     */
    int getBalance(Node* node) {
        if (node == nullptr) {
            return 0;
        }
        return getHeight(node->left) - getHeight(node->right);
    }

    /**
     * Right rotation (LL case)
     *        y              x
     *       / \            / \
     *      x   T3   =>    T1  y
     *     / \                / \
     *    T1 T2              T2 T3
     * Complexity: O(1)
     */
    Node* rotateRight(Node* y) {
        Node* x = y->left;
        Node* T2 = x->right;

        // Perform rotation
        x->right = y;
        y->left = T2;

        // Update heights
        y->height = 1 + max(getHeight(y->left), getHeight(y->right));
        x->height = 1 + max(getHeight(x->left), getHeight(x->right));

        return x; // New root
    }

    /**
     * Left rotation (RR case)
     *        x              y
     *       / \            / \
     *      T1  y    =>    x   T3
     *         / \        / \
     *        T2 T3      T1 T2
     * Complexity: O(1)
     */
    Node* rotateLeft(Node* x) {
        Node* y = x->right;
        Node* T2 = y->left;

        // Perform rotation
        y->left = x;
        x->right = T2;

        // Update heights
        x->height = 1 + max(getHeight(x->left), getHeight(x->right));
        y->height = 1 + max(getHeight(y->left), getHeight(y->right));

        return y; // New root
    }

    //========================================================================
    // AVL Insertion with Balancing  // [AVL] MODIFIÉE
    //========================================================================

    /**
     * Insert a course into the AVL tree with balancing
     *
     * Process:
     * 1. Perform normal BST insertion
     * 2. Update height of current node
     * 3. Calculate balance factor
     * 4. Apply rotations if unbalanced (LL, RR, LR, RL)
     *
     * @param node Current node in the recursion
     * @param course Course to insert
     * @return Updated node pointer
     * Complexity: O(log n) guaranteed
     */
    Node* insertNode(Node* node, Course course) {
        // Step 1: Normal BST insertion
        if (node == nullptr) {
            nodeCount++;
            return new Node(course);
        }

        // Compare course numbers (case-insensitive)
        string currentNumber = node->course.courseNumber;
        string newNumber = course.courseNumber;
        transform(currentNumber.begin(), currentNumber.end(), currentNumber.begin(), ::toupper);
        transform(newNumber.begin(), newNumber.end(), newNumber.begin(), ::toupper);

        if (newNumber < currentNumber) {
            node->left = insertNode(node->left, course);
        }
        else if (newNumber > currentNumber) {
            node->right = insertNode(node->right, course);
        }
        else {
            return node; // Duplicate - no insertion
        }

        // Step 2: Update height of current node
        node->height = 1 + max(getHeight(node->left), getHeight(node->right));

        // Step 3: Calculate balance factor
        int balance = getBalance(node);

        // Step 4: Balance the tree (4 cases)

        // LL Case (left-left) - Right rotation
        if (balance > 1 && newNumber < node->left->course.courseNumber) {
            return rotateRight(node);
        }

        // RR Case (right-right) - Left rotation
        if (balance < -1 && newNumber > node->right->course.courseNumber) {
            return rotateLeft(node);
        }

        // LR Case (left-right) - Left rotation then Right rotation
        if (balance > 1 && newNumber > node->left->course.courseNumber) {
            node->left = rotateLeft(node->left);
            return rotateRight(node);
        }

        // RL Case (right-left) - Right rotation then Left rotation
        if (balance < -1 && newNumber < node->right->course.courseNumber) {
            node->right = rotateRight(node->right);
            return rotateLeft(node);
        }

        return node; // No balancing needed
    }

    //========================================================================
    // Search (UNCHANGED - but now O(log n) guaranteed)
    //========================================================================

    /**
     * Recursive helper to search for a course
     * Complexity: O(log n) guaranteed (AVL property)
     */
    Course* searchNode(Node* node, string courseNumber) {
        if (node == nullptr) {
            return nullptr;
        }

        string currentNumber = node->course.courseNumber;
        string searchNumber = courseNumber;
        transform(currentNumber.begin(), currentNumber.end(), currentNumber.begin(), ::toupper);
        transform(searchNumber.begin(), searchNumber.end(), searchNumber.begin(), ::toupper);

        if (currentNumber == searchNumber) {
            return &(node->course);
        }
        else if (searchNumber < currentNumber) {
            return searchNode(node->left, courseNumber);
        }
        else {
            return searchNode(node->right, courseNumber);
        }
    }

    //========================================================================
    // Traversal (UNCHANGED)
    //========================================================================

    /**
     * Inorder traversal - O(n) always
     * Yields courses sorted alphanumerically by courseNumber
     */
    void inorderCollect(Node* node, vector<Course>& courses) {
        if (node != nullptr) {
            inorderCollect(node->left, courses);
            courses.push_back(node->course);
            inorderCollect(node->right, courses);
        }
    }

    /**
     * Clear tree (for destructor)
     */
    void clearTree(Node* node) {
        if (node != nullptr) {
            clearTree(node->left);
            clearTree(node->right);
            delete node;
        }
    }

    //========================================================================
    // Public Interface
    //========================================================================

public:

    AVLTree() {
        root = nullptr;
        nodeCount = 0;
    }

    ~AVLTree() {
        clearTree(root);
        root = nullptr;
        nodeCount = 0;
    }

    /**
     * Insert a course into the AVL tree
     * Complexity: O(log n) guaranteed
     */
    void insert(Course course) {
        root = insertNode(root, course);
    }

    /**
     * Search for a course by course number
     * Complexity: O(log n) guaranteed
     */
    Course* search(string courseNumber) {
        return searchNode(root, courseNumber);
    }

    /**
     * Get all courses in sorted order (alphanumeric by course number)
     * Complexity: O(n)
     */
    vector<Course> getSortedCourses() {
        vector<Course> courses;
        inorderCollect(root, courses);
        return courses;
    }

    bool isEmpty() {
        return root == nullptr;
    }

    int size() {
        return nodeCount;
    }
};

//============================================================================
// Helper Functions (UNCHANGED)
//============================================================================

string trim(const string& str) {
    size_t start = str.find_first_not_of(" \t\n\r");
    size_t end = str.find_last_not_of(" \t\n\r");
    if (start == string::npos || end == string::npos) {
        return "";
    }
    return str.substr(start, end - start + 1);
}

string toUpper(const string& str) {
    string result = str;
    transform(result.begin(), result.end(), result.begin(), ::toupper);
    return result;
}

vector<string> split(const string& str, char delimiter) {
    vector<string> tokens;
    stringstream ss(str);
    string token;
    while (getline(ss, token, delimiter)) {
        tokens.push_back(trim(token));
    }
    return tokens;
}

//============================================================================
// Core Functions (MODIFIED to use AVLTree)
//============================================================================

/**
 * Load course data from a CSV file into the AVL Tree
 *
 * ENHANCEMENT: Now uses AVLTree instead of BinarySearchTree
 *
 * @param filename Path to the CSV file
 * @param tree The AVL tree to load data into
 * @return true if successful, false otherwise
 */
bool loadCourses(string filename, AVLTree& tree) {
    ifstream file(filename);

    if (!file.is_open()) {
        cout << "Error: Could not open file '" << filename << "'" << endl;
        return false;
    }

    string line;
    int lineNumber = 0;
    bool hasError = false;

    while (getline(file, line)) {
        lineNumber++;

        if (line.empty()) {
            continue;
        }

        vector<string> tokens = split(line, ',');

        if (tokens.size() < 2) {
            cout << "Error: Line " << lineNumber << " is malformed (missing course number or title)" << endl;
            hasError = true;
            continue;
        }

        Course course;
        course.courseNumber = trim(tokens[0]);
        course.title = trim(tokens[1]);

        for (size_t i = 2; i < tokens.size(); i++) {
            string prereq = trim(tokens[i]);
            if (!prereq.empty()) {
                course.prerequisites.push_back(prereq);
            }
        }

        tree.insert(course);
    }

    file.close();

    if (hasError) {
        cout << "Warning: File loaded with errors. Some lines may have been skipped." << endl;
    }

    // Validate prerequisites
    vector<Course> courses = tree.getSortedCourses();
    bool validationError = false;

    for (const Course& course : courses) {
        for (const string& prereq : course.prerequisites) {
            Course* found = tree.search(prereq);
            if (found == nullptr) {
                cout << "Error: Prerequisite '" << prereq << "' for course '"
                    << course.courseNumber << "' does not exist in the course list" << endl;
                validationError = true;
            }
        }
    }

    if (validationError) {
        cout << "Warning: Data validation completed with errors." << endl;
        return false;
    }

    cout << "Successfully loaded " << tree.size() << " courses from '" << filename << "'" << endl;
    return true;
}

/**
 * Print all courses in alphanumeric order
 */
void printCourseList(AVLTree& tree) {
    if (tree.isEmpty()) {
        cout << "No courses loaded. Please load data first (Option 1)." << endl;
        return;
    }

    cout << endl;
    cout << "Here is a sample schedule:" << endl;
    cout << endl;

    vector<Course> courses = tree.getSortedCourses();

    for (const Course& course : courses) {
        cout << course.courseNumber << ", " << course.title << endl;
    }

    cout << endl;
}

/**
 * Print information about a specific course
 */
void printCourseInfo(AVLTree& tree, string courseNumber) {
    if (courseNumber.empty()) {
        cout << "Error: Please enter a course number." << endl;
        return;
    }

    string searchNumber = toUpper(courseNumber);
    Course* course = tree.search(searchNumber);

    if (course == nullptr) {
        cout << "Error: Course '" << courseNumber << "' not found." << endl;
        return;
    }

    cout << course->courseNumber << ", " << course->title;

    if (course->prerequisites.empty()) {
        cout << "\nPrerequisites: None" << endl;
    }
    else {
        cout << "\nPrerequisites: ";
        for (size_t i = 0; i < course->prerequisites.size(); i++) {
            cout << course->prerequisites[i];
            if (i < course->prerequisites.size() - 1) {
                cout << ", ";
            }
        }
        cout << endl;
    }
}

/**
 * Display the main menu
 */
void displayMenu() {
    cout << endl;
    cout << "1. Load Data Structure." << endl;
    cout << "2. Print Course List." << endl;
    cout << "3. Print Course." << endl;
    cout << "9. Exit" << endl;
    cout << endl;
    cout << "What would you like to do? ";
}

//============================================================================
// Main Program
//============================================================================

int main() {
    cout << endl;
    cout << "Welcome to the course planner." << endl;

    AVLTree tree;

    bool dataLoaded = false;
    string filename;
    int choice = 0;

    while (choice != 9) {
        displayMenu();

        // Vérifier si l'utilisateur a entré un nombre valide
        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "Error: Please enter a valid number." << endl;
            continue;
        }

        switch (choice) {
        case 1: {
            cout << "Enter filename: ";
            cin.ignore();  // IMPORTANT: Ignorer le \n restant
            getline(cin, filename);  // Lire toute la ligne

            // Vérifier ce qui a été lu (optionnel, pour déboguer)
            // cout << "DEBUG: filename = [" << filename << "]" << endl;

            ifstream testFile(filename);
            if (!testFile.is_open()) {
                cout << "Error: File '" << filename << "' not found." << endl;
                break;
            }
            testFile.close();

            bool success = loadCourses(filename, tree);
            if (success) {
                dataLoaded = true;
            }
            break;
        }

        case 2: {
            if (!dataLoaded) {
                cout << "Please load data first (Option 1)." << endl;
                break;
            }
            printCourseList(tree);
            break;
        }

        case 3: {
            if (!dataLoaded) {
                cout << "Please load data first (Option 1)." << endl;
                break;
            }

            string courseNumber;
            cout << "What course do you want to know about? ";
            cin >> courseNumber;

            printCourseInfo(tree, courseNumber);
            break;
        }

        case 9: {
            cout << "Thank you for using the course planner!" << endl;
            break;
        }

        default: {
            cout << choice << " is not a valid option." << endl;
            break;
        }
        }
    }

    return 0;
}