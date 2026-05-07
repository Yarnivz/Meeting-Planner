//
// Created by lucas on 5/6/26.
//

#ifndef MEETING_PLANNER_CONSISEERROR_H
#define MEETING_PLANNER_CONSISEERROR_H

#include <iostream>
#include "Error.h"


class ConciseError : public Error
{
public:
    ConciseError(std::ostream& os);

    void uniqueId(const CampusElement& campus) const override;
    void uniqueId(const BuildingElement& building) const override;
    void uniqueId(const RoomElement& room) const override;
    void uniqueId(const MeetingElement& meeting) const override;

    void nonExistentCampus(const BuildingElement& building) const override;
    void nonExistentCampus(const RoomElement& room) const override;
    void nonExistentCampus(const CateringElement& catering) const override;

    void nonExistentBuilding(const RoomElement& room) const override;

    void nonExistentRoom(const MeetingElement& meeting) const override;
    void nonExistentRoom(const RenovationElement& renovation) const override;

    void campusBuildingMismatch(const RoomElement& room, const std::string& other_campus) const override;

    void nonExistentMeeting(const ParticipationElement& participation) const override;

    void externalsNotAllowed(const ParticipationElement& participation) const override;

    void duplicateParticipation(const ParticipationElement& participation) const override;

    void externalMismatch(const ParticipationElement& participation) const override;

    void fileNotFound(const std::string& filepath) const override;
    void fileInvalidXml(const std::string& filepath) const override;
    void syntaxNoRootElement() const override;
    void objectUnrecognizedKind(const std::string& objectKind) const override;
    void objectUnrecognizedProperty(const std::string& propertyKind, const std::string& objectKind) const override;
    void objectDuplicateProperty(const std::string& propertyKind, const std::string& objectKind) const override;
    void objectMissingProperty(const std::string& propertyKind, const std::string& objectKind) const override;
    void propertyEmpty(const std::string& propertyKind) const override;
    void propertyUnrecognizedKind(const std::string& propertyKind) const override;
    void propertyFailedToParse(const std::string& propertyKind, const std::string& contents, const std::string& expected_type, const std::string& why) const override;
    void propertyOutOfRange(const std::string& propertyKind, const std::string& contents, int lower, int upper) const override;
    void propertyNegative(const std::string& propertyKind, const std::string& contents) const override;
    void INTERNAL_objectSwitchCaseMissing(const std::string& objectKind) const override;
    void INTERNAL_propertySwitchCaseMissing(const std::string& propertyKind) const override;
private:
    std::ostream& stream;
};


#endif //MEETING_PLANNER_CONSISEERROR_H
