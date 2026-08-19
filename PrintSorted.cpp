//============================================================================
// Name        : PrintSorted.cpp
// Author      : Lesley Hughes
// Description : Project Two - Printing Courses Sorted Alphanumerically
//============================================================================

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <fstream>
#include <sstream>

using namespace std;

// Struct for Course objects
struct Course {
	string courseNum;
	string courseName;
	vector<string> prerequisites;

	// Default constructor
	Course() {}
	
	// Overloaded constructor
	Course(string courseNum, string courseName, vector<string> prerequisites) {
		this->courseNum = courseNum;
		this->courseName = courseName;
		this->prerequisites = prerequisites;
	}
};

// Struct for the BST Node
struct Node {
	Course course;
	Node* left;
	Node* right;

	// Default constructor
	Node() {
		left = nullptr;
		right = nullptr;
	}

	// Init with a Course
	Node(Course aCourse) : Node() {
		course = aCourse;
	}
};

// Binary Search Tree class
class BinarySearchTree {

private:
	Node* root;
	int size;
	void addNode(Node* node, Course course);
	void inOrder(Node* node);
	Node* removeNode(Node* node, string courseNum);

public:
	BinarySearchTree();
	~BinarySearchTree();
	void InOrder();
	void Insert(Course course);
	void Remove(string courseNum);
	Course Search(string courseNum);
	void ValidatePrerequisites();
	int Size();
};
// Declaration of validatePrerequisites function before it is used.
void validatePrerequisites(Node* node, vector<string>& invalidCourses, BinarySearchTree* courseTree);

// Default Constructor
BinarySearchTree::BinarySearchTree() {
	root = nullptr;
	size = 0;
}

// Destructor
BinarySearchTree::~BinarySearchTree() {
	while (root != nullptr) {
		Remove(root->course.courseNum);
	}
}

// Public InOrder function
void BinarySearchTree::InOrder() {
	inOrder(root);
}

// Public Insert function
void BinarySearchTree::Insert(Course aCourse) {
	// If root is equal to nullptr...
	if (root == nullptr) {
		root = new Node(aCourse);
	}
	// If root is not equal to nullptr...
	else {
		addNode(root, aCourse);
	}
	size++;
}

// Public Search function
Course BinarySearchTree::Search(string courseNum) {
	Node* current = root;

	// While current is not null...
	while (current != nullptr) {
		// If a match is found...
		if (current->course.courseNum == courseNum) {
			return current->course;
		}
		// If courseNum is smaller than the current node's
		if (courseNum < current->course.courseNum) {
			// Traverse left
			current = current->left;
		}
		// Otherwise, if courseNum is larger than the current node's
		else {
			// Traverse right
			current = current->right;
		}
	}
	Course course;
	return course;
}


// Public Remove function
void BinarySearchTree::Remove(string courseNum) {
	root = removeNode(root, courseNum);
}

// Public Validate Prerequisites function
// Note: Needed to give private function access to root.
void BinarySearchTree::ValidatePrerequisites() {
	// Vector to hold courses without valid prerequisites
	vector<string> invalidCourses;
	validatePrerequisites(root, invalidCourses, this);

	// Remove invalid Course objects from BST
	for (string removalNum : invalidCourses) {
		Remove(removalNum);
		cout << "Course Number " << removalNum <<
			" is invalid! All prerequisites must be existing courses." <<
			endl;
	}
}

// Private inOrder function
void BinarySearchTree::inOrder(Node* node) {
	// If node is not equal to nullptr...
	if (node != nullptr) {
		// Recursively call inOrder on the left.
		inOrder(node->left);
		cout << node->course.courseNum << ", " << node->course.courseName << endl;
		// Recursively call inOrder on the right.
		inOrder(node->right);
	}
}


// Private removeNode function
Node* BinarySearchTree::removeNode(Node* node, string courseNum) {
	// If node equals nullptr...
	if (node == nullptr) {
		return node;
	}
	// Otherwise, if courseNum is less than node's courseNum...
	if (courseNum < node->course.courseNum) {
		// Set node's left to the result of recursively calling removeNode on
		// node's left.
		node->left = removeNode(node->left, courseNum);
	}
	// Otherwise, if courseNum is greater than node's courseNum...
	else if (courseNum > node->course.courseNum) {
		// Set node's right to be the result of recursively calling removeNode
		// on node's right.
		node->right = removeNode(node->right, courseNum);
	}
	// Otherwise...
	else {
		// If both node's left and right are null...
		if (node->left == nullptr && node->right == nullptr) {
			// Delete node and set it to nullptr.
			delete node;
			size--;
			node = nullptr;
		}
		// Otherwise, if node's left isn't null and node's right is...
		else if (node->left != nullptr && node->right == nullptr) {
			// Create a temporary node.
			Node* temp = node;
			// Set node to node's left.
			node = node->left;
			// Delete the temporary node.
			delete temp;
			size--;
		}
		// Otherwise, if node's left is null and node's right isn't...
		else if (node->left == nullptr && node->right != nullptr) {
			// Create a temporary node.
			Node* temp = node;
			// Set node to node's right.
			node = node->right;
			// Delete the temporary node.
			delete temp;
			size--;
		}
		// Otherwise, if both node's left and right aren't null...
		else {
			// Create a temporary node and set it to node's right.
			Node* temp = node->right;
			// While temp's left isn't null...
			while (temp->left != nullptr) {
				// Set temp to temp's left.
				temp = temp->left;
			}
			// Set node's course to temp's course.
			node->course = temp->course;
			// Set node's right to the result of recursively calling removeNode on
			// node's right and temp's courseNum.
			node->right = removeNode(node->right, temp->course.courseNum);
		}
	}
	return node;
}

// Private addNode function
void BinarySearchTree::addNode(Node* node, Course aCourse) {
	// If nodes courseNum is greater than the argument course's courseNum...
	if (node->course.courseNum > aCourse.courseNum) {
		// If the left node is null...
		if (node->left == nullptr) {
			// Add the Course to the left node.
			node->left = new Node(aCourse);
		}
		// Otherwise, if the left node is not null...
		else {
			// Recurse to the left.
			addNode(node->left, aCourse);
		}
	}
	// Otherwise, if the nodes courseNum is less than or equal to
	// the argument course's courseNum...
	else {
		// If the right node is null...
		if (node->right == nullptr) {
			// Add the course to the right node.
			node->right = new Node(aCourse);
		}
		// Otherwise, if the right node isn't null...
		else {
			// Recurse to the right. 
			addNode(node->right, aCourse);
		}
	}
}

// Function to return the size of the BST
int BinarySearchTree::Size() {
	return size;
}

// Function for prerequisite validation.
void validatePrerequisites(Node* node, vector<string>& invalidCourses, BinarySearchTree* courseTree) {
	// If the node is null...
	if (node != nullptr) {
		// Recurse down the left side
		validatePrerequisites(node->left, invalidCourses, courseTree);
		// For each prerequisite in the node's course's prerequisite vector...
		for (string prerequisite : node->course.prerequisites) {
			string currNum = prerequisite;
			// Search for the prerequisite as a course
			Course result = courseTree->Search(currNum);
			// If it is a course
			if (!result.courseNum.empty()) {
				continue;
			}
			// If it isn't a course
			else {
				// Add the courseNum of node's course to invalid course list.
				invalidCourses.push_back(node->course.courseNum);
				break;
			}
		}
		// Recurse down the right side.
		validatePrerequisites(node->right, invalidCourses, courseTree);
	}
}

//  Function to Read the file and store the data in a BST
void loadCourses(string fileName, BinarySearchTree* courseTree) {
	cout << "Loading Course file " << fileName << endl;

	// Create a file object and load the file into it.
	ifstream file;
	file.open(fileName);

	// If the file failed to open...
	if (!file.is_open()) {
		cout << "Error opening file or file not found." << endl;
		return;
	}

	// Variable to hold each line.
	string line;
	// While there are lines in the file to read...
	while (getline(file, line)) {

		// Vector to hold the sections of the line.
		vector<string> sections;

		stringstream ss(line);
		string section;

		// While there are sections of the line to read...
		while (getline(ss, section, ',')) {
			// Add the section to the sections vector.
			sections.push_back(section);
		}

		// If there arent at least two arguments for the Course...
		if (sections.size() < 2) {
			cout <<
				"Error: Course could not be added;" <<
				" all courses must have a minimum of two arguments." <<
				endl;
			// Empty the sections vector
			sections.clear();
			// Proceed to the next line in the file.
			continue;
		}

		// If the course number or course name is empty...
		if (sections[0].empty() || sections[1].empty()) {
			cout <<
				"Error; Course could not be added; " <<
				"Course number or course name is empty." << endl;
			// Empty the sections vector
			sections.clear();
			// Proceed to the next line in the file.
			continue;
		}

		vector <string> prerequisites;
		string courseNum = sections[0];
		string courseName = sections[1];

		// If there are more than two Course arguments...
		if (sections.size() > 2) {
			// For all arguments starting at index 2 to end of sections vector...
			for (unsigned int i = 2; i < sections.size(); i++) {
				// If the section is not an empty string...
				if (!sections[i].empty()) {
					// Add it to the prerequisites Vector
					prerequisites.push_back(sections[i]);
				}
			}
		}
		// Check to see if course already exists.
		/* Note: This covers an edge case where a user might want to load different
		 * courses from two different files.
		 */
		Course existing = courseTree->Search(courseNum);
		// If the course to be added is already in the tree...
		if (!existing.courseNum.empty()) {
			cout << "Course Number " << courseNum <<
				" already exists; skipping duplicate."
				<< endl;
			continue;
		}

		// Create a Course object.
		Course course(courseNum, courseName, prerequisites);

		// Insert into BinarySearchTree
		courseTree->Insert(course);
	}

	// Make sure all courses have valid prerequisites
	courseTree->ValidatePrerequisites();
}


// Main function
int main() {
	// Timer Variable
	clock_t ticks;

	// Binary Search Tree
	BinarySearchTree* courseTree;
	courseTree = new BinarySearchTree();
	Course course;

	int choice = 0;

	cout << "Welcome to the course planner." << endl;

	while (choice != 9) {
		cout << "1. Load Data Structure." << endl;
		cout << "2. Print Course List." << endl;
		cout << "9. Exit" << endl;
		cout << endl;
		cout << "What would you like to do?" << endl;
		cin >> choice;

		// Check to handle non integer menu input.
		if (cin.fail()) {
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			cout << "Invalid Input! Please enter an integer." << endl;
			cout << endl;
			continue;
		}

		switch (choice) {
		case 1: { // Load Data
			// Init Timer
			ticks = clock();

			// Prompt the user for the file name.
			string fileName;
			cout << "Enter the name of the file to load: " << endl;
			cin.ignore();
			getline(cin, fileName);

			// Extension Handling
			// If the filename is less than four characters
			if (fileName.size() < 4) {
				// It can't contain an extension so add one
				fileName += ".csv";
			}
			else {
				// Get the last four characters of the filename.
				string extension = fileName.substr(fileName.size() - 4);
				// Convert the file extension to lowercase in place
				transform(extension.begin(), extension.end(), extension.begin(), ::tolower);
				// If the file extension is not .csv, make it .csv
				if (extension != ".csv") {
					fileName += ".csv";
				}
			}

			loadCourses(fileName, courseTree);
			cout << courseTree->Size() << " Courses read" << endl;

			// Display how long the load took.
			ticks = clock() - ticks;
			cout << "time: " << ticks << " clock ticks" << endl;
			cout << "time: " << ticks * 1.0 / CLOCKS_PER_SEC << " seconds" << endl;
			cout << endl;
			break;
		}
		case 2: { // Print Course List
			// Check to ensure courses have been loaded.
			if (courseTree->Size() == 0) {
				cout << "You must use Option 1 to load in Courses before they can be displayed."
					<< endl;
				cout << endl;
				break;
			}
			// If courses have been loaded.
			else {
				cout << "Here is a sample schedule:" << endl;
				courseTree->InOrder();
				cout << endl;
				break;
			}
		}
		case 9: { // Exit.
			break;
		}
		default: {
			cout << choice << " is not a valid option." << endl;
			cout << endl;
			break;
		}
		}
	}
	cout << "Thank you for using the course planner!" << endl;
}
