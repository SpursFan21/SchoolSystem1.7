#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <iostream>
#include <cstdlib> // Include cstdlib for the exit() function

// User base class
class User {
protected:
    std::string username;
    std::string password;

public:
    User() {}
    User(const std::string& username, const std::string& password)
        : username(username), password(password) {};

    std::string getUsername() const { return username; }
    std::string getPassword() const { return password; }

    virtual void displayMenu() const = 0; // Pure virtual function for displaying user menu

    void signOut() const {
        std::cout << "Signing out..." << std::endl;
        // Perform any necessary cleanup or tasks before signing out
        // Return to the initial menu
    }
};

// Admin class derived from User
class Admin : public User {
public:
    Admin() {}
    Admin(const std::string& username, const std::string& password) : User(username, password) {}

    void displayMenu() const override {
        std::cout << "Admin Menu" << std::endl;
        std::cout << "1. Register User" << std::endl;
        std::cout << "2. Update User" << std::endl;
        std::cout << "3. Delete User" << std::endl;
        std::cout << "4. View User Information" << std::endl;
        std::cout << "5. Add Student" << std::endl;
        std::cout << "6. Add Teacher" << std::endl;
        std::cout << "7. Add Class" << std::endl;
        std::cout << "8. Add Grade for Student" << std::endl;
        std::cout << "9. Sign Out" << std::endl;
    }
};

// Teacher class derived from User
class Teacher : public User {
public:
    Teacher() {}
    Teacher(const std::string& username, const std::string& password) : User(username, password) {}

    void displayMenu() const override {
        std::cout << "Teacher Menu" << std::endl;
        std::cout << "1. Add Grade for Student" << std::endl;
        std::cout << "2. Sign Out" << std::endl;
    }
};

// Parent class derived from User
class Parent : public User {
public:
    Parent() {}
    Parent(const std::string& username, const std::string& password) : User(username, password) {}

    void displayMenu() const override {
        std::cout << "Parent Menu" << std::endl;
        std::cout << "1. View Student Report Card" << std::endl;
        std::cout << "2. Sign Out" << std::endl;
    }
};

// Student class
class Student {
private:
    std::string name;
    std::vector<std::string> classes;
    std::vector<int> grades;

public:
    Student(const std::string& name) : name(name) {}

    void addClass(const std::string& className) {
        classes.push_back(className);
        grades.push_back(0); // Initialize grade as 0
    }

    void updateGrade(int index, int grade) {
        if (index >= 0 && index < grades.size()) {
            grades[index] = grade;
        }
    }

    std::string getName() const { return name; }
    const std::vector<std::string>& getClasses() const { return classes; }
    const std::vector<int>& getGrades() const { return grades; }
};

// School information class
class School {
private:
    std::vector<User*> users;
    std::vector<Student> students;

public:
    ~School() {
        // Clean up dynamically allocated user objects
        for (User* user : users) {
            delete user;
        }
    }

    void registerUser(User* user) {
        users.push_back(user);
    }

    User* findUser(const std::string& username) const {
        for (User* user : users) {
            if (user->getUsername() == username) {
                return user;
            }
        }
        return nullptr;
    }

    void addStudent(const std::string& name) {
        students.push_back(Student(name));
    }

    Student* findStudent(const std::string& name) {
        for (Student& student : students) {
            if (student.getName() == name) {
                return &student;
            }
        }
        return nullptr;
    }
};

// Function to split a string into tokens
std::vector<std::string> splitString(const std::string& str, char delimiter) {
    std::vector<std::string> tokens;
    std::stringstream ss(str);
    std::string token;

    while (std::getline(ss, token, delimiter)) {
        tokens.push_back(token);
    }

    return tokens;
}
void exitProgram()
{
    std::cout << "Exiting..." << std::endl;
    exit(0);
}

int main() {
    School school;
    std::string line;

    std::ifstream userFile("users.txt");
    if (userFile.is_open()) {
        while (std::getline(userFile, line)) {
            std::vector<std::string> tokens = splitString(line, ',');
            if (tokens.size() == 3) {
                std::string userType = tokens[0];
                std::string username = tokens[1];
                std::string password = tokens[2];

                if (userType == "admin") {
                    Admin* admin = new Admin(username, password);
                    school.registerUser(admin);
                }
                else if (userType == "teacher") {
                    Teacher* teacher = new Teacher(username, password);
                    school.registerUser(teacher);
                }
                else if (userType == "parent") {
                    Parent* parent = new Parent(username, password);
                    school.registerUser(parent);
                }
            }
        }
        userFile.close();
    }
    else {
        std::cout << "Failed to open users.txt" << std::endl;
        return 1;
    }

    User* currentUser = nullptr;
    bool loggedIn = false;
    bool exitProgram = false;

    int choice;

    while (!loggedIn) {

        std::cout << "===== Start Menu =====" << std::endl;
        std::cout << "1. Log in" << std::endl;
        std::cout << "2. Create New Admin" << std::endl;
        std::cout << "3. Exit" << std::endl;
        std::cout << "======================" << std::endl;
        std::cout << "Enter your choice: ";
        std::cin >> choice;

        switch (choice) {
        case 1: {
            std::string username, password;
            std::cout << "Enter username: ";
            std::cin >> username;
            std::cout << "Enter password: ";
            std::cin >> password;

            User* user = school.findUser(username);
            if (user && user->getPassword() == password) {
                currentUser = user;
                loggedIn = true;
            }
            else {
                std::cout << "Invalid username or password" << std::endl;
            }

            break;
        }
        case 2: {
            std::string username, password;
            std::cout << "Enter new admin username: ";
            std::cin >> username;
            std::cout << "Enter new admin password: ";
            std::cin >> password;

            Admin* newAdmin = new Admin(username, password);
            school.registerUser(newAdmin);
            std::cout << "New admin created successfully!" << std::endl;

            break;
        }
        case 3: {
            std::cout << "Exiting..." << std::endl;
            return 0;
        }
        default:
            std::cout << "Invalid choice" << std::endl;
            break;
        }
    }

    while (loggedIn) {
        currentUser->displayMenu();
        bool validChoice = false;
        std::string continueChoice; // Declare continueChoice variable

        while (!validChoice) {
            // ...

            std::cout << "Enter your choice: ";
            std::cin >> choice;

            switch (choice) {
            case 1: {
                // Register User
                std::string userType, username, password;
                std::cout << "Enter the user type (admin/teacher/parent): ";
                std::cin >> userType;
                std::cout << "Enter the username: ";
                std::cin >> username;
                std::cout << "Enter the password: ";
                std::cin >> password;

                // ...

                std::cout << "User registered successfully." << std::endl;

                std::cout << "Do you want to continue? (yes/no): ";
                std::cin >> continueChoice;

                if (continueChoice == "no") {
                    exitProgram = true;
                }
                else {
                    validChoice = true;
                }
                break;
            }
            case 2: {
                // Update User
                std::string username;
                std::cout << "Enter the username of the user to update: ";
                std::cin >> username;

                User* userToUpdate = school.findUser(username);
                if (userToUpdate) {
                    // Perform the necessary update operations on the user
                    // You can add your implementation here
                    std::cout << "User updated successfully." << std::endl;
                }
                else {
                    std::cout << "User not found." << std::endl;
                }
                validChoice = true;
                break;
            }
            case 3: {
                // Delete User
                std::string username;
                std::cout << "Enter the username of the user to delete: ";
                std::cin >> username;

                User* userToDelete = school.findUser(username);
                if (userToDelete) {
                    // Perform the necessary delete operations on the user
                    // You can add your implementation here
                    std::cout << "User deleted successfully." << std::endl;
                }
                else {
                    std::cout << "User not found." << std::endl;
                }
                validChoice = true;
                break;
            }
            case 4: {
                // View User Information
                std::string username;
                std::cout << "Enter the username of the user to view information: ";
                std::cin >> username;

                User* userToView = school.findUser(username);
                if (userToView) {
                    // Display the user information
                    // You can add your implementation here
                }
                else {
                    std::cout << "User not found." << std::endl;
                }
                validChoice = true;
                break;
            }
            case 5: {
                // Add Student
                std::string studentName;
                std::cout << "Enter the name of the student to add: ";
                std::cin >> studentName;

                school.addStudent(studentName);
                std::cout << "Student added successfully." << std::endl;
                validChoice = true;
                break;
            }
            case 6: {
                // Add Teacher
                std::string teacherName;
                std::cout << "Enter the name of the teacher to add: ";
                std::cin >> teacherName;

                // Perform the necessary operations to add a teacher
                // You can add your implementation here

                std::cout << "Teacher added successfully." << std::endl;
                validChoice = true;
                break;
            }
            case 7: {
                // Add Class
                // Perform the necessary operations to add a class
                // You can add your implementation here
                validChoice = true;
                break;
            }
            case 8: {
                // Add Grade for Student
                // Perform the necessary operations to add a grade for a student
                // You can add your implementation here
                validChoice = true;
                break;
            }
            case 9: {
                currentUser->signOut();
                break;
            }
            default:
                std::cout << "Invalid choice. Please try again." << std::endl;
                break;
            }
          
        }
        std::cout << "Do you want to continue? (yes/no): ";
        std::cin >> continueChoice;

        if (continueChoice == "no") {
            exitProgram = true;
        }

    }
}