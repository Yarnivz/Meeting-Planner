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
    XmlParser(const std::string& filepath);
    void parse(const Error& error) override;
private:
    void parseElement(TiXmlElement* elementObject, const Error& error);
    bool parseProperty(TiXmlElement* propertyObject, const Error& error);
    std::string filepath;
};


#endif //XMLPARSER_H
