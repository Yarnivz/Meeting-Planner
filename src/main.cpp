#include <iostream>

#include "App.h"
#include <fstream>

#include "error/DetailedError.h"
#include "error/ConciseError.h"
#include "output/StreamOutput.h"
#include "parser/XmlParser.h"

int main()
{

    std::ofstream null_stream;
    App* app = new App();
    app->parse(
        XmlParser("./test-files/InvalidData2.xml"),
        DetailedError(std::cerr)
        );
    app->write(StreamOutput(std::cout));
    delete app;
}
