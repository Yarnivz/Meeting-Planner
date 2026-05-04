//
// Created by s0255946 on 18/03/2026.
//

#ifndef XMLPARSER_H
#define XMLPARSER_H
#include "Parser.h"
#include "tinyxml.h"


//TODO: add documentation

class XmlParser : public Parser
{
public:
    explicit XmlParser(std::ostream& errorStream = std::cerr);
    void parse(const std::string& filename) override;
private:
    void parseElement(TiXmlElement* elementObject);
    bool parseProperty(TiXmlElement* propertyObject, std::string& propError);
};


#endif //XMLPARSER_H
