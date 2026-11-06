#ifndef STUDENTSYSTEMQT_H
#define STUDENTSYSTEMQT_H

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include "StudentManager.h"

class StudentSystemQt : public QWidget {
    Q_OBJECT

private:
    // Input Fields
    QLineEdit* editName;
    QLineEdit* editRollNo;
    QLineEdit* editFatherName;
    QLineEdit* editCaste;
    QLineEdit* editDistrict;
    QLineEdit* editGpa;

    // Action Buttons
    QPushButton* btnAdd;
    QPushButton* btnUpdate;
    QPushButton* btnDelete;
    QPushButton* btnNext;
    QPushButton* btnClear;

    // Status Label
    QLabel* lblStatus;

    // Business Logic
    StudentManager manager;
    std::string currentLoadedRollNo;

    void setInitialState();
    void clearFields();
    void updateStatus(const QString& extra = "");

private slots:
    void onAddClicked();
    void onUpdateClicked();
    void onDeleteClicked();
    void onNextClicked();
    void onClearClicked();

public:
    explicit StudentSystemQt(QWidget* parent = nullptr);
    ~StudentSystemQt() override = default;
};

#endif // STUDENTSYSTEMQT_H

