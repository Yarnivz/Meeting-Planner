//
// Created by lucas on 4/23/26.
//

#include "StreamOutput.h"

#include "helper/DesignByContract.h"
#include "objects/Catering.h"
#include "objects/DateTime.h"
#include "objects/Meeting.h"
#include "objects/Room.h"
#include "objects/User.h"

StreamOutput::StreamOutput(std::ostream* stream) : stream(stream)
{
    REQUIRE(stream, "'stream' can't be null");
}

void StreamOutput::printUser(const User* user)
{
    *stream << user->getId();
    if (user->isExternal()) *stream << " (external)";
    *stream << "\n";
    *stream << std::flush;
}

void StreamOutput::printUsers(const Users& users)
{
    if (users.empty())
    {
        *stream << "--==## No Users ##==--\n" << std::flush;
        return;
    }

    *stream << "--==## Users ##==--\n";
    for (const std::pair<const std::string, User*>& u : users)
    {
        printUser(u.second);
        *stream << "\n";
    }
    *stream << std::flush;
}

void StreamOutput::printBuilding(const Building* building)
{
    *stream << building->toString() << std::flush;
}

void StreamOutput::printBuildings(const Buildings& buildings)
{
    if (buildings.empty())
    {
        *stream << "--==## No Buildings ##==--\n" << std::flush;
        return;
    }

    *stream << "--==## Buildings ##==--\n";
    for (const std::pair<const std::string, Building*>& b : buildings)
    {
        printBuilding(b.second);
        *stream << "\n";
    }
    *stream << std::flush;
}

void StreamOutput::printCampus(const Campus* campus)
{
    *stream
    << "[ " << campus->toString() << " (" << campus->getId() << ") ]\n";


    if (campus->getCaterings().empty())
        *stream << "  - No catering\n";
    else {
        *stream << "  - Catering:\n";
        for (const Catering* c : campus->getCaterings())
        {
            *stream << "      * " << c->getEmissions() << "g\n";
        }
    }

    *stream << std::flush;
}

void StreamOutput::printCampuses(const Campuses& campuses)
{
    if (campuses.empty())
    {
        *stream << "--==## No Campuses ##==--\n" << std::flush;
        return;
    }

    *stream << "--==## Campuses ##==--\n";
    for (const std::pair<const std::string, Campus*>& c : campuses)
    {
        printCampus(c.second);
        *stream << "\n";
    }

    *stream << std::flush;
}

void StreamOutput::printMeeting(const Meeting* meeting)
{
    *stream
    << "[ " << meeting->toString() << " (" << meeting->getId() << ") ]\n"
    << "  - Time: " << meeting->getDateTime() << "\n"
    << "  - Location: " << meeting->getRoom()->toString() << "\n"
    << "  - CO2 emitted: " << meeting->getEmissions() << "g\n";

    if (meeting->externalsAllowed())
        *stream
        << "  - Externals allowed\n";

    if (meeting->isOnline())
        *stream
        << "  - Online\n";

    if (meeting->cateringNeeded())
        *stream
        << "  - Catering needed\n"
        << "      * " << meeting->getCateringCosts() << "€\n";




    if (meeting->getParticipantCount() == 0)
    {
        *stream << "  - No participants\n";
    }
    else
    {
        *stream << "  - Participants: \n";
        for (const std::pair<const std::string, User*>& u : meeting->getParticipants())
        {
            *stream << "      * " << u.second->getId();
            if (u.second->isExternal()) *stream << " (external)";
            *stream << "\n";
        }
    }


    if (meeting->isCancelled())
    {
        *stream << "  - Cancellation reason: " << meeting->getCancellationReason() << "\n";
    }

    *stream << std::flush;
}

void StreamOutput::printMeetings(const MeetingRegistry& meetings)
{
    std::list<const Meeting*> cancelled, processed, unprocessed;

    //Sort by state
    for (const std::pair<const std::string, Meeting*>& item : meetings.getRawIdMap())
    {
        const Meeting* m = item.second;
        if (m->isUnProcessed())
        {
            unprocessed.push_back(m);
        }
        else if (m->isProcessed())
        {
            processed.push_back(m);
        }
        else if (m->isCancelled())
        {
            cancelled.push_back(m);
        }
    }

    //Write all past meetings
    if (!processed.empty()) *stream << "--==## Past meetings ##==--\n";
    for (const Meeting* m : processed)
    {
        printMeeting(m);
        *stream << "\n";
    }

    //Write all future meetings
    if (!unprocessed.empty()) *stream << "--==## Future meetings ##==--\n";
    for (const Meeting* m : unprocessed)
    {
        printMeeting(m);
        *stream << "\n";
    }

    //Write all conflicts
    if (!cancelled.empty()) *stream << "--==## Conflicts ##==--\n";
    for (const Meeting* m : cancelled)
    {
        printMeeting(m);
        *stream << "\n";
    }

    *stream << std::flush;
}

void StreamOutput::printRoom(const Room* room)
{
    *stream << "[ " << room->toString() << " (" << room->getId() << ") ]\n";
    *stream << "  - Capacity: " << room->getCapacity() << " people\n";

    *stream << std::flush;
}

void StreamOutput::printRooms(const Rooms& rooms)
{
    if (rooms.empty())
    {
        *stream << "--==## No Rooms ##==--\n" << std::flush;
        return;
    }

    *stream << "--==## Rooms ##==--\n";

    for (const std::pair<std::string, Room*> r : rooms)
    {
        printRoom(r.second);
        *stream << "\n";
    }
}


