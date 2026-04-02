//
// Created by s0255946 on 18/03/2026.
//

#include "XmlParser.h"

#include "helper/DesignByContract.h"
#include "tinyxml.h"

void XmlParser::parse(const std::string& filename, std::ostream& errorStream)
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


    int meetingOrder = 1;

    for (TiXmlElement* objectElement = root->FirstChildElement(); objectElement != nullptr; objectElement =
         objectElement->NextSiblingElement())
    {
        //== The type of the element 'ROOM'/'MEETING'/...
        std::string objectElementType = objectElement->Value();

        if (objectElementType == "ROOM")
        {
            //== string properties from child XML elements
            std::string name, identifier, capacity;

            //== booleans indicating whether the above properties were already found
            bool found_name = false;
            bool found_id = false;
            bool found_capacity = false;


            for (TiXmlElement* propertyElement = objectElement->FirstChildElement(); propertyElement != nullptr;
                 propertyElement = propertyElement->NextSiblingElement())
            {
                std::string propertyElementType = propertyElement->Value();

                if (propertyElement->FirstChild() == nullptr)
                {
                    //TODO o.emptyElement();
                    errorStream << "Property " << propertyElementType << " needs to contain text." << std::endl;
                    goto continue_to_next_object_element;;
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
                    capacity = tempElementChildValue;
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

            //== 'capacity' string parsed into an int
            int capacityInt = 0;

            //> Try to parse 'capacity' to int.
            try
            {
                capacityInt = std::stoi(capacity);
            }
            catch (std::exception& except)
            {
                errorStream << "Capacity could not be converted to an integer. Room will not be added \n\t- " << except.
                    what() << std::endl;
                goto continue_to_next_object_element;
            }

            //> Check if capacity is larger than 0
            if (capacityInt <= 0)
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

            //> Add room if all the checks have passed
            parsed_rooms.push_back(new Room(name, identifier, capacityInt));
        }
        else if (objectElementType == "MEETING")
        {
            //== string properties from child XML elements
            std::string label, identifier, room, dateString;

            //== booleans indicating whether the above properties were already found
            bool found_label = false;
            bool found_id = false;
            bool found_room = false;
            bool found_datestring = false;


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
                    dateString = propertyElementContents;
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


            int day, month, year;

            //> Try to parse date
            try
            {
                day = std::stoi(dateString.substr(8, 2));
                month = std::stoi(dateString.substr(5, 2));
                year = std::stoi(dateString.substr(0, 4));
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
                errorStream << "MEETING \'" << identifier << "\': Date " << dateString << " does not exist." <<
                    std::endl;
                goto continue_to_next_object_element;;
            }


            //> Add meeting if all checks passed
            Meeting* m = new Meeting(label, identifier, room, Date(chrono_date));
            m->setOrder(meetingOrder);
            parsed_meetings.push_back(m);
            // Increment meeting order
            meetingOrder += 1;
        }
        else if (objectElementType == "PARTICIPATION")
        {
            //== string properties from child XML elements
            std::string meeting, user;

            //== booleans indicating whether the above properties were already found
            bool found_meeting = false;
            bool found_user = false;


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


            //> Add participation if all checks passed
            parsed_participations.push_back(new Participation(user, meeting));
        }
        else
        {
            errorStream << "Unrecognized object element: " << objectElementType << std::endl;
            goto continue_to_next_object_element;
        }

    continue_to_next_object_element:;
    }
}
