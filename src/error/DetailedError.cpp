//
// Created by lucas on 5/6/26.
//

#include "DetailedError.h"
#include "parser/Parser.h"

#define UNIQUE_ID_FMT(stream, object_kind, id) \
    (stream) << object_kind " Ids must be unique: \'" << (id) << "\'!" << std::endl;
#define ID_NONEXISTENT_FMT(stream, kind, id, verb, non_existent_kind, non_existent_id) \
(stream) << kind " \'" << id << "\' " verb " a " non_existent_kind " \'" << non_existent_id << "\' which doesn't exist!" << std::endl;
#define NAME_NONEXISTENT_FMT(stream, kind, name, verb, non_existent_kind, non_existent_id) \
(stream) << kind " " << name << " " verb " a " non_existent_kind " \'" << non_existent_id << "\' which doesn't exist!" << std::endl;
#define SHORT_NONEXISTENT_FMT(stream, kind, verb, non_existent_kind, non_existent_id) \
(stream) << kind " " verb " a " non_existent_kind " \'" << non_existent_id << "\' which doesn't exist!" << std::endl;

DetailedError::DetailedError(std::ostream& os) : stream(os) {}

void DetailedError::uniqueId(const CampusElement& campus) const
{
    UNIQUE_ID_FMT(stream, "Campus", campus.id);
}

void DetailedError::uniqueId(const BuildingElement& building) const
{
    UNIQUE_ID_FMT(stream, "Building", building.id);
}

void DetailedError::uniqueId(const RoomElement& room) const
{
    UNIQUE_ID_FMT(stream, "Room", room.id);
}

void DetailedError::uniqueId(const MeetingElement& meeting) const
{
    UNIQUE_ID_FMT(stream, "Meeting", meeting.id);
}


void DetailedError::nonExistentCampus(const BuildingElement& building) const
{
    ID_NONEXISTENT_FMT(stream, "Building", building.id, "is in", "Campus", building.campus_id);
}

void DetailedError::nonExistentCampus(const RoomElement& room) const
{
    ID_NONEXISTENT_FMT(stream, "Room", room.id, "is in", "Campus", room.campus_id);
}

void DetailedError::nonExistentCampus(const CateringElement& catering) const
{
    SHORT_NONEXISTENT_FMT(stream, "Catering", "is for", "Campus", catering.campus_id);
}

void DetailedError::nonExistentBuilding(const RoomElement& room) const
{
    ID_NONEXISTENT_FMT(stream, "Room", room.id, "is in", "Building", room.building_id);
}

void DetailedError::nonExistentRoom(const MeetingElement& meeting) const
{
    ID_NONEXISTENT_FMT(stream, "Meeting", meeting.id, "takes place in" , "Room", meeting.room_id);
}

void DetailedError::nonExistentRoom(const RenovationElement& renovation) const
{
    const std::string name = "from " + renovation.start_date.toString() + " to " + renovation.end_date.toString();
    NAME_NONEXISTENT_FMT(stream, "Renovation", name, "takes place in", "Room", renovation.room);
}

void DetailedError::campusBuildingMismatch(const RoomElement& room, const std::string& other_campus) const
{
    stream << "Room \'" << room.id << "\' is in a campus \'" << room.campus_id << "\', but it's building \'" << room.building_id
    << "\' is in a different campus \'" << other_campus << "\'." << std::endl;
}

void DetailedError::nonExistentMeeting(const ParticipationElement& participation) const
{
    ID_NONEXISTENT_FMT(stream, "User", participation.user, "participates in", "Meeting", participation.meeting);
}

void DetailedError::externalsNotAllowed(const ParticipationElement& participation) const
{
    stream << "External User \'" << participation.user << "\' is not allowed to participate in Meeting \'"
    << participation.meeting << "\' which doesn't allow externals!" << std::endl;
}

void DetailedError::duplicateParticipation(const ParticipationElement& participation) const
{
    stream << "User \'" << participation.user << "\' participates in the same Meeting \'" << participation.meeting << "\' twice!" << std::endl;
}

void DetailedError::externalMismatch(const ParticipationElement& participation) const
{

    const std::string& is = participation.external ? "external" : "internal";
    const std::string& isnt = participation.external ? "internal" : "external";

    stream << "User \'" << participation.user << "\' was marked as " << is << " for Meeting \'" << participation.meeting << "\' "
    "but was registered as " << isnt << " earlier!" << std::endl;
}
