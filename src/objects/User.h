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
     *
     * @post User creation failed. Object was not properly initialized.
     * @post User creation failed. Name/Id was not correctly set.
     * @post User creation failed. External flag was not correctly set.
     */
    User(const std::string& name, bool external = false);


    /**
     * @brief Copies a User object.
     * It copies all important values but makes sure the 'properlyInitialized' test still passes.
     *
     *
     * @pre Attempted to copy a User which was not properly initialized.
     *
     * @param o The user to be copied
     *
     * @post User copy failed. Object was not properly initialized.
     * @post User creation failed. Name/Id was not correctly set.
     * @post User creation failed. External flag was not correctly set.
     */
    User(const User& o);

    /**
     *
     *@pre Attempted to copy a User which was not properly initialized.
     *
     *
     *@post User copy failed. Object was not properly initialized.
     *@post User creation failed. Name/Id was not correctly set.
     *@post User creation failed. External flag was not correctly set.
     */
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
     *
     * @pre Meeting id cannot be empty.
     *
     * @param meetingId The meetings id
     *
     * @post MeetingId must be in MeetingRegistery
     * @return the meeting itself
     */
    Meeting* getMeetingById(const std::string& meetingId) const;

    /**
     * @brief Gets a list of meetings that correspond to the DateTime
     * @param The meetings DateTime
     *
     * @post DateTime must be in MeetingRegistry
     * @return a list the meetings
     */
    std::list<Meeting*>& getMeetingByDateTime(const DateTime& meetingDateTime);

private:

    /**
     *
     *@pre Meeting can not be null
     *@pre Meeting must be properly initialized.
     *@pre Can't add external user %s to meeting %s which doesn't allow external users.
     *
     *
     *@post User %s must be added to meeting %s.
     */
    void _addMeeting(Meeting* meeting);

    std::string name;
    bool external;

    MeetingRegistry meetings;

    void* init_check_this_ptr;
};


#endif //MEETING_PLANNER_USER_H
