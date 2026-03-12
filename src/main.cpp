#include <iostream>

#include "App.h"
#include <fstream>

int main() {
    App* app = new App();
    app->parseFile("./test-files/HappyDay1.xml");
    std::ofstream fout("output.txt");
    app->writeToStream(fout);
    delete app;
}
