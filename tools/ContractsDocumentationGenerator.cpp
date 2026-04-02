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
                if (baseFilename == "Date")
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

    for (size_t i = 0; i < headerFileLines.size(); ++i)
    {
        if ((headerFileLines[i].find(");") != std::string::npos ||
            headerFileLines[i].find(") const;") != std::string::npos) &&
            headerFileLines[i].find("ENSURE") == std::string::npos &&
            headerFileLines[i].find("REQUIRE") == std::string::npos)
        {
            std::vector<std::string> preContracts;
            std::vector<std::string> postContracts;

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
                        std::cout << "REQUIRE contract detected for " << currentFunction << std::endl;
                        size_t startIndex = codeFileLines[k].find('"') + 1;
                        size_t endIndex = codeFileLines[k].find_last_of('"');
                        std::string contractComment = codeFileLines[k].substr(startIndex, endIndex - startIndex);
                        std::cout << "contractcomment is " << contractComment << std::endl;
                        preContracts.push_back(contractComment);
                    } else if (codeFileLines[k].find("ENSURE") != std::string::npos)
                    {
                        std::cout << "ENSURE contract detected for " << currentFunction << std::endl;
                        size_t startIndex = codeFileLines[k].find('"') + 1;
                        size_t endIndex = codeFileLines[k].find_last_of('"');
                        std::string contractComment = codeFileLines[k].substr(startIndex, endIndex - startIndex);
                        postContracts.push_back(contractComment);
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
                        break;
                    }
                }
            }


            /*preContracts.push_back("test" + currentFunction);
            preContracts.push_back("test1");
            preContracts.push_back("test2");*/
            size_t documentationFirstLine = 0;
            size_t documentationLastLine = 0;
            size_t preDocumentationFirstLine = 0;
            size_t preDocumentationLastLine = 0;
            size_t postDocumentationFirstLine = 0;
            size_t postDocumentationLastLine = 0;
            bool documentationFound = false;
            std::string documenationAsterix;
            for (size_t j = previousFunctionLine; j <= headerFileLines.size(); ++j)
            {
                // if for loop reaches currentfunctionline, stop loop
                if (headerFileLines[j].find(currentFunction) != std::string::npos || j == i)
                {
                    break;
                }
                // if documentation opening is found, set documentationfirstline and if documentation end is found, set documentationendline
                if (headerFileLines[j].find("/**") != std::string::npos)
                {
                    documenationAsterix.resize(headerFileLines[j].find("/**") + 1, ' ');
                    documenationAsterix += "* ";
                    documentationFound = true;
                    documentationFirstLine = j;
                } else if (headerFileLines[j].find("*/") != std::string::npos && documentationFound)
                {
                    documentationLastLine = j;
                }
                // Check for eligible precondition line
                if (headerFileLines[j].find("@pre") != std::string::npos && documentationFound)
                {
                    if (preDocumentationFirstLine == 0)
                    {
                        preDocumentationFirstLine = j;
                    }
                    preDocumentationLastLine = j;
                } else if (headerFileLines[j].find("@param") != std::string::npos && documentationFound && preDocumentationFirstLine == 0)
                {
                        preDocumentationFirstLine = j;
                } else if (headerFileLines[j].find("@return") != std::string::npos && documentationFound && preDocumentationFirstLine == 0)
                {
                        preDocumentationFirstLine = j;
                } else if (headerFileLines[j].find("*/") != std::string::npos && documentationFound && preDocumentationFirstLine == 0)
                {
                        preDocumentationFirstLine = j;
                }

                // Check for eligible postcondition line
                if (headerFileLines[j].find("@post") != std::string::npos && documentationFound)
                {
                    if (postDocumentationFirstLine == 0)
                    {
                        postDocumentationFirstLine = j;
                    }
                    postDocumentationLastLine = j;
                } else if (headerFileLines[j].find("@return") != std::string::npos && documentationFound && postDocumentationLastLine == 0)
                {
                    postDocumentationFirstLine = j+1;
                } else if (headerFileLines[j].find("*/") != std::string::npos && documentationFound && postDocumentationLastLine == 0)
                {
                    postDocumentationFirstLine = j;
                }
            }
            std::cout << postDocumentationFirstLine << " and " << postDocumentationLastLine << std::endl;
            // If no documentation is found, continue (probably will change later to auto add documentation with just contracts)
            if (documentationFirstLine == 0 && documentationLastLine == 0)
            {
                previousFunctionLine = i;
                previousFunction = currentFunction;
                std::cout << "No documentation found for " << headerFileLines[i] << ", cant insert contracts" << std::endl;
                continue;
            }
            // Erase pre or postcondition contracts if none are found in code but exist in documentation
            if (preContracts.empty() && preDocumentationLastLine != 0)
            {
                headerFileLines.erase(headerFileLines.begin() + preDocumentationFirstLine, headerFileLines.begin() + preDocumentationLastLine + 1);
                i -= preDocumentationLastLine - preDocumentationFirstLine;
                previousFunctionLine = i;
                previousFunction = currentFunction;
                continue;
            }
            if (postContracts.empty() && postDocumentationLastLine != 0)
            {
                headerFileLines.erase(headerFileLines.begin() + postDocumentationFirstLine, headerFileLines.begin() + postDocumentationLastLine + 1);
                i -= postDocumentationLastLine - postDocumentationFirstLine;
                previousFunctionLine = i;
                previousFunction = currentFunction;
                continue;
            }
            // If no contracts in code or documentation are found, just continue to the next function
            if (preContracts.empty() && postContracts.empty() && preDocumentationLastLine == 0 && postDocumentationLastLine == 0)
            {
                previousFunctionLine = i;
                previousFunction = currentFunction;
                std::cout << "No contracts in code or documentation found for " << headerFileLines[i] << std::endl;
                continue;
            }
            // if precondition contracts are found in documentations are found, clear it and replace with current ones, else just add
            if (preDocumentationFirstLine != 0 && preDocumentationLastLine != 0 && !preContracts.empty())
            {
                std::cout << "updating existing pre contracts" << std::endl;
                headerFileLines.erase(headerFileLines.begin() + preDocumentationFirstLine, headerFileLines.begin() + preDocumentationLastLine + 1);
                i -= preDocumentationLastLine - preDocumentationFirstLine + 1;
                if (postDocumentationFirstLine != 0) postDocumentationFirstLine -= preDocumentationLastLine - preDocumentationFirstLine + 1;
                if (postDocumentationLastLine != 0) postDocumentationLastLine -= preDocumentationLastLine - preDocumentationFirstLine + 1;


                headerFileLines.insert((headerFileLines.begin() + preDocumentationFirstLine ), documenationAsterix + "@pre " + preContracts[0]);
                for (size_t c = 1; c < preContracts.size(); ++c)
                {
                    headerFileLines.insert((headerFileLines.begin() + preDocumentationFirstLine + c), documenationAsterix + "@pre " + preContracts[c]);
                }
                i += preContracts.size();
                if (postDocumentationFirstLine != 0) postDocumentationFirstLine += preContracts.size();
                if (postDocumentationLastLine != 0) postDocumentationLastLine += preContracts.size();
            } else if (preDocumentationFirstLine != 0 && !preContracts.empty())
            {
                std::cout << "adding new pre contracts" << std::endl;

                headerFileLines.insert((headerFileLines.begin() + preDocumentationFirstLine), documenationAsterix);
                headerFileLines.insert((headerFileLines.begin() + preDocumentationFirstLine + 1), documenationAsterix + "@pre " + preContracts[0]);
                for (size_t c = 1; c < preContracts.size(); ++c)
                {
                    headerFileLines.insert((headerFileLines.begin() + preDocumentationFirstLine + c + 1), documenationAsterix + "@pre " + preContracts[c]);
                }
                headerFileLines.insert((headerFileLines.begin() + preDocumentationFirstLine + preContracts.size() + 1), documenationAsterix);
                i += preContracts.size() + 2;
                if (postDocumentationFirstLine != 0) postDocumentationFirstLine += preContracts.size() + 2;
                if (postDocumentationLastLine != 0) postDocumentationLastLine += preContracts.size() + 2;
            }
            // if postcondition contracts are found in documentations are found, clear it and replace with current ones, else just add
            if (postDocumentationFirstLine != 0 && postDocumentationLastLine != 0 && !postContracts.empty())
            {
                std::cout << "updating existing post contracts" << std::endl;
                headerFileLines.erase(headerFileLines.begin() + postDocumentationFirstLine, headerFileLines.begin() + postDocumentationLastLine + 1);
                i -= postDocumentationLastLine - postDocumentationFirstLine + 1;

                headerFileLines.insert((headerFileLines.begin() + postDocumentationFirstLine ), documenationAsterix + "@post " + postContracts[0]);
                for (size_t c = 1; c < postContracts.size(); ++c)
                {
                    headerFileLines.insert((headerFileLines.begin() + postDocumentationFirstLine + c), documenationAsterix + "@post " + postContracts[c]);
                }
                i += postContracts.size();
            } else if (postDocumentationFirstLine != 0 && !postContracts.empty())
            {
                std::cout << "adding new post contracts" << std::endl;

                headerFileLines.insert((headerFileLines.begin() + postDocumentationFirstLine), documenationAsterix);
                headerFileLines.insert((headerFileLines.begin() + postDocumentationFirstLine + 1), documenationAsterix + "@post " + postContracts[0]);
                for (size_t c = 1; c < postContracts.size(); ++c)
                {
                    headerFileLines.insert((headerFileLines.begin() + postDocumentationFirstLine + c + 1), documenationAsterix + "@post " + postContracts[c]);
                }
                i += postContracts.size() + 1;
            }


            previousFunction = currentFunction;
            previousFunctionLine = i;
            std::cout << " finished function " << currentFunction << " in " << baseFilename+".h"<<std::endl;
            std::cout << "\n" << std::endl;
        }

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