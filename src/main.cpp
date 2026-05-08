#include <iostream>

#include "App.h"
#include <fstream>

#include "error/DetailedError.h"
#include "error/ConciseError.h"
#include "output/TextOutput.h"
#include "parser/XmlParser.h"

int main()
{

    std::ofstream null_stream;
    App* app = new App();
    app->parse(
        XmlParser("./test-files/InvalidData2.xml"),
        DetailedError(std::cerr)
        );
    app->write(TextOutput(std::cout));
    delete app;
}
