#include "StudentManager.h"
#include <fstream>
#include <iostream>

StudentManager::StudentManager(const std::string& filename)
    : filePath(filename), currentIndex(-1) {
    std::string err;
    loadFromFile(err);
}

bool StudentManager::loadFromFile(std::string& errorMsg) {
    students.clear();
    currentIndex = -1;

    std::ifstream inFile(filePath.c_str());
    if (!inFile.is_open()) {
        // If file does not exist yet, that is acceptable on first run.
        return true;
    }

    std::string line;
    int lineNum = 0;
    while (std::getline(inFile, line)) {
        lineNum++;
        line = Student::trim(line);
        if (line.empty()) continue; // Skip blank lines

        Student s;
        std::string parseErr;
        if (Student::fromCSV(line, s, parseErr)) {
            students.push_back(s);
        } else {
            // Log or record parsing issue for line, but continue loading valid ones
            errorMsg = "Warning: Skipped line " + std::to_string(lineNum) + " (" + parseErr + ")";
        }
    }

    inFile.close();
    return true;
}

bool StudentManager::saveToFile(std::string& errorMsg) {
    std::ofstream outFile(filePath.c_str(), std::ios::out | std::ios::trunc);
    if (!outFile.is_open()) {
        errorMsg = "Failed to open file for writing: " + filePath;
        return false;
    }

    for (size_t i = 0; i < students.size(); ++i) {
        outFile << students[i].toCSV() << "\n";
    }

    if (outFile.fail()) {
        errorMsg = "An error occurred while writing data to " + filePath;
        outFile.close();
        return false;
    }

    outFile.close();
    return true;
}

bool StudentManager::addStudent(const Student& student, std::string& errorMsg) {
    if (!student.validate(errorMsg)) {
        return false;
    }

    if (rollNumberExists(student.getRollNo())) {
        errorMsg = "A student with Roll Number '" + student.getRollNo() + "' already exists.";
        return false;
    }

    students.push_back(student);

    if (!saveToFile(errorMsg)) {
        // Revert in-memory modification if save failed
        students.pop_back();
        return false;
    }

    return true;
}

bool StudentManager::updateStudent(const Student& student, std::string& errorMsg) {
    if (!student.validate(errorMsg)) {
        return false;
    }

    int idx = findIndexByRollNo(student.getRollNo());
    if (idx == -1) {
        errorMsg = "Cannot update: Student with Roll Number '" + student.getRollNo() + "' not found.";
        return false;
    }

    Student oldStudent = students[idx];
    students[idx] = student;

    if (!saveToFile(errorMsg)) {
        students[idx] = oldStudent; // Revert on failure
        return false;
    }

    return true;
}

bool StudentManager::deleteStudent(const std::string& rollNo, std::string& errorMsg) {
    int idx = findIndexByRollNo(rollNo);
    if (idx == -1) {
        errorMsg = "Cannot delete: Student with Roll Number '" + rollNo + "' not found.";
        return false;
    }

    Student removedStudent = students[idx];
    students.erase(students.begin() + idx);

    if (!saveToFile(errorMsg)) {
        students.insert(students.begin() + idx, removedStudent); // Revert on failure
        return false;
    }

    // Adjust currentIndex
    if (students.empty()) {
        currentIndex = -1;
    } else if (currentIndex >= static_cast<int>(students.size())) {
        currentIndex = static_cast<int>(students.size()) - 1;
    }

    return true;
}

bool StudentManager::getNextStudent(Student& outStudent, int& outIndex, int& outTotal, bool& outWrapped) {
    outWrapped = false;
    if (students.empty()) {
        currentIndex = -1;
        outIndex = -1;
        outTotal = 0;
        return false;
    }

    currentIndex++;
    if (currentIndex >= static_cast<int>(students.size())) {
        currentIndex = 0; // Wrap around to the beginning
        outWrapped = true;
    }

    outStudent = students[currentIndex];
    outIndex = currentIndex + 1; // 1-based index for user display
    outTotal = static_cast<int>(students.size());
    return true;
}

bool StudentManager::getCurrentStudent(Student& outStudent) const {
    if (currentIndex >= 0 && currentIndex < static_cast<int>(students.size())) {
        outStudent = students[currentIndex];
        return true;
    }
    return false;
}

void StudentManager::resetNavigation() {
    currentIndex = -1;
}

bool StudentManager::rollNumberExists(const std::string& rollNo) const {
    return findIndexByRollNo(rollNo) != -1;
}

int StudentManager::findIndexByRollNo(const std::string& rollNo) const {
    for (size_t i = 0; i < students.size(); ++i) {
        if (students[i].getRollNo() == rollNo) {
            return static_cast<int>(i);
        }
    }
    return -1;
}

size_t StudentManager::getCount() const {
    return students.size();
}

int StudentManager::getCurrentIndex() const {
    return currentIndex;
}

std::string StudentManager::getFilePath() const {
    return filePath;
}

const std::vector<Student>& StudentManager::getAllStudents() const {
    return students;
}

