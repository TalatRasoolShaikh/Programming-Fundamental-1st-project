#include "MainWindow.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    // Suppress unused parameter warnings
    (void)hPrevInstance;
    (void)lpCmdLine;

    MainWindow app;
    if (!app.initialize(hInstance, nCmdShow)) {
        return 1;
    }

    return app.run();
}

