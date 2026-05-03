//
// Created by lucas on 4/5/26.
//

#ifndef MEETING_PLANNER_USER_H
#define MEETING_PLANNER_USER_H

#include <string>

#include "MeetingRegistry.h"
class Meeting;
class Date;


class User
{
    friend class Meeting;

public:

    /**
     * @brief Creates the User class.
     *
     * @param name The users name
     * @param external The external status of the user
     */
    User(const std::string& name, bool external = false);


    /**
     * @brief Copies a User object.
     * It copies all important values but makes sure the 'properlyInitialized' test still passes.
     *
     * @param o The user to be copied
     */
    User(const User& o);
    User& operator=(const User& o);

    ~User() = default;

    /**
     * @brief Checks whether this User was properly initialized by the constructor.
     * @return bool indicating result
     */
    bool isProperlyInitialized() const;

    /**
     * @brief Id getter.
     *
     * @pre name/Id cannot be empty
     *
     * @return the Id of the user.
     */
    const std::string& getId() const;

    /**
     * @brief checks wether user is external
     * @return bool indicating the result
    */
    bool isExternal() const;

    /**
     * @brief adds the meeting to the users Meetingregistery
     * @param m meeting to add
     */
    void addMeeting(Meeting* m);

    /**
     * @brief Gets the meeting that corresponds to the id
     * @param meetingId The meetings id
     * @return the meeting itself
     */
    Meeting* getMeetingById(const std::string& meetingId) const;

    /**
     * @brief Gets a list of meetings that correspond to the DateTime
     * @param The meetings DateTime
     * @return a list the meetings
     */
    std::list<Meeting*>& getMeetingByDateTime(const DateTime& meetingDateTime);

private:
    void _addMeeting(Meeting* meeting);

    std::string name;
    bool external;

    MeetingRegistry meetings;

    void* init_check_this_ptr;
};


#endif //MEETING_PLANNER_USER_H
