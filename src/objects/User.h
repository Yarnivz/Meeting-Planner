//
// Created by lucas on 4/5/26.
//

#ifndef MEETING_PLANNER_USER_H
#define MEETING_PLANNER_USER_H

#include <string>

#include "MeetingRegistry.h"
class Meeting;
class Date;


class User {
    friend class Meeting;

public:

    User(const std::string& name, bool external = false);
    ~User() = default;

    /**
     * @brief Checks whether this User was properly initialized by the constructor.
     * @return bool indicating result
     */
    bool isProperlyInitialized() const;

    /**
     *
     *@pre name/Id cannot be empty
     *
     */
    const std::string& getId() const;
    bool isExternal() const;

    void addMeeting(Meeting* m);
    Meeting* getMeetingById(const std::string& meetingId);
    std::list<Meeting*>& getMeetingByDateTime(const DateTime& meetingDateTime);

private:
    void _addMeeting(Meeting* meeting);

    std::string name;
    bool external;

    MeetingRegistry meetings;

    void* init_check_this_ptr;
};



#endif //MEETING_PLANNER_USER_H
