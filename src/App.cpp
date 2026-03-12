//
// Created by User on 2/26/2026.
//

#include "App.h"
#include <tinyxml.h>

App::App() : rooms(), meetings(), participation_lists() {
    init_check_this_ptr = this;
}
#include "DesignByContract.h"

bool App::isProperlyInitialized() const {
    return init_check_this_ptr == this;
}

/**
 * Takes an xml filename as input and extracts the relevant data, then sets the relevant class variables to it.
 * @param filename
 */
void App::parseFile(const std::string& filename)
{
    TiXmlDocument doc;
    REQUIRE(!filename.empty(), "The provided file cannot be empty");
    REQUIRE(doc.LoadFile(filename.c_str()), "The provided file doesn't exist in your current work directory or cannot be opened.");

    if(!doc.LoadFile(filename.c_str())) {
        std::cerr << doc.ErrorDesc() << std::endl;
    }
    TiXmlElement* root = doc.FirstChildElement();
    if(root == NULL)
    {
        std::cerr << "Failed to load file: No root element." << std::endl;
        doc.Clear();
        return;
    }
    for (TiXmlElement* rootChildElement = root->FirstChildElement(); rootChildElement != NULL; rootChildElement = rootChildElement->NextSiblingElement())
    {
        if (std::string(rootChildElement->Value()) == "ROOM" && rootChildElement->FirstChild() != NULL)
        {
            std::string name, identifier, capacity;

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
                        capacity = tempElementChildValue;
                    }
                }
            }

            int capacityInt = 0;
            //Check if Capacity can be converted to an int
            try {
                capacityInt = std::stoi(capacity);
            } catch (std::exception& except) {
                std::cerr << "Capacity could not be converted to an integer. Room will not be added \n\t- " << except.what() << std::endl;
                continue;
            }
            //Check if Capacity is larger than 0
            if (capacityInt <= 0) {
                std::cerr << "Room capacity needs to be larger than 0. Room will not be added." << std::endl;
                continue;
            }
            //Check if Id is not empty
            if (identifier.empty()) {
                std::cerr << "Room identifier cannot be empty or non-existent. Room will not be added." << std::endl;
                continue;
            }
            //Check if Name is not empty
            if (name.empty()) {
                std::cerr << "Room name cannot be empty or non-existent. Room will not be added." << std::endl;
                continue;
            }
            //Add room if all the checks have passed
            addRoom(new Room(name, identifier, capacityInt));

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
                        room = tempElementChildValue;
                    } else if (std::string(currentElement->Value()) == "DATE")
                    {
                        dateString = tempElementChildValue;
                    }
                }
            }
            //Check if Date is valid
            try {
                day   = std::stoi(dateString.substr(8, 2));
                month = std::stoi(dateString.substr(5, 2));
                year  = std::stoi(dateString.substr(0, 4));
            } catch (std::exception& except) {
                std::cerr << "Date value could not be converted to a date format: \n\t- " << except.what() << std::endl;
                continue;
            }
            //Check if Id is not empty
            if (identifier.empty()) {
                std::cerr << "Meeting identifier cannot be empty or non-existent. Meeting will not be added." << std::endl;
                continue;
            }
            //Check if Label is not empty
            if (label.empty()) {
                std::cerr << "Meeting label cannot be empty or non-existent. Meeting will not be added." << std::endl;
                continue;
            }
            //Check if Room is not empty
            if (label.empty()) {
                std::cerr << "Meeting room cannot be empty or non-existent. Meeting will not be added." << std::endl;
                continue;
            }
            Date date(year, month, day);
            addMeeting(new Meeting(label, identifier, room, date));

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
            //Check if meeting is not empty
            if (meeting.empty()) {
                std::cerr << "Meeting cannot be empty or non-existent. Participation will not be added." << std::endl;
                continue;
            }
            //Check if user is not empty
            if (user.empty()) {
                std::cerr << "User cannot be empty or non-existent. Participation will not be added." << std::endl;
                continue;
            }
            addParticipation(new Participation(user, meeting));
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


