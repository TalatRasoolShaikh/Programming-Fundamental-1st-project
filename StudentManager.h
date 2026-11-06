#ifndef STUDENT_MANAGER_H
#define STUDENT_MANAGER_H

#include "Student.h"
#include <vector>
#include <string>

/**
 * @brief Manages student records, file I/O operations (students.txt),
 *        and record navigation state.
 */
class StudentManager {
private:
    std::string filePath;
    std::vector<Student> students;
    int currentIndex; // -1 indicates no record is currently selected

public:
    explicit StudentManager(const std::string& filename = "students.txt");

    // File I/O
    bool loadFromFile(std::string& errorMsg);
    bool saveToFile(std::string& errorMsg);

    // CRUD operations
    bool addStudent(const Student& student, std::string& errorMsg);
    bool updateStudent(const Student& student, std::string& errorMsg);
    bool deleteStudent(const std::string& rollNo, std::string& errorMsg);

    // Navigation
    bool getNextStudent(Student& outStudent, int& outIndex, int& outTotal, bool& outWrapped);
    bool getCurrentStudent(Student& outStudent) const;
    void resetNavigation();

    // Query & State
    bool rollNumberExists(const std::string& rollNo) const;
    int findIndexByRollNo(const std::string& rollNo) const;
    size_t getCount() const;
    int getCurrentIndex() const;
    std::string getFilePath() const;
    const std::vector<Student>& getAllStudents() const;
};

#endif // STUDENT_MANAGER_H

