//
// Created by tishaak on 3/29/26.
//
#include "ContractsDocumentationGenerator.h"
#include <iostream>
#include <filesystem> //reminder to put std before its own namespace since it seems to be part of iostream(i think?)
#include <vector>
#include <fstream>

int main() {
    std::cout << "test" << std::endl;
    std::string sourceDirectory = "./src/";
    for (const std::filesystem::directory_entry& entry: std::filesystem::directory_iterator(sourceDirectory))
    {
        std::cout << "passed " << entry.path().filename().string() << std::endl;
        if (entry.path().extension() == ".cpp")
        {
            std::string baseFilename = entry.path().stem().string();
            if (std::filesystem::exists(sourceDirectory+baseFilename+".h"))
            {
                ContractsDocumentationGenerator::generateContractsDocumentation(baseFilename, sourceDirectory);
                std::cout << baseFilename << " has header file "<< std::endl;
            }
        }
    }
}
void ContractsDocumentationGenerator::generateContractsDocumentation(const std::string& baseFilename, const std::string& sourceDirectory)
{
    std::vector<std::string> codeFileLines;
    std::vector<std::string> headerFileLines;
    std::ifstream codeFile(sourceDirectory+baseFilename+".cpp");
    std::ifstream headerFile(sourceDirectory+baseFilename+".h");

    std::string line;
    while (getline(codeFile, line))
    {
        codeFileLines.push_back(line);
    }
    codeFile.close();

    while (getline(headerFile, line))
    {
        headerFileLines.push_back(line);
    }
    headerFile.close();

    std::string currentFunction;
    std::string previousFunction;
    for (size_t i=0; i < headerFileLines.size(); ++i)
    {
        if ((headerFileLines[i].find(");") != std::string::npos ||
            headerFileLines[i].find(") const;") != std::string::npos) &&
            headerFileLines[i].find("ENSURE") == std::string::npos &&
            headerFileLines[i].find("REQUIRE") == std::string::npos)
        {
            currentFunction = headerFileLines[i];
            currentFunction.pop_back();

            std::cout << " found function " << currentFunction << " in " << baseFilename+".h"<<std::endl;
        }
    }

}