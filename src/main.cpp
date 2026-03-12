#include <iostream>
#include "App.h"
int main() {
    std::cout << "Hello, World!" << std::endl;

    App app;
    app.parseFile("test.xml");
    app.processMeetings();
    return 0;
}