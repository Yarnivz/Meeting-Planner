#include <iostream>

#include "App.h"
#include <fstream>

#include "parser/XmlParser.h"

int main()
{
    XmlParser* parser = new XmlParser();
    std::ostream* scout = &std::cout;
    std::ofstream* fout = new std::ofstream("output.txt");
    App* app = new App(parser, scout);
    app->parseFile("./test-files/HappyDay1.xml");
    app->processAllMeetings();
    app->writeToStream();
    delete parser;
    delete fout;
    delete app;
}