//
// Created by lucas on 4/23/26.
//

#include "StreamOutput.h"

#include "helper/DesignByContract.h"
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
    const DateTime& date_time = meeting->getDateTime();

    *stream << "- " << *(meeting->getRoom()) << ", " << date_time.getWeekDay() << " " << date_time << std::endl;
    *stream << "  " << *meeting << std::endl;

    *stream << "  ";
    for (Users::const_iterator it = meeting->getParticipants().begin(); it != meeting->getParticipants().end(); ++it)
    {
        const User* participant = it->second;
        *stream << participant->getId();
        if (std::next(it) != meeting->getParticipants().end()) *stream << ", ";
    }
    *stream << std::endl;
    *stream << "  Meeting ID: " << meeting->getId() << std::endl;
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
    if (!processed.empty()) *stream << std::endl << "Past meetings:" << std::endl;
    for (const Meeting* m : processed)
    {
        printMeeting(m);
    }

    //Write all future meetings
    if (!unprocessed.empty()) *stream << std::endl << "Future meetings:" << std::endl;
    for (const Meeting* m : unprocessed)
    {
        printMeeting(m);
    }

    //Write all conflicts
    if (!cancelled.empty()) *stream << std::endl << "Conflicts:" << std::endl;
    for (const Meeting* m : cancelled)
    {
        printMeeting(m);
        *stream << "  Reason: " << m->getCancellationReason() << std::endl;
    }
}

void StreamOutput::printRoom(const Room* room)
{
    *stream << "- " << *room << std::endl;
    *stream << "  Capacity: " << room->getCapacity() << " people" << std::endl;
}

void StreamOutput::printRooms(const Rooms& rooms)
{
    if (!rooms.empty()) *stream << std::endl << "Rooms:" << std::endl;
    for (const std::pair<std::string, Room*> item : rooms)
    {
        const Room* room = item.second;
        printRoom(room);
    }
}
