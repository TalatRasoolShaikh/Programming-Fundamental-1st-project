#include "MainWindow.h"
#include <cstdio>
#include <stdexcept>

MainWindow::MainWindow()
    : hInstance(NULL),
      hwndMain(NULL),
      hEditName(NULL),
      hEditRollNo(NULL),
      hEditFatherName(NULL),
      hEditCaste(NULL),
      hEditDistrict(NULL),
      hEditGpa(NULL),
      hBtnAdd(NULL),
      hBtnUpdate(NULL),
      hBtnDelete(NULL),
      hBtnNext(NULL),
      hBtnClear(NULL),
      hStatusLabel(NULL),
      hFontNormal(NULL),
      hFontBold(NULL),
      hFontTitle(NULL),
      hBackgroundBrush(NULL),
      manager("students.txt"),
      currentLoadedRollNo("") {
    hBackgroundBrush = CreateSolidBrush(RGB(245, 247, 250));
}

MainWindow::~MainWindow() {
    if (hFontNormal) DeleteObject(hFontNormal);
    if (hFontBold) DeleteObject(hFontBold);
    if (hFontTitle) DeleteObject(hFontTitle);
    if (hBackgroundBrush) DeleteObject(hBackgroundBrush);
}

bool MainWindow::initialize(HINSTANCE hInst, int nCmdShow) {
    hInstance = hInst;

    // Initialize Common Controls
    INITCOMMONCONTROLSEX icex;
    icex.dwSize = sizeof(INITCOMMONCONTROLSEX);
    icex.dwICC = ICC_WIN95_CLASSES | ICC_STANDARD_CLASSES;
    InitCommonControlsEx(&icex);

    const char CLASS_NAME[] = "StudentRecordManagementSystemClass";

    WNDCLASSEX wc = {};
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = MainWindow::WindowProc;
    wc.hInstance = hInstance;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = hBackgroundBrush;
    wc.lpszClassName = CLASS_NAME;
    wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

    if (!RegisterClassEx(&wc)) {
        MessageBox(NULL, "Window Registration Failed!", "Error", MB_ICONEXCLAMATION | MB_OK);
        return false;
    }

    // Window dimensions
    int winWidth = 560;
    int winHeight = 500;

    // Center window on screen
    int screenWidth = GetSystemMetrics(SM_CXSCREEN);
    int screenHeight = GetSystemMetrics(SM_CYSCREEN);
    int posX = (screenWidth - winWidth) / 2;
    int posY = (screenHeight - winHeight) / 2;

    hwndMain = CreateWindowEx(
        WS_EX_CLIENTEDGE,
        CLASS_NAME,
        "Student Record Management System",
        WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
        posX, posY, winWidth, winHeight,
        NULL,
        NULL,
        hInstance,
        this
    );

    if (hwndMain == NULL) {
        MessageBox(NULL, "Window Creation Failed!", "Error", MB_ICONEXCLAMATION | MB_OK);
        return false;
    }

    ShowWindow(hwndMain, nCmdShow);
    UpdateWindow(hwndMain);

    return true;
}

void MainWindow::setupFonts() {
    hFontTitle = CreateFont(
        22, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
        CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "Segoe UI");

    hFontBold = CreateFont(
        16, 0, 0, 0, FW_SEMIBOLD, FALSE, FALSE, FALSE,
        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
        CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "Segoe UI");

    hFontNormal = CreateFont(
        15, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
        CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "Segoe UI");
}

void MainWindow::createControls(HWND hwnd) {
    // 1. Header Banner
    HWND hTitle = CreateWindowEx(
        0, "STATIC", "STUDENT RECORD MANAGEMENT SYSTEM",
        WS_CHILD | WS_VISIBLE | SS_CENTER,
        20, 16, 500, 26,
        hwnd, NULL, hInstance, NULL
    );
    SendMessage(hTitle, WM_SETFONT, (WPARAM)hFontTitle, TRUE);

    HWND hSubtitle = CreateWindowEx(
        0, "STATIC", "Pure C++ Desktop Application with Standard File Stream Storage",
        WS_CHILD | WS_VISIBLE | SS_CENTER,
        20, 44, 500, 20,
        hwnd, NULL, hInstance, NULL
    );
    SendMessage(hSubtitle, WM_SETFONT, (WPARAM)hFontNormal, TRUE);

    // Form fields configuration
    const int labelX = 45;
    const int labelW = 120;
    const int editX = 175;
    const int editW = 320;
    const int fieldH = 26;
    const int startY = 78;
    const int spacingY = 38;

    struct FieldDef {
        const char* labelText;
        int editId;
        HWND* pEditHwnd;
    };

    FieldDef fields[6] = {
        { "Student Name:",        ID_EDIT_NAME,       &hEditName },
        { "Roll Number:",         ID_EDIT_ROLLNO,     &hEditRollNo },
        { "Father's Name:",       ID_EDIT_FATHERNAME, &hEditFatherName },
        { "Caste:",               ID_EDIT_CASTE,      &hEditCaste },
        { "District:",            ID_EDIT_DISTRICT,   &hEditDistrict },
        { "GPA (0.00 - 4.00):",   ID_EDIT_GPA,        &hEditGpa }
    };

    for (int i = 0; i < 6; ++i) {
        int currentY = startY + (i * spacingY);

        HWND hLabel = CreateWindowEx(
            0, "STATIC", fields[i].labelText,
            WS_CHILD | WS_VISIBLE | SS_LEFT,
            labelX, currentY + 3, labelW, fieldH,
            hwnd, NULL, hInstance, NULL
        );
        SendMessage(hLabel, WM_SETFONT, (WPARAM)hFontBold, TRUE);

        HWND hEdit = CreateWindowEx(
            WS_EX_CLIENTEDGE, "EDIT", "",
            WS_CHILD | WS_VISIBLE | WS_TABSTOP | ES_AUTOHSCROLL,
            editX, currentY, editW, fieldH,
            hwnd, (HMENU)(INT_PTR)fields[i].editId, hInstance, NULL
        );
        SendMessage(hEdit, WM_SETFONT, (WPARAM)hFontNormal, TRUE);
        *fields[i].pEditHwnd = hEdit;
    }

    // 2. Action Buttons
    const int btnY = startY + (6 * spacingY) + 12;
    const int btnW = 86;
    const int btnH = 34;
    const int btnSpacing = 8;
    const int totalBtnW = (5 * btnW) + (4 * btnSpacing);
    const int btnStartX = (540 - totalBtnW) / 2;

    // Add Button
    hBtnAdd = CreateWindowEx(
        0, "BUTTON", "Add",
        WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_PUSHBUTTON,
        btnStartX, btnY, btnW, btnH,
        hwnd, (HMENU)ID_BTN_ADD, hInstance, NULL
    );
    SendMessage(hBtnAdd, WM_SETFONT, (WPARAM)hFontBold, TRUE);

    // Update Button
    hBtnUpdate = CreateWindowEx(
        0, "BUTTON", "Update",
        WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_PUSHBUTTON,
        btnStartX + (btnW + btnSpacing), btnY, btnW, btnH,
        hwnd, (HMENU)ID_BTN_UPDATE, hInstance, NULL
    );
    SendMessage(hBtnUpdate, WM_SETFONT, (WPARAM)hFontBold, TRUE);

    // Delete Button
    hBtnDelete = CreateWindowEx(
        0, "BUTTON", "Delete",
        WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_PUSHBUTTON,
        btnStartX + 2 * (btnW + btnSpacing), btnY, btnW, btnH,
        hwnd, (HMENU)ID_BTN_DELETE, hInstance, NULL
    );
    SendMessage(hBtnDelete, WM_SETFONT, (WPARAM)hFontBold, TRUE);

    // Next Button
    hBtnNext = CreateWindowEx(
        0, "BUTTON", "Next",
        WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_PUSHBUTTON,
        btnStartX + 3 * (btnW + btnSpacing), btnY, btnW, btnH,
        hwnd, (HMENU)ID_BTN_NEXT, hInstance, NULL
    );
    SendMessage(hBtnNext, WM_SETFONT, (WPARAM)hFontBold, TRUE);

    // Clear / New Button
    hBtnClear = CreateWindowEx(
        0, "BUTTON", "Clear / New",
        WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_PUSHBUTTON,
        btnStartX + 4 * (btnW + btnSpacing), btnY, btnW, btnH,
        hwnd, (HMENU)ID_BTN_CLEAR, hInstance, NULL
    );
    SendMessage(hBtnClear, WM_SETFONT, (WPARAM)hFontNormal, TRUE);

    // 3. Status Display Label
    const int statusY = btnY + btnH + 18;
    hStatusLabel = CreateWindowEx(
        0, "STATIC", "Status: Ready",
        WS_CHILD | WS_VISIBLE | SS_CENTER,
        20, statusY, 500, 22,
        hwnd, (HMENU)ID_STATIC_STATUS, hInstance, NULL
    );
    SendMessage(hStatusLabel, WM_SETFONT, (WPARAM)hFontNormal, TRUE);
}

void MainWindow::setInitialState() {
    // STRICT REQUIREMENT 3:
    // On Application Startup:
    // - The 'Update' and 'Delete' buttons must be DISABLED.
    // - The 'Add' button must be ENABLED.
    EnableWindow(hBtnAdd, TRUE);
    EnableWindow(hBtnUpdate, FALSE);
    EnableWindow(hBtnDelete, FALSE);
    EnableWindow(hBtnNext, TRUE);
    EnableWindow(hBtnClear, TRUE);

    currentLoadedRollNo.clear();
    manager.resetNavigation();
    updateStatusBar("Ready. Mode: Add New Record");
}

void MainWindow::updateStatusBar(const std::string& customStatus) {
    if (!hStatusLabel) return;

    size_t count = manager.getCount();
    std::string text = customStatus;
    if (text.empty()) {
        text = "Total Records: " + std::to_string(count) + " | Storage: " + manager.getFilePath();
    } else {
        text = text + "  [Total: " + std::to_string(count) + "]";
    }

    SetWindowText(hStatusLabel, text.c_str());
}

std::string MainWindow::getControlText(HWND hControl) const {
    int length = GetWindowTextLength(hControl);
    if (length <= 0) return "";
    std::string text(length + 1, '\0');
    GetWindowText(hControl, &text[0], length + 1);
    text.resize(length);
    return Student::trim(text);
}

void MainWindow::setControlText(HWND hControl, const std::string& text) {
    SetWindowText(hControl, text.c_str());
}

void MainWindow::clearFormFields() {
    setControlText(hEditName, "");
    setControlText(hEditRollNo, "");
    setControlText(hEditFatherName, "");
    setControlText(hEditCaste, "");
    setControlText(hEditDistrict, "");
    setControlText(hEditGpa, "");
}

void MainWindow::onAdd() {
    std::string name = getControlText(hEditName);
    std::string rollNo = getControlText(hEditRollNo);
    std::string fatherName = getControlText(hEditFatherName);
    std::string caste = getControlText(hEditCaste);
    std::string district = getControlText(hEditDistrict);
    std::string gpaStr = getControlText(hEditGpa);

    if (name.empty() || rollNo.empty() || fatherName.empty() ||
        caste.empty() || district.empty() || gpaStr.empty()) {
        MessageBox(hwndMain,
            "Please fill in all fields before clicking 'Add'.",
            "Missing Information", MB_ICONWARNING | MB_OK);
        return;
    }

    double gpa = 0.0;
    try {
        gpa = std::stod(gpaStr);
    } catch (...) {
        MessageBox(hwndMain,
            "Please enter a valid numeric value for GPA (e.g., 3.50).",
            "Invalid GPA", MB_ICONWARNING | MB_OK);
        return;
    }

    Student s(name, rollNo, fatherName, caste, district, gpa);
    std::string errorMsg;
    if (!manager.addStudent(s, errorMsg)) {
        MessageBox(hwndMain, errorMsg.c_str(), "Add Record Error", MB_ICONERROR | MB_OK);
        return;
    }

    // STRICT REQUIREMENT 2: Add shows a confirmation message.
    MessageBox(hwndMain,
        ("Student record for '" + name + "' (Roll No: " + rollNo + ") was successfully added to students.txt!").c_str(),
        "Record Added", MB_ICONINFORMATION | MB_OK);

    clearFormFields();
    setInitialState();
    SetFocus(hEditName);
}

void MainWindow::onNext() {
    Student s;
    int index = 0;
    int total = 0;
    bool wrapped = false;

    if (!manager.getNextStudent(s, index, total, wrapped)) {
        MessageBox(hwndMain,
            "No student records found in 'students.txt'.\nPlease enter details and click 'Add' to create your first record.",
            "Record Navigation", MB_ICONINFORMATION | MB_OK);
        return;
    }

    // Display student record details in the text fields
    setControlText(hEditName, s.getName());
    setControlText(hEditRollNo, s.getRollNo());
    setControlText(hEditFatherName, s.getFatherName());
    setControlText(hEditCaste, s.getCaste());
    setControlText(hEditDistrict, s.getDistrict());

    char gpaFormatted[32];
    snprintf(gpaFormatted, sizeof(gpaFormatted), "%.2f", s.getGpa());
    setControlText(hEditGpa, gpaFormatted);

    currentLoadedRollNo = s.getRollNo();

    // STRICT REQUIREMENT 3:
    // When the 'Next' Button is Clicked:
    // - The 'Add' button must become DISABLED.
    // - The 'Update' and 'Delete' buttons must become ENABLED so the displayed record can be modified or deleted.
    EnableWindow(hBtnAdd, FALSE);
    EnableWindow(hBtnUpdate, TRUE);
    EnableWindow(hBtnDelete, TRUE);

    std::string msg = "Viewing Record " + std::to_string(index) + " of " + std::to_string(total);
    if (wrapped) {
        msg += " (Cycled to first record)";
    }
    updateStatusBar(msg);
}

void MainWindow::onUpdate() {
    if (currentLoadedRollNo.empty()) {
        MessageBox(hwndMain,
            "No record is currently selected for update.\nClick 'Next' to browse to the record you want to modify.",
            "Update Notice", MB_ICONWARNING | MB_OK);
        return;
    }

    std::string name = getControlText(hEditName);
    std::string rollNo = getControlText(hEditRollNo);
    std::string fatherName = getControlText(hEditFatherName);
    std::string caste = getControlText(hEditCaste);
    std::string district = getControlText(hEditDistrict);
    std::string gpaStr = getControlText(hEditGpa);

    if (name.empty() || rollNo.empty() || fatherName.empty() ||
        caste.empty() || district.empty() || gpaStr.empty()) {
        MessageBox(hwndMain,
            "All fields must be filled out to update this record.",
            "Validation Error", MB_ICONWARNING | MB_OK);
        return;
    }

    double gpa = 0.0;
    try {
        gpa = std::stod(gpaStr);
    } catch (...) {
        MessageBox(hwndMain,
            "Please enter a valid numeric value for GPA (e.g., 3.50).",
            "Invalid GPA", MB_ICONWARNING | MB_OK);
        return;
    }

    // Check if the user modified the Roll Number (unique identifier)
    if (rollNo != currentLoadedRollNo) {
        if (manager.rollNumberExists(rollNo)) {
            MessageBox(hwndMain,
                ("Cannot change Roll Number to '" + rollNo + "'. A record with this Roll Number already exists.").c_str(),
                "Roll Number Conflict", MB_ICONERROR | MB_OK);
            return;
        }

        int choice = MessageBox(hwndMain,
            ("You changed the Roll Number from '" + currentLoadedRollNo + "' to '" + rollNo + "'.\nDo you want to apply this change?").c_str(),
            "Confirm Roll Number Change", MB_ICONQUESTION | MB_YESNO);
        if (choice != IDYES) {
            return;
        }
    }

    Student updatedStudent(name, rollNo, fatherName, caste, district, gpa);
    std::string errorMsg;
    bool success = false;

    if (rollNo == currentLoadedRollNo) {
        success = manager.updateStudent(updatedStudent, errorMsg);
    } else {
        // Roll number changed: delete old key and add new
        if (manager.deleteStudent(currentLoadedRollNo, errorMsg)) {
            success = manager.addStudent(updatedStudent, errorMsg);
            if (success) {
                currentLoadedRollNo = rollNo;
            }
        }
    }

    if (!success) {
        MessageBox(hwndMain, errorMsg.c_str(), "Update Failed", MB_ICONERROR | MB_OK);
        return;
    }

    // STRICT REQUIREMENT 2: Update confirmation
    MessageBox(hwndMain,
        ("Student record for Roll Number '" + rollNo + "' was successfully updated in students.txt!").c_str(),
        "Record Updated", MB_ICONINFORMATION | MB_OK);

    updateStatusBar("Record " + rollNo + " updated successfully.");
}

void MainWindow::onDelete() {
    if (currentLoadedRollNo.empty()) {
        MessageBox(hwndMain,
            "No record is currently selected for deletion.\nClick 'Next' to browse to the record you want to delete.",
            "Delete Notice", MB_ICONWARNING | MB_OK);
        return;
    }

    std::string confirmMsg = "Are you sure you want to permanently delete the student record for Roll Number '"
                           + currentLoadedRollNo + "' from students.txt?";
    int choice = MessageBox(hwndMain, confirmMsg.c_str(), "Confirm Record Deletion", MB_ICONQUESTION | MB_YESNO | MB_DEFBUTTON2);
    if (choice != IDYES) {
        return;
    }

    std::string errorMsg;
    if (!manager.deleteStudent(currentLoadedRollNo, errorMsg)) {
        MessageBox(hwndMain, errorMsg.c_str(), "Delete Failed", MB_ICONERROR | MB_OK);
        return;
    }

    // STRICT REQUIREMENT 2: Delete removes record and confirms
    MessageBox(hwndMain,
        ("Student record for Roll Number '" + currentLoadedRollNo + "' was successfully deleted from students.txt.").c_str(),
        "Record Deleted", MB_ICONINFORMATION | MB_OK);

    clearFormFields();
    setInitialState();
}

void MainWindow::onClear() {
    clearFormFields();
    setInitialState();
    SetFocus(hEditName);
}

int MainWindow::run() {
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        // Support keyboard tab navigation between dialog controls
        if (!IsDialogMessage(hwndMain, &msg)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }
    return (int)msg.wParam;
}

LRESULT CALLBACK MainWindow::WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    MainWindow* pThis = NULL;

    if (uMsg == WM_CREATE) {
        CREATESTRUCT* pCreate = reinterpret_cast<CREATESTRUCT*>(lParam);
        pThis = reinterpret_cast<MainWindow*>(pCreate->lpCreateParams);
        SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pThis));
        pThis->hwndMain = hwnd;
        pThis->setupFonts();
        pThis->createControls(hwnd);
        pThis->setInitialState();
        return 0;
    } else {
        pThis = reinterpret_cast<MainWindow*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
    }

    if (pThis) {
        switch (uMsg) {
        case WM_COMMAND: {
            int wmId = LOWORD(wParam);
            switch (wmId) {
            case ID_BTN_ADD:
                pThis->onAdd();
                return 0;
            case ID_BTN_UPDATE:
                pThis->onUpdate();
                return 0;
            case ID_BTN_DELETE:
                pThis->onDelete();
                return 0;
            case ID_BTN_NEXT:
                pThis->onNext();
                return 0;
            case ID_BTN_CLEAR:
                pThis->onClear();
                return 0;
            }
            break;
        }
        case WM_CTLCOLORSTATIC: {
            HDC hdcStatic = (HDC)wParam;
            SetBkColor(hdcStatic, RGB(245, 247, 250));
            SetTextColor(hdcStatic, RGB(30, 41, 59));
            return (LRESULT)pThis->hBackgroundBrush;
        }
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
        }
    }

    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

