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
    *stream << "[ " << user->getId() << " ]\n";
    if (user->isExternal()) *stream << "  - external\n";
    *stream << std::flush;
}

void StreamOutput::printUsers(const Users& users)
{
    if (users.empty())
    {
        *stream << "--==## No Users ##==--\n\n" << std::flush;
        return;
    }

    *stream << "--==## Users ##==--\n\n";
    for (const std::pair<const std::string, User*>& u : users)
    {
        printUser(u.second);
        //*stream << "\n";
    }
    *stream << std::endl;
}

void StreamOutput::printBuilding(const Building* building)
{
    *stream << "[ " << building->toString() << " (" << building->getId() << ") ]\n"
    << "  - Campus: " << building->getCampus()->toString() << " ("<< building->getCampus()->getId() <<")" << std::endl;
}

void StreamOutput::printBuildings(const Buildings& buildings)
{
    if (buildings.empty())
    {
        *stream << "--==## No Buildings ##==--\n\n" << std::flush;
        return;
    }

    *stream << "--==## Buildings ##==--\n\n";
    for (const std::pair<const std::string, Building*>& b : buildings)
    {
        printBuilding(b.second);
        //*stream << "\n";
    }
    *stream << std::endl;
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
        *stream << "--==## No Campuses ##==--\n\n" << std::flush;
        return;
    }

    *stream << "--==## Campuses ##==--\n\n";
    for (const std::pair<const std::string, Campus*>& c : campuses)
    {
        printCampus(c.second);
        //*stream << "\n";
    }

    *stream << std::endl;
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
    if (processed.empty()) *stream << "--==## No past meetings ##==--\n\n";
    else
    {
        *stream << "--==## Past meetings ##==--\n\n";
        for (const Meeting* m : processed)
        {
            printMeeting(m);
            //*stream << "\n";
        }
        *stream << "\n";
    }



    //Write all future meetings
    if (unprocessed.empty()) *stream << "--==## No future meetings ##==--\n\n";
    else
    {
        *stream << "--==## Future meetings ##==--\n\n";
        for (const Meeting* m : unprocessed)
        {
            printMeeting(m);
            //*stream << "\n";
        }
        *stream << "\n";
    }



    //Write all conflicts
    if (cancelled.empty()) *stream << "--==## No cancelled meetings ##==--\n\n";
    else
    {
        *stream << "--==## Cancelled meetings ##==--\n\n";
        for (const Meeting* m : cancelled)
        {
            printMeeting(m);
            //*stream << "\n";
        }
        *stream << "\n";
    }

    *stream << std::flush;
}

void StreamOutput::printRoom(const Room* room)
{
    const unsigned cap = room->getCapacity();
    *stream << "[ " << room->toString() << " (" << room->getId() << ") ]\n"
    << "  - Capacity: " << cap << (cap == 1 ? " person\n" : " people\n")
    << "  - Building: " << room->getBuilding()->toString() << " (" << room->getBuilding()->getId() << ")\n"
    << "  - Campus: " << room->getCampus()->toString() << " (" << room->getCampus()->getId() << ")\n";

    *stream << std::flush;
}

void StreamOutput::printRooms(const Rooms& rooms)
{
    if (rooms.empty())
    {
        *stream << "--==## No Rooms ##==--\n\n" << std::flush;
        return;
    }

    *stream << "--==## Rooms ##==--\n\n";

    for (const std::pair<std::string, Room*> r : rooms)
    {
        printRoom(r.second);
        //*stream << "\n";
    }
    *stream << std::endl;
}


void StreamOutput::printMeetingCO2(const Meeting* meeting)
{
    unsigned num_externals; float externals_emissions;
    unsigned num_internals; float internals_emissions;
    unsigned num_online; float online_emissions;
    unsigned num_catering_participants; float catering_emission;
    meeting->getEmissionDetails(num_externals, externals_emissions, num_internals, internals_emissions, num_online, online_emissions, num_catering_participants, catering_emission);

    *stream << "[ " << meeting->toString() << " (" << meeting->getId() << ") ]\n";
    if (num_externals > 0)
    {
        *stream
        << "  - Externals (" << num_externals << "): "
        << externals_emissions << "g, "
        << externals_emissions/num_externals << "g/person\n";
    }
    if (num_internals > 0)
    {
        *stream
        << "  - Internals (" << num_internals << "): "
        << internals_emissions << "g, "
        << internals_emissions/num_internals << "g/person\n";
    }
    if (num_online > 0)
    {
        *stream
        << "  - Online (" << num_online << "): "
        << online_emissions << "g, "
        << online_emissions/num_online << "g/person\n";
    }
    if (num_catering_participants > 0)
    {
        *stream
            << "  - Catering (" << num_catering_participants << "): "
            << catering_emission << "g, "
            << catering_emission/num_catering_participants << "g/person\n";
    }

    float total_emissions = externals_emissions + internals_emissions + online_emissions + catering_emission;
    unsigned total_participants = meeting->getParticipantCount();
    *stream << "    = Total: " << total_emissions << "g";
    if (total_participants > 0)
    {
        *stream << ", " << total_emissions/total_participants << "g/person";
    }
    *stream << "\n";

    *stream << std::flush;
}

void StreamOutput::printMeetingsCO2(const MeetingRegistry& registry)
{
    *stream << "--==## CO2 Summary ##==--\n\n";
    for (const std::pair<const std::string, Meeting*>& item : registry.getRawIdMap())
    {
        const Meeting* m = item.second;
        printMeetingCO2(m);
    }
}

