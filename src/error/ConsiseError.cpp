//
// Created by lucas on 5/6/26.
//

#include "ConsiseError.h"

#define UNIQUE_ID_FMT(stream, id) \
(stream) << "\'" << id << "\': duplicate id!" << std::endl;
#define ID_NONEXISTENT_FMT(stream, id, non_existent_kind) \
(stream) <<  "\'" << id << "\': " non_existent_kind " doesn't exist!" << std::endl;
#define SHORT_NONEXISTENT_FMT(stream, kind, non_existent_kind) \
(stream) << kind ": " non_existent_kind " doesn't exist!" << std::endl;


ConsiseError::ConsiseError(std::ostream& os) : stream(os) {}

void ConsiseError::uniqueId(const CampusElement& campus) const
{
    UNIQUE_ID_FMT(stream, campus.id);
}

void ConsiseError::uniqueId(const BuildingElement& building) const
{
    UNIQUE_ID_FMT(stream, building.id);
}

void ConsiseError::uniqueId(const RoomElement& room) const
{
    UNIQUE_ID_FMT(stream, room.id);
}

void ConsiseError::uniqueId(const MeetingElement& meeting) const
{
    UNIQUE_ID_FMT(stream, meeting.id);
}


void ConsiseError::nonExistentCampus(const BuildingElement& building) const
{
    ID_NONEXISTENT_FMT(stream, building.id, "Campus");
}

void ConsiseError::nonExistentCampus(const RoomElement& room) const
{
    ID_NONEXISTENT_FMT(stream, room.id, "Campus");
}

void ConsiseError::nonExistentCampus(const CateringElement& catering) const
{
    SHORT_NONEXISTENT_FMT(stream, "Catering", "Campus");
}

void ConsiseError::nonExistentBuilding(const RoomElement& room) const
{
    ID_NONEXISTENT_FMT(stream, room.id, "Building");
}

void ConsiseError::nonExistentRoom(const MeetingElement& meeting) const
{
    ID_NONEXISTENT_FMT(stream, meeting.id, "Room");
}

void ConsiseError::nonExistentRoom(const RenovationElement& renovation) const
{
    SHORT_NONEXISTENT_FMT(stream, "Renovation", "Room");
}

void ConsiseError::campusBuildingMismatch(const RoomElement& room, const std::string& other_campus) const
{
    (void)room; (void)other_campus; // Supress 'unused parameter warnings'

    stream << "\'" << room.id << "\': mismatched Campus and Building!" << std::endl;
}

void ConsiseError::nonExistentMeeting(const ParticipationElement& participation) const
{
    ID_NONEXISTENT_FMT(stream, participation.user, "Meeting");
}

void ConsiseError::externalsNotAllowed(const ParticipationElement& participation) const
{
    stream << "\'" << participation.user << "\': external User not allowed in Meeting!" << std::endl;
}

void ConsiseError::duplicateParticipation(const ParticipationElement& participation) const
{
    stream << "\'" << participation.user << "\': participates in same Meeting twice!" << std::endl;
}

void ConsiseError::externalMismatch(const ParticipationElement& participation) const
{

    const std::string& is = participation.external ? "external" : "internal";
    const std::string& isnt = participation.external ? "internal" : "external";

    stream << "\'" << participation.user << "\': registered as " << isnt << ", but now marked as " << is << "!" << std::endl;
}