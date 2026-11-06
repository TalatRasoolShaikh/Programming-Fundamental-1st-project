#include "StudentSystemQt.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QMessageBox>
#include <QDoubleValidator>
#include <QFont>

StudentSystemQt::StudentSystemQt(QWidget* parent)
    : QWidget(parent), manager("students.txt"), currentLoadedRollNo("") {
    setWindowTitle("Student Record Management System (Qt Edition)");
    resize(560, 480);

    // Main layout
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(14);
    mainLayout->setContentsMargins(24, 20, 24, 20);

    // Title & Subtitle
    QLabel* lblTitle = new QLabel("STUDENT RECORD MANAGEMENT SYSTEM", this);
    QFont titleFont = lblTitle->font();
    titleFont.setPointSize(15);
    titleFont.setBold(true);
    lblTitle->setFont(titleFont);
    lblTitle->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(lblTitle);

    QLabel* lblSub = new QLabel("Qt Pure C++ GUI & Standard File Stream Storage", this);
    lblSub->setAlignment(Qt::AlignCenter);
    lblSub->setStyleSheet("color: #64748b; font-size: 11pt;");
    mainLayout->addWidget(lblSub);

    // Form Layout
    QGridLayout* formGrid = new QGridLayout();
    formGrid->setHorizontalSpacing(16);
    formGrid->setVerticalSpacing(10);

    editName = new QLineEdit(this);
    editRollNo = new QLineEdit(this);
    editFatherName = new QLineEdit(this);
    editCaste = new QLineEdit(this);
    editDistrict = new QLineEdit(this);
    editGpa = new QLineEdit(this);
    editGpa->setPlaceholderText("0.00 - 4.00");

    QStringList labels = {
        "Student Name:",
        "Roll Number:",
        "Father's Name:",
        "Caste:",
        "District:",
        "GPA (0.00 - 4.00):"
    };

    QLineEdit* edits[6] = {
        editName, editRollNo, editFatherName, editCaste, editDistrict, editGpa
    };

    for (int i = 0; i < 6; ++i) {
        QLabel* lbl = new QLabel(labels[i], this);
        QFont f = lbl->font();
        f.setBold(true);
        lbl->setFont(f);
        formGrid->addWidget(lbl, i, 0);

        edits[i]->setMinimumHeight(30);
        formGrid->addWidget(edits[i], i, 1);
    }

    mainLayout->addLayout(formGrid);

    // Buttons Layout
    QHBoxLayout* btnLayout = new QHBoxLayout();
    btnLayout->setSpacing(10);

    btnAdd = new QPushButton("Add", this);
    btnUpdate = new QPushButton("Update", this);
    btnDelete = new QPushButton("Delete", this);
    btnNext = new QPushButton("Next", this);
    btnClear = new QPushButton("Clear / New", this);

    QPushButton* allBtns[5] = { btnAdd, btnUpdate, btnDelete, btnNext, btnClear };
    for (int i = 0; i < 5; ++i) {
        allBtns[i]->setMinimumHeight(34);
        btnLayout->addWidget(allBtns[i]);
    }

    mainLayout->addLayout(btnLayout);

    // Status Label
    lblStatus = new QLabel("Ready.", this);
    lblStatus->setAlignment(Qt::AlignCenter);
    lblStatus->setStyleSheet("color: #475569; font-weight: 500;");
    mainLayout->addWidget(lblStatus);

    // Connect signals & slots
    connect(btnAdd, &QPushButton::clicked, this, &StudentSystemQt::onAddClicked);
    connect(btnUpdate, &QPushButton::clicked, this, &StudentSystemQt::onUpdateClicked);
    connect(btnDelete, &QPushButton::clicked, this, &StudentSystemQt::onDeleteClicked);
    connect(btnNext, &QPushButton::clicked, this, &StudentSystemQt::onNextClicked);
    connect(btnClear, &QPushButton::clicked, this, &StudentSystemQt::onClearClicked);

    // Set initial state
    setInitialState();
}

void StudentSystemQt::setInitialState() {
    // STRICT REQUIREMENT 3:
    // Startup state: Update & Delete DISABLED, Add ENABLED
    btnAdd->setEnabled(true);
    btnUpdate->setEnabled(false);
    btnDelete->setEnabled(false);
    currentLoadedRollNo.clear();
    manager.resetNavigation();
    updateStatus("Ready. Mode: Add New Record");
}

void StudentSystemQt::clearFields() {
    editName->clear();
    editRollNo->clear();
    editFatherName->clear();
    editCaste->clear();
    editDistrict->clear();
    editGpa->clear();
}

void StudentSystemQt::updateStatus(const QString& extra) {
    QString status = extra;
    if (status.isEmpty()) {
        status = QString("Total Records: %1").arg(manager.getCount());
    } else {
        status += QString("  [Total: %1]").arg(manager.getCount());
    }
    lblStatus->setText(status);
}

void StudentSystemQt::onAddClicked() {
    std::string name = editName->text().trimmed().toStdString();
    std::string rollNo = editRollNo->text().trimmed().toStdString();
    std::string fatherName = editFatherName->text().trimmed().toStdString();
    std::string caste = editCaste->text().trimmed().toStdString();
    std::string district = editDistrict->text().trimmed().toStdString();
    std::string gpaStr = editGpa->text().trimmed().toStdString();

    if (name.empty() || rollNo.empty() || fatherName.empty() ||
        caste.empty() || district.empty() || gpaStr.empty()) {
        QMessageBox::warning(this, "Validation Error", "All fields are required. Please fill in all fields.");
        return;
    }

    double gpa = 0.0;
    try {
        gpa = std::stod(gpaStr);
    } catch (...) {
        QMessageBox::warning(this, "Validation Error", "Please enter a valid numeric GPA (e.g. 3.45).");
        return;
    }

    Student s(name, rollNo, fatherName, caste, district, gpa);
    std::string err;
    if (!manager.addStudent(s, err)) {
        QMessageBox::critical(this, "Error Adding Record", QString::fromStdString(err));
        return;
    }

    // Confirmation message
    QMessageBox::information(this, "Record Added",
        QString("Student record for '%1' (Roll No: %2) was saved to students.txt!").arg(QString::fromStdString(name), QString::fromStdString(rollNo)));

    clearFields();
    setInitialState();
    editName->setFocus();
}

void StudentSystemQt::onNextClicked() {
    Student s;
    int index = 0, total = 0;
    bool wrapped = false;

    if (!manager.getNextStudent(s, index, total, wrapped)) {
        QMessageBox::information(this, "No Records",
            "No student records found in 'students.txt'.\nPlease add a record first.");
        return;
    }

    editName->setText(QString::fromStdString(s.getName()));
    editRollNo->setText(QString::fromStdString(s.getRollNo()));
    editFatherName->setText(QString::fromStdString(s.getFatherName()));
    editCaste->setText(QString::fromStdString(s.getCaste()));
    editDistrict->setText(QString::fromStdString(s.getDistrict()));
    editGpa->setText(QString::number(s.getGpa(), 'f', 2));

    currentLoadedRollNo = s.getRollNo();

    // STRICT REQUIREMENT 3:
    // When 'Next' is clicked: Add DISABLED, Update & Delete ENABLED
    btnAdd->setEnabled(false);
    btnUpdate->setEnabled(true);
    btnDelete->setEnabled(true);

    QString msg = QString("Viewing Record %1 of %2").arg(index).arg(total);
    if (wrapped) msg += " (Cycled to first record)";
    updateStatus(msg);
}

void StudentSystemQt::onUpdateClicked() {
    if (currentLoadedRollNo.empty()) {
        QMessageBox::warning(this, "Update Notice", "Please load a record using 'Next' first.");
        return;
    }

    std::string name = editName->text().trimmed().toStdString();
    std::string rollNo = editRollNo->text().trimmed().toStdString();
    std::string fatherName = editFatherName->text().trimmed().toStdString();
    std::string caste = editCaste->text().trimmed().toStdString();
    std::string district = editDistrict->text().trimmed().toStdString();
    std::string gpaStr = editGpa->text().trimmed().toStdString();

    if (name.empty() || rollNo.empty() || fatherName.empty() ||
        caste.empty() || district.empty() || gpaStr.empty()) {
        QMessageBox::warning(this, "Validation Error", "All fields are required for update.");
        return;
    }

    double gpa = 0.0;
    try {
        gpa = std::stod(gpaStr);
    } catch (...) {
        QMessageBox::warning(this, "Validation Error", "Please enter a valid numeric GPA.");
        return;
    }

    Student updated(name, rollNo, fatherName, caste, district, gpa);
    std::string err;
    bool ok = false;

    if (rollNo == currentLoadedRollNo) {
        ok = manager.updateStudent(updated, err);
    } else {
        if (manager.rollNumberExists(rollNo)) {
            QMessageBox::critical(this, "Roll Number Conflict",
                QString("Roll Number '%1' is already in use by another student.").arg(QString::fromStdString(rollNo)));
            return;
        }
        if (manager.deleteStudent(currentLoadedRollNo, err)) {
            ok = manager.addStudent(updated, err);
            if (ok) currentLoadedRollNo = rollNo;
        }
    }

    if (!ok) {
        QMessageBox::critical(this, "Update Failed", QString::fromStdString(err));
        return;
    }

    QMessageBox::information(this, "Record Updated",
        QString("Student record for Roll No '%1' was successfully updated!").arg(QString::fromStdString(rollNo)));
    updateStatus(QString("Updated Roll No: %1").arg(QString::fromStdString(rollNo)));
}

void StudentSystemQt::onDeleteClicked() {
    if (currentLoadedRollNo.empty()) {
        QMessageBox::warning(this, "Delete Notice", "Please load a record using 'Next' first.");
        return;
    }

    auto reply = QMessageBox::question(this, "Confirm Deletion",
        QString("Are you sure you want to permanently delete student with Roll Number '%1'?")
            .arg(QString::fromStdString(currentLoadedRollNo)),
        QMessageBox::Yes | QMessageBox::No);

    if (reply != QMessageBox::Yes) {
        return;
    }

    std::string err;
    if (!manager.deleteStudent(currentLoadedRollNo, err)) {
        QMessageBox::critical(this, "Delete Failed", QString::fromStdString(err));
        return;
    }

    QMessageBox::information(this, "Record Deleted",
        QString("Student record for '%1' was deleted successfully.").arg(QString::fromStdString(currentLoadedRollNo)));

    clearFields();
    setInitialState();
}

void StudentSystemQt::onClearClicked() {
    clearFields();
    setInitialState();
    editName->setFocus();
}

