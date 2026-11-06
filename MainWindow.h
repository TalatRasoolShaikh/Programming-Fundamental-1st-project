#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#define _WIN32_IE 0x0501
#define _WIN32_WINNT 0x0600

#include <windows.h>
#include <commctrl.h>
#include <string>
#include "StudentManager.h"

class MainWindow {
private:
    HINSTANCE hInstance;
    HWND hwndMain;

    // Form Edit Controls
    HWND hEditName;
    HWND hEditRollNo;
    HWND hEditFatherName;
    HWND hEditCaste;
    HWND hEditDistrict;
    HWND hEditGpa;

    // Action Buttons
    HWND hBtnAdd;
    HWND hBtnUpdate;
    HWND hBtnDelete;
    HWND hBtnNext;
    HWND hBtnClear;

    // Status Label
    HWND hStatusLabel;

    // Fonts & Brushes
    HFONT hFontNormal;
    HFONT hFontBold;
    HFONT hFontTitle;
    HBRUSH hBackgroundBrush;

    // Business Logic Manager
    StudentManager manager;

    // Currently displayed record roll number
    std::string currentLoadedRollNo;

    // Control IDs
    enum ControlId {
        ID_EDIT_NAME = 101,
        ID_EDIT_ROLLNO = 102,
        ID_EDIT_FATHERNAME = 103,
        ID_EDIT_CASTE = 104,
        ID_EDIT_DISTRICT = 105,
        ID_EDIT_GPA = 106,
        ID_BTN_ADD = 201,
        ID_BTN_UPDATE = 202,
        ID_BTN_DELETE = 203,
        ID_BTN_NEXT = 204,
        ID_BTN_CLEAR = 205,
        ID_STATIC_STATUS = 301
    };

    // Internal initialization and UI setup
    void createControls(HWND hwnd);
    void setupFonts();
    void setInitialState();
    void updateStatusBar(const std::string& customStatus = "");

    // Button event handlers
    void onAdd();
    void onUpdate();
    void onDelete();
    void onNext();
    void onClear();

    // Helper functions
    std::string getControlText(HWND hControl) const;
    void setControlText(HWND hControl, const std::string& text);
    void clearFormFields();

public:
    MainWindow();
    ~MainWindow();

    bool initialize(HINSTANCE hInst, int nCmdShow);
    int run();

    // Static Window Procedure
    static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
};

#endif // MAIN_WINDOW_H

