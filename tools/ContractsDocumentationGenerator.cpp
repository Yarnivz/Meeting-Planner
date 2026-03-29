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
                //Temporarily use Date only for small scale test
                if (baseFilename == "Date")
                {
                    ContractsDocumentationGenerator::generateContractsDocumentation(baseFilename, sourceDirectory);
                    std::cout << baseFilename << " has header file "<< std::endl;
                }
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
    std::vector<std::string> contracts;
    for (size_t i = 0; i < headerFileLines.size(); ++i)
    {
        if ((headerFileLines[i].find(");") != std::string::npos ||
            headerFileLines[i].find(") const;") != std::string::npos) &&
            headerFileLines[i].find("ENSURE") == std::string::npos &&
            headerFileLines[i].find("REQUIRE") == std::string::npos)
        {
            currentFunction = headerFileLines[i];
            currentFunction.pop_back();

            //TODO add codeFile for loop to find contracts in current function
            
            contracts.push_back("test " + currentFunction);
            contracts.push_back("test1");
            contracts.push_back("test2");
            size_t documentationLastLine = 0;
            size_t contractDocumentationFirstLine = 0;
            for (size_t j = i-1; j > 0; --j)
            {
                if (headerFileLines[j].find(previousFunction) != std::string::npos || j < 5 )
                {
                    if (documentationLastLine != 0 && contractDocumentationFirstLine != 0)
                    {

                    } else if (documentationLastLine != 0 && contractDocumentationFirstLine == 0)
                    {
                        std::cout << "adding fresh contract                          --------";
                        headerFileLines.insert((headerFileLines.begin() + documentationLastLine), "     *");
                        headerFileLines.insert((headerFileLines.begin() + documentationLastLine + 1), "     * @contracts");

                        headerFileLines.insert((headerFileLines.begin() + documentationLastLine + 2), "     * " + contracts[0]);
                        for (size_t c = 1; c < contracts.size(); ++c)
                        {
                            headerFileLines.insert((headerFileLines.begin() + documentationLastLine + c + 2), "     * @n " + contracts[c]);
                        }
                        i += contracts.size() + 2;
                    } else
                    {
                        std::cout << "No documentation found for " << headerFileLines[i] << std::endl;
                        break;
                    }
                } else if (headerFileLines[j].find("@contracts") != std::string::npos)
                {
                   contractDocumentationFirstLine = j;

                } else if (headerFileLines[j].find("*/") != std::string::npos)
                {
                    documentationLastLine = j;
                }
            }
            previousFunction = currentFunction;
            std::cout << " found function " << currentFunction << " in " << baseFilename+".h"<<std::endl;
            contracts.clear();
        }

    }
    codeFile.close();
    headerFile.close();

    //std::ofstream writableHeaderFile(sourceDirectory+baseFilename+".h"); //hold off until tested properly enough
    for (std::string writeLine : headerFileLines)
    {
        std::cout << writeLine << std::endl;
        //writableHeaderFile << writeLine << "\n"; //hold off until tested properly enough
    }
    //writableHeaderFile.close(); //hold off until tested properly enough

}