#include <QApplication>
#include "StudentSystemQt.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    StudentSystemQt window;
    window.show();
    return app.exec();
}

