#ifndef STUDENT_H
#define STUDENT_H

#include <string>

/**
 * @brief Represents a Student entity with personal and academic details.
 */
class Student {
private:
    std::string name;
    std::string rollNo;
    std::string fatherName;
    std::string caste;
    std::string district;
    double gpa;

public:
    // Constructors
    Student();
    Student(const std::string& name,
            const std::string& rollNo,
            const std::string& fatherName,
            const std::string& caste,
            const std::string& district,
            double gpa);

    // Getters
    std::string getName() const;
    std::string getRollNo() const;
    std::string getFatherName() const;
    std::string getCaste() const;
    std::string getDistrict() const;
    double getGpa() const;

    // Setters
    void setName(const std::string& name);
    void setRollNo(const std::string& rollNo);
    void setFatherName(const std::string& fatherName);
    void setCaste(const std::string& caste);
    void setDistrict(const std::string& district);
    void setGpa(double gpa);

    // Data validation
    bool validate(std::string& errorMsg) const;

    // CSV Serialization & Deserialization
    std::string toCSV() const;
    static bool fromCSV(const std::string& csvLine, Student& outStudent, std::string& errorMsg);

    // Helper utilities
    static std::string trim(const std::string& str);
};

#endif // STUDENT_H

