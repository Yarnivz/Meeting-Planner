//
// Created by lucas on 5/7/26.
//

#ifndef MEETING_PLANNER_MUTEERROR_H
#define MEETING_PLANNER_MUTEERROR_H
#include "Error.h"


class MuteError : public Error
{
public:
    void uniqueId(const CampusElement& campus) const override {}

    void uniqueId(const BuildingElement& building) const override{}

    void uniqueId(const RoomElement& room) const override{}

    void uniqueId(const MeetingElement& meeting) const override{}

    void nonExistentCampus(const BuildingElement& building) const override{}

    void nonExistentCampus(const RoomElement& room) const override{}

    void nonExistentCampus(const CateringElement& catering) const override{}

    void nonExistentBuilding(const RoomElement& room) const override{}

    void nonExistentRoom(const MeetingElement& meeting) const override{}

    void nonExistentRoom(const RenovationElement& renovation) const override{}

    void campusBuildingMismatch(const RoomElement& room, const std::string& other_campus) const override{}

    void nonExistentMeeting(const ParticipationElement& participation) const override{}

    void externalsNotAllowed(const ParticipationElement& participation) const override{}

    void duplicateParticipation(const ParticipationElement& participation) const override{}

    void externalMismatch(const ParticipationElement& participation) const override{}
};
#endif //MEETING_PLANNER_MUTEERROR_H
