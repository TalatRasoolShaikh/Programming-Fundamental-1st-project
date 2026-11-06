cmake_minimum_required(VERSION 3.10)
project(StudentRecordManagementSystem CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Option 1: Native Windows GUI (Default on Windows, 0 external dependencies)
if(WIN32)
    add_executable(StudentRecordSystem WIN32
        main.cpp
        MainWindow.cpp
        MainWindow.h
        Student.cpp
        Student.h
        StudentManager.cpp
        StudentManager.h
        resource.rc
    )
    target_link_libraries(StudentRecordSystem comctl32)
endif()

# Option 2: FLTK GUI (If FLTK is installed)
find_package(FLTK QUIET)
if(FLTK_FOUND)
    add_executable(StudentRecordSystemFLTK
        StudentSystemFLTK.cpp
        Student.cpp
        StudentManager.cpp
    )
    target_link_libraries(StudentRecordSystemFLTK ${FLTK_LIBRARIES})
    target_include_directories(StudentRecordSystemFLTK PRIVATE ${FLTK_INCLUDE_DIRS})
endif()

# Option 3: Qt GUI (If Qt5 or Qt6 is installed)
find_package(QT NAMES Qt6 Qt5 COMPONENTS Widgets QUIET)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets QUIET)
if(Qt${QT_VERSION_MAJOR}Widgets_FOUND)
    set(CMAKE_AUTOMOC ON)
    add_executable(StudentRecordSystemQt
        main_qt.cpp
        StudentSystemQt.cpp
        StudentSystemQt.h
        Student.cpp
        StudentManager.cpp
    )
    target_link_libraries(StudentRecordSystemQt Qt${QT_VERSION_MAJOR}::Widgets)
endif()

# Unit / Integration Tests Runner
add_executable(run_tests
    test_runner.cpp
    Student.cpp
    StudentManager.cpp
)

