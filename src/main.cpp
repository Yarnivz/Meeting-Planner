#include <iostream>

#include "App.h"
#include <fstream>

#include "output/StreamOutput.h"
#include "parser/XmlParser.h"

int main()
{

    XmlParser parser =  XmlParser("./test-files/Renovations.xml");

    App* app = new App();
    app->parseFile(parser);
    app->processAllMeetings();
    app->writeToStream(StreamOutput(std::cout));
    delete app;
}