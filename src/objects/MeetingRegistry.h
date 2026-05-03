//
// Created by lucas on 4/5/26.
//

#ifndef MEETING_PLANNER_MEETINGREGISTRY_H
#define MEETING_PLANNER_MEETINGREGISTRY_H

#include <cstddef>
#include <list>
#include <unordered_map>
#include "objects/DateTime.h"
#include "objects/Meeting.h"

struct DateTimeHash
{
    std::size_t operator()(const DateTime& d) const;
};

class MeetingRegistry
{
public:
    MeetingRegistry() = default;

    /**
     * @brief prevents the MeetingRegistry object from being copied
     */
    MeetingRegistry(const MeetingRegistry&) = delete;

    /**
     * @brief Adds the meeting to the MeetingRegistry meeting map
     *
     * @param meeting The meeting to be added
     */
    void add(Meeting* meeting);

    /**
     * @brief gets the meeting that corresponds to the Id from the MeetinRegistry
     *
     * @param id The id of the meeting
     *
     * @return the meeting itself
     */
    Meeting* getById(const std::string& id) const;

    /**
     * @brief Gets a list of meetings in the MeetingRegistry meeting map that have a corresponding DateTime
     * @param date The DateTime to search with
     * @return the List containing all corresponding meetings
     */
    std::list<Meeting*>& getByDateTime(const DateTime& date);

    /**
     * @brief Removes the Meeting that corresponds to the Id from the MeetingRegistry meeting map and its DateTime
     * Will only remove the DateTime if there arent multiple meetings with the same one
     *
     * @param id the corresponding id of the meeting
     */
    void removeById(const std::string& id);

    /**
     * @brief gets the full MeetingRegistry meeting map
     * @return The meeting map itself
     */
    const std::unordered_map<std::string, Meeting*>& getRawIdMap() const;

    /**
     * @brief gets the full MeetingRegistry DateTime map
     * @return The DateTime map itself
     */
    const std::unordered_map<DateTime, std::list<Meeting*>, DateTimeHash>& getRawDateMap() const;
    //void removeByDate(const Date& date);

private:
    std::unordered_map<std::string, Meeting*> by_id;
    std::unordered_map<DateTime, std::list<Meeting*>, DateTimeHash> by_date;
};


#endif //MEETING_PLANNER_MEETINGREGISTRY_H
