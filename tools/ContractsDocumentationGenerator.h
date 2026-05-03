//
// Created by tishaak on 3/29/26.
//
#include <iostream>
#include <vector>
#ifndef MEETING_PLANNER_CONTRACTSDOCUMENTATIONGENERATOR_H
#define MEETING_PLANNER_CONTRACTSDOCUMENTATIONGENERATOR_H

class ContractsDocumentationGenerator
{
public:
    static void generateContractsDocumentation(const std::string& baseFilename, const std::string& sourceDirectory);

    static void getCodeContracts(const std::string& baseFilename, bool& multicomment, const std::string& currentFunction, const std::vector<std::string>& codeFileLines, std::vector<std::string>& preContracts, std::vector<std::string>& postContracts);

    static void findHeaderDocumentationAndContractsStuff(const std::string& currentFunction, const size_t& i, const size_t& previousFunctionLine, const std::vector<std::string>& headerFileLines, size_t& documentationFirstLine, size_t& documentationLastLine, size_t& preDocumentationFirstLine, size_t& preDocumentationLastLine, size_t& postDocumentationFirstLine, size_t& postDocumentationLastLine, std::string& documenationAsterix);


    static void removeEndWhitespaces(std::string& targetString);

};

#endif //MEETING_PLANNER_CONTRACTSDOCUMENTATIONGENERATOR_H