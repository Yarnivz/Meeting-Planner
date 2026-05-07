//
// Created by s0255946 on 18/03/2026.
//

#include "XmlParser.h"

#include <filesystem>
#include <unordered_set>
#include <unordered_map>
#include <functional>
#include <objects/Date.h>

#include "helper/DesignByContract.h"


static inline bool parse_int(const std::string& input, int& output, const Error& error, const std::string& propertyKind_for_error)
{
    int integer;
    //Try to convert to int
    try
    {
        integer = std::stoi(input);
    }
    catch (std::exception& except)
    {
        error.propertyFailedToParse(propertyKind_for_error, input, "int", "");
        return false;
    }
    output = integer;
    return true;
}

static inline bool parse_float(const std::string& input, float& output, const Error& error, const std::string& propertyKind_for_error)
{
    float floating;
    //Try to convert to float
    try
    {
        floating = std::stof(input);
    }
    catch (std::exception& except)
    {
        error.propertyFailedToParse(propertyKind_for_error, input, "float", "");
        return false;
    }
    output = floating;
    return true;
}

static inline bool parse_boolean(const std::string& input, bool& output, const Error& error, const std::string& propertyKind_for_error)
{
    std::string s = input;
    std::transform(s.begin(), s.end(), s.begin(), ::tolower);

    if (s == "true" || s == "1")
    {
        output = true;
        return true;
    }

    if (s == "false" || s == "0")
    {
        output = false;
        return true;
    }

    error.propertyFailedToParse(propertyKind_for_error, input, "boolean", "Value must be one of '0'/'1'/'false'/'true'!");
    return false;
}

static inline bool parse_date (const std::string& input, Date& output, const Error& error, const std::string& propertyKind_for_error)
{
    //Try to convert to Date
    int day;
    int month;
    int year;

    try
    {
        day = std::stoi(input.substr(8, 2));
        month = std::stoi(input.substr(5, 2));
        year = std::stoi(input.substr(0, 4));
    }
    catch (std::exception& except)
    {
        error.propertyFailedToParse(propertyKind_for_error, input, "date", "Value must be in the format: YYYY-MM-DD.");
        return false;
    }
    const std::chrono::year_month_day chrono_date = {
        std::chrono::year(year), std::chrono::month(month), std::chrono::day(day)
    };
    //Check if date exists
    if (!chrono_date.ok() || year <= 0)
    {
        error.propertyFailedToParse(propertyKind_for_error, input, "date", "This date does not exist.");
        return false;
    }

    output = Date(year, month, day);
    return true;
}

XmlParser::XmlParser(const std::string& filepath) : filepath(filepath) {}

void XmlParser::parse(const Error& error)
{
    REQUIRE(!filepath.empty(), "The file cannot be empty");
    TiXmlDocument doc;

    if (!std::filesystem::exists(filepath))
    {
        error.fileNotFound(filepath);
        return;
    }

    if (!doc.LoadFile(filepath.c_str()))
    {
        error.fileInvalidXml(filepath);
        return;
    }


    TiXmlElement* root = doc.FirstChildElement();
    if (root == nullptr)
    {
        error.syntaxNoRootElement();
        doc.Clear();
        return;
    }


    for (TiXmlElement* objectElement = root->FirstChildElement(); objectElement != nullptr; objectElement =
         objectElement->NextSiblingElement())
    {
        parseElement(objectElement, error);
    }
}

void XmlParser::parseElement(TiXmlElement* elementObject, const Error& error)
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



    const std::string objectKind = elementObject->Value();

    if (!elementValues.contains(objectKind))
    {
        error.objectUnrecognizedKind(objectKind);
        return;
    }

    std::unordered_set<std::string> requiredProps;
    std::unordered_set<std::string> foundProps;
    std::function<void()> parseHandler;

    switch (elementValues.at(objectKind))
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
                    .date_time = DateTime(parseObject.date, parseObject.hour),
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
            break;

        default:
            error.INTERNAL_objectSwitchCaseMissing(objectKind);
            break;
    }

    for (TiXmlElement* propertyObject = elementObject->FirstChildElement(); propertyObject != nullptr;
    propertyObject = propertyObject->NextSiblingElement())
    {
        std::string propertyKind = propertyObject->Value();

        //UNKNOWN PROPERTY FOUND
        if (!requiredProps.contains(propertyKind))
        {
            error.objectUnrecognizedProperty(propertyKind, objectKind);
            return;
        }
        //DUPLICATE PROPERTY FOUND
        if (!foundProps.insert(propertyKind).second)
        {
            error.objectDuplicateProperty(propertyKind, objectKind);
            return;
        }


        if (!parseProperty(propertyObject, error)) return;
    }


    //CHECK IF ANY REQUIRED PROPERTIES ARE MISSING

    bool missing_props = false;
    for (const std::string& requiredPropertyKind : requiredProps)
    {
        if (!foundProps.contains(requiredPropertyKind))
        {
            error.objectMissingProperty(requiredPropertyKind, objectKind);
            missing_props = true;
        }
    }
    if (missing_props) return;

    parseHandler();

}

bool XmlParser::parseProperty(TiXmlElement* propertyObject, const Error& error)
{
    std::string propertyKind = propertyObject->Value();
    std::string propertyContents;
    {
        const char* c_str = propertyObject->GetText();
        if (c_str == nullptr)
        {
            error.propertyEmpty(propertyKind);
            return false;
        }
        propertyContents = c_str;
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

    if (!propValues.contains(propertyKind))
    {
        error.propertyUnrecognizedKind(propertyKind);
        return false;
    }

    switch (propValues.at(propertyKind))
    {
        case PropType::IDENTIFIER:
        {
            parseObject.identifier = propertyContents;
            break;
        }
        case PropType::NAME:
        {
            parseObject.name = propertyContents;
            break;
        }
        case PropType::LABEL:
        {
            parseObject.label = propertyContents;
            break;
        }
        case PropType::CAMPUS:
        {
            parseObject.campus_id = propertyContents;
            break;
        }
        case PropType::BUILDING:
        {
            parseObject.building_id = propertyContents;
            break;
        }
        case PropType::CAPACITY:
        {
            int capacity;
            if (!parse_int(propertyContents, capacity, error, propertyKind)) return false;
            if (capacity < 0)
            {
                error.propertyNegative(propertyKind, propertyContents);
                return false;
            }
            parseObject.capacity = capacity;
            break;
        }
        case PropType::ROOM:
        {
            parseObject.room_id = propertyContents;
            break;
        }
        case PropType::ONLINE:
        {
            bool online;
            if (!parse_boolean(propertyContents, online, error, propertyKind)) return false;
            parseObject.online = online;
            break;
        }
        case PropType::DATE:
        {
            Date date = Date();
            if (!parse_date(propertyContents, date, error, propertyKind)) return false;
            parseObject.date = date;
            break;
        }
        case PropType::HOUR:
        {
            int hour;
            if (!parse_int(propertyContents, hour, error, propertyKind)) return false;
            //Check if int >= 0 and int < 24
            if (hour < 0 || hour > 23)
            {
                error.propertyOutOfRange(propertyKind, propertyContents, 0, 23);
                return false;
            }

            parseObject.hour = hour;
            break;
        }
        case PropType::EXTERNALS:
        {
            bool externals;
            if (!parse_boolean(propertyContents, externals, error, propertyKind))
            {
                error.propertyFailedToParse(propertyKind, propertyContents, "boolean", "0/1 or false/true");
                return false;
            }
            parseObject.externals = externals;
            break;
        }
        case PropType::USER:
        {
            parseObject.user_id = propertyContents;
            break;
        }
        case PropType::EXTERNAL:
        {
            bool external;
            if (!parse_boolean(propertyContents, external, error, propertyKind)) return false;
            parseObject.external = external;
            break;
        }
        case PropType::MEETING:
        {
            parseObject.meeting_id = propertyContents;
            break;
        }
        case PropType::CO2:
        {
            float co2;
            if (!parse_float(propertyContents, co2, error, propertyKind)) return false;
            //Check if co2 is < 0
            if (co2 < 0)
            {
                error.propertyNegative(propertyKind, propertyContents);
                return false;
            }
            parseObject.co2_count = co2;
            break;
        }
        case PropType::CATERINGNEEDED:
        {
            bool catering;
            if (!parse_boolean(propertyContents, catering, error, propertyKind)) return false;
            parseObject.catering_needed = catering;
            break;
        }
        case PropType::STARTDATE:
        {
            Date startDate;
            if (!parse_date(propertyContents, startDate, error, propertyKind)) return false;
            parseObject.start_date = startDate;
            break;
        }
        case PropType::ENDDATE:
        {
            Date endDate;
            if (!parse_date(propertyContents, endDate, error, propertyKind)) return false;
            parseObject.end_date = endDate;
            break;
        }
        default:
        {
            error.INTERNAL_propertySwitchCaseMissing(propertyKind);
            return false;
        }
    }
    return true;
}