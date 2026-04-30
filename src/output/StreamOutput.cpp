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

void StreamOutput::printMeeting(const Meeting* meeting)
{
    *stream
    << "[ " << meeting->toString() << " (" << meeting->getId() << ") ]\n"
    << "  - Time: " << meeting->getDateTime() << "\n"
    << "  - Location: " << meeting->getRoom()->toString() << "\n"
    << "  - CO2 emitted: " << "999" << "g\n";

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
        *stream << "\n";
        printMeeting(m);
    }

    //Write all future meetings
    if (!unprocessed.empty()) *stream << "--==## Future meetings ##==--\n";
    for (const Meeting* m : unprocessed)
    {
        *stream << "\n";
        printMeeting(m);
    }

    //Write all conflicts
    if (!cancelled.empty()) *stream << "--==## Conflicts ##==--\n";
    for (const Meeting* m : cancelled)
    {
        *stream << "\n";
        printMeeting(m);
    }
}

void StreamOutput::printRoom(const Room* room)
{
    *stream << "[ " << room->toString() << " (" << room->getId() << ") ]\n";
    *stream << "  - Capacity: " << room->getCapacity() << " people\n";

    *stream << std::flush;
}

void StreamOutput::printRooms(const Rooms& rooms)
{
    if (!rooms.empty()) *stream << "--==## Rooms ##==--\n";
    for (const std::pair<std::string, Room*> item : rooms)
    {
        //*stream << "\n";
        const Room* room = item.second;
        printRoom(room);
    }
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


