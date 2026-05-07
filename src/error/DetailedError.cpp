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














void DetailedError::fileNotFound(const std::string& filepath) const
{
    stream << "Could not find file '" << filepath << "', does it exist?" << std::endl;
}

void DetailedError::fileInvalidXml(const std::string& filepath) const
{
    stream << "Could not parse XML file '" << filepath << "', check the syntax!" << std::endl;
}

void DetailedError::syntaxNoRootElement() const
{
    stream << "Could not parse XML, no root element exists!" << std::endl;
}

void DetailedError::objectUnrecognizedKind(const std::string& objectKind) const
{
    stream << "'" << objectKind << "' is not a recognized element type." << std::endl;
}

void DetailedError::objectUnrecognizedProperty(const std::string& propertyKind, const std::string& objectKind) const
{
    stream << objectKind << " can not have property '" << propertyKind << "'!"<< std::endl;
}

void DetailedError::objectDuplicateProperty(const std::string& propertyKind, const std::string& objectKind) const
{
    stream << objectKind << " has duplicate property " << propertyKind << "!" << std::endl;
}

void DetailedError::objectMissingProperty(const std::string& propertyKind, const std::string& objectKind) const
{
    stream << objectKind << " is missing required property " << propertyKind << std::endl;
}

void DetailedError::propertyEmpty(const std::string& propertyKind) const
{
    stream << propertyKind << " property must contain text." << std::endl;
}

void DetailedError::propertyUnrecognizedKind(const std::string& propertyKind) const
{
    stream << "'" << propertyKind << "' is not a recognized property!" << std::endl;
}

void DetailedError::propertyFailedToParse(const std::string& propertyKind, const std::string& contents, const std::string& expected_type, const std::string& why) const
{
    stream << propertyKind << ": failed to parse " << contents << "' as " << expected_type << "!";
    if (!why.empty()) stream << " " << why;
    stream << std::endl;
}

void DetailedError::propertyOutOfRange(const std::string& propertyKind, const std::string& contents, int lower, int upper) const
{
    stream << propertyKind << " must contain a value between " << lower << " and " << upper << ", not " << contents << "!" << std::endl;
}

void DetailedError::propertyNegative(const std::string& propertyKind, const std::string& contents) const
{
    stream << propertyKind << " must be non-negative, not " << contents << std::endl;
}

void DetailedError::INTERNAL_objectSwitchCaseMissing(const std::string& objectKind) const
{
    stream << "[INTERNAL] Element type " << objectKind << " recognized by elementValue map, but not handled in switch statement!" << std::endl;
}

void DetailedError::INTERNAL_propertySwitchCaseMissing(const std::string& propertyKind) const
{
    stream << "[INTERNAL] Property type " << propertyKind << " recognized by elementValue map, but not handled in switch statement!" << std::endl;
}