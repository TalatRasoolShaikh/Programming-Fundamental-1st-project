#include "Student.h"
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <cctype>
#include <vector>

Student::Student()
    : name(""), rollNo(""), fatherName(""), caste(""), district(""), gpa(0.0) {}

Student::Student(const std::string& name,
                 const std::string& rollNo,
                 const std::string& fatherName,
                 const std::string& caste,
                 const std::string& district,
                 double gpa)
    : name(trim(name)),
      rollNo(trim(rollNo)),
      fatherName(trim(fatherName)),
      caste(trim(caste)),
      district(trim(district)),
      gpa(gpa) {}

std::string Student::getName() const { return name; }
std::string Student::getRollNo() const { return rollNo; }
std::string Student::getFatherName() const { return fatherName; }
std::string Student::getCaste() const { return caste; }
std::string Student::getDistrict() const { return district; }
double Student::getGpa() const { return gpa; }

void Student::setName(const std::string& val) { name = trim(val); }
void Student::setRollNo(const std::string& val) { rollNo = trim(val); }
void Student::setFatherName(const std::string& val) { fatherName = trim(val); }
void Student::setCaste(const std::string& val) { caste = trim(val); }
void Student::setDistrict(const std::string& val) { district = trim(val); }
void Student::setGpa(double val) { gpa = val; }

std::string Student::trim(const std::string& str) {
    size_t first = str.find_first_not_of(" \t\r\n");
    if (first == std::string::npos) return "";
    size_t last = str.find_last_not_of(" \t\r\n");
    return str.substr(first, (last - first + 1));
}

bool Student::validate(std::string& errorMsg) const {
    if (name.empty()) {
        errorMsg = "Student Name cannot be empty.";
        return false;
    }
    if (name.find(',') != std::string::npos) {
        errorMsg = "Student Name cannot contain a comma (',').";
        return false;
    }

    if (rollNo.empty()) {
        errorMsg = "Roll Number cannot be empty.";
        return false;
    }
    if (rollNo.find(',') != std::string::npos) {
        errorMsg = "Roll Number cannot contain a comma (',').";
        return false;
    }

    if (fatherName.empty()) {
        errorMsg = "Father's Name cannot be empty.";
        return false;
    }
    if (fatherName.find(',') != std::string::npos) {
        errorMsg = "Father's Name cannot contain a comma (',').";
        return false;
    }

    if (caste.empty()) {
        errorMsg = "Caste cannot be empty.";
        return false;
    }
    if (caste.find(',') != std::string::npos) {
        errorMsg = "Caste cannot contain a comma (',').";
        return false;
    }

    if (district.empty()) {
        errorMsg = "District cannot be empty.";
        return false;
    }
    if (district.find(',') != std::string::npos) {
        errorMsg = "District cannot contain a comma (',').";
        return false;
    }

    if (gpa < 0.0 || gpa > 4.0) {
        errorMsg = "GPA must be between 0.00 and 4.00.";
        return false;
    }

    return true;
}

std::string Student::toCSV() const {
    std::ostringstream oss;
    oss << name << ","
        << rollNo << ","
        << fatherName << ","
        << caste << ","
        << district << ","
        << std::fixed << std::setprecision(2) << gpa;
    return oss.str();
}

bool Student::fromCSV(const std::string& csvLine, Student& outStudent, std::string& errorMsg) {
    std::string line = trim(csvLine);
    if (line.empty()) {
        errorMsg = "Empty record line.";
        return false;
    }

    std::stringstream ss(line);
    std::string token;
    std::vector<std::string> tokens;

    while (std::getline(ss, token, ',')) {
        tokens.push_back(trim(token));
    }

    // A valid student record must have exactly 6 fields: Name,RollNo,FatherName,Caste,District,GPA
    if (tokens.size() != 6) {
        errorMsg = "Invalid record format. Expected 6 comma-separated fields.";
        return false;
    }

    double parsedGpa = 0.0;
    try {
        parsedGpa = std::stod(tokens[5]);
    } catch (...) {
        errorMsg = "Failed to parse GPA value: " + tokens[5];
        return false;
    }

    outStudent = Student(tokens[0], tokens[1], tokens[2], tokens[3], tokens[4], parsedGpa);
    return outStudent.validate(errorMsg);
}
