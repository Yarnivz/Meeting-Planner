//
// Created by tishaak on 3/29/26.
//
#include <iostream>
#ifndef MEETING_PLANNER_CONTRACTSDOCUMENTATIONGENERATOR_H
#define MEETING_PLANNER_CONTRACTSDOCUMENTATIONGENERATOR_H

class ContractsDocumentationGenerator
{
public:
    static void generateContractsDocumentation(const std::string& baseFilename, const std::string& sourceDirectory);

    static void removeWhitespace(std::string& targetString);
};

#endif //MEETING_PLANNER_CONTRACTSDOCUMENTATIONGENERATOR_H