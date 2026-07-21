#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <vector>

using namespace std;

class Student {
private:
    int rollNumber;
    string name;
    string course;
    float marks;

public:
    // Constructors
    Student() : rollNumber(0), name(""), course(""), marks(0.0) {}
    Student(int r, string n, string c, float m) : rollNumber(r), name(n), course(c), marks(m) {}

    // Getters
    int getRollNumber() const { return rollNumber; }
    string getName() const { return name; }
    string getCourse() const { return course; }
    float getMarks() const { return marks; }

    // Setters
    void setName(const string& n) { name = n; }
    void setCourse(const string& c) { course = c; }
    void setMarks(float m) { marks = m; }

    // Display formatted record row
    void displayRow() const {
        cout << left << setw(12) << rollNumber 
             << setw(25) << name 
             << setw(20) << course 
             << setw(10) << fixed << setprecision(2) << marks << endl;
    }

    // Input student details from user
    void inputDetails() {
        cout << "Enter Roll Number: ";
        cin >> rollNumber;
        cin.ignore(); // Clear buffer
        cout << "Enter Full Name: ";
        getline(cin, name);
        cout << "Enter Course: ";
        getline(cin, course);
        cout << "Enter Marks: ";
        cin >> marks;
    }

    // File Handling: Write record to stream
    void writeToFile(ofstream& outFile) const {
        outFile << rollNumber << "\n" << name << "\n" << course << "\n" << marks << "\n";
    }

    // File Handling: Read record from stream
    bool readFromFile(ifstream& inFile) {
        if (inFile >> rollNumber) {
            inFile.ignore();
            getline(inFile, name);
            getline(inFile, course);
            inFile >> marks;
            inFile.ignore();
            return true;
        }
        return false;
    }
};

// System Manager Class
class StudentManagementSystem {
private:
    const string filename = "students.txt";

public:
    // Add a new student record
    void addStudent() {
        ofstream outFile(filename, ios::app);
        if (!outFile) {
            cout << "Error opening file for writing!\n";
            return;
        }

        Student s;
        s.inputDetails();

        // Check for duplicate roll number
        if (searchByRoll(s.getRollNumber(), false)) {
            cout << "\nError: Student with Roll Number " << s.getRollNumber() << " already exists!\n";
            outFile.close();
            return;
        }

        s.writeToFile(outFile);
        outFile.close();
        cout << "\nStudent record added successfully!\n";
    }

    // Display all student records
    void displayAll() {
        ifstream inFile(filename);
        if (!inFile) {
            cout << "\nNo records found (file does not exist or is empty).\n";
            return;
        }

        Student s;
        bool found = false;

        cout << "\n---------------------------------------------------------------\n";
        cout << left << setw(12) << "Roll No" 
             << setw(25) << "Name" 
             << setw(20) << "Course" 
             << setw(10) << "Marks" << endl;
        cout << "---------------------------------------------------------------\n";

        while (s.readFromFile(inFile)) {
            s.displayRow();
            found = true;
        }

        cout << "---------------------------------------------------------------\n";
        if (!found) {
            cout << "No student records stored yet.\n";
        }
        inFile.close();
    }

    // Search for a student by roll number
    bool searchByRoll(int targetRoll, bool printDetails = true) {
        ifstream inFile(filename);
        if (!inFile) return false;

        Student s;
        while (s.readFromFile(inFile)) {
            if (s.getRollNumber() == targetRoll) {
                if (printDetails) {
                    cout << "\nStudent Found:\n";
                    cout << "-----------------------------------\n";
                    cout << "Roll Number : " << s.getRollNumber() << endl;
                    cout << "Name        : " << s.getName() << endl;
                    cout << "Course      : " << s.getCourse() << endl;
                    cout << "Marks       : " << s.getMarks() << endl;
                    cout << "-----------------------------------\n";
                }
                inFile.close();
                return true;
            }
        }
        inFile.close();
        return false;
    }

    // Update existing student details
    void updateStudent() {
        int roll;
        cout << "Enter Roll Number of student to update: ";
        cin >> roll;

        ifstream inFile(filename);
        if (!inFile) {
            cout << "No records found.\n";
            return;
        }

        vector<Student> students;
        Student s;
        bool found = false;

        while (s.readFromFile(inFile)) {
            if (s.getRollNumber() == roll) {
                found = true;
                cout << "\nExisting Record Found. Enter New Details:\n";
                cin.ignore();
                string name, course;
                float marks;

                cout << "Enter New Name: ";
                getline(cin, name);
                cout << "Enter New Course: ";
                getline(cin, course);
                cout << "Enter New Marks: ";
                cin >> marks;

                s.setName(name);
                s.setCourse(course);
                s.setMarks(marks);
            }
            students.push_back(s);
        }
        inFile.close();

        if (!found) {
            cout << "Student with Roll Number " << roll << " not found!\n";
            return;
        }

        // Save updated list back to file
        ofstream outFile(filename, ios::trunc);
        for (const auto& student : students) {
            student.writeToFile(outFile);
        }
        outFile.close();

        cout << "\nStudent record updated successfully!\n";
    }

    // Delete a student record
    void deleteStudent() {
        int roll;
        cout << "Enter Roll Number of student to delete: ";
        cin >> roll;

        ifstream inFile(filename);
        if (!inFile) {
            cout << "No records found.\n";
            return;
        }

        vector<Student> students;
        Student s;
        bool found = false;

        while (s.readFromFile(inFile)) {
            if (s.getRollNumber() == roll) {
                found = true;
            } else {
                students.push_back(s);
            }
        }
        inFile.close();

        if (!found) {
            cout << "Student with Roll Number " << roll << " not found!\n";
            return;
        }

        // Write remaining records back to file
        ofstream outFile(filename, ios::trunc);
        for (const auto& student : students) {
            student.writeToFile(outFile);
        }
        outFile.close();

        cout << "\nStudent record deleted successfully!\n";
    }
};

// Main Menu Loop
int main() {
    StudentManagementSystem sys;
    int choice;

    do {
        cout << "\n=========================================\n";
        cout << "     STUDENT MANAGEMENT SYSTEM (C++)     \n";
        cout << "=========================================\n";
        cout << "1. Add Student Record\n";
        cout << "2. Display All Student Records\n";
        cout << "3. Search Student by Roll Number\n";
        cout << "4. Update Student Record\n";
        cout << "5. Delete Student Record\n";
        cout << "6. Exit\n";
        cout << "=========================================\n";
        cout << "Enter your choice (1-6): ";
        cin >> choice;

        switch (choice) {
            case 1:
                sys.addStudent();
                break;
            case 2:
                sys.displayAll();
                break;
            case 3: {
                int roll;
                cout << "Enter Roll Number to search: ";
                cin >> roll;
                if (!sys.searchByRoll(roll, true)) {
                    cout << "Student not found.\n";
                }
                break;
            }
            case 4:
                sys.updateStudent();
                break;
            case 5:
                sys.deleteStudent();
                break;
            case 6:
                cout << "\nExiting System. Good luck with your internship submission!\n";
                break;
            default:
                cout << "\nInvalid choice! Please try again.\n";
        }
    } while (choice != 6);

    return 0;
}