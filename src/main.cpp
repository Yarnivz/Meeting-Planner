#include <iostream>

#include "App.h"
#include <fstream>

int main() {
    App* app = new App();
    app->parseFile("./test-files/HappyDay2.xml");
    app->processSingleMeeting("Meeting_478463");
    std::ofstream fout("output.txt");
    app->writeToStream(fout);
    delete app;
}
