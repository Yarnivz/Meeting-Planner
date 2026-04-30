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

static inline bool parse_boolean(const std::string& s)
{
    if (s == "true") return true;
    return false;
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
    //     //== The type of the element 'ROOM'/'MEETING'/...
    //     const std::string objectElementType = objectElement->Value();
    //
    //     if (objectElementType == "CAMPUS")
    //     {
    //         //== string properties from child XML elements
    //         std::string name, identifier;
    //
    //         //== booleans indicating whether the above properties were already found
    //         bool found_name = false;
    //         bool found_id = false;
    //
    //         for (TiXmlElement* propertyElement = objectElement->FirstChildElement(); propertyElement != nullptr;
    //              propertyElement = propertyElement->NextSiblingElement())
    //         {
    //             const std::string propertyElementType = propertyElement->Value();
    //
    //             if (propertyElement->FirstChild() == nullptr)
    //             {
    //                 //TODO o.emptyElement();
    //                 errorStream << "Property " << propertyElementType << " needs to contain text." << std::endl;
    //                 goto continue_to_next_object_element;
    //             }
    //
    //             const std::string tempElementChildValue = propertyElement->FirstChild()->Value();
    //
    //             if (propertyElementType == "NAME")
    //             {
    //                 //> Check if we already encountered another <NAME> tag.
    //                 //  Which would mean multiple <NAME> tags are present => ERROR
    //                 if (found_name)
    //                 {
    //                     errorStream << "CAMPUS element " << name << " cant have more than one NAME property." << std::endl;
    //                     goto continue_to_next_object_element;
    //                 }
    //
    //                 found_name = true;
    //                 name = tempElementChildValue;
    //             }
    //             else if (propertyElementType == "IDENTIFIER")
    //             {
    //                 //> Check if we already encountered another <IDENTIFIER> tag.
    //                 //  Which would mean multiple <IDENTIFIER> tags are present => ERROR
    //                 if (found_id)
    //                 {
    //                     errorStream << "CAMPUS element " << identifier << " cant have more than one IDENTIFIER property." << std::endl;
    //                     goto continue_to_next_object_element;
    //                 }
    //
    //                 found_id = true;
    //                 identifier = tempElementChildValue;
    //             }
    //             else
    //             {
    //                 //> Filter out any other unrecognized tags
    //                 errorStream << "Unrecognized property for CAMPUS: \"" << propertyElementType << "\"" << std::endl;
    //                 goto continue_to_next_object_element;
    //             }
    //         }
    //
    //         //> Check if all required properties were provided
    //         if (!found_id)
    //         {
    //             if (found_name) errorStream << "CAMPUS \'"<< name << "\' must have a IDENTIFIER property" << std::endl;
    //             else errorStream << "CAMPUS must have a IDENTIFIER property" << std::endl;
    //             goto continue_to_next_object_element;
    //         }
    //         if (!found_name)
    //         {
    //             errorStream << "CAMPUS \'" << identifier << "\' must have a NAME property" << std::endl;
    //             goto continue_to_next_object_element;
    //         }
    //
    //
    //         //> Add campus if all the checks have passed
    //         parsed_campuses.push_back((CampusElement){.name = name, .id = identifier});
    //     }
    //     else if (objectElementType == "BUILDING")
    //     {
    //         //== string properties from child XML elements
    //         std::string name, identifier, campus_id;
    //
    //         //== booleans indicating whether the above properties were already found
    //         bool found_name = false;
    //         bool found_id = false;
    //         bool found_campus = false;
    //
    //         for (TiXmlElement* propertyElement = objectElement->FirstChildElement(); propertyElement != nullptr;
    //              propertyElement = propertyElement->NextSiblingElement())
    //         {
    //             std::string propertyElementType = propertyElement->Value();
    //
    //             if (propertyElement->FirstChild() == nullptr)
    //             {
    //                 //TODO o.emptyElement();
    //                 errorStream << "Property " << propertyElementType << " needs to contain text." << std::endl;
    //                 goto continue_to_next_object_element;
    //             }
    //
    //             std::string tempElementChildValue = propertyElement->FirstChild()->Value();
    //
    //             if (propertyElementType == "NAME")
    //             {
    //                 //> Check if we already encountered another <NAME> tag.
    //                 //  Which would mean multiple <NAME> tags are present => ERROR
    //                 if (found_name)
    //                 {
    //                     errorStream << "BUILDING element cant have more than one NAME property." << std::endl;
    //                     goto continue_to_next_object_element;
    //                 }
    //
    //                 found_name = true;
    //                 name = tempElementChildValue;
    //             }
    //             else if (propertyElementType == "IDENTIFIER")
    //             {
    //                 //> Check if we already encountered another <IDENTIFIER> tag.
    //                 //  Which would mean multiple <IDENTIFIER> tags are present => ERROR
    //                 if (found_id)
    //                 {
    //                     errorStream << "BUILDING element cant have more than one IDENTIFIER property." << std::endl;
    //                     goto continue_to_next_object_element;
    //                 }
    //
    //                 found_id = true;
    //                 identifier = tempElementChildValue;
    //             }
    //             else if (propertyElementType == "CAMPUS")
    //             {
    //                 //> Check if we already encountered another <CAMPUS> tag.
    //                 // Which would mean multiple <CAMPUS> tags are present => ERROR
    //                 if (found_campus)
    //                 {
    //                     errorStream << "BUILDING element cant have more than one CAMPUS property" << std::endl;
    //                     goto continue_to_next_object_element;
    //                 }
    //
    //                 found_campus = true;
    //                 campus_id = tempElementChildValue;
    //             }
    //             else
    //             {
    //                 //> Filter out any other unrecognized tags
    //                 errorStream << "Unrecognized property for BUILDING: \"" << propertyElementType << "\"" << std::endl;
    //                 goto continue_to_next_object_element;
    //             }
    //         }
    //
    //         //> Check if all required properties were provided
    //         if (!found_name)
    //         {
    //             errorStream << "BUILDING must have a NAME property" << std::endl;
    //             goto continue_to_next_object_element;
    //         }
    //         if (!found_id)
    //         {
    //             errorStream << "BUILDING must have a IDENTIFIER property" << std::endl;
    //             goto continue_to_next_object_element;
    //         }
    //         if (!found_campus)
    //         {
    //             errorStream << "BUILDING must have a CAMPUS property" << std::endl;
    //             goto continue_to_next_object_element;
    //         }
    //
    //
    //         //> Add building if all the checks have passed
    //         parsed_buildings.push_back((BuildingElement){.name = name, .id = identifier, .campus_id = campus_id});
    //     }
    //
    //     else if (objectElementType == "ROOM")
    //     {
    //         //== string properties from child XML elements
    //         std::string name, identifier, capacity_str, campus_id, building_id;
    //
    //         //== booleans indicating whether the above properties were already found
    //         bool found_name = false;
    //         bool found_id = false;
    //         bool found_capacity = false;
    //         bool found_campus = false;
    //         bool found_building = false;
    //
    //
    //         for (TiXmlElement* propertyElement = objectElement->FirstChildElement(); propertyElement != nullptr;
    //              propertyElement = propertyElement->NextSiblingElement())
    //         {
    //             const std::string propertyElementType = propertyElement->Value();
    //
    //             if (propertyElement->FirstChild() == nullptr)
    //             {
    //                 //TODO o.emptyElement();
    //                 errorStream << "Property " << propertyElementType << " needs to contain text." << std::endl;
    //                 goto continue_to_next_object_element;
    //             }
    //
    //
    //             const std::string tempElementChildValue = propertyElement->FirstChild()->Value();
    //
    //             if (propertyElementType == "NAME")
    //             {
    //                 //> Check if we already encountered another <NAME> tag.
    //                 //  Which would mean multiple <NAME> tags are present => ERROR
    //                 if (found_name)
    //                 {
    //                     errorStream << "ROOM element cant have more than one NAME property." << std::endl;
    //                     goto continue_to_next_object_element;
    //                 }
    //
    //                 found_name = true;
    //                 name = tempElementChildValue;
    //             }
    //             else if (propertyElementType == "IDENTIFIER")
    //             {
    //                 //> Check if we already encountered another <IDENTIFIER> tag.
    //                 //  Which would mean multiple <IDENTIFIER> tags are present => ERROR
    //                 if (found_id)
    //                 {
    //                     errorStream << "ROOM element cant have more than one IDENTIFIER property." << std::endl;
    //                     goto continue_to_next_object_element;
    //                 }
    //
    //                 found_id = true;
    //                 identifier = tempElementChildValue;
    //             }
    //             else if (propertyElementType == "CAPACITY")
    //             {
    //                 //> Check if we already encountered another <CAPACITY> tag.
    //                 //  Which would mean multiple <CAPACITY> tags are present => ERROR
    //                 if (found_capacity)
    //                 {
    //                     errorStream << "ROOM element cant have more than one CAPACITY property." << std::endl;
    //                     goto continue_to_next_object_element;
    //                 }
    //
    //                 found_capacity = true;
    //                 capacity_str = tempElementChildValue;
    //             }
    //             else if (propertyElementType == "CAMPUS")
    //             {
    //                 //> Check if we already encountered another <CAMPUS> tag.
    //                 //  Which would mean multiple <CAMPUS> tags are present => ERROR
    //                 if (found_campus)
    //                 {
    //                     errorStream << "ROOM element cant have more than one CAMPUS property." << std::endl;
    //                     goto continue_to_next_object_element;
    //                 }
    //
    //                 found_campus = true;
    //                 campus_id = tempElementChildValue;
    //             }
    //             else if (propertyElementType == "BUILDING")
    //             {
    //                 //> Check if we already encountered another <BUILDING> tag.
    //                 //  Which would mean multiple <BUILDING> tags are present => ERROR
    //                 if (found_building)
    //                 {
    //                     errorStream << "ROOM element cant have more than one BUILDING property." << std::endl;
    //                     goto continue_to_next_object_element;
    //                 }
    //
    //                 found_building = true;
    //                 building_id = tempElementChildValue;
    //             }
    //             else
    //             {
    //                 //> Filter out any other unrecognized tags
    //                 errorStream << "Unrecognized property for ROOM: \"" << propertyElementType << "\"" << std::endl;
    //                 goto continue_to_next_object_element;
    //             }
    //         }
    //
    //
    //         //> Check if all required properties were provided
    //         if (!found_name)
    //         {
    //             errorStream << "ROOM must have a NAME property" << std::endl;
    //             goto continue_to_next_object_element;
    //         }
    //         if (!found_id)
    //         {
    //             errorStream << "ROOM must have a IDENTIFIER property" << std::endl;
    //             goto continue_to_next_object_element;
    //         }
    //         if (!found_capacity)
    //         {
    //             errorStream << "ROOM must have a CAPACITY property" << std::endl;
    //             goto continue_to_next_object_element;
    //         }
    //         /*if (!found_campus)
    //         {
    //             errorStream << "ROOM must have a CAMPUS property" << std::endl;
    //             goto continue_to_next_object_element;
    //         }
    //         if (!found_building)
    //         {
    //             errorStream << "ROOM must have a BUILDING property" << std::endl;
    //             goto continue_to_next_object_element;
    //         }*/
    //
    //         //== 'capacity' string parsed into an int
    //         int capacity = 0;
    //
    //         //> Try to parse 'capacity' to int.
    //         try
    //         {
    //             capacity = std::stoi(capacity_str);
    //         }
    //         catch (std::exception& except)
    //         {
    //             errorStream << "Capacity could not be converted to an integer. Room will not be added \n\t- " << except.
    //                 what() << std::endl;
    //             goto continue_to_next_object_element;
    //         }
    //
    //         //> Check if capacity is larger than 0
    //         if (capacity <= 0)
    //         {
    //             errorStream << "Room capacity needs to be larger than 0. Room will not be added." << std::endl;
    //             goto continue_to_next_object_element;
    //         }
    //
    //
    //         //> Add room if all the checks have passed
    //         parsed_rooms.push_back((RoomElement)
    //         {
    //             .
    //             name = std::move(name),
    //             .
    //             id = std::move(identifier),
    //             .
    //             capacity = capacity
    //         });
    //     }
    //     else if (objectElementType == "MEETING")
    //     {
    //         //== string properties from child XML elements
    //         std::string label, identifier, room, date_string, hour_string, externals_allowed_string, catering_string;
    //
    //         //== booleans indicating whether the above properties were already found
    //         bool found_label = false;
    //         bool found_id = false;
    //         bool found_room = false;
    //         bool found_datestring = false;
    //         bool found_hourstring = false;
    //         bool found_externals_allowed = false;
    //         bool found_cateringstring = false;
    //
    //
    //         for (TiXmlElement* propertyElement = objectElement->FirstChildElement(); propertyElement != nullptr;
    //              propertyElement = propertyElement->NextSiblingElement())
    //         {
    //             const std::string propertyElementType = propertyElement->Value();
    //
    //             if (propertyElement->FirstChild() == nullptr)
    //             {
    //                 errorStream << "Property " << propertyElementType << " needs to contain text." << std::endl;
    //                 goto continue_to_next_object_element;;
    //             }
    //
    //             const std::string propertyElementContents = propertyElement->FirstChild()->Value();
    //
    //             if (propertyElementType == "LABEL")
    //             {
    //                 //> Check if we already encountered another <LABEL> tag.
    //                 //  Which would mean multiple <LABEL> tags are present => ERROR
    //                 if (found_label)
    //                 {
    //                     errorStream << "MEETING element cant have more than one LABEL property." << std::endl;
    //                     goto continue_to_next_object_element;
    //                 }
    //
    //                 found_label = true;
    //                 label = propertyElementContents;
    //             }
    //             else if (propertyElementType == "IDENTIFIER")
    //             {
    //                 //> Check if we already encountered another <IDENTIFIER> tag.
    //                 //  Which would mean multiple <IDENTIFIER> tags are present => ERROR
    //                 if (found_id)
    //                 {
    //                     errorStream << "MEETING element cant have more than one IDENTIFIER property." << std::endl;
    //                     goto continue_to_next_object_element;
    //                 }
    //
    //                 found_id = true;
    //                 identifier = propertyElementContents;
    //             }
    //             else if (propertyElementType == "ROOM")
    //             {
    //                 //> Check if we already encountered another <ROOM> tag.
    //                 //  Which would mean multiple <ROOM> tags are present => ERROR
    //                 if (found_room)
    //                 {
    //                     errorStream << "MEETING element cant have more than one ROOM property." << std::endl;
    //                     goto continue_to_next_object_element;
    //                 }
    //
    //                 found_room = true;
    //                 room = propertyElementContents;
    //             }
    //             else if (propertyElementType == "DATE")
    //             {
    //                 //> Check if we already encountered another <DATE> tag.
    //                 //  Which would mean multiple <DATE> tags are present => ERROR
    //                 if (found_datestring)
    //                 {
    //                     errorStream << "MEETING element cant have more than one DATE property." << std::endl;
    //                     goto continue_to_next_object_element;
    //                 }
    //
    //                 found_datestring = true;
    //                 date_string = propertyElementContents;
    //             }
    //             else if (propertyElementType == "HOUR")
    //             {
    //                 //> Check if we already encountered another <HOUR> tag.
    //                 //  Which would mean multiple <HOUR> tags are present => ERROR
    //                 if (found_hourstring)
    //                 {
    //                     errorStream << "MEETING element cant have more than one HOUR property." << std::endl;
    //                     goto continue_to_next_object_element;
    //                 }
    //
    //                 found_hourstring = true;
    //                 hour_string = propertyElementContents;
    //             }
    //             else if (propertyElementType == "EXTERNALS")
    //             {
    //                 if (found_externals_allowed)
    //                 {
    //                     errorStream << "MEETING element cant have more than one EXTERNALS property." << std::endl;
    //                     goto continue_to_next_object_element;
    //                 }
    //
    //                 found_externals_allowed = true;
    //                 externals_allowed_string = propertyElementContents;
    //             }
    //             else if (propertyElementType == "CATERING")
    //             {
    //                 if (found_cateringstring)
    //                 {
    //                     errorStream << "MEETING element cant have more than one CATERING property." << std::endl;
    //                     goto continue_to_next_object_element;
    //                 }
    //
    //                 found_cateringstring = true;
    //                 catering_string = propertyElementContents;
    //             }
    //             else
    //             {
    //                 //> Filter out any other unrecognized tags
    //                 errorStream << "Unrecognized property for MEETING: \"" << propertyElementType << "\"" << std::endl;
    //                 goto continue_to_next_object_element;
    //             }
    //         }
    //
    //
    //         //> Check if all required properties were provided
    //         if (!found_id)
    //         {
    //             errorStream << "MEETING must have a IDENTIFIER property" << std::endl;
    //             goto continue_to_next_object_element;
    //         }
    //         if (!found_label)
    //         {
    //             errorStream << "MEETING must have a LABEL property" << std::endl;
    //             goto continue_to_next_object_element;
    //         }
    //         if (!found_room)
    //         {
    //             errorStream << "MEETING must have a ROOM property" << std::endl;
    //             goto continue_to_next_object_element;
    //         }
    //         if (!found_datestring)
    //         {
    //             errorStream << "MEETING must have a DATE property" << std::endl;
    //             goto continue_to_next_object_element;
    //         }
    //         if (!found_hourstring)
    //         {
    //             errorStream << "MEETING must have an HOUR property" << std::endl;
    //             goto continue_to_next_object_element;
    //         }
    //
    //         if (!found_externals_allowed)
    //         { /* Is allowed, optional property for now */ }
    //
    //         if (!found_cateringstring)
    //         { /* Is allowed, optional property for now */ }
    //
    //
    //         int day, month, year, hour;
    //
    //         //> Try to parse date
    //         try
    //         {
    //             day = std::stoi(date_string.substr(8, 2));
    //             month = std::stoi(date_string.substr(5, 2));
    //             year = std::stoi(date_string.substr(0, 4));
    //         }
    //         catch (std::exception& except)
    //         {
    //             errorStream << "MEETING \'" << identifier <<
    //                 "\': Date value could not be converted to a date format: \n\t- " << except.what() << std::endl;
    //             goto continue_to_next_object_element;
    //         }
    //
    //         std::chrono::year_month_day chrono_date = {
    //             std::chrono::year(year), std::chrono::month(month), std::chrono::day(day)
    //         };
    //         //> Check if date exists
    //         if (!chrono_date.ok())
    //         {
    //             errorStream << "MEETING \'" << identifier << "\': Date " << date_string << " does not exist." <<
    //                 std::endl;
    //             goto continue_to_next_object_element;;
    //         }
    //
    //         try
    //         {
    //             hour = std::stoi(hour_string);
    //         }
    //         catch (std::exception& except)
    //         {
    //             errorStream << "MEETING \'" << identifier << "\': "
    //                 "Hour value could not be converted to an integer: \n\t- " << except.what() << std::endl;
    //             goto continue_to_next_object_element;
    //         }
    //
    //         if (hour < 0 || hour > 23)
    //         {
    //             errorStream << "MEETING \'" << identifier << "\': Hour must be non-negative and smaller than 24, not " << hour << "." << std::endl;
    //             goto continue_to_next_object_element;
    //         }
    //
    //
    //         // Parse boolean or give default value
    //         bool externals_allowed = found_externals_allowed ? parse_boolean(externals_allowed_string) : false;
    //         bool catering_needed = found_cateringstring ? parse_boolean(catering_string) : false;
    //
    //
    //         //> Add meeting if all checks passed
    //         parsed_meetings.push_back(
    //             (MeetingElement)
    //         {
    //             .label = std::move(label),
    //             .id = std::move(identifier),
    //             .room_id = std::move(room),
    //             .date_time = DateTime(year, month, day, hour),
    //             .externals_allowed = externals_allowed,
    //             .catering_needed = catering_needed
    //         });
    //     }
    //     else if (objectElementType == "PARTICIPATION")
    //     {
    //         //== string properties from child XML elements
    //         std::string meeting, user, external_string;
    //
    //         //== booleans indicating whether the above properties were already found
    //         bool found_meeting = false;
    //         bool found_user = false;
    //         bool found_external_string = false;
    //
    //
    //         for (TiXmlElement* propertyElement = objectElement->FirstChildElement(); propertyElement != nullptr;
    //              propertyElement = propertyElement->NextSiblingElement())
    //         {
    //             const std::string propertyElementType = propertyElement->Value();
    //
    //             if (propertyElement->FirstChild() == nullptr)
    //             {
    //                 errorStream << "Property " << propertyElementType << " needs to contain text." << std::endl;
    //                 goto continue_to_next_object_element;;
    //             }
    //
    //
    //             const std::string tempElementChildValue = propertyElement->FirstChild()->Value();
    //
    //             if (propertyElementType == "MEETING")
    //             {
    //                 if (found_meeting)
    //                 {
    //                     errorStream << "PARTICIPATION element cant have more than one MEETING property." << std::endl;
    //                     goto continue_to_next_object_element;
    //                 }
    //
    //                 found_meeting = true;
    //                 meeting = tempElementChildValue;
    //             }
    //             else if (propertyElementType == "USER")
    //             {
    //                 if (found_user)
    //                 {
    //                     errorStream << "PARTICIPATION element cant have more than one USER property." << std::endl;
    //                     goto continue_to_next_object_element;
    //                 }
    //
    //                 found_user = true;
    //                 user = tempElementChildValue;
    //             }
    //             else if (propertyElementType == "EXTERNAL")
    //             {
    //                 if (found_external_string)
    //                 {
    //                     errorStream << "PARTICIPATION element cant have more than one EXTERNAL property." << std::endl;
    //                     goto continue_to_next_object_element;
    //                 }
    //
    //                 found_external_string = true;
    //                 external_string = tempElementChildValue;
    //             }
    //             else
    //             {
    //                 //> Filter out any other unrecognized tags
    //                 errorStream << "Unrecognized property for PARTICIPATION: \"" << propertyElementType << "\"" << std::endl;
    //                 goto continue_to_next_object_element;
    //             }
    //         }
    //
    //
    //         //> Check if all required properties are present
    //         if (!found_user)
    //         {
    //             errorStream << "PARTICIPATION must have a USER property" << std::endl;
    //             goto continue_to_next_object_element;
    //         }
    //         if (!found_meeting)
    //         {
    //             errorStream << "PARTICIPATION must have a MEETING property" << std::endl;
    //             goto continue_to_next_object_element;
    //         }
    //
    //         if (!found_external_string)
    //         {
    //             /* Is allowed, optional property */
    //         }
    //
    //
    //
    //         bool external = found_external_string ? parse_boolean(external_string) : false;
    //
    //
    //         //> Add participation if all checks passed
    //         parsed_participations.push_back((ParticipationElement)
    //         {
    //             .meeting = std::move(meeting),
    //             .user = std::move(user),
    //             .external = external
    //         });
    //     } else if (objectElementType == "CATERING")
    //     {
    //         std::string campus_id, co2_string;
    //         bool found_campus_id = false;
    //         bool found_co2_string = false;
    //
    //         for (TiXmlElement* propertyElement = objectElement->FirstChildElement(); propertyElement != nullptr;
    //              propertyElement = propertyElement->NextSiblingElement())
    //         {
    //             const std::string propertyElementType = propertyElement->Value();
    //
    //             if (propertyElement->FirstChild() == nullptr)
    //             {
    //                 errorStream << "Property " << propertyElementType << " needs to contain text." << std::endl;
    //                 goto continue_to_next_object_element;;
    //             }
    //
    //
    //             const std::string tempElementChildValue = propertyElement->FirstChild()->Value();
    //
    //             if (propertyElementType == "CAMPUS")
    //             {
    //                 if (found_campus_id)
    //                 {
    //                     errorStream << "CATERING element cant have more than one CAMPUS property." << std::endl;
    //                     goto continue_to_next_object_element;
    //                 }
    //
    //                 found_campus_id = true;
    //                 campus_id = tempElementChildValue;
    //             }
    //             else if (propertyElementType == "CO2")
    //             {
    //                 if (found_co2_string)
    //                 {
    //                     errorStream << "CATERING element cant have more than one CO2 property." << std::endl;
    //                     goto continue_to_next_object_element;
    //                 }
    //
    //                 found_co2_string = true;
    //                 co2_string = tempElementChildValue;
    //             }
    //         }
    //
    //
    //         if (!found_campus_id)
    //         {
    //             errorStream << "CATERING must have a CAMPUS property" << std::endl;
    //             goto continue_to_next_object_element;
    //         }
    //         if (!found_co2_string)
    //         {
    //             errorStream << "CATERING must have a CO2 property" << std::endl;
    //             goto continue_to_next_object_element;
    //         }
    //
    //         float co2 = -1.0;
    //         try
    //         {
    //             co2 = std::stof(co2_string);
    //         } catch (std::exception& e)
    //         {
    //             errorStream << "Failed to parse " << co2_string << " as float: " << e.what() << std::endl;
    //             goto continue_to_next_object_element;
    //         }
    //
    //         if (co2 < 0.0)
    //         {
    //             errorStream << "CATERING for campus " << campus_id << " can't have negative emissions: " << co2_string << std::endl;
    //             goto continue_to_next_object_element;
    //         }
    //
    //         parsed_caterings.push_back((CateringElement){
    //             .campus_id = campus_id,
    //             .co2_count = co2
    //         });
    //     }
    //     else
    //     {
    //         errorStream << "Unrecognized object element: " << objectElementType << std::endl;
    //         goto continue_to_next_object_element;
    //     }
    //
    // continue_to_next_object_element:;
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
        {"PARTICIPATION", ElementType::PARTICIPATION}
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
            requiredProps = {"LABEL", "IDENTIFIER", "ROOM", "ONLINE", "DATE", "HOUR", "EXTERNALS"};
            parseHandler = [&]()
            {
                parsed_meetings.push_back((MeetingElement){
                    .label = parseObject.label,
                    .id = parseObject.identifier,
                    .room_id = parseObject.room_id,
                    .date_time = DateTime(parseObject.year, parseObject.month, parseObject.day, parseObject.hour),
                    .externals_allowed = parseObject.externals,
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
    if (foundProps.size() != requiredProps.size())
    {
        for (const std::string& prop : requiredProps)
        {
            if (!foundProps.contains(prop))
            {
                errorStream << elementType << " must have a " << prop << " property" << std::endl;
            }
        }
    }

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
            parseError = std::string("Property") + propType + " needs to contain text.";
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
                }
                catch (std::exception& except)
                {
                    parseError = std::string("Capacity could not be converted to an integer\n\t- ") + except.what();
                    return false;
                }
                //Check if capacity is < 0
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
                    //unclear wether the xml ONLINE prop has uppercases so converting all to lowercase for good measure
                    std::transform(prop.begin(), prop.end(), prop.begin(), ::tolower);
                    if (prop == "true")
                    {
                        online = true;
                    } else if (prop == "false")
                    {
                        online = false;
                    } else
                    {
                        parseError = std::string("ONLINE could not be converted to a bool (string is not a valid bool keyword)");
                        return false;
                    }
                    online = prop.data();
                }
                catch (std::exception& except)
                {
                    parseError = std::string("ONLINE could not be converted to a bool\n\t- ") + except.what();
                    return false;
                }
                parseObject.online = online;
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
                parseObject.externals = parse_boolean(prop);
                break;
            }
        case PropType::USER:
            {
                parseObject.user_id = prop;
                break;
            }
        case PropType::EXTERNAL:
            {
                parseObject.external = parse_boolean(prop);
                break;
            }
        case PropType::MEETING:
            {
                parseObject.meeting_id = prop;
                break;
            }
        default:
            {
                parseError = "Property exists in property_map but isnt defined in switch case: " + prop;
                return false;
            }
    }
    return true;
}