# Student Record Management System (100% Pure C++ GUI)

A complete, production-grade **Student Record Management System** implemented in **100% Pure C++** featuring a Graphical User Interface (GUI), structured state management, and file stream handling with standard CSV formatting.

---

## 🌟 Key Features

1. **Pure C++ Compliance**:
   - Zero foreign languages (no Python, C#, HTML/JS, etc.).
   - Implemented with modern C++ Object-Oriented Architecture (classes, encapsulation, separation of concerns).
   - Direct native Windows GUI (Win32 / Common Controls) with modern Segoe UI typography and visual styling.
   - Also includes complete, drop-in alternative implementations for **Qt** (`StudentSystemQt.cpp`) and **FLTK** (`StudentSystemFLTK.cpp`) sharing the identical data layer.

2. **Form Data Fields**:
   - **Student Name** (`std::string`)
   - **Roll Number** (`std::string`, unique primary identifier)
   - **Father's Name** (`std::string`)
   - **Caste** (`std::string`)
   - **District** (`std::string`)
   - **GPA** (`double`, validated in range `0.00` to `4.00`)

3. **Action Buttons**:
   - **Add**: Validates all input fields, checks for duplicate Roll Numbers, appends the record to `students.txt`, and displays a confirmation dialog.
   - **Update**: Modifies the currently loaded student record using Roll Number as the unique identifier.
   - **Delete**: Prompts for confirmation and permanently removes the displayed student record from `students.txt`.
   - **Next**: Sequentially cycles through records stored in `students.txt`, populates the form fields, and updates button states.
   - **Clear / New**: Resets the form fields and switches the application back to "Add Record" mode.

4. **Strict State Management**:
   - **On Startup / Reset**:
     - `Add` button: **ENABLED**
     - `Update` button: **DISABLED**
     - `Delete` button: **DISABLED**
   - **When 'Next' is Clicked (Record Displayed)**:
     - `Add` button: **DISABLED**
     - `Update` button: **ENABLED**
     - `Delete` button: **ENABLED**

5. **Robust File Stream Storage**:
   - Managed via standard C++ `<fstream>` (`std::ifstream` and `std::ofstream`).
   - Saved in standard CSV format inside `students.txt`:
     ```text
     Name,RollNo,FatherName,Caste,District,GPA
     ```
   - Complete input sanitization (whitespace trimming, comma delimiter collision prevention, and numeric range enforcement).

---

## 📁 Project Structure

```text
PF Project/
├── Student.h                 # Student entity declaration
├── Student.cpp               # Student implementation, validation, and CSV serialization
├── StudentManager.h          # Record manager declaration (file I/O, search, navigation)
├── StudentManager.cpp        # File stream CRUD logic using <fstream>
├── MainWindow.h              # Native C++ GUI window declaration
├── MainWindow.cpp            # Native C++ GUI event loop, controls, and state logic
├── main.cpp                  # WinMain entry point
├── app.manifest              # Windows Common-Controls 6.0 modern visual style manifest
├── resource.rc               # Resource script linking the manifest
├── StudentRecordSystem.exe   # Pre-built Windows binary (Ready to run!)
├── StudentRecordSystem.dev   # Dev-C++ project file
├── build.bat                 # One-click Windows build script
├── CMakeLists.txt            # Modern CMake build configuration
├── test_runner.cpp           # Automated unit and integration test suite
├── students.txt              # Text database file
├── StudentSystemQt.h         # Qt edition header
├── StudentSystemQt.cpp       # Qt edition implementation
├── main_qt.cpp               # Qt main entry point
├── StudentRecordSystem.pro   # Qt Creator qmake project file
└── StudentSystemFLTK.cpp     # FLTK edition single-file GUI
```

---

## 🚀 How to Build and Run

### Method 1: Run the Pre-Built Executable (Instant)
The executable is already compiled and ready in the folder:
- Double-click **`StudentRecordSystem.exe`** or run in terminal:
  ```powershell
  .\StudentRecordSystem.exe
  ```

---

### Method 2: One-Click Build (`build.bat`)
Run the included batch script in the project directory:
```powershell
.\build.bat
```
This automatically compiles the manifest with `windres` and builds `StudentRecordSystem.exe` with `g++`.

---

### Method 3: Command Line (MinGW / GCC `g++`)
From PowerShell or Command Prompt, run:
```powershell
# 1. Compile Windows visual styles manifest
windres resource.rc -O coff -o resource.res

# 2. Compile and link pure C++ application
g++ -std=c++11 -Wall -Wextra -O2 main.cpp MainWindow.cpp Student.cpp StudentManager.cpp resource.res -o StudentRecordSystem.exe -mwindows -lcomctl32

# 3. Launch the application
.\StudentRecordSystem.exe
```

---

### Method 4: Dev-C++ IDE (5.11 / Embarcadero)
1. Open Dev-C++.
2. Click **File -> Open Project or File...** and select **`StudentRecordSystem.dev`**.
3. Press **F11** (or click **Execute -> Compile & Run**).

---

### Method 5: Run Automated Unit Tests
To verify all CRUD operations, CSV parsing, and validation rules:
```powershell
g++ -std=c++11 -Wall -Wextra test_runner.cpp Student.cpp StudentManager.cpp -o test_runner.exe
.\test_runner.exe
```
Expected output:
```text
[TEST] Running Student Record Management System Tests...
  Passed: Empty name validation check.
  Passed: Comma sanitization check.
  Passed: GPA upper-bound (4.0) check.
  Passed: GPA lower-bound (0.0) check.
  Passed: Valid student data validation.
  Passed: CSV serialization format matches specification.
  Passed: CSV deserialization round-trip.
  Passed: Adding students to storage.
  Passed: Duplicate Roll Number rejection.
  Passed: Persistence loaded correctly from file.
  Passed: Next record navigation and wraparound.
  Passed: Record update by Roll Number.
  Passed: Record deletion by Roll Number.
  Passed: Verified disk content format.

ALL TESTS PASSED SUCCESSFULLY! (8/8)
```

---

## 🎨 Alternative GUI Frameworks (Qt & FLTK)

### Qt Edition
If you are working in **Qt Creator** or have Qt 5 / Qt 6 installed:
1. Open `StudentRecordSystem.pro` in **Qt Creator**.
2. Click **Run** (Ctrl + R).
*Or via terminal:*
```bash
qmake StudentRecordSystem.pro
make # or mingw32-make / nmake
```

### FLTK Edition
If compiling on a system with FLTK:
```bash
fltk-config --compile StudentSystemFLTK.cpp Student.cpp StudentManager.cpp
# Or directly:
g++ -std=c++11 StudentSystemFLTK.cpp Student.cpp StudentManager.cpp -o StudentSystemFLTK -lfltk -lole32 -luuid -lcomctl32
```

---

## 📝 Text File Storage Format (`students.txt`)

Records are stored line-by-line as comma-separated values:
```text
Muhammad Ali,CS-2023-01,Ahmed Ali,Ansari,Karachi,3.75
Fatima Khan,SE-2023-14,Tariq Khan,Pathan,Lahore,3.88
Usman Tariq,IT-2023-22,Tariq Mehmood,Arain,Islamabad,3.42
Ayesha Malik,CS-2023-45,Abdul Malik,Malik,Rawalpindi,3.95
```

- **Roll Number** is enforced as the unique identifier.
- Any attempt to add an existing Roll Number displays an informative duplicate warning.
- Modifying fields and clicking **Update** rewrites the record while preserving other entries.
- Clicking **Delete** prompts for confirmation and cleanly removes the entry.

