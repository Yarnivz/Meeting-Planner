//
// Created by s0255946 on 18/03/2026.
//

#include "XmlParser.h"

#include <unordered_map>
#include <unordered_set>

#include "helper/DesignByContract.h"
#include "tinyxml.h"
#include "objects/Date.h"
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
        //== The type of the element 'ROOM'/'MEETING'/...
        std::string objectElementType = objectElement->Value();

        if (objectElementType == "CAMPUS")
        {
            //== string properties from child XML elements
            std::string name, identifier;

            //== booleans indicating whether the above properties were already found
            bool found_name = false;
            bool found_id = false;

            for (TiXmlElement* propertyElement = objectElement->FirstChildElement(); propertyElement != nullptr;
                 propertyElement = propertyElement->NextSiblingElement())
            {
                std::string propertyElementType = propertyElement->Value();

                if (propertyElement->FirstChild() == nullptr)
                {
                    //TODO o.emptyElement();
                    errorStream << "Property " << propertyElementType << " needs to contain text." << std::endl;
                    goto continue_to_next_object_element;
                }

                std::string tempElementChildValue = propertyElement->FirstChild()->Value();

                if (propertyElementType == "NAME")
                {
                    //> Check if we already encountered another <NAME> tag.
                    //  Which would mean multiple <NAME> tags are present => ERROR
                    if (found_name)
                    {
                        errorStream << "BUILDING element cant have more than one NAME property." << std::endl;
                        goto continue_to_next_object_element;
                    }

                    found_name = true;
                    name = tempElementChildValue;
                }
                else if (propertyElementType == "IDENTIFIER")
                {
                    //> Check if we already encountered another <IDENTIFIER> tag.
                    //  Which would mean multiple <IDENTIFIER> tags are present => ERROR
                    if (found_id)
                    {
                        errorStream << "BUILDING element cant have more than one IDENTIFIER property." << std::endl;
                        goto continue_to_next_object_element;
                    }

                    found_id = true;
                    identifier = tempElementChildValue;
                }
                else
                {
                    //> Filter out any other unrecognized tags
                    errorStream << "Unrecognized property for CAMPUS: \"" << propertyElementType << "\"" << std::endl;
                    goto continue_to_next_object_element;
                }

                //> Check if all required properties were provided
                if (!found_name)
                {
                    errorStream << "CAMPUS must have a NAME property" << std::endl;
                    goto continue_to_next_object_element;
                }
                if (!found_id)
                {
                    errorStream << "CAMPUS must have a IDENTIFIER property" << std::endl;
                    goto continue_to_next_object_element;
                }

                //> Add campus if all the checks have passed
                parsed_campuses.push_back((CampusElement){.name = name, .id = identifier});
            }
        }

        if (objectElementType == "BUILDING")
        {
            //== string properties from child XML elements
            std::string name, identifier, campus_id;

            //== booleans indicating whether the above properties were already found
            bool found_name = false;
            bool found_id = false;
            bool found_campus = false;

            for (TiXmlElement* propertyElement = objectElement->FirstChildElement(); propertyElement != nullptr;
                 propertyElement = propertyElement->NextSiblingElement())
            {
                std::string propertyElementType = propertyElement->Value();

                if (propertyElement->FirstChild() == nullptr)
                {
                    //TODO o.emptyElement();
                    errorStream << "Property " << propertyElementType << " needs to contain text." << std::endl;
                    goto continue_to_next_object_element;
                }

                std::string tempElementChildValue = propertyElement->FirstChild()->Value();

                if (propertyElementType == "NAME")
                {
                    //> Check if we already encountered another <NAME> tag.
                    //  Which would mean multiple <NAME> tags are present => ERROR
                    if (found_name)
                    {
                        errorStream << "BUILDING element cant have more than one NAME property." << std::endl;
                        goto continue_to_next_object_element;
                    }

                    found_name = true;
                    name = tempElementChildValue;
                }
                else if (propertyElementType == "IDENTIFIER")
                {
                    //> Check if we already encountered another <IDENTIFIER> tag.
                    //  Which would mean multiple <IDENTIFIER> tags are present => ERROR
                    if (found_id)
                    {
                        errorStream << "BUILDING element cant have more than one IDENTIFIER property." << std::endl;
                        goto continue_to_next_object_element;
                    }

                    found_id = true;
                    identifier = tempElementChildValue;
                }
                else if (propertyElementType == "CAMPUS")
                {
                    //> Check if we already encountered another <CAMPUS> tag.
                    // Which would mean multiple <CAMPUS> tags are present => ERROR
                    if (found_campus)
                    {
                        errorStream << "BUILDING element cant have more than one CAMPUS property" << std::endl;
                    }

                    found_campus = true;
                    campus_id = tempElementChildValue;
                }
                else
                {
                    //> Filter out any other unrecognized tags
                    errorStream << "Unrecognized property for BUILDING: \"" << propertyElementType << "\"" << std::endl;
                    goto continue_to_next_object_element;
                }

                //> Check if all required properties were provided
                if (!found_name)
                {
                    errorStream << "BUILDING must have a NAME property" << std::endl;
                    goto continue_to_next_object_element;
                }
                if (!found_id)
                {
                    errorStream << "BUILDING must have a IDENTIFIER property" << std::endl;
                    goto continue_to_next_object_element;
                }

                if (!found_campus)
                {
                    errorStream << "BUILDING must have a CAMPUS property" << std::endl;
                    goto continue_to_next_object_element;
                }


                //> Add building if all the checks have passed
                parsed_buildings.push_back((BuildingElement){.name = name, .id = identifier, .campus_id = campus_id});
            }
        }

        if (objectElementType == "ROOM")
        {
            //== string properties from child XML elements
            std::string name, identifier, capacity_str, campus_id, building_id;

            //== booleans indicating whether the above properties were already found
            bool found_name = false;
            bool found_id = false;
            bool found_capacity = false;
            bool found_campus = false;
            bool found_building = false;


            for (TiXmlElement* propertyElement = objectElement->FirstChildElement(); propertyElement != nullptr;
                 propertyElement = propertyElement->NextSiblingElement())
            {
                std::string propertyElementType = propertyElement->Value();

                if (propertyElement->FirstChild() == nullptr)
                {
                    //TODO o.emptyElement();
                    errorStream << "Property " << propertyElementType << " needs to contain text." << std::endl;
                    goto continue_to_next_object_element;
                }


                std::string tempElementChildValue = propertyElement->FirstChild()->Value();

                if (propertyElementType == "NAME")
                {
                    //> Check if we already encountered another <NAME> tag.
                    //  Which would mean multiple <NAME> tags are present => ERROR
                    if (found_name)
                    {
                        errorStream << "ROOM element cant have more than one NAME property." << std::endl;
                        goto continue_to_next_object_element;
                    }

                    found_name = true;
                    name = tempElementChildValue;
                }
                else if (propertyElementType == "IDENTIFIER")
                {
                    //> Check if we already encountered another <IDENTIFIER> tag.
                    //  Which would mean multiple <IDENTIFIER> tags are present => ERROR
                    if (found_id)
                    {
                        errorStream << "ROOM element cant have more than one IDENTIFIER property." << std::endl;
                        goto continue_to_next_object_element;
                    }

                    found_id = true;
                    identifier = tempElementChildValue;
                }
                else if (propertyElementType == "CAPACITY")
                {
                    //> Check if we already encountered another <CAPACITY> tag.
                    //  Which would mean multiple <CAPACITY> tags are present => ERROR
                    if (found_capacity)
                    {
                        errorStream << "ROOM element cant have more than one CAPACITY property." << std::endl;
                        goto continue_to_next_object_element;
                    }

                    found_capacity = true;
                    capacity_str = tempElementChildValue;
                }
                else if (propertyElementType == "CAMPUS")
                {
                    //> Check if we already encountered another <CAMPUS> tag.
                    //  Which would mean multiple <CAMPUS> tags are present => ERROR
                    if (found_campus)
                    {
                        errorStream << "ROOM element cant have more than one CAMPUS property." << std::endl;
                        goto continue_to_next_object_element;
                    }

                    found_campus = true;
                    campus_id = tempElementChildValue;
                }
                else if (propertyElementType == "BUILDING")
                {
                    //> Check if we already encountered another <BUILDING> tag.
                    //  Which would mean multiple <BUILDING> tags are present => ERROR
                    if (found_building)
                    {
                        errorStream << "ROOM element cant have more than one BUILDING property." << std::endl;
                        goto continue_to_next_object_element;
                    }

                    found_building = true;
                    building_id = tempElementChildValue;
                }
                else
                {
                    //> Filter out any other unrecognized tags
                    errorStream << "Unrecognized property for ROOM: \"" << propertyElementType << "\"" << std::endl;
                    goto continue_to_next_object_element;
                }
            }


            //> Check if all required properties were provided
            if (!found_name)
            {
                errorStream << "ROOM must have a NAME property" << std::endl;
                goto continue_to_next_object_element;
            }
            if (!found_id)
            {
                errorStream << "ROOM must have a IDENTIFIER property" << std::endl;
                goto continue_to_next_object_element;
            }
            if (!found_capacity)
            {
                errorStream << "ROOM must have a CAPACITY property" << std::endl;
                goto continue_to_next_object_element;
            }
            /*if (!found_campus)
            {
                errorStream << "ROOM must have a CAMPUS property" << std::endl;
                goto continue_to_next_object_element;
            }
            if (!found_building)
            {
                errorStream << "ROOM must have a BUILDING property" << std::endl;
                goto continue_to_next_object_element;
            }*/

            //== 'capacity' string parsed into an int
            int capacity = 0;

            //> Try to parse 'capacity' to int.
            try
            {
                capacity = std::stoi(capacity_str);
            }
            catch (std::exception& except)
            {
                errorStream << "Capacity could not be converted to an integer. Room will not be added \n\t- " << except.
                    what() << std::endl;
                goto continue_to_next_object_element;
            }

            //> Check if capacity is larger than 0
            if (capacity <= 0)
            {
                errorStream << "Room capacity needs to be larger than 0. Room will not be added." << std::endl;
                goto continue_to_next_object_element;
            }

            //> Check if id is not empty
            if (identifier.empty())
            {
                errorStream << "Room identifier cannot be empty. Room will not be added." << std::endl;
                goto continue_to_next_object_element;
            }

            //> Check if Name is not empty
            if (name.empty())
            {
                errorStream << "Room name cannot be empty. Room will not be added." << std::endl;
                goto continue_to_next_object_element;
            }

            /*//> Check if Campus is not empty
            if (campus_id.empty())
            {
                errorStream << "Room campus cannot be empty. Room will not be added." << std::endl;
                goto continue_to_next_object_element;
            }

            //> Check if Building is not empty
            if (building_id.empty())
            {
                errorStream << "Room building cannot be empty. Room will not be added." << std::endl;
                goto continue_to_next_object_element;
            }*/

            //> Add room if all the checks have passed
            parsed_rooms.push_back((RoomElement)
            {
                .
                name = std::move(name),
                .
                id = std::move(identifier),
                .
                capacity = capacity
            });
        }
        else if (objectElementType == "MEETING")
        {
            //== string properties from child XML elements
            std::string label, identifier, room, date_string, hour_string, externals_allowed_string;

            //== booleans indicating whether the above properties were already found
            bool found_label = false;
            bool found_id = false;
            bool found_room = false;
            bool found_datestring = false;
            bool found_hourstring = false;
            bool found_externals_allowed = false;


            for (TiXmlElement* propertyElement = objectElement->FirstChildElement(); propertyElement != nullptr;
                 propertyElement = propertyElement->NextSiblingElement())
            {
                std::string propertyElementType = propertyElement->Value();

                if (propertyElement->FirstChild() == nullptr)
                {
                    errorStream << "Property " << propertyElementType << " needs to contain text." << std::endl;
                    goto continue_to_next_object_element;;
                }

                std::string propertyElementContents = propertyElement->FirstChild()->Value();

                if (propertyElementType == "LABEL")
                {
                    //> Check if we already encountered another <LABEL> tag.
                    //  Which would mean multiple <LABEL> tags are present => ERROR
                    if (found_label)
                    {
                        errorStream << "MEETING element cant have more than one LABEL property." << std::endl;
                        goto continue_to_next_object_element;
                    }

                    found_label = true;
                    label = propertyElementContents;
                }
                else if (propertyElementType == "IDENTIFIER")
                {
                    //> Check if we already encountered another <IDENTIFIER> tag.
                    //  Which would mean multiple <IDENTIFIER> tags are present => ERROR
                    if (found_id)
                    {
                        errorStream << "MEETING element cant have more than one IDENTIFIER property." << std::endl;
                        goto continue_to_next_object_element;
                    }

                    found_id = true;
                    identifier = propertyElementContents;
                }
                else if (propertyElementType == "ROOM")
                {
                    //> Check if we already encountered another <ROOM> tag.
                    //  Which would mean multiple <ROOM> tags are present => ERROR
                    if (found_room)
                    {
                        errorStream << "MEETING element cant have more than one ROOM property." << std::endl;
                        goto continue_to_next_object_element;
                    }

                    found_room = true;
                    room = propertyElementContents;
                }
                else if (propertyElementType == "DATE")
                {
                    //> Check if we already encountered another <DATE> tag.
                    //  Which would mean multiple <DATE> tags are present => ERROR
                    if (found_datestring)
                    {
                        errorStream << "MEETING element cant have more than one DATE property." << std::endl;
                        goto continue_to_next_object_element;
                    }

                    found_datestring = true;
                    date_string = propertyElementContents;
                }
                else if (propertyElementType == "HOUR")
                {
                    //> Check if we already encountered another <HOUR> tag.
                    //  Which would mean multiple <HOUR> tags are present => ERROR
                    if (found_hourstring)
                    {
                        errorStream << "MEETING element cant have more than one HOUR property." << std::endl;
                        goto continue_to_next_object_element;
                    }

                    found_hourstring = true;
                    hour_string = propertyElementContents;
                }
                else if (propertyElementType == "EXTERNALS")
                {
                    if (found_externals_allowed)
                    {
                        errorStream << "MEETING element cant have more than one EXTERNALS property." << std::endl;
                        goto continue_to_next_object_element;
                    }

                    found_externals_allowed = true;
                    externals_allowed_string = propertyElementContents;
                }
                else
                {
                    //> Filter out any other unrecognized tags
                    errorStream << "Unrecognized property for MEETING: \"" << propertyElementType << "\"" << std::endl;
                    goto continue_to_next_object_element;
                }
            }


            //> Check if all required properties were provided
            if (!found_id)
            {
                errorStream << "MEETING must have a IDENTIFIER property" << std::endl;
                goto continue_to_next_object_element;
            }
            if (!found_label)
            {
                errorStream << "MEETING must have a LABEL property" << std::endl;
                goto continue_to_next_object_element;
            }
            if (!found_room)
            {
                errorStream << "MEETING must have a ROOM property" << std::endl;
                goto continue_to_next_object_element;
            }
            if (!found_datestring)
            {
                errorStream << "MEETING must have a DATE property" << std::endl;
                goto continue_to_next_object_element;
            }
            if (!found_hourstring)
            {
                errorStream << "MEETING must have an HOUR property" << std::endl;
                goto continue_to_next_object_element;
            }

            if (!found_externals_allowed)
            { /* Is allowed, optional property for now */ }


            //> Check if id is not empty
            if (identifier.empty())
            {
                errorStream << "Meeting identifier cannot be empty. Meeting will not be added." << std::endl;
                goto continue_to_next_object_element;
            }

            //> Check if label is not empty
            if (label.empty())
            {
                errorStream << "Meeting label cannot be empty. Meeting will not be added." << std::endl;
                goto continue_to_next_object_element;
            }

            //> Check if room is not empty
            if (room.empty())
            {
                errorStream << "Meeting room cannot be empty. Meeting will not be added." << std::endl;
                goto continue_to_next_object_element;
            }


            int day, month, year, hour;

            //> Try to parse date
            try
            {
                day = std::stoi(date_string.substr(8, 2));
                month = std::stoi(date_string.substr(5, 2));
                year = std::stoi(date_string.substr(0, 4));
            }
            catch (std::exception& except)
            {
                errorStream << "MEETING \'" << identifier <<
                    "\': Date value could not be converted to a date format: \n\t- " << except.what() << std::endl;
                goto continue_to_next_object_element;
            }

            std::chrono::year_month_day chrono_date = {
                std::chrono::year(year), std::chrono::month(month), std::chrono::day(day)
            };
            //> Check if date exists
            if (!chrono_date.ok())
            {
                errorStream << "MEETING \'" << identifier << "\': Date " << date_string << " does not exist." <<
                    std::endl;
                goto continue_to_next_object_element;;
            }

            try
            {
                hour = std::stoi(hour_string);
            }
            catch (std::exception& except)
            {
                errorStream << "MEETING \'" << identifier << "\': "
                    "Hour value could not be converted to an integer: \n\t- " << except.what() << std::endl;
                goto continue_to_next_object_element;
            }

            if (hour < 0 || hour > 23)
            {
                errorStream << "MEETING \'" << identifier << "\': Hour must be non-negative and smaller than 24, not " << hour << "." << std::endl;
                goto continue_to_next_object_element;
            }


            // Parse boolean or give default value
            bool externals_allowed = found_externals_allowed ? parse_boolean(externals_allowed_string) : false;


            //> Add meeting if all checks passed
            parsed_meetings.push_back(
                (MeetingElement)
            {
                .label = std::move(label),
                .id = std::move(identifier),
                .room_id = std::move(room),
                .date_time = DateTime(year, month, day, hour),
                .externals_allowed = externals_allowed
            }
            );
        }
        else if (objectElementType == "PARTICIPATION")
        {
            //== string properties from child XML elements
            std::string meeting, user, external_string;

            //== booleans indicating whether the above properties were already found
            bool found_meeting = false;
            bool found_user = false;
            bool found_external_string = false;


            for (TiXmlElement* propertyElement = objectElement->FirstChildElement(); propertyElement != nullptr;
                 propertyElement = propertyElement->NextSiblingElement())
            {
                std::string propertyElementType = propertyElement->Value();

                if (propertyElement->FirstChild() == nullptr)
                {
                    errorStream << "Property " << propertyElementType << " needs to contain text." << std::endl;
                    goto continue_to_next_object_element;;
                }


                std::string tempElementChildValue = propertyElement->FirstChild()->Value();

                if (propertyElementType == "MEETING")
                {
                    if (found_meeting)
                    {
                        errorStream << "PARTICIPATION element cant have more than one MEETING property." << std::endl;
                        goto continue_to_next_object_element;
                    }

                    found_meeting = true;
                    meeting = tempElementChildValue;
                }
                else if (propertyElementType == "USER")
                {
                    if (found_user)
                    {
                        errorStream << "PARTICIPATION element cant have more than one USER property." << std::endl;
                        goto continue_to_next_object_element;
                    }

                    found_user = true;
                    user = tempElementChildValue;
                }
                else if (propertyElementType == "EXTERNAL")
                {
                    if (found_external_string)
                    {
                        errorStream << "PARTICIPATION element cant have more than one EXTERNAL property." << std::endl;
                        goto continue_to_next_object_element;
                    }

                    found_external_string = true;
                    external_string = tempElementChildValue;
                }
                else
                {
                    //> Filter out any other unrecognized tags
                    errorStream << "Unrecognized property for PARTICIPATION: \"" << propertyElementType << "\"" << std::endl;
                    goto continue_to_next_object_element;
                }
            }


            //> Check if all required properties are present
            if (!found_user)
            {
                errorStream << "PARTICIPATION must have a USER property" << std::endl;
                goto continue_to_next_object_element;
            }
            if (!found_meeting)
            {
                errorStream << "PARTICIPATION must have a MEETING property" << std::endl;
                goto continue_to_next_object_element;
            }

            if (!found_external_string)
            {
                /* Is allowed, optional property */
            }


            //> Check if meeting is not empty
            if (meeting.empty())
            {
                errorStream << "Meeting cannot be empty. Participation will not be added." << std::endl;
                goto continue_to_next_object_element;
            }

            //> Check if user is not empty
            if (user.empty())
            {
                errorStream << "User cannot be empty. Participation will not be added." << std::endl;
                goto continue_to_next_object_element;
            }


            bool external = found_external_string ? parse_boolean(external_string) : false;


            //> Add participation if all checks passed
            parsed_participations.push_back((ParticipationElement)
            {
                .meeting = std::move(meeting),
                .user = std::move(user),
                .external = external
            }
            );
        }
        else
        {
            errorStream << "Unrecognized object element: " << objectElementType << std::endl;
            goto continue_to_next_object_element;
        }

    continue_to_next_object_element:;
    }
}



static bool parseProperty(void* out_element, const std::string& string, PropertyType type)
{
    switch (type)
    {
    case PROPERTYTYPE_STRING:
        {
            *(std::string*)out_element = string;
            return true;
        }

    case PROPERTYTYPE_INT:
        {
            try {
                *(int*)out_element = std::stoi(string);
            } catch (std::exception& e) {
                return false;
            }
            return true;
        }

    case PROPERTYTYPE_BOOL:
        {
            if (string == "true")
            {
                *(bool*)out_element = false;
                return true;
            }
            if (string == "false")
            {
                *(bool*)out_element = true;
                return true;
            }
            return false;
        }

    case PROPERTYTYPE_DATE:
        {
            int day, month, year;

            try
            {
                day = std::stoi(string.substr(8, 2));
                month = std::stoi(string.substr(5, 2));
                year = std::stoi(string.substr(0, 4));
            }
            catch (std::exception& except)
            {
                return false;
            }

            std::chrono::year_month_day chrono_date = {
                std::chrono::year(year), std::chrono::month(month), std::chrono::day(day)
            };
            //> Check if date exists
            if (!chrono_date.ok()) return false;

            *(Date*)out_element = Date(year, month, day);
            return true;
        }

    case PROPERTYTYPE_HOUR:
        {
            int hour;
            try
            {
                hour = std::stoi(string);
            }
            catch (std::exception& except)
            {
                return false;
            }

            if (hour < 0 || hour > 23) return false;

            *(int*)out_element = hour;
            return true;
        }
    }
}



ElementType XmlParser::parseElement(const TiXmlElement* element, ElementUnion* out_element)
{
    const std::string objectType = element->Value();

    // const static std::unordered_map<std::string, ElementType> properties = {
    //     {"CAMPUS", {
    //         "NAME",
    //         "IDENTIFIER"
    //     }},
    //     {"BUILDING", {
    //         "NAME",
    //         "IDENTIFIER",
    //         "CAMPUS"
    //     }},
    //     {"ROOM", {
    //         "NAME",
    //         "IDENTIFIER",
    //         "CAPACITY",
    //         "CAMPUS",
    //         "BUILDING"
    //     }},
    //     {"MEETING", {
    //         "LABEL",
    //         "IDENTIFIER",
    //         "ROOM",
    //         "DATE",
    //         "HOUR",
    //         "EXTERNALS"
    //     }},
    //     {"PARTICIPATION", {
    //         "USER",
    //         "EXTERNAL",
    //         "MEETING"
    //     }}
    // };

    const static std::unordered_map<std::string, ElementType> element_name_to_type = {
        {"CAMPUS", ELEMENTTYPE_CAMPUS},
        {"BUILDING", ELEMENTTYPE_BUILDING},
        {"ROOM", ELEMENTTYPE_ROOM},
        {"MEETING", ELEMENTTYPE_MEETING},
        {"PARTICIPATION", ELEMENTTYPE_PARTICIPATION}
    };

    using PropertyList = std::unordered_map<std::string, PropertyType>;
    const static std::unordered_map<ElementType, PropertyList> element_type_to_properties = {
        {ELEMENTTYPE_CAMPUS, {
            {"NAME", PROPERTYTYPE_STRING},
            {"IDENTIFIER", PROPERTYTYPE_STRING}
        }},
        {ELEMENTTYPE_BUILDING, {
            {"NAME", PROPERTYTYPE_STRING},
            {"IDENTIFIER", PROPERTYTYPE_STRING},
            {"CAMPUS", PROPERTYTYPE_STRING}
        }},
        {ELEMENTTYPE_ROOM, {
            {"NAME", PROPERTYTYPE_STRING},
            {"IDENTIFIER", PROPERTYTYPE_STRING},
            {"BUILDING", PROPERTYTYPE_STRING},
            {"CAMPUS", PROPERTYTYPE_STRING}
        }},
        {ELEMENTTYPE_MEETING, {
            {"LABEL", PROPERTYTYPE_STRING},
            {"IDENTIFIER", PROPERTYTYPE_STRING},
            {"ROOM", PROPERTYTYPE_STRING},
            {"DATE", PROPERTYTYPE_DATE},
            {"HOUR", PROPERTYTYPE_HOUR},
            {"EXTERNALS", PROPERTYTYPE_BOOL}
        }},
        {ELEMENTTYPE_PARTICIPATION, {
            {"USER", PROPERTYTYPE_STRING},
            {"EXTERNAL", PROPERTYTYPE_BOOL},
            {"MEETING", PROPERTYTYPE_STRING}
        }}
    };

    std::unordered_map<std::string, ElementType>::const_iterator it = element_name_to_type.find(objectType);

    if (it == element_name_to_type.end())
    {
        errorStream << "Unrecognized object element: " << element << std::endl;
        return ELEMENTTYPE_FAILED;
    }

    const ElementType type = it->second;
    assert(element_type_to_properties.contains(type));

    const PropertyList& expected_props = element_type_to_properties.at(type);

    std::unordered_map<std::string, std::string> found_props = {};

    for (const TiXmlElement* propertyElement = element->FirstChildElement(); propertyElement != nullptr;
                 propertyElement = propertyElement->NextSiblingElement())
    {
        const std::string propertyType = propertyElement->Value();

        if (!expected_props.contains(propertyType))
        {
            errorStream << "Unknown property " << propertyType << " of object " << objectType << std::endl;
            return ELEMENTTYPE_FAILED;
        }

        if (propertyElement->FirstChild() == nullptr)
        {
            errorStream << "Property " << propertyType << " needs to contain text." << std::endl;
            return ELEMENTTYPE_FAILED;
        }

        const std::string propertyValue = propertyElement->FirstChild()->Value();

        found_props.insert({propertyType, propertyValue});
    }

    for (
        PropertyList::const_iterator it = expected_props.begin();
        it != expected_props.end(); ++it
        )
    {
        if (!found_props.contains(it->first))
        {
            errorStream << "Required property " << it->first << " of object " << objectType << " not found." << std::endl;
            return ELEMENTTYPE_FAILED;
        }
    }

    switch (type)
    {
        case ELEMENTTYPE_CAMPUS:
            parseProperty(
                &out_element->campus.id,
                found_props.at("IDENTIFIER"),
                PROPERTYTYPE_STRING
                );
            parseProperty(
                &out_element->campus.name,
                found_props.at("NAME"),
                PROPERTYTYPE_STRING
                );
            return ELEMENTTYPE_CAMPUS;
        case ELEMENTTYPE_BUILDING:
            parseProperty(
                &out_element->building.id,
                found_props.at("IDENTIFIER"),
                PROPERTYTYPE_STRING
                );
            parseProperty(
                &out_element->building.name,
                found_props.at("NAME"),
                PROPERTYTYPE_STRING
                );
            parseProperty(
                &out_element->building.campus_id,
                found_props.at("CAMPUS"),
                PROPERTYTYPE_STRING
                );
            return ELEMENTTYPE_BUILDING;
        case ELEMENTTYPE_ROOM:
            parseProperty(
                &out_element->room.,
                found_props.at("IDENTIFIER"),
                PROPERTYTYPE_STRING
                );
            parseProperty(
                &out_element->building.name,
                found_props.at("NAME"),
                PROPERTYTYPE_STRING
                );
            parseProperty(
                &out_element->building.campus_id,
                found_props.at("CAMPUS"),
                PROPERTYTYPE_STRING
                );
        case ELEMENTTYPE_MEETING:
        case ELEMENTTYPE_PARTICIPATION:
        case ELEMENTTYPE_FAILED:
            break;
    }
}







union ElementUnion
{
    CampusElement campus;
    BuildingElement building;
    RoomElement room;
    MeetingElement meeting;
    ParticipationElement participation;
};

enum ElementType
{
    ELEMENTTYPE_FAILED = 0,
    ELEMENTTYPE_CAMPUS,
    ELEMENTTYPE_BUILDING,
    ELEMENTTYPE_ROOM,
    ELEMENTTYPE_MEETING,
    ELEMENTTYPE_PARTICIPATION
};

union PropertyUnion
{
    std::string as_string;
    int as_int;
    float as_float;
    bool as_bool;
    Date as_date;
    int as_hour;
};

enum PropertyType
{
    PROPERTYTYPE_FAILED = 0,
    PROPERTYTYPE_STRING,
    PROPERTYTYPE_INT,
    PROPERTYTYPE_FLOAT,
    PROPERTYTYPE_BOOL,
    PROPERTYTYPE_DATE,
    PROPERTYTYPE_HOUR
};

/*
 * Other idea:
 */

static ElementType get_element_type(const std::string& name)
{
    using ElementTypeMap = std::unordered_map<std::string, ElementType>;
    const static ElementTypeMap element_type_map = {
        {"CAMPUS", ELEMENTTYPE_CAMPUS},
        {"BUILDING", ELEMENTTYPE_BUILDING},
        {"ROOM", ELEMENTTYPE_ROOM},
        {"MEETING", ELEMENTTYPE_MEETING},
        {"PARTICIPATION", ELEMENTTYPE_PARTICIPATION}
    };

    if (!element_type_map.contains(name)) return ELEMENTTYPE_FAILED;
    return element_type_map.at(name);
}

struct PropertyInfo {const PropertyType type; PropertyUnion* storage;};
using PropertyInfoMap = std::unordered_map<std::string, PropertyInfo>;
using ElementPropertiesMap = std::unordered_map<ElementType, PropertyInfoMap>;

static ElementPropertiesMap property_map = {
    {ELEMENTTYPE_CAMPUS, {
        {"NAME", {PROPERTYTYPE_STRING, nullptr}}
    }}
};

static PropertyType get_property_type(ElementType element, const std::string& name)
{
    assert(property_map.contains(element));
    if (!property_map.at(element).contains(name)) return PROPERTYTYPE_FAILED;
    return property_map.at(element).at(name).type;
}



static void reset_property_map()
{
    for (ElementPropertiesMap::iterator it = property_map.begin(); it != property_map.end(); ++it)
    {
        //const std::string& element_type = it->first;
        PropertyInfoMap& property_info_map = it->second;
        for (PropertyInfoMap::iterator jt = property_info_map.begin(); jt != property_info_map.end(); ++jt)
        {
            PropertyInfo& prop = jt->second;
            prop.storage = nullptr;
        }
    }
}
