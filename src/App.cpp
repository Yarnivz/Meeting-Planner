//
// Created by User on 2/26/2026.
//

#include "App.h"
#include <tinyxml.h>

App::App() : rooms(), meetings(), participation_lists() {
    init_check_this_ptr = this;
}

bool App::isProperlyInitialized() const {
    return init_check_this_ptr == this;
}


void App::parseFile(const std::string& filename)
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
        try
        {
            for (TiXmlElement* rootChildElement = root->FirstChildElement(); rootChildElement != NULL; rootChildElement = rootChildElement->NextSiblingElement())
            {
                if (std::string(rootChildElement->Value()) == "ROOM" && rootChildElement->FirstChild() != NULL)
                {
                    std::string name, identifier;
                    unsigned int capacity = 0;
                    for (TiXmlElement* currentElement = rootChildElement->FirstChildElement(); currentElement != NULL; currentElement = currentElement->NextSiblingElement())
                    {
                        if (currentElement->FirstChild() != NULL)
                        {
                            std::string tempElementChildValue = currentElement->FirstChild()->Value();
                            if (std::string(currentElement->Value()) == "NAME")
                            {
                                name = tempElementChildValue;
                            } else if (std::string(currentElement->Value()) == "IDENTIFIER")
                            {
                                identifier = tempElementChildValue;
                            } else if (std::string(currentElement->Value()) == "CAPACITY")
                            {
                                try
                                {
                                    capacity = std::stoi(tempElementChildValue);
                                } catch (const std::exception& e)
                                {
                                    std::cerr << e.what() << " (could not convert room capacity from string to int defaulting to 303)" << std::endl;
                                }
                            }
                        }
                    }
                    try
                    {
                        rooms[identifier] = new Room(name, identifier, capacity);
                    } catch (std::exception& e)
                    {
                        std::cerr << e.what() << " (could not assign a value to a specific room/add it to the map of rooms)" << std::endl;
                    }
                } else if (std::string(rootChildElement->Value()) == "MEETING")
                {
                    std::string label, identifier, room, dateString;
                    int day, month, year;

                    for (TiXmlElement* currentElement = rootChildElement->FirstChildElement(); currentElement != NULL; currentElement = currentElement->NextSiblingElement())
                    {
                        if (currentElement->FirstChild() != NULL)
                        {
                            std::string tempElementChildValue = currentElement->FirstChild()->Value();
                            if (std::string(currentElement->Value()) == "LABEL")
                            {
                                label = tempElementChildValue;
                            } else if (std::string(currentElement->Value()) == "IDENTIFIER")
                            {
                                identifier = tempElementChildValue;
                            } else if (std::string(currentElement->Value()) == "ROOM")
                            {
                                room = std::string(currentElement->FirstChild()->Value());
                            } else if (std::string(currentElement->Value()) == "DATE")
                            {
                                dateString = std::string(currentElement->FirstChild()->Value());
                            }
                        }
                    }
                    try
                    {
                        day   = std::stoi(dateString.substr(8, 2));
                        month = std::stoi(dateString.substr(5, 2));
                        year  = std::stoi(dateString.substr(0, 4));
                    } catch (const std::exception& e)
                    {
                        std::cerr << e.what() << " (make sure the date syntax follows xx/xx/xxxx, defaulting to 0000/00/00) " << std::endl;
                        day = 00;
                        month = 00;
                        year = 0000;
                    }
                    Date date(year, month, day);
                    try
                    {
                        meetings[identifier] = new Meeting(label, identifier, room, date);
                    } catch (std::exception& e)
                    {
                        std::cerr << e.what() << " (could not assign a value to a specific meeting/add it to the map of meetings)" << std::endl;
                    }

                } else if (std::string(rootChildElement->Value()) == "PARTICIPATION")
                {
                    std::string meeting, user;

                    for (TiXmlElement* currentElement = rootChildElement->FirstChildElement(); currentElement != NULL; currentElement = currentElement->NextSiblingElement())
                    {
                        if (currentElement->FirstChild() != NULL)
                        {
                            std::string tempElementChildValue = currentElement->FirstChild()->Value();
                            if (std::string(currentElement->Value()) == "MEETING")
                            {
                                meeting = tempElementChildValue;
                            } else if (std::string(currentElement->Value()) == "USER")
                            {
                                user = tempElementChildValue;
                            }
                        }
                    }
                    try
                    {
                        participations[user] = new Participation(user, meeting);
                    } catch (std::exception& e)
                    {
                        std::cerr << e.what() << " (could not assign a value to a specific participation/add it to the map of participations)" << std::endl;
                    }
                }
            }
        } catch (std::exception& e)
        {
            std::cerr << e.what() << " (something unaccounted for went wrong while parsing the xml file)" << std::endl;
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
    const std::unordered_map<std::string, Room*>::iterator it = rooms.find(id);

    if (it == rooms.end()) return nullptr;

    return it->second;
}

void App::addMeeting(Meeting *meeting) {
    if (!meeting) return;

    meetings.insert({meeting->getId(), meeting});
}

Meeting* App::getMeeting(const std::string& id) {
    const std::unordered_map<std::string, Meeting*>::iterator it = meetings.find(id);

    if (it == meetings.end()) return nullptr;

    return it->second;
}

void App::addParticipation(Participation *participation) {
    if (!participation) return;

    const std::string u = participation->getUser();

    const std::unordered_map<std::string, std::list<Participation*>>::iterator it = participation_lists.find(u);

    if (it == participation_lists.end()) {
        participation_lists.insert({participation->getUser(), {participation}});
        return;
    }

    it->second.push_back(participation);
}

const std::list<Participation *> *App::getParticipationList(const std::string &username) {
    const std::unordered_map<std::string, std::list<Participation*>>::iterator it = participation_lists.find(username);

    if (it == participation_lists.end()) return nullptr;

    return &it->second;
}



App::~App() {
    for (auto& r : rooms) delete r.second;
    for (auto& m : meetings) delete m.second;
    for (const auto& pl : participation_lists) {
        for (const auto& p : pl.second ) {
            delete p;
        }
    }
}


