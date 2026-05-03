#include <iostream>

#include "App.h"
#include <fstream>

#include "output/StreamOutput.h"
#include "parser/XmlParser.h"

int main()
{

    XmlParser* parser = new XmlParser();
    StreamOutput* output = new StreamOutput();

    //std::ofstream* fout = new std::ofstream("output.txt");

    App* app = new App(parser, output);
    app->parseFile("./test-files/HappyDay3.xml");
    app->processAllMeetings();
    app->writeToStream();
    delete app;
}