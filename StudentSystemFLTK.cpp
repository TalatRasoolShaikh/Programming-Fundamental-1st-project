/**
 * @file StudentSystemFLTK.cpp
 * @brief Complete FLTK GUI implementation of the Student Record Management System.
 * 
 * To compile with FLTK on MinGW / Linux / macOS:
 *   fltk-config --compile StudentSystemFLTK.cpp Student.cpp StudentManager.cpp
 * or:
 *   g++ -std=c++11 StudentSystemFLTK.cpp Student.cpp StudentManager.cpp -o StudentSystemFLTK -lfltk -lole32 -luuid -lcomctl32
 */

#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Input.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Box.H>
#include <FL/fl_ask.H>
#include <cstdio>
#include <string>
#include "StudentManager.h"

class StudentSystemFLTK {
public:
    Fl_Window* win;
    Fl_Box* lblTitle;
    Fl_Box* lblSubtitle;

    Fl_Input* inpName;
    Fl_Input* inpRollNo;
    Fl_Input* inpFatherName;
    Fl_Input* inpCaste;
    Fl_Input* inpDistrict;
    Fl_Input* inpGpa;

    Fl_Button* btnAdd;
    Fl_Button* btnUpdate;
    Fl_Button* btnDelete;
    Fl_Button* btnNext;
    Fl_Button* btnClear;

    Fl_Box* lblStatus;

    StudentManager manager;
    std::string currentLoadedRollNo;

    StudentSystemFLTK() : manager("students.txt"), currentLoadedRollNo("") {
        win = new Fl_Window(560, 480, "Student Record Management System (FLTK Edition)");

        lblTitle = new Fl_Box(20, 16, 520, 26, "STUDENT RECORD MANAGEMENT SYSTEM");
        lblTitle->labelfont(FL_HELVETICA_BOLD);
        lblTitle->labelsize(16);

        lblSubtitle = new Fl_Box(20, 42, 520, 20, "Pure C++ GUI with Standard File Stream Storage");
        lblSubtitle->labelsize(11);

        const int labelX = 180;
        const int fieldW = 320;
        const int fieldH = 26;
        const int startY = 76;
        const int spacingY = 38;

        inpName = new Fl_Input(labelX, startY, fieldW, fieldH, "Student Name:");
        inpRollNo = new Fl_Input(labelX, startY + spacingY, fieldW, fieldH, "Roll Number:");
        inpFatherName = new Fl_Input(labelX, startY + 2 * spacingY, fieldW, fieldH, "Father's Name:");
        inpCaste = new Fl_Input(labelX, startY + 3 * spacingY, fieldW, fieldH, "Caste:");
        inpDistrict = new Fl_Input(labelX, startY + 4 * spacingY, fieldW, fieldH, "District:");
        inpGpa = new Fl_Input(labelX, startY + 5 * spacingY, fieldW, fieldH, "GPA (0.00-4.00):");

        const int btnY = startY + 6 * spacingY + 14;
        const int btnW = 86;
        const int btnH = 34;
        const int btnSpacing = 8;
        const int btnStartX = (560 - (5 * btnW + 4 * btnSpacing)) / 2;

        btnAdd = new Fl_Button(btnStartX, btnY, btnW, btnH, "Add");
        btnUpdate = new Fl_Button(btnStartX + (btnW + btnSpacing), btnY, btnW, btnH, "Update");
        btnDelete = new Fl_Button(btnStartX + 2 * (btnW + btnSpacing), btnY, btnW, btnH, "Delete");
        btnNext = new Fl_Button(btnStartX + 3 * (btnW + btnSpacing), btnY, btnW, btnH, "Next");
        btnClear = new Fl_Button(btnStartX + 4 * (btnW + btnSpacing), btnY, btnW, btnH, "Clear / New");

        lblStatus = new Fl_Box(20, btnY + btnH + 18, 520, 22, "Status: Ready");
        lblStatus->labelsize(12);

        // Callbacks
        btnAdd->callback(cb_add, this);
        btnUpdate->callback(cb_update, this);
        btnDelete->callback(cb_delete, this);
        btnNext->callback(cb_next, this);
        btnClear->callback(cb_clear, this);

        win->end();

        // STRICT REQUIREMENT 3: Startup state
        setInitialState();
    }

    void setInitialState() {
        btnAdd->activate();
        btnUpdate->deactivate();
        btnDelete->deactivate();
        currentLoadedRollNo.clear();
        manager.resetNavigation();
        updateStatus("Ready. Mode: Add New Record");
    }

    void clearFields() {
        inpName->value("");
        inpRollNo->value("");
        inpFatherName->value("");
        inpCaste->value("");
        inpDistrict->value("");
        inpGpa->value("");
    }

    void updateStatus(const std::string& custom) {
        static std::string buffer;
        buffer = custom + "  [Total: " + std::to_string(manager.getCount()) + "]";
        lblStatus->copy_label(buffer.c_str());
    }

    void onAdd() {
        std::string name = inpName->value();
        std::string rollNo = inpRollNo->value();
        std::string fatherName = inpFatherName->value();
        std::string caste = inpCaste->value();
        std::string district = inpDistrict->value();
        std::string gpaStr = inpGpa->value();

        if (name.empty() || rollNo.empty() || fatherName.empty() ||
            caste.empty() || district.empty() || gpaStr.empty()) {
            fl_alert("Please fill in all fields before clicking 'Add'.");
            return;
        }

        double gpa = 0.0;
        try {
            gpa = std::stod(gpaStr);
        } catch (...) {
            fl_alert("Please enter a valid numeric GPA (0.00 - 4.00).");
            return;
        }

        Student s(name, rollNo, fatherName, caste, district, gpa);
        std::string err;
        if (!manager.addStudent(s, err)) {
            fl_alert("%s", err.c_str());
            return;
        }

        fl_message("Student record for '%s' (Roll No: %s) added successfully!", name.c_str(), rollNo.c_str());
        clearFields();
        setInitialState();
    }

    void onNext() {
        Student s;
        int index = 0, total = 0;
        bool wrapped = false;

        if (!manager.getNextStudent(s, index, total, wrapped)) {
            fl_alert("No student records found in 'students.txt'.\nPlease add records first.");
            return;
        }

        inpName->value(s.getName().c_str());
        inpRollNo->value(s.getRollNo().c_str());
        inpFatherName->value(s.getFatherName().c_str());
        inpCaste->value(s.getCaste().c_str());
        inpDistrict->value(s.getDistrict().c_str());

        char gpaBuf[32];
        snprintf(gpaBuf, sizeof(gpaBuf), "%.2f", s.getGpa());
        inpGpa->value(gpaBuf);

        currentLoadedRollNo = s.getRollNo();

        // STRICT REQUIREMENT 3: When Next is clicked: Add DISABLED, Update & Delete ENABLED
        btnAdd->deactivate();
        btnUpdate->activate();
        btnDelete->activate();

        std::string msg = "Record " + std::to_string(index) + " of " + std::to_string(total);
        if (wrapped) msg += " (Cycled to beginning)";
        updateStatus(msg);
    }

    void onUpdate() {
        if (currentLoadedRollNo.empty()) {
            fl_alert("No record is loaded. Click 'Next' to load a record first.");
            return;
        }

        std::string name = inpName->value();
        std::string rollNo = inpRollNo->value();
        std::string fatherName = inpFatherName->value();
        std::string caste = inpCaste->value();
        std::string district = inpDistrict->value();
        std::string gpaStr = inpGpa->value();

        if (name.empty() || rollNo.empty() || fatherName.empty() ||
            caste.empty() || district.empty() || gpaStr.empty()) {
            fl_alert("All fields are required for update.");
            return;
        }

        double gpa = 0.0;
        try {
            gpa = std::stod(gpaStr);
        } catch (...) {
            fl_alert("Invalid GPA value.");
            return;
        }

        Student updated(name, rollNo, fatherName, caste, district, gpa);
        std::string err;
        bool ok = false;

        if (rollNo == currentLoadedRollNo) {
            ok = manager.updateStudent(updated, err);
        } else {
            if (manager.rollNumberExists(rollNo)) {
                fl_alert("Roll Number '%s' is already in use.", rollNo.c_str());
                return;
            }
            if (manager.deleteStudent(currentLoadedRollNo, err)) {
                ok = manager.addStudent(updated, err);
                if (ok) currentLoadedRollNo = rollNo;
            }
        }

        if (!ok) {
            fl_alert("%s", err.c_str());
            return;
        }

        fl_message("Student record updated successfully!");
        updateStatus("Record " + rollNo + " updated.");
    }

    void onDelete() {
        if (currentLoadedRollNo.empty()) {
            fl_alert("No record loaded. Click 'Next' first.");
            return;
        }

        if (fl_choice("Are you sure you want to delete student '%s'?", "No", "Yes", 0, currentLoadedRollNo.c_str()) != 1) {
            return;
        }

        std::string err;
        if (!manager.deleteStudent(currentLoadedRollNo, err)) {
            fl_alert("%s", err.c_str());
            return;
        }

        fl_message("Record deleted successfully.");
        clearFields();
        setInitialState();
    }

    void onClear() {
        clearFields();
        setInitialState();
    }

    static void cb_add(Fl_Widget*, void* data) { static_cast<StudentSystemFLTK*>(data)->onAdd(); }
    static void cb_update(Fl_Widget*, void* data) { static_cast<StudentSystemFLTK*>(data)->onUpdate(); }
    static void cb_delete(Fl_Widget*, void* data) { static_cast<StudentSystemFLTK*>(data)->onDelete(); }
    static void cb_next(Fl_Widget*, void* data) { static_cast<StudentSystemFLTK*>(data)->onNext(); }
    static void cb_clear(Fl_Widget*, void* data) { static_cast<StudentSystemFLTK*>(data)->onClear(); }
};

int main(int argc, char** argv) {
    StudentSystemFLTK app;
    app.win->show(argc, argv);
    return Fl::run();
}

