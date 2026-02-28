//
// Created by User on 2/26/2026.
//

#include "App.h"
#include <tinyxml.h>

App::App() : rooms(), meetings(), participations(){}

void App::parseFile(std::string filename)
{
    TiXmlDocument doc;
    if(!doc.LoadFile(filename.c_str())) {
        std::cerr << doc.ErrorDesc() << std::endl;
    }
    TiXmlElement* root = doc.FirstChildElement();
    if(root == NULL)
    {
        std::cerr << "Failed to load file: No root element." << std::endl;
        doc.Clear();
    } else
    {

        for (TiXmlElement* rootChildElement = root->FirstChildElement(); rootChildElement != NULL; rootChildElement = rootChildElement->NextSiblingElement())
        {
            if (std::string(rootChildElement->Value()) == "ROOM")
            {
                std::string name, identifier;
                unsigned int capacity = 0;

                for (TiXmlElement* currentElement = rootChildElement->FirstChildElement(); currentElement != NULL; currentElement = currentElement->NextSiblingElement())
                {
                    if (std::string(currentElement->Value()) == "NAME")
                    {
                        name = std::string(currentElement->FirstChild()->Value());
                    } else if (std::string(currentElement->Value()) == "IDENTIFIER")
                    {
                        identifier = std::string(currentElement->FirstChild()->Value());
                    } else if (std::string(currentElement->Value()) == "CAPACITY")
                    {
                        capacity = std::stoi(currentElement->FirstChild()->Value());
                    }
                }
                rooms[identifier] = new Room(name, identifier, capacity);
            } else if (std::string(rootChildElement->Value()) == "MEETING")
            {
                std::string label, identifier, room, dateString;
                int day, month, year;

                for (TiXmlElement* currentElement = rootChildElement->FirstChildElement(); currentElement != NULL; currentElement = currentElement->NextSiblingElement())
                {

                    if (std::string(currentElement->Value()) == "LABEL")
                    {
                        label = std::string(currentElement->FirstChild()->Value());
                    } else if (std::string(currentElement->Value()) == "IDENTIFIER")
                    {identifier = std::string(currentElement->FirstChild()->Value());
                    } else if (std::string(currentElement->Value()) == "ROOM")
                    {
                        room = std::string(currentElement->FirstChild()->Value());
                    } else if (std::string(currentElement->Value()) == "DATE")
                    {
                        dateString = std::string(currentElement->FirstChild()->Value());
                    }

                }
                day   = std::stoi(dateString.substr(8, 2));
                month = std::stoi(dateString.substr(5, 2));
                year  = std::stoi(dateString.substr(0, 4));
                Date date(year, month, day);
                meetings[identifier] = new Meeting(label, identifier, room, date);

            } else if (std::string(rootChildElement->Value()) == "PARTICIPATION")
            {
                std::string meeting, user;

                for (TiXmlElement* currentElement = rootChildElement->FirstChildElement(); currentElement != NULL; currentElement = currentElement->NextSiblingElement())
                {
                    if (std::string(currentElement->Value()) == "MEETING")
                    {
                        meeting = std::string(currentElement->FirstChild()->Value());
                    } else if (std::string(currentElement->Value()) == "USER")
                    {
                        user = std::string(currentElement->FirstChild()->Value());
                    }
                }
                participations[user] = new Participation(meeting, user);
            }
        }
    }
}

void App::writeToStream(std::ostream stream) {
}


void App::processMeetings() {
}


void App::addRoom(Room *room) {
    if (!room) return;

    rooms.insert({room->getId(), room});
}

Room* App::getRoom(const std::string& id) {
    auto it = rooms.find(id);

    if (it == rooms.end()) return nullptr;

    return it->second;
}

void App::addMeeting(Meeting *meeting) {
    if (!meeting) return;

    meetings.insert({meeting->getId(), meeting});
}

Meeting* App::getMeeting(const std::string& id) {
    auto it = meetings.find(id);
    if (it == meetings.end()) return nullptr;

    return it->second;
}

void App::addParticipation(Participation *participation) {
    if (!participation) return;

    //! participations.insert({std::string(), participation});
}

Participation* App::getParticipation(const std::string& id) {
    auto it = participations.find(id);

    if (it == participations.end()) return nullptr;

    return it->second;
}



App::~App() {
    for (auto& r : rooms) delete r.second;
    for (auto& m : meetings) delete m.second;
    for (auto& p : participations) delete p.second;
}