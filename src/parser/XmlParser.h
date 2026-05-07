//
// Created by s0255946 on 18/03/2026.
//

#ifndef XMLPARSER_H
#define XMLPARSER_H
#include "Parser.h"
#include "tinyxml.h"

#include <fstream>


//TODO: add documentation

class XmlParser : public Parser
{
public:
    explicit XmlParser(const std::string& filepath, std::ostream& errorStream = std::cerr);
    void parse() override;
private:
    void parseElement(TiXmlElement* elementObject);
    bool parseProperty(TiXmlElement* propertyObject, std::string& propError);
    std::string filepath;
};


#endif //XMLPARSER_H
