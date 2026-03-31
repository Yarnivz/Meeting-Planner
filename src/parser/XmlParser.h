//
// Created by s0255946 on 18/03/2026.
//

#ifndef XMLPARSER_H
#define XMLPARSER_H
#include "Parser.h"


class XmlParser : public Parser
{
public:
    explicit XmlParser() = default;
    void parse(const std::string& filename, std::ostream& errorStream) override;
};


#endif //XMLPARSER_H
