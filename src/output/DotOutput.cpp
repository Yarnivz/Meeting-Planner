//
// Created by tishaak on 5/14/26.
//

#include "DotOutput.h"

#include "objects/Building.h"
#include "objects/Campus.h"
#include "objects/Meeting.h"
#include "objects/MeetingRegistry.h"
#include "objects/Room.h"
#include "objects/User.h"

DotOutput::DotOutput(const std::string& fileName) : stream(fileName)
{
    if (!stream.is_open())
    {
        std::cerr << "couldnt open/create output.dot";
        return;
    }
    stream << "graph Meeting" << '\n' << "{" << '\n';
}

DotOutput::~DotOutput()
{
    stream << "}" << '\n';
    stream.close();

    //int code = system("dot -Tpng output.dot -o dotOutput.png");
    //std::cout << "dot png conversion command exited with code: " << code << '\n';
}

void DotOutput::printUser(const User* user) const
{

    for (const std::pair<const std::string, Meeting*>& meeting : meetingsMap)
    {
        if (meeting.second->getParticipants().contains(user->getId()))
        {
            stream << "\"" << meeting.first << "\"" << " -- " << "\"" << user->getId() << "\"" << '\n';
        }
    }
}

void DotOutput::printUsers(const Users& users) const
{
    for (const std::pair<const std::string, User*>& user : users)
    {
        printUser(user.second);
    }
}

void DotOutput::printBuilding(const Building* building) const
{
    stream << "\"" << building->getId() << "\" [label=\"" << building->toString() << " (" << building->getId() << ")\"]\n";
    stream << "\"" << building->getCampus()->getId() << "\"" << " -- " << "\"" << building->getId() << "\"" << '\n';
}

void DotOutput::printBuildings(const Buildings& buildings) const
{
    for (const std::pair<const std::string, Building*>& building : buildings)
    {
        printBuilding(building.second);
    }
}

void DotOutput::printCampus(const Campus* campus) const
{
    stream << "\"" << campus->getId() << "\" [label=\"" << campus->toString() << " (" << campus->getId() << ")\"]\n";
    stream << "Uantwerpen -- " << "\"" << campus->getId() << "\"" << '\n';
}

void DotOutput::printCampuses(const Campuses& campuses) const
{
    for (const std::pair<const std::string, Campus*>& campus : campuses)
    {
        printCampus(campus.second);
    }
}



void DotOutput::printMeeting(const Meeting* meeting) const
{
    stream << "\"" << meeting->getId() << "\" [label=\"" << meeting->toString() << " (" << meeting->getId() << ")\"]\n";
    stream << "\"" << meeting->getRoom()->getId() << "\"" << " -- " << "\"" << meeting->getId() << "\"" << '\n';
}

void DotOutput::printMeetings(const MeetingRegistry& meetings) const
{
    for (const std::pair<const std::string, Meeting*>& meeting : meetings.getRawIdMap())
    {
        printMeeting(meeting.second);
    }
    meetingsMap = meetings.getRawIdMap();
}

void DotOutput::printRoom(const Room* room) const
{
    stream << "\"" << room->getId() << "\" [label=\"" << room->toString() << " (" << room->getId() << ")\"]\n";
    stream << "\"" << room->getBuilding()->getId() << "\"" << " -- " << "\"" << room->getId() << "\"" << '\n';
}

void DotOutput::printRooms(const Rooms& rooms) const
{
    for (const std::pair<const std::string, Room*>& room : rooms)
    {
        printRoom(room.second);
    }
}
