/**
 * Name        : ProjectTwo.cpp
 * Author      : Jean Lukenson Collin
 * Course      : CS 300 - Data Structures and Algorithms
 *
 * DATA STRUCTURE: Binary Search Tree (BST)
 *
 * RECOMMENDATION JUSTIFICATION:
 * - BST maintains natural sorted order via inorder traversal (O(n) for display)
 * - Search is O(log n) average, O(n) worst-case (if unbalanced)
 * - Hash Table offers O(1) search but requires O(n log n) sorting for display
 * - Vector offers O(n) search and requires O(n log n) sorting for display
 * - BST provides the best balance between search and sorted display
 *
 * NOTE: Tree balance depends on insertion order. Random input yields
 *       near-optimal balance. Sorted input would create a degenerate tree.
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
// Course Structure
//============================================================================

/**
 * Structure to hold course information
 * - courseNumber: unique identifier (e.g., "CSCI400")
 * - title: course title (e.g., "Large Software Development")
 * - prerequisites: vector of prerequisite course numbers
 */
struct Course {
    string courseNumber;
    string title;
    vector<string> prerequisites;

    // Default constructor
    Course() {}

    // Parameterized constructor
    Course(string number, string title) {
        this->courseNumber = number;
        this->title = title;
    }
};

//============================================================================
// Binary Search Tree (BST) Class
//============================================================================

/**
 * Binary Search Tree implementation for storing Course objects.
 * Ordered by courseNumber (alphanumeric).
 *
 * Complexity:
 * - Insert: O(log n) average, O(n) worst-case (if unbalanced)
 * - Search: O(log n) average, O(n) worst-case (if unbalanced)
 * - Inorder traversal: O(n) always
 *
 * The tree remains balanced when input is in random order.
 * Sorted input would create a degenerate tree (O(n) performance).
 */
class BinarySearchTree {

private:

    /**
     * Node structure for BST
     * - course: Course data
     * - left: pointer to left child (smaller course numbers)
     * - right: pointer to right child (larger course numbers)
     */
    struct Node {
        Course course;
        Node* left;
        Node* right;

        // Default constructor
        Node() {
            left = nullptr;
            right = nullptr;
        }

        // Constructor with Course
        Node(Course aCourse) : Node() {
            course = aCourse;
        }
    };

    Node* root;
    int nodeCount;

    /**
     * Recursive helper to insert a course into the BST
     * Uses alphanumeric comparison on courseNumber
     * @param node Current node in the recursion
     * @param course Course to insert
     * @return Updated node pointer
     */
    Node* insertNode(Node* node, Course course) {
        // Base case: empty subtree
        if (node == nullptr) {
            nodeCount++;
            return new Node(course);
        }

        // Compare course numbers (case-insensitive for consistency)
        string currentNumber = node->course.courseNumber;
        string newNumber = course.courseNumber;

        // Convert to uppercase for case-insensitive comparison
        transform(currentNumber.begin(), currentNumber.end(), currentNumber.begin(), ::toupper);
        transform(newNumber.begin(), newNumber.end(), newNumber.begin(), ::toupper);

        // Recursively insert into left or right subtree
        if (newNumber < currentNumber) {
            node->left = insertNode(node->left, course);
        }
        else if (newNumber > currentNumber) {
            node->right = insertNode(node->right, course);
        }
        // If course number already exists, do nothing (no duplicates)

        return node;
    }

    /**
     * Recursive helper to search for a course by course number
     * @param node Current node in the recursion
     * @param courseNumber Course number to search for (case-insensitive)
     * @return Pointer to Course if found, nullptr otherwise
     */
    Course* searchNode(Node* node, string courseNumber) {
        // Base case: not found
        if (node == nullptr) {
            return nullptr;
        }

        // Convert to uppercase for case-insensitive comparison
        string currentNumber = node->course.courseNumber;
        string searchNumber = courseNumber;
        transform(currentNumber.begin(), currentNumber.end(), currentNumber.begin(), ::toupper);
        transform(searchNumber.begin(), searchNumber.end(), searchNumber.begin(), ::toupper);

        // Match found
        if (currentNumber == searchNumber) {
            return &(node->course);
        }
        // Search left or right subtree
        else if (searchNumber < currentNumber) {
            return searchNode(node->left, courseNumber);
        }
        else {
            return searchNode(node->right, courseNumber);
        }
    }

    /**
     * Recursive helper to collect all courses in sorted order (inorder traversal)
     * This is O(n) and yields courses sorted alphanumerically by courseNumber
     * @param node Current node in the recursion
     * @param courses Vector to collect courses into
     */
    void inorderCollect(Node* node, vector<Course>& courses) {
        if (node != nullptr) {
            inorderCollect(node->left, courses);
            courses.push_back(node->course);
            inorderCollect(node->right, courses);
        }
    }

    /**
     * Recursive helper to clear all nodes (for destructor)
     * @param node Current node in the recursion
     */
    void clearTree(Node* node) {
        if (node != nullptr) {
            clearTree(node->left);
            clearTree(node->right);
            delete node;
        }
    }

public:

    /**
     * Default constructor - initializes empty BST
     */
    BinarySearchTree() {
        root = nullptr;
        nodeCount = 0;
    }

    /**
     * Destructor - deletes all nodes to prevent memory leaks
     */
    ~BinarySearchTree() {
        clearTree(root);
        root = nullptr;
        nodeCount = 0;
    }

    /**
     * Insert a course into the BST
     * @param course Course to insert
     * Complexity: O(log n) average, O(n) worst-case
     */
    void insert(Course course) {
        root = insertNode(root, course);
    }

    /**
     * Search for a course by course number (case-insensitive)
     * @param courseNumber Course number to search for
     * @return Course pointer if found, nullptr otherwise
     * Complexity: O(log n) average, O(n) worst-case
     */
    Course* search(string courseNumber) {
        return searchNode(root, courseNumber);
    }

    /**
     * Get all courses in sorted order (alphanumeric by course number)
     * Uses inorder traversal - O(n)
     * @return Vector of courses sorted by course number
     */
    vector<Course> getSortedCourses() {
        vector<Course> courses;
        inorderCollect(root, courses);
        return courses;
    }

    /**
     * Check if the BST is empty
     * @return true if empty, false otherwise
     */
    bool isEmpty() {
        return root == nullptr;
    }

    /**
     * Get the number of courses in the BST
     * @return Number of courses
     */
    int size() {
        return nodeCount;
    }
};

//============================================================================
// Helper Functions
//============================================================================

/**
 * Trim whitespace from both ends of a string
 * @param str String to trim
 * @return Trimmed string
 */
string trim(const string& str) {
    size_t start = str.find_first_not_of(" \t\n\r");
    size_t end = str.find_last_not_of(" \t\n\r");

    if (start == string::npos || end == string::npos) {
        return "";
    }

    return str.substr(start, end - start + 1);
}

/**
 * Convert a string to uppercase
 * @param str String to convert
 * @return Uppercase string
 */
string toUpper(const string& str) {
    string result = str;
    transform(result.begin(), result.end(), result.begin(), ::toupper);
    return result;
}

/**
 * Split a string by a delimiter
 * @param str String to split
 * @param delimiter Delimiter character
 * @return Vector of tokens
 */
vector<string> split(const string& str, char delimiter) {
    vector<string> tokens;
    stringstream ss(str);
    string token;

    while (getline(ss, token, delimiter)) {
        tokens.push_back(trim(token));
    }

    return tokens;
}

/**
 * Validate that a course number exists in the BST
 * @param bst The BST containing all courses
 * @param courseNumber Course number to validate
 * @return true if course exists, false otherwise
 */
bool validatePrerequisite(BinarySearchTree& bst, string courseNumber) {
    Course* course = bst.search(courseNumber);
    return course != nullptr;
}

//============================================================================
// Core Functions
//============================================================================

/**
 * Load course data from a CSV file into the BST
 *
 * Pseudocode:
 * 1. Open the file
 * 2. Read each line
 * 3. Split by comma
 * 4. Validate at least 2 fields (course number + title)
 * 5. Create Course object with number, title, and prerequisites
 * 6. Insert into BST
 * 7. After all lines read, validate prerequisites exist
 *
 * @param filename Path to the CSV file
 * @param bst The BST to load data into
 * @return true if successful, false otherwise
 */
bool loadCourses(string filename, BinarySearchTree& bst) {
    ifstream file(filename);

    // Check if file can be opened
    if (!file.is_open()) {
        cout << "Error: Could not open file '" << filename << "'" << endl;
        return false;
    }

    string line;
    int lineNumber = 0;
    bool hasError = false;

    // Read file line by line
    while (getline(file, line)) {
        lineNumber++;

        // Skip empty lines
        if (line.empty()) {
            continue;
        }

        // Parse line: split by comma
        vector<string> tokens = split(line, ',');

        // VALIDATION 1: At least 2 parameters (course number and title)
        if (tokens.size() < 2) {
            cout << "Error: Line " << lineNumber << " is malformed (missing course number or title)" << endl;
            hasError = true;
            continue;
        }

        // Create course object
        Course course;
        course.courseNumber = trim(tokens[0]);
        course.title = trim(tokens[1]);

        // Add prerequisites (tokens 2 and beyond)
        for (size_t i = 2; i < tokens.size(); i++) {
            string prereq = trim(tokens[i]);
            if (!prereq.empty()) {
                course.prerequisites.push_back(prereq);
            }
        }

        // Insert course into BST
        bst.insert(course);
    }

    file.close();

    if (hasError) {
        cout << "Warning: File loaded with errors. Some lines may have been skipped." << endl;
    }

    // VALIDATION 2: All prerequisites must exist as courses
    vector<Course> courses = bst.getSortedCourses();
    bool validationError = false;

    for (const Course& course : courses) {
        for (const string& prereq : course.prerequisites) {
            if (!validatePrerequisite(bst, prereq)) {
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

    cout << "Successfully loaded " << bst.size() << " courses from '" << filename << "'" << endl;
    return true;
}

/**
 * Print all courses in alphanumeric order (by courseNumber)
 * Uses inorder traversal - O(n) - no additional sorting needed!
 *
 * Pseudocode:
 * 1. Get sorted courses via inorder traversal
 * 2. Print each course as "courseNumber, title"
 * 3. Display total count
 *
 * @param bst The BST containing all courses
 */
void printCourseList(BinarySearchTree& bst) {
    if (bst.isEmpty()) {
        cout << "No courses loaded. Please load data first (Option 1)." << endl;
        return;
    }

    cout << endl;
    cout << "Here is a sample schedule:" << endl;
    cout << endl;

    // Get sorted courses using inorder traversal (already sorted!)
    vector<Course> courses = bst.getSortedCourses();

    // Print each course in alphanumeric order by courseNumber
    for (const Course& course : courses) {
        cout << course.courseNumber << ", " << course.title << endl;
    }

    cout << endl;
}

/**
 * Print information about a specific course
 *
 * Pseudocode:
 * 1. Search for course in BST
 * 2. If found, print course number, title, and prerequisites
 * 3. If not found, display error message
 *
 * @param bst The BST containing all courses
 * @param courseNumber Course number to look up
 */
void printCourseInfo(BinarySearchTree& bst, string courseNumber) {
    // Validate input
    if (courseNumber.empty()) {
        cout << "Error: Please enter a course number." << endl;
        return;
    }

    // Convert to uppercase for case-insensitive search
    string searchNumber = toUpper(courseNumber);

    // Search for the course in the BST
    Course* course = bst.search(searchNumber);

    // Check if course was found
    if (course == nullptr) {
        cout << "Error: Course '" << courseNumber << "' not found." << endl;
        return;
    }

    // Print course information (matching sample output format)
    cout << course->courseNumber << ", " << course->title;

    // Print prerequisites (if any)
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

/**
 * Main program entry point
 *
 * Pseudocode:
 * 1. Initialize BST
 * 2. Display welcome message
 * 3. Initialize choice = 0
 * 4. Loop while choice != 9:
 *    a. Display menu
 *    b. Read user choice
 *    c. Validate choice
 *    d. Execute selected option
 * 5. Exit with thank you message
 */
int main() {
    cout << endl;
    cout << "Welcome to the course planner." << endl;

    // Initialize the BST
    BinarySearchTree bst;

    // Track if data is loaded
    bool dataLoaded = false;

    string filename;
    int choice = 0;  // Initialize choice before loop (per professor feedback)

    // Main program loop
    while (choice != 9) {
        displayMenu();
        cin >> choice;

        // Handle invalid input (non-integer)
        if (cin.fail()) {
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "Error: Please enter a valid number." << endl;
            continue;
        }

        switch (choice) {
        case 1: {
            cout << "Enter filename: ";
            cin.ignore();                    
            getline(cin, filename);         

            ifstream testFile(filename);
            if (!testFile.is_open()) {
                cout << "Error: File '" << filename << "' not found." << endl;
                break;
            }
            testFile.close();

            bool success = loadCourses(filename, bst);
            if (success) {
                dataLoaded = true;
            }
            break;
        }

        case 2: {
            // Print Course List
            if (!dataLoaded) {
                cout << "Please load data first (Option 1)." << endl;
                break;
            }
            printCourseList(bst);
            break;
        }

        case 3: {
            // Print Course
            if (!dataLoaded) {
                cout << "Please load data first (Option 1)." << endl;
                break;
            }

            string courseNumber;
            cout << "What course do you want to know about? ";
            cin >> courseNumber;

            printCourseInfo(bst, courseNumber);
            break;
        }

        case 9: {
            // Exit
            cout << "Thank you for using the course planner!" << endl;
            break;
        }

        default: {
            // Invalid option - matches sample output format
            cout << choice << " is not a valid option." << endl;
            break;
        }
        }
    }

    return 0;
}