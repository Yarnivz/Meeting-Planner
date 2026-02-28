#include <iostream>
#include "App.h"
int main() {
    std::cout << "Hello, World!" << std::endl;

    App app;
    app.parseFile("test.xml");
    std::cout << app.getMeeting("Meeting_478463")->getId();
    return 0;
}