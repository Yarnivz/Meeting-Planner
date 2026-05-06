#include <iostream>

#include "App.h"
#include <fstream>

#include "error/DetailedError.h"
#include "error/ConsiseError.h"
#include "output/StreamOutput.h"
#include "parser/XmlParser.h"

int main()
{

    std::ofstream null_stream;
    XmlParser parser =  XmlParser("./test-files/InvalidDataSystemErrors.xml");

    App* app = new App();
    app->parseFile(parser, ConsiseError(std::cerr));
    //app->writeToStream(StreamOutput(std::cout));
    delete app;
}