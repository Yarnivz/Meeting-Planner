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

void ConciseError::fileNotFound(const std::string& filepath) const
{
    stream << "File '" << filepath << "' not found." << std::endl;
}

void ConciseError::fileInvalidXml(const std::string& filepath) const
{
    stream << "Invalid XML in '" << filepath << "'." << std::endl;
}

void ConciseError::syntaxNoRootElement() const
{
    stream << "No root XML element." << std::endl;
}

void ConciseError::objectUnrecognizedKind(const std::string& objectKind) const
{
    stream << objectKind << ": Unrecognized element." << std::endl;
}

void ConciseError::objectUnrecognizedProperty(const std::string& propertyKind, const std::string& objectKind) const
{
    stream << objectKind << ": Unrecognized property '" << propertyKind << "'." << std::endl;
}

void ConciseError::objectDuplicateProperty(const std::string& propertyKind, const std::string& objectKind) const
{
    stream << objectKind << ": Duplicate property " << propertyKind << "." << std::endl;
}

void ConciseError::objectMissingProperty(const std::string& propertyKind, const std::string& objectKind) const
{
    stream << objectKind << ": Missing property " << propertyKind << "." << std::endl;
}

void ConciseError::propertyEmpty(const std::string& propertyKind) const
{
    stream << propertyKind << ": Empty property." << std::endl;
}

void ConciseError::propertyUnrecognizedKind(const std::string& propertyKind) const
{
    stream << propertyKind << ": Unrecognized property." << std::endl;
}

void ConciseError::propertyFailedToParse(const std::string& propertyKind, const std::string& contents, const std::string& expected_type, const std::string& why) const
{
    stream << propertyKind << " (" << contents << "): Failed to parse as " << expected_type << ".";
    if (!why.empty()) stream << " (" << why << ")";
    stream << std::endl;
}

void ConciseError::propertyOutOfRange(const std::string& propertyKind, const std::string& contents, int lower, int upper) const
{
    stream << propertyKind << " (" << contents << "): Value must be between " << lower << " and " << upper << "." << std::endl;
}

void ConciseError::propertyNegative(const std::string& propertyKind, const std::string& contents) const
{
    stream << propertyKind << " (" << contents << "): Value cant be negative." << std::endl;
}

void ConciseError::INTERNAL_objectSwitchCaseMissing(const std::string& objectKind) const
{
    stream << "[INTERNAL] Element type " << objectKind << " recognized by elementValue map, but not handled in switch statement!" << std::endl;
}

void ConciseError::INTERNAL_propertySwitchCaseMissing(const std::string& propertyKind) const
{
    stream << "[INTERNAL] Property type " << propertyKind << " recognized by elementValue map, but not handled in switch statement!" << std::endl;
}
