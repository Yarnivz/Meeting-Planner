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
    for (const std::filesystem::directory_entry& entry: std::filesystem::recursive_directory_iterator(sourceDirectory))
    {
        std::cout << "passed " << entry.path().filename().string() << std::endl;
        if (entry.path().extension() == ".cpp")
        {
            std::filesystem::path headerFile = entry.path();
            headerFile.replace_extension(".h");
            std::string baseFilename = entry.path().stem().string();
            if (std::filesystem::exists(headerFile))
            {
                std::cout << "base " << baseFilename << std::endl;
                //Temporarily use Date only for small scale test
                if (baseFilename == "App")
                {
                    std::cout << baseFilename << " has header and cpp file "<< std::endl;
                    ContractsDocumentationGenerator::generateContractsDocumentation(baseFilename, entry.path().parent_path().string()+'/');

                }
            }
        }
    }
}
void ContractsDocumentationGenerator::generateContractsDocumentation(const std::string& baseFilename, const std::string& fileDirectory)
{
    std::vector<std::string> codeFileLines;
    std::vector<std::string> headerFileLines;
    std::ifstream codeFile(fileDirectory+baseFilename+".cpp");
    std::ifstream headerFile(fileDirectory+baseFilename+".h");

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
    size_t previousFunctionLine = 1;
    std::vector<std::string> contracts;

    for (size_t i = 0; i < headerFileLines.size(); ++i)
    {
        if ((headerFileLines[i].find(");") != std::string::npos ||
            headerFileLines[i].find(") const;") != std::string::npos) &&
            headerFileLines[i].find("ENSURE") == std::string::npos &&
            headerFileLines[i].find("REQUIRE") == std::string::npos)
        {
            currentFunction = headerFileLines[i];
            removeWhitespace(currentFunction);
            currentFunction.pop_back();
            std::cout << "currentfunctionbase is " << currentFunction << std::endl;

            int layer = 0;
            bool functionFound = false;
            bool processingFunction = false;
            for (size_t k = 0; k < codeFileLines.size(); ++k)

            {
                std::string stringToRemove = baseFilename+"::";
                size_t removeStringIndex = codeFileLines[k].find(stringToRemove);
                if (removeStringIndex != std::string::npos) {
                    codeFileLines[k].erase(removeStringIndex, stringToRemove.length());
                }
                if (codeFileLines[k].find(currentFunction) != std::string::npos)
                {
                    functionFound = true;
                }

                if (functionFound)
                {
                    if (codeFileLines[k].find("REQUIRE") != std::string::npos)
                    {
                        std::cout << "contract detected for " << currentFunction << std::endl;
                        std::erase(codeFileLines[k], ' ');
                        contracts.push_back(codeFileLines[k]);
                    } else if (codeFileLines[k].find("ENSURE") != std::string::npos)
                    {
                        std::cout << "contract detected for " << currentFunction << std::endl;
                        std::erase(codeFileLines[k], ' ');
                        contracts.push_back(codeFileLines[k]);
                    }

                    if (codeFileLines[k].find("{") != std::string::npos)
                    {
                        layer +=1;
                        processingFunction = true;
                    }
                    if (codeFileLines[k].find("}") != std::string::npos)
                    {
                        layer -=1;
                    }
                    if (layer <= 0 && processingFunction && functionFound)
                    {
                        std::cout << "gone" << std::endl;
                        break;
                    }
                }
            }


            /*contracts.push_back("test" + currentFunction);
            contracts.push_back("test1");
            contracts.push_back("test2");*/
            size_t documentationLastLine = 0;
            size_t contractDocumentationFirstLine = 0;
            for (size_t j = i-1; j >= 1; --j)
            {
                if (headerFileLines[j].find(previousFunction) != std::string::npos || j == previousFunctionLine )
                {
                    break;
                } else if (headerFileLines[j].find("@contracts") != std::string::npos)
                {
                   contractDocumentationFirstLine = j;
                } else if (headerFileLines[j].find("*/") != std::string::npos)
                {
                    documentationLastLine = j;
                }
            }

            if (contracts.empty() && contractDocumentationFirstLine != 0)
            {
                headerFileLines.erase(headerFileLines.begin() + contractDocumentationFirstLine, headerFileLines.begin() + documentationLastLine);
                i -= documentationLastLine - contractDocumentationFirstLine;
                previousFunctionLine = i;
                previousFunction = currentFunction;
                std::cout << "yes?";
                continue;
            } else if (contracts.empty())
            {
                previousFunctionLine = i;
                previousFunction = currentFunction;
                continue;
            }

            if (documentationLastLine != 0 && contractDocumentationFirstLine != 0)
            {
                std::cout << "updating existing contracts" << std::endl;
                headerFileLines.erase(headerFileLines.begin() + contractDocumentationFirstLine + 1, headerFileLines.begin() + documentationLastLine);
                i -= documentationLastLine - contractDocumentationFirstLine - 1 ;

                headerFileLines.insert((headerFileLines.begin() + contractDocumentationFirstLine + 1), "     * " + contracts[0]);
                for (size_t c = 1; c < contracts.size(); ++c)
                {
                    headerFileLines.insert((headerFileLines.begin() + contractDocumentationFirstLine + c + 1), "     * @n " + contracts[c]);
                }
                i += contracts.size();
            } else if (documentationLastLine != 0 && contractDocumentationFirstLine == 0)
            {
                std::cout << "adding new contract" << std::endl;
                headerFileLines.insert((headerFileLines.begin() + documentationLastLine), "     * @contracts");
                headerFileLines.insert((headerFileLines.begin() + documentationLastLine + 1), "     * " + contracts[0]);
                for (size_t c = 1; c < contracts.size(); ++c)
                {
                    headerFileLines.insert((headerFileLines.begin() + documentationLastLine + c + 1), "     * @n " + contracts[c]);
                }
                i += contracts.size() + 1;
            } else
            {
                std::cout << "No documentation or contracts found for " << headerFileLines[i] << std::endl;
            }
            previousFunction = currentFunction;
            previousFunctionLine = i;
            std::cout << " finished function " << currentFunction << " in " << baseFilename+".h"<<std::endl;
            std::cout << "\n" << std::endl;
        }
        contracts.clear();
    }

    //std::ofstream writableHeaderFile(fileDirectory+baseFilename+".h"); //hold off until tested properly enough
    for (std::string writeLine : headerFileLines)
    {
        std::cout << writeLine << std::endl;
        //writableHeaderFile << writeLine << "\n"; //hold off until tested properly enough
    }
    //writableHeaderFile.close(); //hold off until tested properly enough
}

void ContractsDocumentationGenerator::removeWhitespace(std::string& targetString)
{
    size_t preEmptySpace = targetString.find_first_not_of(' ');
    if (preEmptySpace != std::string::npos) {
        targetString.erase(0, preEmptySpace);
    }
    size_t postEmptySpace = targetString.find_last_not_of(' ');
    if (postEmptySpace != std::string::npos) {
        targetString.erase(postEmptySpace + 1);
    }
}