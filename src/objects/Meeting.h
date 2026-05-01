//
// Created by Yarni on 2/26/2026.
//

#ifndef MEETING_PLANNER_MEETING_H
#define MEETING_PLANNER_MEETING_H

#include <string>

#include "DateTime.h"
#include "../TypeDefs.h"

class User;
class Room;


class Meeting
{
    friend class User;

public:
    //put here for now as fill in for later
    std::vector<std::pair<unsigned int, unsigned int>> participantsToRoomsSize;

    /**
     * @brief Creates the Meeting class.
     * The process of users gathering in a specific room for a specific purpose for a certain amount of time on a set date.
     * A meeting can only be initialized using the label, id, room id and date parameters as these are required.
     * @param label title of the meeting
     * @param id identifier of the meeting
     * @param room identifier of the room where the meeting takes place
     * @param date_time of when the meeting takes/took place
     * @param order order in which the meeting is added to the system
     */
    Meeting(const std::string& label, const std::string& id, Room* room, const DateTime& date_time = DateTime(), const  bool& online = false, bool externals_allowed = false, bool catering_needed = false);

    Meeting(const Meeting&) = delete;

    /**
     * @brief Checks whether this Participation was properly initialized by the constructor.
     * @return bool indicating result
     */
    bool isProperlyInitialized() const;

    /**
     * @brief Id getter.
     *
     *
     * @pre Failed to get id. Meeting has to be properly initialized with the constructor.
     *
     * @return the identifier of this meeting
     */
    const std::string& getId() const;

    /**
     * @brief Room id getter.
     *
     *
     * @pre Failed to get room. Meeting has to be properly initialized with the constructor.
     *
     * @return the identifier of the room where this meeting takes place
     */
    Room* getRoom() const;


    /**
     * @brief Date getter.
     *
     *
     * @pre Failed to get date. Meeting has to be properly initialized with the constructor.
     *
     * @return the date of when this meeting takes place
     */

    const DateTime& getDateTime() const;

    /**
     * @brief Converts the meeting class to a readable string format.
     *
     *
     * @pre Failed to convert to string. Meeting has to be properly initialized with the constructor.
     *
     * @return the meeting in string format
     */
    const std::string& toString() const;

    /**
     * @brief The orders getter
     * @post Order can not be negative
     */
    int getOrder() const;

    /**
     * @brief The orders setter
     *
     * Sets the order of the Meeting.
     * When processing multiple meetings, the meeting with the *lowest* order is prioritized.
     * An order of zero means you allow the App class to decide the order of this meeting when registering it.
     *
     * @param orderAdded of the meeting
     */
    void setOrder(const int orderAdded);


    /**
     *
     * @pre Something went wrong. Meeting was not processed.
     *
     *
     * @post Meeting was already processed or canceled.
     */
    void process();
    /**
     *
     *@pre Something went wrong. Meeting was not cancelled.
     *
     *
     *@post Meeting was already processed or canceled
     */
    void cancel(const std::string& reason);

    bool isUnProcessed() const;

    bool isProcessed() const;

    bool isCancelled() const;

    bool externalsAllowed() const;
    bool cateringNeeded() const;

    /**
     * @brief online bool getter.
     *
     *
     * @pre Failed to get online status. Meeting has to be properly initialized with the constructor.
     *
     * @return online status of meeting
     */
    bool isOnline() const;



    /**
     *
     *@post Meeting was not cancelled.
     */
    const std::string& getCancellationReason() const;

    float getCateringCosts() const;

    /**
     * @brief writes the output of 'toString' to a stream
     *
     *
     * @pre meeting must be properly initialized
     *
     * @param os stream to write to
     * @param meeting to print out
     * @post ostream variable is not usable
     */
    friend std::ostream& operator<<(std::ostream& os, const Meeting& meeting);

    /**
     * @brief Destroys the Meeting class.
     *
     * ((default constructor))
     *
     * @pre User can not be null
     *
     */
    ~Meeting();

    /**
     *
     * @pre User can not be null
     *
     */
    void addParticipant(User* user);
    /**
     *
     * @pre UserId cannot be empty
     *
     *
     * @post Something went wrong, The user which was found did not have the correct id.
     */
    float getEmissions() const;
    User* getParticipant(const std::string& userId);
    size_t getParticipantCount() const;
    const Users& getParticipants() const;

private:
    /**
     *
     * @pre User can not be null
     * @pre User needs to be properly initialized.
     * @pre Something went wrong. User was not added.
     *
     */
    void _addParticipant(User* user);

    using State = enum { UNPROCESSED, PROCESSED, CANCELLED };
    std::string label;
    std::string id;
    Room* room;
    DateTime date_time;
    State state;
    int order;


    Users participants;
    bool externals_allowed;
    bool online;
    bool catering_needed;

    std::string cancellation_reason;

    void* init_check_this_ptr;
};


#endif //MEETING_PLANNER_MEETING_H
