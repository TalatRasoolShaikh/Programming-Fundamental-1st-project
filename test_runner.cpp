#include <iostream>
#include <cassert>
#include <fstream>
#include <cstdio>
#include "Student.h"
#include "StudentManager.h"

void runTests() {
    const std::string testFile = "test_students.txt";
    std::remove(testFile.c_str());

    std::cout << "[TEST] Running Student Record Management System Tests...\n";

    // Test 1: Validation
    {
        Student s1("", "CS101", "Father", "Caste", "District", 3.5);
        std::string err;
        assert(!s1.validate(err)); // Empty name should fail
        std::cout << "  Passed: Empty name validation check.\n";

        Student s2("John, Doe", "CS101", "Father", "Caste", "District", 3.5);
        assert(!s2.validate(err)); // Comma in name should fail
        std::cout << "  Passed: Comma sanitization check.\n";

        Student s3("John Doe", "CS101", "Father", "Caste", "District", 4.5);
        assert(!s3.validate(err)); // GPA > 4.0 should fail
        std::cout << "  Passed: GPA upper-bound (4.0) check.\n";

        Student s4("John Doe", "CS101", "Father", "Caste", "District", -0.5);
        assert(!s4.validate(err)); // GPA < 0.0 should fail
        std::cout << "  Passed: GPA lower-bound (0.0) check.\n";

        Student s5("John Doe", "CS101", "Father", "Caste", "District", 3.75);
        assert(s5.validate(err)); // Valid student
        std::cout << "  Passed: Valid student data validation.\n";
    }

    // Test 2: CSV Serialization and Parsing
    {
        Student s("Alice Smith", "SE-2023-01", "Robert Smith", "Scholar", "Central", 3.85);
        std::string csv = s.toCSV();
        assert(csv == "Alice Smith,SE-2023-01,Robert Smith,Scholar,Central,3.85");
        std::cout << "  Passed: CSV serialization format matches specification.\n";

        Student parsed;
        std::string err;
        assert(Student::fromCSV(csv, parsed, err));
        assert(parsed.getName() == "Alice Smith");
        assert(parsed.getRollNo() == "SE-2023-01");
        assert(parsed.getFatherName() == "Robert Smith");
        assert(parsed.getCaste() == "Scholar");
        assert(parsed.getDistrict() == "Central");
        assert(parsed.getGpa() == 3.85);
        std::cout << "  Passed: CSV deserialization round-trip.\n";
    }

    // Test 3: StudentManager Add and Duplicate Check
    {
        StudentManager mgr(testFile);
        std::string err;

        Student s1("Alice", "R001", "Bob", "General", "North", 3.5);
        Student s2("Charlie", "R002", "David", "General", "South", 3.8);

        assert(mgr.addStudent(s1, err));
        assert(mgr.addStudent(s2, err));
        assert(mgr.getCount() == 2);
        std::cout << "  Passed: Adding students to storage.\n";

        // Try adding duplicate roll number
        Student sDup("Duplicate Alice", "R001", "Bob", "General", "North", 3.9);
        assert(!mgr.addStudent(sDup, err));
        assert(mgr.getCount() == 2);
        std::cout << "  Passed: Duplicate Roll Number rejection.\n";
    }

    // Test 4: File Persistence across restarts
    {
        StudentManager mgr2(testFile);
        assert(mgr2.getCount() == 2);
        assert(mgr2.rollNumberExists("R001"));
        assert(mgr2.rollNumberExists("R002"));
        std::cout << "  Passed: Persistence loaded correctly from file.\n";

        // Test 5: Next Student Navigation
        Student s;
        int idx = 0, total = 0;
        bool wrapped = false;

        assert(mgr2.getNextStudent(s, idx, total, wrapped));
        assert(s.getRollNo() == "R001");
        assert(idx == 1 && total == 2 && !wrapped);

        assert(mgr2.getNextStudent(s, idx, total, wrapped));
        assert(s.getRollNo() == "R002");
        assert(idx == 2 && total == 2 && !wrapped);

        // Next wraps around
        assert(mgr2.getNextStudent(s, idx, total, wrapped));
        assert(s.getRollNo() == "R001");
        assert(idx == 1 && total == 2 && wrapped);
        std::cout << "  Passed: Next record navigation and wraparound.\n";

        // Test 6: Update Record
        std::string err;
        Student updatedS1("Alice Modified", "R001", "Bob Modified", "General", "East", 3.95);
        assert(mgr2.updateStudent(updatedS1, err));
        std::cout << "  Passed: Record update by Roll Number.\n";

        // Test 7: Delete Record
        assert(mgr2.deleteStudent("R001", err));
        assert(mgr2.getCount() == 1);
        assert(!mgr2.rollNumberExists("R001"));
        assert(mgr2.rollNumberExists("R002"));
        std::cout << "  Passed: Record deletion by Roll Number.\n";
    }

    // Test 8: Verify file content on disk after modifications
    {
        std::ifstream file(testFile.c_str());
        std::string line;
        std::getline(file, line);
        assert(line == "Charlie,R002,David,General,South,3.80");
        file.close();
        std::cout << "  Passed: Verified disk content format.\n";
    }

    std::remove(testFile.c_str());
    std::cout << "\nALL TESTS PASSED SUCCESSFULLY! (8/8)\n";
}

int main() {
    runTests();
    return 0;
}

