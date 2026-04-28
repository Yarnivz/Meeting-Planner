//
// Created by s0255946 on 18/03/2026.
//

#ifndef XMLPARSER_H
#define XMLPARSER_H
#include "Parser.h"
#include "tinyxml.h"


//TODO: add documentation

enum class ElementType
{
    CAMPUS,
    BUILDING,
    ROOM,
    MEETING,
    PARTICIPATION
};

enum class PropType
{
    IDENTIFIER,
    NAME,
    LABEL,
    CAMPUS,
    BUILDING,
    CAPACITY,
    ROOM,
    DATE,
    HOUR,
    EXTERNALS,
    USER,
    EXTERNAL,
    MEETING
};

class XmlParser : public Parser
{
public:
    explicit XmlParser(std::ostream& errorStream = std::cerr);
    void parse(const std::string& filename) override;
private:
    void parseElement(TiXmlElement* elementObject);
    bool parseProperty(const std::string& prop, Element& parseObject, std::string& propError);
};


#endif //XMLPARSER_H