//
// Created by lucas on 5/6/26.
//

#include "ConciseError.h"

#define UNIQUE_ID_FMT(stream, id) \
(stream) << "\'" << id << "\': duplicate id!" << std::endl;
#define ID_NONEXISTENT_FMT(stream, id, non_existent_kind) \
(stream) <<  "\'" << id << "\': " non_existent_kind " doesn't exist!" << std::endl;
#define SHORT_NONEXISTENT_FMT(stream, kind, non_existent_kind) \
(stream) << kind ": " non_existent_kind " doesn't exist!" << std::endl;


ConciseError::ConciseError(std::ostream& os) : stream(os) {}

void ConciseError::uniqueId(const CampusElement& campus) const
{
    UNIQUE_ID_FMT(stream, campus.id);
}

void ConciseError::uniqueId(const BuildingElement& building) const
{
    UNIQUE_ID_FMT(stream, building.id);
}

void ConciseError::uniqueId(const RoomElement& room) const
{
    UNIQUE_ID_FMT(stream, room.id);
}

void ConciseError::uniqueId(const MeetingElement& meeting) const
{
    UNIQUE_ID_FMT(stream, meeting.id);
}


void ConciseError::nonExistentCampus(const BuildingElement& building) const
{
    ID_NONEXISTENT_FMT(stream, building.id, "Campus");
}

void ConciseError::nonExistentCampus(const RoomElement& room) const
{
    ID_NONEXISTENT_FMT(stream, room.id, "Campus");
}

void ConciseError::nonExistentCampus(const CateringElement& catering) const
{
    SHORT_NONEXISTENT_FMT(stream, "Catering", "Campus");
}

void ConciseError::nonExistentBuilding(const RoomElement& room) const
{
    ID_NONEXISTENT_FMT(stream, room.id, "Building");
}

void ConciseError::nonExistentRoom(const MeetingElement& meeting) const
{
    ID_NONEXISTENT_FMT(stream, meeting.id, "Room");
}

void ConciseError::nonExistentRoom(const RenovationElement& renovation) const
{
    SHORT_NONEXISTENT_FMT(stream, "Renovation", "Room");
}

void ConciseError::campusBuildingMismatch(const RoomElement& room, const std::string& other_campus) const
{
    (void)room; (void)other_campus; // Supress 'unused parameter warnings'

    stream << "\'" << room.id << "\': mismatched Campus and Building!" << std::endl;
}

void ConciseError::nonExistentMeeting(const ParticipationElement& participation) const
{
    ID_NONEXISTENT_FMT(stream, participation.user, "Meeting");
}

void ConciseError::externalsNotAllowed(const ParticipationElement& participation) const
{
    stream << "\'" << participation.user << "\': external User not allowed in Meeting!" << std::endl;
}

void ConciseError::duplicateParticipation(const ParticipationElement& participation) const
{
    stream << "\'" << participation.user << "\': participates in same Meeting twice!" << std::endl;
}

void ConciseError::externalMismatch(const ParticipationElement& participation) const
{

    const std::string& is = participation.external ? "external" : "internal";
    const std::string& isnt = participation.external ? "internal" : "external";

    stream << "\'" << participation.user << "\': registered as " << isnt << ", but now marked as " << is << "!" << std::endl;
}