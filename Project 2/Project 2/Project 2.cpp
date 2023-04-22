//============================================================================
// Name        : ABCU_Project.cpp
// Author      : Mariam Haji
// Version     :
// Copyright   : SNHU Project 2023
// Description : ABCU_Project BST
//============================================================================

#include <iostream>
#include <fstream>

#include "CSVparser.hpp"


using namespace std;


// A struct to represent a course, including its name, number and prerequisites
struct CourseNode {
	string courseName;
	string courseNum;
	vector<string> preRequisite;

	CourseNode() {}
};

// A struct to represent a node in the binary search tree
struct Node {
	CourseNode course;
	Node* left;
	Node* right;

	// default constructor
	Node() {
		left = nullptr;
		right = nullptr;
	}

	// constructor that initializes the course and calls the default constructor for left and right pointers
	Node(CourseNode aCourse) : Node(){
		this->course = aCourse;
	}
};

// A class to represent the binary search tree
class CourseBST {

private:
	// pointer to the root of the binary search tree
	Node* root;

	// helper function to add a new node to the binary search tree
	// helper function to print a sample course schedule
	// helper function to print information about a specific course
	void addNode(Node* node, CourseNode course);
	void printSampleSchedule(Node* node);
	void printCourseInformation(Node* node, string courseNum);

public:

	// default constructor
	CourseBST();

	//destructor
	virtual ~CourseBST();

	// function to recursively delete nodes from the binary search tree
	void DeleteRecursive(Node* node);

	// function to insert a new course into the binary search tree
	void Insert(CourseNode course);

	// function to count the number of prerequisites for a given course
	int NumPrerequisiteCourses(CourseNode course);

	// function to print a sample course schedule
	void PrintSampleSchedule();

	// function to print information about a specific course
	void PrintCourseInformation(string courseNum);
};

// Implement the default constructor for the CourseBST class
CourseBST::CourseBST() {
	root = nullptr;
}

// Implement the destructor for the CourseBST class
CourseBST::~CourseBST() {
	DeleteRecursive(root);
}

// Helper function to recursively delete nodes from the binary search tree
void CourseBST::DeleteRecursive(Node* node) {
	if (node) {
		DeleteRecursive(node->left);
		DeleteRecursive(node->right);
		delete node;
	}
}

// Function to insert a new course into the binary search tree
void CourseBST::Insert(CourseNode course) {
	if (root == nullptr)
		root = new Node(course);

	else
		this->addNode(root, course);

}

// Function to count the number of prerequisites for a given course
int CourseBST::NumPrerequisiteCourses(CourseNode course) {
	int count = 0;
	for (unsigned int i = 0; i < course.preRequisite.size(); i++) {
		if (course.preRequisite.at(i).length() > 0)
			count++;
	}
	return count;
}

// Function to print a sample course schedule
void CourseBST::PrintSampleSchedule() {
	this->printSampleSchedule(root);
}

// Function to print information about a specific course
void CourseBST::PrintCourseInformation(string courseNum){
	this->printCourseInformation(root, courseNum);
}

// Helper function to add a new node to the binary search tree
void CourseBST::addNode(Node* node, CourseNode course) {
	if (node->course.courseNum.compare(course.courseNum) > 0) {
		if (node->left == nullptr)
			node->left = new Node(course);

		else
			this->addNode(node->left, course);
	}
	else {
		if (node->right == nullptr)
			node->right = new Node(course);
		else
			this->addNode(node->right, course);
	}
}

// Function to print schedule sample
void CourseBST::printSampleSchedule(Node* node) {
	// Check if the node is null (base case for the recursion)
	if (node != nullptr) {

		// Recursively traverse the left subtree of the node and print course number and name
		printSampleSchedule(node->left);
		cout << node->course.courseNum << ", " << node->course.courseName << endl;

		// Recursively traverse the right subtree of the node
		printSampleSchedule(node->right);
	}
	return;
}

// print out course information for a given course number
void CourseBST::printCourseInformation(Node* curr, string courseNum) {
	while (curr != nullptr) {

		/** Check if the current node has a course number that matches the given course number
		 * Print out the course number and name
		 * Calculate the number of prerequisite courses for the current course
		 * Print out the prerequisite courses, if any
		 */
		if (curr->course.courseNum.compare(courseNum) == 0) {

			cout << endl << curr->course.courseNum << ", " << curr->course.courseName << endl;
			unsigned int size = NumPrerequisiteCourses(curr->course);
			cout << "Prerequisite(s): ";

			unsigned int i = 0;
			for (i = 0; i < size; i++) {
				cout << curr->course.preRequisite.at(i);
				if (i != size - 1)
					cout << ", ";
			}

			/** If there are no prerequisites, print out a message saying so
			 * Return from the method
			 */
			if (i == 0)
				cout << "No prerequisites required.";
			cout << endl;
			return;
		}
		/**
		 * If the current node's course number is greater than the given course number,
		 * traverse the left subtree of the current node
		 */
		 else if (courseNum.compare(curr->course.courseNum) < 0)
			 curr = curr->left;

		/** If the current node's course number is less than the given course number,
		 * traverse the right subtree of the current node
		 */

		 else
			 curr = curr->right;
	}
	// If there is no node with matching course number print course not found
	 cout << "Course " << courseNum << " not found." << endl;
}

// Method to load courses from a CSV file and insert them into a BST
bool loadCourses(string csvPath, CourseBST* coursesBST) {
	try {

		// Open the CSV file for reading
		ifstream courseFile(csvPath);
		if (courseFile.is_open()) {

			// Loop through each line in the CSV file until the end of the file is reached
			while (!courseFile.eof()) {

				// Create a vector to store the course information
				vector<string> courseInfo;

				// Read a line from the CSV file
				string courseData;
				getline(courseFile, courseData);

				// Parse the line into individual fields separated by commas
				while (courseData.length() > 0) {

					/* Find the next comma in the line if the comma is within the expected range,
					 * extract the field and remove it from the line else treat the rest of the
					 * line as a single field and exit the loop
					**/
					unsigned int comma = courseData.find(',');
					if (comma < 100) {
						courseInfo.push_back(courseData.substr(0, comma));
						courseData.erase(0, comma + 1);
					}
					else{
						courseInfo.push_back(courseData.substr(0, courseData.length()));
						courseData = "";
					}
				}

				// Create a new course node and populate its fields with the parsed information
				CourseNode course;
				course.courseNum = courseInfo[0];
				course.courseName = courseInfo[1];

				for (unsigned int i = 2; i < courseInfo.size(); i++) {
					course.preRequisite.push_back(courseInfo[i]);
				}

				// Insert the course node into the BST
				coursesBST->Insert(course);
			}

			// Close the CSV file and return true to indicate success
			courseFile.close();
			return true;
		}
	}

	// Catch any exceptions thrown during parsing and print an error message
	catch (csv::Error& e) {
		cerr << e.what() << endl;
	}
	return false;
}

//Main Method
int main(int argc, char* argv[]) {

	// Initialize variables for command line arguments
	string csvPath, courseId;
	switch (argc) {

	case 2:
		csvPath = argv[1];
		break;

	case 3:
		csvPath = argv[1];
		courseId = argv[2];
		break;

	default:
		csvPath = "";
		break;
	}

	// Create a pointer to the course binary search tree and print welcome message
	CourseBST* coursesBST = nullptr;
	cout << "\nWelcome to the course planner!\n" << endl;

	// Initialize variables for user input
	string choice = "0";
	int userChoice = choice[0] - '0';

	while (userChoice != 9) {

		// Print menu options
		cout << "   1. Load Data Structure" << endl;
		cout << "   2. Print Course List" << endl;
		cout << "   3. Print Course" << endl;
		cout << "   9. Exit" << endl;
		cout << "\nWhat would you like to do? ";

		// Get user input and convert it to integer
		cin >> choice;

		if (choice.length() == 1)
			userChoice = choice[0] - '0';
		else
			userChoice = 0;
		bool success = 1;

		// Switch statement that executes a different code block depending on the value of option.
		switch (userChoice){
		case 1:
			// initialize coursesBST
			if (coursesBST == nullptr)
				coursesBST = new CourseBST();

			// set csvPath and prompt user to input filename
			if (csvPath.length() == 0) {
				cout << "Enter the file name that contains the course data: ";
				cin >> csvPath;
			}

			//load course data from file into the BST
			success = loadCourses(csvPath, coursesBST);
			if (success)
				cout << "Courses have been loaded.\n" << endl;
			else
				cout << "File not found.\n" << endl;
			csvPath = "";
			break;

		case 2:
			// print the sample schedule
			if (coursesBST != nullptr && success) {
				cout << "Here is a sample schedule:\n" << endl;
				coursesBST->PrintSampleSchedule();
				cout << endl;
			}
			// if courses aren't loaded promt user to load courses
			else
				cout << "Load courses first - option 1\n" << endl;
			break;

		case 3:
			//prompt user to enter course ID and print course information
			if (coursesBST != nullptr && success) {
				if (courseId.length() == 0){
					cout << "What course do you want to know about? ";
					cin >> courseId;
					for (auto& userChoice : courseId) userChoice = toupper(userChoice);
				}
				coursesBST->PrintCourseInformation(courseId);
				cout << endl;
			}
			else
				cout << "Load courses first - option 1\n" << endl;
			courseId = "";
			break;

		case 4:
			// exit program
			cout << "Thank you for using the course planner!" << endl;
			return 0;

		default:
			if (userChoice != 9)
				cout << choice << " is not a valid option\n" << endl;
			break;
		}
	}
	cout << "\nThank you for using the course planner!" << endl;
	return 0;
}






