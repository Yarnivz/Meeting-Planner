#include <iostream>
#include "App.h"
int main() {
    App app;
    app.parseFile("test-files/WrongData1.xml");
    app.processMeetings();
    return 0;
}