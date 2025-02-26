// Project2.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>

// Course structure to store course details
struct Course {
    std::string courseID;
    std::string courseName;
    int preCount;
    std::string preList;

    Course() : courseID(""), courseName(""), preCount(0), preList("") {}
};

// Function to parse the CSV file and load course data into a vector
std::vector<Course> txtParser(const std::string& filePath) {
    std::vector<Course> tempList;
    std::ifstream file(filePath);

    if (!file.is_open()) {
        std::cerr << "Error opening file: " << filePath << std::endl;
        return tempList;
    }

    std::string line;
    while (getline(file, line)) {
        std::stringstream ss(line);
        Course course;
        std::getline(ss, course.courseID, ',');
        std::getline(ss, course.courseName, ',');

        std::string preName;
        course.preCount = 0;
        while (std::getline(ss, preName, ',')) {
            if (!preName.empty()) {
                if (course.preCount > 0) course.preList += ",";
                course.preList += preName;
                course.preCount++;
            }
        }

        tempList.push_back(course);
    }

    file.close();
    return tempList;
}

// Function to search for a course by ID
Course searchList(const std::string& searchID, const std::vector<Course>& courseList) {
    for (const auto& course : courseList) {
        if (course.courseID == searchID) {
            return course;
        }
    }
    return Course(); // Return an empty course if not found
}

// Function to print course details and its prerequisites
void printCourse(const std::string& courseID, const std::vector<Course>& courseList) {
    Course course = searchList(courseID, courseList);

    if (course.courseID.empty()) {
        std::cout << "Course not found." << std::endl;
        return;
    }

    std::cout << "Course ID: " << course.courseID << std::endl;
    std::cout << "Course Name: " << course.courseName << std::endl;

    if (course.preCount > 0) {
        std::cout << "Prerequisites:" << std::endl;
        std::stringstream ss(course.preList);
        std::string prerequisite;
        while (std::getline(ss, prerequisite, ',')) {
            printCourse(prerequisite, courseList); // Recursive call for prerequisites
        }
    }
    else {
        std::cout << "No prerequisites." << std::endl;
    }
}

// Function to validate course prerequisites
bool validateList(const std::vector<Course>& courseList) {
    for (const auto& course : courseList) {
        std::stringstream ss(course.preList);
        std::string prerequisite;
        while (std::getline(ss, prerequisite, ',')) {
            Course preCourse = searchList(prerequisite, courseList);
            if (preCourse.courseID.empty()) {
                std::cout << "Error: Prerequisite " << prerequisite << " not found." << std::endl;
                return false;
            }
        }
    }
    return true;
}

// Function to print all courses alphanumerically
void printSortedCourses(std::vector<Course>& courseList) {
    std::sort(courseList.begin(), courseList.end(), [](const Course& a, const Course& b) {
        return a.courseID < b.courseID;
        });

    for (const auto& course : courseList) {
        std::cout << course.courseID << ": " << course.courseName << std::endl;
    }
}

// Function to display the main menu
void displayMenu() {
    std::cout << "Welcome to the Advising Assistance Program!" << std::endl;
    std::cout << "1. Load course data into the data structure" << std::endl;
    std::cout << "2. Print alphanumerically ordered list of all courses" << std::endl;
    std::cout << "3. Print course title and prerequisites for a specific course" << std::endl;
    std::cout << "9. Exit" << std::endl;
}

// Main program flow
int main() {
    std::vector<Course> courseList;
    bool courseDataLoaded = false;
    std::string filePath = "courses.csv";  // Default file path

    while (true) {
        displayMenu();
        int option;
        std::cout << "Enter your choice: ";
        std::cin >> option;
        std::cin.ignore(); // Clear the input buffer

        switch (option) {
        case 1: {
            if (!courseDataLoaded) {
                std::cout << "Enter the file path for course data (or press Enter to use default): ";
                std::getline(std::cin, filePath);
                if (filePath.empty()) {
                    filePath = "courses.csv"; // Use default path if none provided
                }
                courseList = txtParser(filePath);

                if (!courseList.empty() && validateList(courseList)) {
                    courseDataLoaded = true;
                    std::cout << "Course data successfully loaded!" << std::endl;
                }
                else {
                    std::cout << "Error loading course data." << std::endl;
                }
            }
            else {
                std::cout << "Course data has already been loaded!" << std::endl;
            }
            break;
        }
        case 2: {
            if (courseDataLoaded) {
                printSortedCourses(courseList);
            }
            else {
                std::cout << "Please load the course data first!" << std::endl;
            }
            break;
        }
        case 3: {
            if (courseDataLoaded) {
                std::string courseID;
                std::cout << "Enter course ID to search for: ";
                std::getline(std::cin, courseID);
                printCourse(courseID, courseList);
            }
            else {
                std::cout << "Please load the course data first!" << std::endl;
            }
            break;
        }
        case 9:
            std::cout << "Exiting the program. Goodbye!" << std::endl;
            return 0;
        default:
            std::cout << "Invalid option, please try again." << std::endl;
        }
    }

    return 0;
}

