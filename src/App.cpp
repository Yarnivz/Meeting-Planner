//
// Created by User on 2/26/2026.
//

#include "App.h"

#include <filesystem>
#include <queue>
#include <tinyxml.h>
#include "DesignByContract.h"

App::App() : rooms(), meetings_by_room() {
    init_check_this_ptr = this;
}

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
    REQUIRE(!filename.empty(), "The provided filepath cannot be empty");
    REQUIRE(doc.LoadFile(filename.c_str()), "The provided file doesn't exist in your current work directory or cannot be opened.");


    if(!doc.LoadFile(filename.c_str())) {
        std::cerr << doc.ErrorDesc() << std::endl;
        return;
    }


    TiXmlElement* root = doc.FirstChildElement();
    if(root == nullptr)
    {
        std::cerr << "Failed to load file: No root element." << std::endl;
        doc.Clear();
        return;
    }


    std::list<Meeting*> meetings_parsed = {};
    std::list<Room*> rooms_parsed = {};
    std::list<Participation*> participations_parsed = {};

    int meetingOrder = 1;

    for (TiXmlElement* objectElement = root->FirstChildElement(); objectElement != nullptr; objectElement = objectElement->NextSiblingElement())
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


            for (TiXmlElement* propertyElement = objectElement->FirstChildElement(); propertyElement != nullptr; propertyElement = propertyElement->NextSiblingElement())
            {
                std::string propertyElementType = propertyElement->Value();

                if (propertyElement->FirstChild() == nullptr) {
                    std::cerr << "Property " << propertyElementType << " needs to contain text." << std::endl;
                    goto continue_to_next_object_element;;
                }


                std::string tempElementChildValue = propertyElement->FirstChild()->Value();

                if (propertyElementType == "NAME")
                {
                    //> Check if we already encountered another <NAME> tag.
                    //  Which would mean multiple <NAME> tags are present => ERROR
                    if (found_name) {
                        std::cerr << "ROOM element cant have more than one NAME property." << std::endl;
                        goto continue_to_next_object_element;
                    }

                    found_name = true;
                    name = tempElementChildValue;
                } else if (propertyElementType == "IDENTIFIER")
                {
                    //> Check if we already encountered another <IDENTIFIER> tag.
                    //  Which would mean multiple <IDENTIFIER> tags are present => ERROR
                    if (found_id) {
                        std::cerr << "ROOM element cant have more than one IDENTIFIER property." << std::endl;
                        goto continue_to_next_object_element;
                    }

                    found_id = true;
                    identifier = tempElementChildValue;
                } else if (propertyElementType == "CAPACITY")
                {
                    //> Check if we already encountered another <CAPACITY> tag.
                    //  Which would mean multiple <CAPACITY> tags are present => ERROR
                    if (found_capacity) {
                        std::cerr << "ROOM element cant have more than one CAPACITY property." << std::endl;
                        goto continue_to_next_object_element;
                    }

                    found_capacity = true;
                    capacity = tempElementChildValue;
                } else {
                    //> Filter out any other unrecognized tags
                    std::cerr << "Unrecognized property for ROOM: \"" << propertyElementType << "\"" << std::endl;
                    goto continue_to_next_object_element;
                }
            }


            //> Check if all required properties were provided
            if (!found_name) {
                std::cerr << "ROOM must have a NAME property" << std::endl;
                goto continue_to_next_object_element;
            }
            if (!found_id) {
                std::cerr << "ROOM must have a IDENTIFIER property" << std::endl;
                goto continue_to_next_object_element;
            }
            if (!found_capacity) {
                std::cerr << "ROOM must have a CAPACITY property" << std::endl;
                goto continue_to_next_object_element;
            }

            //== 'capacity' string parsed into an int
            int capacityInt = 0;

            //> Try to parse 'capacity' to int.
            try {
                capacityInt = std::stoi(capacity);
            } catch (std::exception& except) {
                std::cerr << "Capacity could not be converted to an integer. Room will not be added \n\t- " << except.what() << std::endl;
                goto continue_to_next_object_element;
            }

            //> Check if capacity is larger than 0
            if (capacityInt <= 0) {
                std::cerr << "Room capacity needs to be larger than 0. Room will not be added." << std::endl;
                goto continue_to_next_object_element;
            }

            //> Check if id is not empty
            if (identifier.empty()) {
                std::cerr << "Room identifier cannot be empty. Room will not be added." << std::endl;
                goto continue_to_next_object_element;
            }

            //> Check if Name is not empty
            if (name.empty()) {
                std::cerr << "Room name cannot be empty. Room will not be added." << std::endl;
                goto continue_to_next_object_element;
            }

            //> Add room if all the checks have passed
            rooms_parsed.push_back(new Room(name, identifier, capacityInt));








        } else if (objectElementType == "MEETING")
        {
            //== string properties from child XML elements
            std::string label, identifier, room, dateString;

            //== booleans indicating whether the above properties were already found
            bool found_label = false;
            bool found_id = false;
            bool found_room = false;
            bool found_datestring = false;


            for (TiXmlElement* propertyElement = objectElement->FirstChildElement(); propertyElement != nullptr; propertyElement = propertyElement->NextSiblingElement())
            {
                std::string propertyElementType = propertyElement->Value();

                if (propertyElement->FirstChild() == nullptr) {
                    std::cerr << "Property " << propertyElementType << " needs to contain text." << std::endl;
                    goto continue_to_next_object_element;;
                }

                std::string propertyElementContents = propertyElement->FirstChild()->Value();

                if (propertyElementType == "LABEL")
                {
                    //> Check if we already encountered another <LABEL> tag.
                    //  Which would mean multiple <LABEL> tags are present => ERROR
                    if (found_label) {
                        std::cerr << "MEETING element cant have more than one LABEL property." << std::endl;
                        goto continue_to_next_object_element;
                    }

                    found_label = true;
                    label = propertyElementContents;

                } else if (propertyElementType == "IDENTIFIER")
                {
                    //> Check if we already encountered another <IDENTIFIER> tag.
                    //  Which would mean multiple <IDENTIFIER> tags are present => ERROR
                    if (found_id) {
                        std::cerr << "MEETING element cant have more than one IDENTIFIER property." << std::endl;
                        goto continue_to_next_object_element;
                    }

                    found_id = true;
                    identifier = propertyElementContents;

                } else if (propertyElementType == "ROOM")
                {
                    //> Check if we already encountered another <ROOM> tag.
                    //  Which would mean multiple <ROOM> tags are present => ERROR
                    if (found_room) {
                        std::cerr << "MEETING element cant have more than one ROOM property." << std::endl;
                        goto continue_to_next_object_element;
                    }

                    found_room = true;
                    room = propertyElementContents;

                } else if (propertyElementType == "DATE")
                {
                    //> Check if we already encountered another <DATE> tag.
                    //  Which would mean multiple <DATE> tags are present => ERROR
                    if (found_datestring) {
                        std::cerr << "MEETING element cant have more than one DATE property." << std::endl;
                        goto continue_to_next_object_element;
                    }

                    found_datestring = true;
                    dateString = propertyElementContents;
                }
            }


            //> Check if all required properties were provided
            if (!found_id) {
                std::cerr << "MEETING must have a IDENTIFIER property" << std::endl;
                goto continue_to_next_object_element;
            }
            if (!found_label) {
                std::cerr << "MEETING must have a LABEL property" << std::endl;
                goto continue_to_next_object_element;
            }
            if (!found_room) {
                std::cerr << "MEETING must have a ROOM property" << std::endl;
                goto continue_to_next_object_element;
            }
            if (!found_datestring) {
                std::cerr << "MEETING must have a DATE property" << std::endl;
                goto continue_to_next_object_element;
            }


            //> Check if id is not empty
            if (identifier.empty()) {
                std::cerr << "Meeting identifier cannot be empty. Meeting will not be added." << std::endl;
                goto continue_to_next_object_element;
            }

            //> Check if label is not empty
            if (label.empty()) {
                std::cerr << "Meeting label cannot be empty. Meeting will not be added." << std::endl;
                goto continue_to_next_object_element;
            }

            //> Check if room is not empty
            if (room.empty()) {
                std::cerr << "Meeting room cannot be empty. Meeting will not be added." << std::endl;
                goto continue_to_next_object_element;
            }





            int day, month, year;

            //> Try to parse date
            try {
                day   = std::stoi(dateString.substr(8, 2));
                month = std::stoi(dateString.substr(5, 2));
                year  = std::stoi(dateString.substr(0, 4));
            } catch (std::exception& except) {
                std::cerr << "MEETING \'" << identifier << "\': Date value could not be converted to a date format: \n\t- " << except.what() << std::endl;
                goto continue_to_next_object_element;
            }

            std::chrono::year_month_day chrono_date = {std::chrono::year(year), std::chrono::month(month), std::chrono::day(day)};
            //> Check if date exists
            if (!chrono_date.ok()) {
                std::cerr << "MEETING \'" << identifier << "\': Date " << dateString << " does not exist." << std::endl;
                goto continue_to_next_object_element;;
            }


            //> Add meeting if all checks passed
            meetings_parsed.push_back(new Meeting(label, identifier, room, Date(chrono_date), meetingOrder));
            meetingOrder+=1;









        } else if (objectElementType == "PARTICIPATION")
        {
            //== string properties from child XML elements
            std::string meeting, user;

            //== booleans indicating whether the above properties were already found
            bool found_meeting = false;
            bool found_user = false;



            for (TiXmlElement* propertyElement = objectElement->FirstChildElement(); propertyElement != nullptr; propertyElement = propertyElement->NextSiblingElement())
            {

                std::string propertyElementType = propertyElement->Value();

                if (propertyElement->FirstChild() == nullptr) {
                    std::cerr << "Property " << propertyElementType << " needs to contain text." << std::endl;
                    goto continue_to_next_object_element;;
                }




                std::string tempElementChildValue = propertyElement->FirstChild()->Value();

                if (propertyElementType == "MEETING")
                {
                    if (found_meeting) {
                        std::cerr << "PARTICIPATION element cant have more than one MEETING property." << std::endl;
                        goto continue_to_next_object_element;
                    }

                    found_meeting = true;
                    meeting = tempElementChildValue;
                } else if (propertyElementType == "USER")
                {
                    if (found_user) {
                        std::cerr << "PARTICIPATION element cant have more than one USER property." << std::endl;
                        goto continue_to_next_object_element;
                    }

                    found_user = true;
                    user = tempElementChildValue;
                }
            }


            //> Check if all required properties are present
            if (!found_user) {
                std::cerr << "PARTICIPATION must have a USER property" << std::endl;
                goto continue_to_next_object_element;
            }
            if (!found_meeting) {
                std::cerr << "PARTICIPATION must have a MEETING property" << std::endl;
                goto continue_to_next_object_element;
            }



            //> Check if meeting is not empty
            if (meeting.empty()) {
                std::cerr << "Meeting cannot be empty. Participation will not be added." << std::endl;
                goto continue_to_next_object_element;
            }

            //> Check if user is not empty
            if (user.empty()) {
                std::cerr << "User cannot be empty. Participation will not be added." << std::endl;
                goto continue_to_next_object_element;
            }



            //> Add participation if all checks passed
            participations_parsed.push_back(new Participation(user, meeting));




        } else {
            std::cerr << "Unrecognized object element: " << objectElementType << std::endl;
            goto continue_to_next_object_element;
        }

        continue_to_next_object_element:;
    }



    //> Add all elements in the correct order
    //  Final integrity checks

    for (Room* r : rooms_parsed) {
        if (getRoom(r->getId()) != nullptr) {
            std::cerr << "Room ids must be unique: " << r->getId() << std::endl;
            continue;
        }

        addRoom(r);
    }

    for (Meeting* m : meetings_parsed) {
        if (getRoom(m->getRoomId()) == nullptr) {
            std::cerr << "Meeting " << m->getId() << " takes place in a room \'" << m->getRoomId() << "\' which doesnt exist." << std::endl;
            continue;
        }

        addMeeting(m);
    }

    for (Participation* p : participations_parsed) {
        if (getMeeting(p->getMeetingId()) == nullptr) {
            std::cerr << "User \'" << p->getUser() << "\' participates in a meeting \'" << p->getMeetingId() << "\' which doesnt exist." << std::endl;
            continue;
        }

        addParticipation(p);
    }

}

void App::writeToStream(std::ostream& onStream) {

    std::list<Meeting*> today;
    std::list<Meeting*> past;
    std::list<Meeting*> future;
    Date currentDate;
    for (const std::pair<std::string, Meeting*> item : all_meetings) {
        Meeting* meeting = item.second;
        Date meetingDate = meeting->getDate();
        if (meetingDate < currentDate) past.push_back(meeting);
        else if (meetingDate > currentDate) future.push_back(meeting);
        else today.push_back(meeting);
    }

    //Write all today's meetings
    if (!today.empty()) onStream << std::endl << "Today's meetings:" << std::endl;
    for (const Meeting* meeting : today) {
        writeMeeting(onStream, meeting);
    }

    //Write all past meetings
    if (!past.empty()) onStream << std::endl << "Past meetings:" << std::endl;
    for (const Meeting* meeting : past) {
        writeMeeting(onStream, meeting);
    }

    //Write all future meetings
    if (!future.empty()) onStream << std::endl << "Future meetings:" << std::endl;
    for (const Meeting* meeting : future) {
        writeMeeting(onStream, meeting);

    }

    //Write all conflicts
    if (!cancelling_meetings.empty()) onStream << std::endl << "Conflicts:" << std::endl;
    for (const std::pair<const std::string, Meeting *>& item : cancelling_meetings) {
        const Meeting* meeting = item.second;
        writeMeeting(onStream, meeting);
        onStream << "  Reason: " << getCancellationReason(meeting->getId()) << std::endl;
    }

    //Write all rooms
    if (!rooms.empty()) onStream << std::endl << "Rooms:" << std::endl;
    for (const std::pair<std::string, Room*> item : rooms) {
        const Room* room = item.second;
        writeRoom(onStream, room);
    }
}

void App::processSingleMeeting(const std::string &meetingId)
{
    //REQUIRE(!meetingId.empty(), "Meeting id is empty");
    Meeting* meeting = getMeeting(meetingId);
    REQUIRE(meeting, "This meeting doesn't exist.");
    REQUIRE(meeting->isProperlyInitialized(), "Meeting needs to be properly initialized.");

    bool meetingProcessed = false;

    Meeting* conflict;
    if ((conflict = findConflictingMeeting(meetingId)))
    {
        cancelMeeting(meetingId, "conflict with meeting " + conflict->getId());
        std::cout << meeting->getId() << " has been cancelled due to '" + getCancellationReason(meeting->getId()) << "'" << std::endl;
        meetingProcessed = true;
    } else
    {
        doMeeting(meetingId);
        std::cout << meeting->getId() << " has taken place" << std::endl;
        meetingProcessed = true;
    }

    ENSURE(meetingProcessed, "Meeting hasnt been processed");
}

void App::processAllMeetings()
{
    std::vector<Meeting*> sortedMeetings;
    for (Meetings::iterator it = all_meetings.begin(); it != all_meetings.end(); ++it)
    {
        sortedMeetings.push_back(it->second);
    }
    std::sort(sortedMeetings.begin(), sortedMeetings.end(), [](Meeting* comparedMeeting1, Meeting* comparedMeeting2)
    {
        if (comparedMeeting1->getDate() != comparedMeeting2->getDate())
        {
            return comparedMeeting1->getDate() < comparedMeeting2->getDate();
        }
        return comparedMeeting1->getId() < comparedMeeting2->getId();
    });

    for (size_t i = 0; i < sortedMeetings.size(); ++i)
    {
        Meeting* currentMeeting = sortedMeetings[i];
        REQUIRE(currentMeeting, "Meeting can not be null.");
        REQUIRE(currentMeeting->isProperlyInitialized(), "Meeting needs to be properly initialized.");
        processSingleMeeting(currentMeeting->getId());
    }
    int totalMeetingsProcessed = cancelling_meetings.size() + ongoing_meetings.size();
    int allMeetingsSize = all_meetings.size();
    ENSURE(allMeetingsSize == totalMeetingsProcessed, "Not all meetings have been processed");
}

void App::addRoom(Room *room) {
    REQUIRE(room, "The provided room cannot be null.");
    REQUIRE(room->isProperlyInitialized(), "Room needs to be properly initialized by the constructor.");
    REQUIRE(!rooms.contains(room->getId()), "Room id has to be unique.");

    rooms.insert({room->getId(), room});          // Add room
    meetings_by_room.insert({room->getId(), {}}); // Add empty meetings map

    ENSURE(getRoom(room->getId()) == room, "The room was not added to the App");
}

Room* App::getRoom(const std::string& roomId) {
    const Rooms::iterator it = rooms.find(roomId);

    if (it == rooms.end()) return nullptr;

    ENSURE(it->second->getId() == roomId, "Something went wrong. The room which was found did not have the right id.");
    return it->second;
}


const Rooms & App::getAllRooms() const {
    return rooms;
}




bool App::isRoomOccupied(const std::string &roomId, const Date& date) {
    Meetings *mt_list = _getMutMeetingsByRoom(roomId);
    REQUIRE(mt_list, "This room doesnt exist.");

    for (Meetings::iterator it = mt_list->begin(); it != mt_list->end(); ++it) {
        const Meeting* m = it->second;

        if (getDoneMeeting(m->getId()) && m->getDate() == date)
            return true;
    }

    return false;
}

Meeting* App::findConflictingMeeting(const std::string &meetingId) {
    Meeting* m = getMeeting(meetingId);
    REQUIRE(m, "This meeting doesn't exist.");
    Meetings *mt_list = _getMutMeetingsByRoom(m->getRoomId());
    ENSURE(mt_list, "Encountered a meeting with a roomId which doesnt exist.");

    for (Meetings::iterator it = mt_list->begin(); it != mt_list->end(); ++it) {
        Meeting* other_m = it->second;

        if (getDoneMeeting(other_m->getId()) && other_m->getDate() == m->getDate())
            return other_m;
    }

    return nullptr;
}













void App::addMeeting(Meeting *meeting) {
    REQUIRE(meeting, "Meeting can not be null.");
    REQUIRE(meeting->isProperlyInitialized(), "Meeting needs to be properly initialized.");
    REQUIRE(!all_meetings.contains(meeting->getId()), "Meetings Id needs to be unique!");

    Room* rm = getRoom(meeting->getRoomId());

    REQUIRE(
        rm,
        "The meeting has to take place in a room which was already registered."
        );

    REQUIRE(
        !isRoomOccupied(rm->getId(), meeting->getDate()),
        "There is already a meeting planned in this room at that date."
        );


    Meetings *mt_list = _getMutMeetingsByRoom(meeting->getRoomId());
    ENSURE(mt_list, "Something went wrong, the meeting list was not found.");


    all_meetings.insert({meeting->getId(), meeting}); // Insert into flat map by meetingId
    mt_list->insert({meeting->getId(), meeting});     // Also into nested map by roomId => meetingId

    participations_by_meeting.insert({meeting->getId(), {}}); // Insert empty participations list


    ENSURE(getMeetingInRoom(meeting->getId(), meeting->getRoomId()) == meeting, "Something went wrong, The meeting was not added to the App");
    ENSURE(getMeeting(meeting->getId()) == meeting, "Something went wrong, the meeting was not added to the App");
}



Meeting * App::getMeetingInRoom(const std::string &meetingId, const std::string &roomId) {
    Meetings* ms = _getMutMeetingsByRoom(roomId);
    if (!ms) return nullptr;

    const Meetings::iterator ms_it = ms->find(meetingId);

    if (ms_it == ms->end()) return nullptr;

    ENSURE(ms_it->second->getId() == meetingId, "Something went wrong, The meeting which was found did not have the correct id.");
    return ms_it->second;
}


Meeting* App::getMeeting(const std::string& meetingId) {

    const Meetings::iterator it = all_meetings.find(meetingId);

    if (it == all_meetings.end()) return nullptr;

    ENSURE(it->second->getId() == meetingId, "Something went wrong, The meeting which was found did not have the correct id.");
    return it->second;
}

Meeting* App::getCanceledMeeting(const std::string& meetingId) {
    const Meetings::iterator it = cancelling_meetings.find(meetingId);

    if (it == cancelling_meetings.end()) return nullptr;

    ENSURE(it->second->getId() == meetingId, "Something went wrong, The meeting which was found did not have the correct id.");
    return it->second;
}

const std::string& App::getCancellationReason(const std::string& meetingId) {
    REQUIRE(getCanceledMeeting(meetingId), "That meeting does not exist or it isn't cancelled");

    const std::unordered_map<std::string, std::string>::iterator it = canceled_meeting_reasons.find(meetingId);

    ENSURE(it != canceled_meeting_reasons.end(), "Something went wrong, The meeting was found but the cancellation reason wasn't.");
    ENSURE(it->first == meetingId, "Something went wrong, The wrong cancellation reason was retrieved.");

    return it->second;
}



Meeting* App::getDoneMeeting(const std::string& meetingId) {
    const Meetings::iterator it = ongoing_meetings.find(meetingId);

    if (it == ongoing_meetings.end()) return nullptr;

    ENSURE(it->second->getId() == meetingId, "Something went wrong, The meeting which was found did not have the correct id.");
    return it->second;
}




void App::cancelMeeting(const std::string& meetingId, const std::string& reason) {
    Meeting* m;
    REQUIRE((m = getMeeting(meetingId)), "This meeting does not exist.");
    REQUIRE(getCanceledMeeting(meetingId) == nullptr, "This meeting was already canceled.");
    REQUIRE(getDoneMeeting(meetingId) == nullptr, "This meeting was already done.");

    cancelling_meetings.insert({meetingId, m});
    canceled_meeting_reasons.insert({meetingId, reason});

    ENSURE(getDoneMeeting(meetingId) == nullptr, "Something went wrong. The meeting was not canceled.");
}

void App::uncancelMeeting(const std::string& meetingId) {
    Meeting* m;
    REQUIRE((m = getMeeting(meetingId)), "This meeting does not exist.");
    REQUIRE(getCanceledMeeting(meetingId), "This meeting was not canceled.");
    REQUIRE(getDoneMeeting(meetingId) == nullptr, "This meeting was already done.");

    cancelling_meetings.erase(meetingId);
    canceled_meeting_reasons.erase(meetingId);

    ENSURE(getDoneMeeting(meetingId) == nullptr, "Something went wrong. The meeting was not uncanceled.");
}

void App::doMeeting(const std::string& meetingId) {
    Meeting* m;
    REQUIRE((m = getMeeting(meetingId)), "This meeting does not exist.");
    REQUIRE(getCanceledMeeting(meetingId) == nullptr, "This meetings was canceled.");
    REQUIRE(getDoneMeeting(meetingId) == nullptr, "This meeting was already done");

    ongoing_meetings.insert({meetingId, m});

    ENSURE(getDoneMeeting(meetingId) == m, "Something went wrong. The meeting was not done.");
}

void App::undoMeeting(const std::string& meetingId) {
    Meeting* m;
    REQUIRE((m = getMeeting(meetingId)), "This meeting does not exist.");
    REQUIRE(getCanceledMeeting(meetingId) == nullptr, "This meetings was canceled.");
    REQUIRE(getDoneMeeting(meetingId), "This meeting was not done.");

    ongoing_meetings.erase(meetingId);

    ENSURE(getDoneMeeting(meetingId) == nullptr, "Something went wrong. The meeting was not undone.");

}



const Meetings & App::getAllMeetings() const {
    return all_meetings;
}

const Meetings* App::getMeetingsByRoom(const std::string &roomId) {
    return _getMutMeetingsByRoom(roomId);
}


















void App::addParticipation(Participation *participation) {
    REQUIRE(participation != nullptr, "Participation can not be null.");
    REQUIRE(participation->isProperlyInitialized(), "Participation needs to be properly initialized.");


    const Meeting* mt = getMeeting(participation->getMeetingId());
    REQUIRE(mt, "Could not add participation: The requested meeting does not exist.");
    REQUIRE(!isUserOccupied(participation->getUser(), mt->getDate()), "Could not add participation: This user already participates in another meeting.");


    const Room* rm = getRoom(mt->getRoomId());
    ENSURE(rm, "Something went wrong. Encountered a meeting with invalid room id.");

    const Room* mt_room = getRoom(mt->getRoomId());
    ENSURE(mt_room, "Something went wrong. Found meeting in App but it did not have an assigned room.");

    Participations* ps_by_meeting = _getMutParticipationsByMeeting(participation->getMeetingId());
    ENSURE(ps_by_meeting, "Something went wrong. The list of participations by meeting went out of sync.");
    REQUIRE(ps_by_meeting->size() <= rm->getCapacity(), "Could not add participation: The room the meeting takes place in was full.");

    //REQUIRE(!isRoomOccupied(mt_room->getId(), mt->getDate()), "This rooms is already occupied.");
    REQUIRE(!isUserOccupied(participation->getUser(), mt->getDate()), "This user already participates in another meeting.");



    all_participations.push_back(participation); //Add it to a flat list of all participations


    //Also insert into map by userId
    Participations* ps_by_user =  _getMutParticipationsByUser(participation->getUser());
    if (ps_by_user) {
        ps_by_user->push_back(participation);
    } else {
        participations_by_user.insert({participation->getUser(), {participation}});
    }



    ps_by_meeting->push_back(participation);

    ENSURE(all_participations.back() == participation, "Something went wrong. The participation was not added to the global list.");
    ENSURE(_getMutParticipationsByUser(participation->getUser())->back() == participation, "Something went wrong. The participation was not added to the list by user.");
    ENSURE(_getMutParticipationsByMeeting(participation->getMeetingId())->back() == participation, "Something went wrong. The participation was not added to the list by app.");
}





const Participations & App::getAllParticipations() const {
    return all_participations;
}

const Participations* App::getParticipationsByUser(const std::string &userId) {
    return _getMutParticipationsByUser(userId);
}

const Participations* App::getParticipationsByMeeting(const std::string &meetindId) {
    return _getMutParticipationsByMeeting(meetindId);
}




















bool App::isUserOccupied(const std::string &userId, const Date &date) {
    Participations *prt_list = _getMutParticipationsByUser(userId);

    if (prt_list == nullptr) return false;

    for (Participations::iterator it = prt_list->begin(); it != prt_list->end(); ++it) {

        Participation* p = *it;
        const Meeting* m = getMeeting(p->getMeetingId());
        ENSURE(m, "Something went wrong. Encountered a participation without a valid meeting Id.");

        if (m->getDate() == date) return true;
    }

    return false;
}









App::~App() {
    for (const std::pair<const std::string, Room*>& r : rooms) delete r.second;
    for (const std::pair<const std::string, Meeting*>& m : all_meetings) delete m.second;
    for (Participation* p : all_participations) delete p;
}



void App::writeMeeting(std::ostream &onStream, const Meeting *meeting) {
    const Date date{meeting->getDate()};
    onStream << "- " << *getRoom(meeting->getRoomId()) << ", " << date.getWeekDay() << " " << date << std::endl;
    onStream << "  " << *meeting << std::endl;
    const Participations* participations = getParticipationsByMeeting(meeting->getId());
    onStream << "  ";
    for (Participation* participation : *participations) {
        onStream << participation->getUser();
        if (participation != participations->back()) onStream << ", ";
    }
    onStream << std::endl;
    onStream << "  Meeting ID: " << meeting->getId() << std::endl;
}

void App::writeRoom(std::ostream &onStream, const Room *room) {
    onStream << "- " << *room << std::endl;
    onStream << "  Capacity: " << room->getCapacity() << " people" << std::endl;
}

Meetings * App::_getMutMeetingsByRoom(const std::string &roomId) {
    const MeetingsByRoomMap::iterator it = meetings_by_room.find(roomId);

    if (it == meetings_by_room.end()) return nullptr;

    return &it->second;
}

Participations *App::_getMutParticipationsByUser(const std::string &userId) {
    const ParticipationsByUserMap::iterator it = participations_by_user.find(userId);

    if (it == participations_by_user.end()) return nullptr;

    return &it->second;
}

Participations *App::_getMutParticipationsByMeeting(const std::string &meetingId) {
    const ParticipationsByMeetingMap::iterator it = participations_by_meeting.find(meetingId);

    if (it == participations_by_meeting.end()) return nullptr;

    return &it->second;
}


