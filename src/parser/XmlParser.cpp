//
// Created by s0255946 on 18/03/2026.
//

#include "XmlParser.h"

#include <unordered_set>
#include <unordered_map>
#include <functional>
#include <objects/Date.h>

#include "helper/DesignByContract.h"
#include "objects/Room.h"

static inline bool parse_boolean(std::string s)
{
    std::transform(s.begin(), s.end(), s.begin(), ::tolower);
    if (s == "true" || s == "1")
    {
        return true;
    }

    if (s == "false" || s == "0")
    {
        return false;
    }

    throw std::exception();
}

static inline bool parse_date (const std::string& input, Date& output, std::string& parseError)
{
    //Try to convert to DateTime check
    int day = 0;
    int month = 0;
    int year = 0;

    try
    {
        day = std::stoi(input.substr(8, 2));
        month = std::stoi(input.substr(5, 2));
        year = std::stoi(input.substr(0, 4));
    }
    catch (std::exception& except)
    {
        parseError = std::string("Date value could not be converted to a date format: \n\t- ") + except.what();
        return false;
    }
    std::chrono::year_month_day chrono_date = {
        std::chrono::year(year), std::chrono::month(month), std::chrono::day(day)
    };
    //Check if date exists
    if (!chrono_date.ok() || year <= 0)
    {
        parseError = "Date " + input + " does not exist.";
        return false;
    }

    output = Date(year, month, day);
    return true;
}

XmlParser::XmlParser(std::ostream& errorStream): Parser(errorStream) {}

void XmlParser::parse(const std::string& filename)
{
    TiXmlDocument doc;
    REQUIRE(!filename.empty(), "The file cannot be empty");
    REQUIRE(doc.LoadFile(filename.c_str()),
            "The provided file \'%s\' doesn't exist in your current work directory or cannot be opened.",
            filename.c_str());


    if (!doc.LoadFile(filename.c_str()))
    {
        errorStream << doc.ErrorDesc() << std::endl;
        return;
    }


    TiXmlElement* root = doc.FirstChildElement();
    if (root == nullptr)
    {
        errorStream << "Failed to load file: No root element." << std::endl;
        doc.Clear();
        return;
    }


    for (TiXmlElement* objectElement = root->FirstChildElement(); objectElement != nullptr; objectElement =
         objectElement->NextSiblingElement())
    {
        parseElement(objectElement);
    }
}

void XmlParser::parseElement(TiXmlElement* elementObject)
{
    parseObject = ParseObject();
    const static std::unordered_map<std::string, ElementType> elementValues = {
        {"CAMPUS", ElementType::CAMPUS},
        {"BUILDING", ElementType::BUILDING},
        {"ROOM", ElementType::ROOM},
        {"MEETING", ElementType::MEETING},
        {"PARTICIPATION", ElementType::PARTICIPATION},
        {"CATERING", ElementType::CATERING},
        {"RENOVATION", ElementType::RENOVATION},
    };



    const std::string elementType = elementObject->Value();

    if (!elementValues.contains(elementType))
    {
        errorStream << "Unrecognized object element:  " << elementType << std::endl;
        return;
    }

    std::unordered_set<std::string> requiredProps;
    std::unordered_set<std::string> foundProps;
    std::function<void()> parseHandler;

    switch (elementValues.at(elementType))
    {
        //CAMPUS
        case ElementType::CAMPUS:
            requiredProps = {"NAME", "IDENTIFIER"};
            parseHandler = [&]() {
                parsed_campuses.push_back((CampusElement){
                    .name = parseObject.name,
                    .id = parseObject.identifier
                });
            };
            break;
        //BUILDING
        case ElementType::BUILDING:
            requiredProps = {"NAME", "IDENTIFIER", "CAMPUS"};
            parseHandler = [&]() {
                parsed_buildings.push_back((BuildingElement){
                    .name = parseObject.name,
                    .id = parseObject.identifier,
                    .campus_id = parseObject.campus_id
                });
            };
            break;
        //ROOM
        case ElementType::ROOM:
            requiredProps = {"NAME", "IDENTIFIER", "CAPACITY", "CAMPUS", "BUILDING"};
            parseHandler = [&]() {
                parsed_rooms.push_back((RoomElement){
                    .name = parseObject.name,
                    .id = parseObject.identifier,
                    .campus_id = parseObject.campus_id,
                    .building_id = parseObject.building_id,
                    .capacity = parseObject.capacity
                });
            };
            break;
        //MEETING
        case ElementType::MEETING:
            requiredProps = {"LABEL", "IDENTIFIER", "ROOM", "ONLINE", "DATE", "HOUR", "EXTERNALS", "CATERING"};
            parseHandler = [&]()
            {
                parsed_meetings.push_back((MeetingElement){
                    .label = parseObject.label,
                    .id = parseObject.identifier,
                    .room_id = parseObject.room_id,
                    .date_time = DateTime(parseObject.year, parseObject.month, parseObject.day, parseObject.hour),
                    .externals_allowed = parseObject.externals,
                    .catering_needed = parseObject.catering_needed,
                    .online = parseObject.online
                });
            };
            break;
        //PARTICIPATION
        case ElementType::PARTICIPATION:
            requiredProps = {"USER", "EXTERNAL", "MEETING"};
            parseHandler = [&]()
            {
                parsed_participations.push_back((ParticipationElement){
                    .meeting = parseObject.meeting_id,
                    .user = parseObject.user_id,
                    .external = parseObject.external
                });
            };
            break;
        //CATERING
        case ElementType::CATERING:
            requiredProps = {"CAMPUS", "CO2"};
            parseHandler = [&]()
            {
                parsed_caterings.push_back((CateringElement){
                    .campus_id = parseObject.campus_id,
                    .co2_count = parseObject.co2_count
                });
            };
            break;

        case ElementType::RENOVATION:
            requiredProps = {"ROOM", "START", "END"};
            parseHandler = [&]()
            {
                parsed_renovations.push_back((RenovationElement){
                    .room = parseObject.room_id,
                    .start_date = parseObject.start_date,
                    .end_date = parseObject.end_date
                });
            };

        default:
            errorStream << "Object element exists in elementValues map but not in switch case:  " << elementType << std::endl;
            break;
    }

    for (TiXmlElement* propertyObject = elementObject->FirstChildElement(); propertyObject != nullptr;
    propertyObject = propertyObject->NextSiblingElement())
    {
        std::string propType = propertyObject->Value();
        std::string propError;

        //UNKNOWN PROPERTY FOUND
        if (!requiredProps.contains(propType))
        {
            errorStream << "Unrecognized property for " << elementType << ": \"" << propType << "\"" << std::endl;
            return;
        }
        //DUPLICATE PROPERTY FOUND
        if (!foundProps.insert(propType).second)
        {
            errorStream << elementType << "element can't have more than one " << propType << " property." << std::endl;
            return;
        }


        if (!parseProperty(propertyObject, propError))
        {
            errorStream << "Failed to parse " << elementType << " element: " << std::endl;
            errorStream << "\t" << propError << std::endl;
            return;
        }

    }


    //CHECK IF ANY REQUIRED PROPERTIES ARE MISSING

    bool missing_props = false;
    for (const std::string& prop : requiredProps)
    {
        if (!foundProps.contains(prop))
        {
            errorStream << elementType << " must have a " << prop << " property" << std::endl;
            missing_props = true;
        }
    }
    if (missing_props) return;

    parseHandler();

}

bool XmlParser::parseProperty(TiXmlElement* propertyObject, std::string& parseError)
{
    std::string propType = propertyObject->Value();
    std::string prop;
    {
        const char* c_str = propertyObject->GetText();
        if (c_str == nullptr)
        {
            parseError = std::string("Property ") + propType + " needs to contain text.";
            return false;
        }
        prop = c_str;
    }

    const static std::unordered_map<std::string, PropType> propValues = {
        {"IDENTIFIER", PropType::IDENTIFIER},
        {"NAME", PropType::NAME},
        {"LABEL", PropType::LABEL},
        {"CAMPUS", PropType::CAMPUS},
        {"BUILDING", PropType::BUILDING},
        {"CAPACITY", PropType::CAPACITY},
        {"ROOM", PropType::ROOM},
        {"ONLINE", PropType::ONLINE},
        {"DATE", PropType::DATE},
        {"HOUR", PropType::HOUR},
        {"EXTERNALS", PropType::EXTERNALS},
        {"USER", PropType::USER},
        {"EXTERNAL", PropType::EXTERNAL},
        {"MEETING", PropType::MEETING},
        {"CO2", PropType::CO2},
        {"CATERING", PropType::CATERINGNEEDED},
        {"START", PropType::STARTDATE},
        {"END", PropType::ENDDATE}
    };

    if (!propValues.contains(propType))
    {
        parseError = "Unrecognized property: " + prop;
        return false;
    }

    switch (propValues.at(propType))
    {
        case PropType::IDENTIFIER:
        {
            parseObject.identifier = prop;
            break;
        }
        case PropType::NAME:
        {
            parseObject.name = prop;
            break;
        }
        case PropType::LABEL:
        {
            parseObject.label = prop;
            break;
        }
        case PropType::CAMPUS:
        {
            parseObject.campus_id = prop;
            break;
        }
        case PropType::BUILDING:
        {
            parseObject.building_id = prop;
            break;
        }
        case PropType::CAPACITY:
        {
            int capacity;
            //Try to convert to int check
            try
            {
                capacity = std::stoi(prop);
                //Check if capacity is < 0
                if (capacity < 0)
                {
                    parseError = std::string("Capacity could not be converted to an integer (value is negative)");
                    return false;
                }
            }
            catch (std::exception& except)
            {
                parseError = std::string("Capacity could not be converted to an integer\n\t- ") + except.what();
                return false;
            }
            parseObject.capacity = capacity;
            break;
        }
        case PropType::ROOM:
        {
            parseObject.room_id = prop;
            break;
        }
        case PropType::ONLINE:
        {
            bool online;
            try
            {
                online = parse_boolean(prop);
            }
            catch (std::exception& except)
            {
                parseError = std::string("ONLINE: ") + prop + std::string(" could not be converted to a bool");
                return false;
            }
            parseObject.online = online;
            break;
        }
        case PropType::DATE:
        {
            //Try to convert to DateTime check
            int day = 0;
            int month = 0;
            int year = 0;

            try
            {
                day = std::stoi(prop.substr(8, 2));
                month = std::stoi(prop.substr(5, 2));
                year = std::stoi(prop.substr(0, 4));
            }
            catch (std::exception& except)
            {
                parseError = std::string("Date value could not be converted to a date format: \n\t- ") + except.what();
                return false;
            }
            std::chrono::year_month_day chrono_date = {
                std::chrono::year(year), std::chrono::month(month), std::chrono::day(day)
            };
            //Check if date exists
            if (!chrono_date.ok() || year <= 0)
            {
                parseError = "Date " + prop + " does not exist.";
                return false;
            }

            parseObject.year = year;
            parseObject.month = month;
            parseObject.day = day;
            break;
        }
        case PropType::HOUR:
        {
            int hour;
            //Try to convert to int
            try
            {
                hour = std::stoi(prop);
            }
            catch (std::exception& except)
            {
                parseError = std::string("Hour value could not be converted to an integer: \n\t- ") + except.what();
                return false;
            }
            //Check if int >= 0 and int < 24
            if (hour < 0 || hour > 23)
            {
                parseError = std::string("Hour must be non-negative and smaller than 24, not ") + prop + ".";
                return false;
            }

            parseObject.hour = hour;
            break;
        }
        case PropType::EXTERNALS:
        {
            bool externals;
            try
            {
                externals = parse_boolean(prop);
            }
            catch (std::exception& except)
            {
                parseError = prop + std::string(" could not be converted to a bool");
                return false;
            }
            parseObject.externals = externals;
            break;
        }
        case PropType::USER:
        {
            parseObject.user_id = prop;
            break;
        }
        case PropType::EXTERNAL:
        {
            bool external;
            try
            {
                external = parse_boolean(prop);
            }
            catch (std::exception& except)
            {
                parseError = prop + std::string(" could not be converted to a bool");
                return false;
            }
            parseObject.external = external;
            break;
        }
        case PropType::MEETING:
        {
            parseObject.meeting_id = prop;
            break;
        }
        case PropType::CO2:
        {
            float co2;
            //Try to convert to int check
            try
            {
                co2 = std::stof(prop);
                //Check if co2 is < 0
                if (co2 < 0)
                {
                    parseError = std::string("CO2 could not be converted to a float (value is negative)");
                    return false;
                }
            }
            catch (std::exception& except)
            {
                parseError = std::string("CO2 could not be converted to a float\n\t- ") + except.what();
                return false;
            }
            parseObject.co2_count = co2;
            break;
        }
        case PropType::CATERINGNEEDED:
        {
            bool catering;
            try
            {
                catering = parse_boolean(prop);
            }
            catch (std::exception& except)
            {
                parseError = prop + std::string(" could not be converted to a bool");
                return false;
            }
            parseObject.catering_needed = catering;
            break;
        }
        case PropType::STARTDATE:
        {
            Date startDate;
            if (!parse_date(prop, startDate, parseError)) return false;
            parseObject.start_date = startDate;
            break;
        }
        case PropType::ENDDATE:
        {
            Date startDate;
            if (!parse_date(prop, startDate, parseError)) return false;
            parseObject.start_date = startDate;
            break;
        }
        default:
        {
            parseError = std::string("Property exists in property_map but isn't defined in switch case: ") + prop;
            return false;
        }
    }
    return true;
}