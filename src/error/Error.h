//
// Created by lucas on 5/6/26.
//

#ifndef MEETING_PLANNER_ERROR_H
#define MEETING_PLANNER_ERROR_H
#include <string>
#include "parser/Elements.h"

class Error
{
public:
    Error() = default;
    virtual ~Error() = default;

    /* App */
    virtual void uniqueId(const CampusElement& campus) const = 0;
    virtual void uniqueId(const BuildingElement& building) const = 0;
    virtual void uniqueId(const RoomElement& room) const = 0;
    virtual void uniqueId(const MeetingElement& meeting) const = 0;

    virtual void nonExistentCampus( const BuildingElement& building ) const = 0;
    virtual void nonExistentCampus( const RoomElement& room ) const = 0;
    virtual void nonExistentCampus( const CateringElement& catering ) const = 0;

    virtual void nonExistentBuilding( const RoomElement& room ) const = 0;

    virtual void nonExistentRoom( const MeetingElement& meeting ) const = 0;
    virtual void nonExistentRoom( const RenovationElement& renovation ) const = 0;

    virtual void campusBuildingMismatch(const RoomElement& room, const std::string& other_campus) const = 0;

    virtual void nonExistentMeeting( const ParticipationElement& participation ) const = 0;

    virtual void externalsNotAllowed( const ParticipationElement& participation ) const = 0;

    virtual void duplicateParticipation( const ParticipationElement& participation ) const = 0;

    virtual void externalMismatch( const ParticipationElement& participation ) const = 0;


    /* Parser */
    virtual void fileNotFound(const std::string& filepath) const = 0;
    virtual void fileInvalidXml(const std::string& filepath) const = 0;

    virtual void syntaxNoRootElement() const = 0;

    virtual void objectUnrecognizedKind(const std::string& objectKind) const = 0;
    virtual void objectUnrecognizedProperty(const std::string& propertyKind, const std::string& objectKind) const = 0;
    virtual void objectDuplicateProperty(const std::string& propertyKind, const std::string& objectKind) const = 0;
    virtual void objectMissingProperty(const std::string& propertyKind, const std::string& objectKind) const = 0;

    virtual void propertyEmpty(const std::string& propertyKind) const = 0;
    virtual void propertyUnrecognizedKind(const std::string& propertyKind) const = 0;

    virtual void propertyFailedToParse(const std::string& propertyKind, const std::string& contents, const std::string& expected_type, const std::string& why = "") const = 0;
    virtual void propertyOutOfRange(const std::string& propertyKind, const std::string& contents, int lower, int upper) const = 0;
    virtual void propertyNegative(const std::string& propertyKind, const std::string& contents) const = 0;

    virtual void INTERNAL_objectSwitchCaseMissing(const std::string& objectKind) const = 0;
    virtual void INTERNAL_propertySwitchCaseMissing(const std::string& propertyKind) const = 0;
};


#endif //MEETING_PLANNER_ERROR_H
