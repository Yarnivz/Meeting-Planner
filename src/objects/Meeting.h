//
// Created by Yarni on 2/26/2026.
//

#ifndef MEETING_PLANNER_MEETING_H
#define MEETING_PLANNER_MEETING_H

#include <string>

#include "App.h"
#include "DateTime.h"
#include "../TypeDefs.h"

class User;
class Room;
class App;


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
     *
     *
     * 
     * @pre REQUIRE(!id.empty(), "Failed to construct meeting. 'id' can not be empty.")
     * @pre REQUIRE(room != nullptr, "Failed to construct meeting. 'room' can not be empty.")
     * @pre REQUIRE(date_time.isProperlyInitialized(), "Failed to construct meeting. 'date' has to be properly initialized with the constructor.")
     * 
     * @param app the app instance this meeting is a part of
     * @param label title of the meeting
     * @param id identifier of the meeting
     * @param room identifier of the room where the meeting takes place
     * @param date_time of when the meeting takes/took place
     * @param online status of the meeting
     * @param externals_allowed status of the meeting
     * @param catering_needed status of the meeting
     *
     * 
     * @post ENSURE(isProperlyInitialized(), "Meeting creation failed. Object was not properly initialized.")
     * @post ENSURE(getOrder() == this->order, "Meeting creation failed. Order was not correctly set.")
     * @post ENSURE(isUnProcessed() && !isProcessed() && !isCancelled(), "Meeting creation failed, state was not correctly set.")
     */
    Meeting(App* app, const std::string& label, const std::string& id, Room* room, const DateTime& date_time = DateTime(), const  bool& online = false, bool externals_allowed = false, bool catering_needed = false);

    /**
     * @brief prevents the Meeting object from being copied
     */
    Meeting(const Meeting&) = delete;
    Meeting& operator=(const Meeting&) = delete;

    /**
     * @brief Checks whether this Meeting was properly initialized by the constructor.
     * @return bool indicating result
     */
    bool isProperlyInitialized() const;

    /**
     * @brief Id getter.
     *
     *
     * @pre REQUIRE(isProperlyInitialized(), "Failed to get id. Meeting has to be properly initialized with the constructor.")
     *
     * @return the identifier of this meeting
     */
    const std::string& getId() const;

    /**
     * @brief Room id getter.
     *
     *
     * @pre REQUIRE(isProperlyInitialized(), "Failed to get room. Meeting has to be properly initialized with the constructor.")
     *
     * @return the identifier of the room where this meeting takes place
     */
    Room* getRoom() const;


    /**
     * @brief Date getter.
     *
     *
     * @pre REQUIRE(isProperlyInitialized(), "Failed to get date. Meeting has to be properly initialized with the constructor.")
     *
     * @return the date of when this meeting takes place
     */

    const DateTime& getDateTime() const;

    /**
     * @brief Converts the meeting class to a readable string format.
     *
     *
     * @pre REQUIRE(isProperlyInitialized(), "Failed to convert to string. Meeting has to be properly initialized with the constructor.")
     *
     *
     * @post ENSURE(!label.empty(), "label cannot be empty")
     * @return the meeting in string format
     */
    const std::string& toString() const;

    /**
     * @brief The orders getter
     * @post ENSURE(order >= 0, "Order can not be negative")
     */
    int getOrder() const;

    /**
     * @brief The orders setter
     *
     * Sets the order of the Meeting.
     * When processing multiple meetings, the meeting with the *lowest* order is prioritized.
     * An order of zero means you allow the App class to decide the order of this meeting when registering it.
     *
     *
     * @pre REQUIRE(orderAdded >= 0, "cannot set order to a negative value %d", orderAdded)
     *
     * @param orderAdded of the meeting
     *
     * @post ENSURE(orderAdded == getOrder(), "OrderAdded does not match GetOrder return value")
     */
    void setOrder(const int orderAdded);

    /**
     * @brief processes the meeting
     *
     * @pre REQUIRE(state == UNPROCESSED, "Meeting was already processed or canceled.")
     *
     * @param catering_planning_output
     *
     * @post ENSURE(isProcessed(), "Meeting must be processed.")
     */
    void process(std::ostream* catering_planning_output = nullptr);

    /**
     * @brief cancels the meeting and updates the reason for it
     *
     * @pre REQUIRE(state == UNPROCESSED, "Meeting was already processed or canceled")
     *
     * @param the reason for the meetings cancellation
     *
     * @post ENSURE(isCancelled(), "Meeting must be cancelled.")
     */
    void cancel(const std::string& reason);

    /**
     * @brief Checks wether the meeting is unprocessed.
     * @return bool indicating result
     */
    bool isUnProcessed() const;

    /**
     * @brief Checks wether the meeting is processed.
     * @return bool indicating result
     */
    bool isProcessed() const;

    /**
     * @brief Checks wether the meeting is cancelled.
     * @return bool indicating result
     */
    bool isCancelled() const;

    /**
     * @brief Checks wether the meeting allows external users.
     * @return bool indicating result
     */
    bool externalsAllowed() const;

    /**
     * @brief Checks wether the meeting needs catering.
     * @return bool indicating result
     */
    bool cateringNeeded() const;

    /**
     * @brief online bool getter.
     *
     *
     * @pre REQUIRE(isProperlyInitialized(), "Failed to get online status. Meeting has to be properly initialized with the constructor.")
     *
     * @return online status of meeting
     */
    bool isOnline() const;

    /**
     * @brief gets the reason on why on the meeting is cancelled
     *
     * @post ENSURE(isCancelled(), "Meeting was not cancelled.")
     *
     * @return the meetings cancellation reason as a string
     */
    const std::string& getCancellationReason() const;

    float getCateringCosts() const;

    /**
     * @brief writes the output of 'toString' to a stream
     *
     *
     * @pre REQUIRE(meeting.isProperlyInitialized(), "meeting must be properly initialized")
     *
     * @param os stream to write to
     * @param meeting to print out
     *
     * @post ENSURE(os, "ostream variable is not usable")
     */
    friend std::ostream& operator<<(std::ostream& os, const Meeting& meeting);

    /**
     * @brief Destroys the Meeting class.
     *
     * ((default constructor))
     *
     * @pre REQUIRE(user != nullptr, "User can not be null")
     *
     *
     * @post ENSURE(participants.contains(user->getId()), "User must be added to meeting participants")
     * @post ENSURE(user->meetings.getById(id) == this, "MeetingRegistery must contain current meeting to wich the user was added")
     */
    ~Meeting();
    /**
    * @brief adds the users to the meetings Meetingregistery
    *
     * @pre REQUIRE(user != nullptr, "User can not be null")
    *
    * @param user the user to add
     *
     * @post ENSURE(participants.contains(user->getId()), "User must be added to meeting participants")
     * @post ENSURE(user->meetings.getById(id) == this, "MeetingRegistery must contain current meeting to wich the user was added")
    */
    void addParticipant(User* user);

    void getEmissionDetails(
        unsigned& num_externals, float& externals_emissions,
        unsigned& num_internals, float& internals_emissions,
        unsigned& num_online, float& online_emissions,
        unsigned& num_catering_participants, float& catering_emissions
        ) const;

    /**
     * @brief calculates the amount of co2 the meeting uses
     *
     * 
     * @pre REQUIRE(room != nullptr, "Room cannot be null")
     * @pre REQUIRE(room->getCampus() != nullptr, "Campus cannot be null")
     * @pre REQUIRE(!(catering_needed && online), "Catering and online cannot be true at the same time.")
     * 
     * 
     * @post ENSURE(addedEmissions >= 0, "Emissions additive cannot be negative.")
     * @return the total co2 emission amount
     */
    float getEmissions() const;

    /**
     * @brief gets the user in the meeting that corresponds to the id
     *
     * @pre REQUIRE(!userId.empty(), "UserId cannot be empty")
     *
     * @param userId the Id of the user
     *
     * @post ENSURE(it->second->getId() == userId, "User must have a correct id.")
     * @return the user itself
     */
    User* getParticipant(const std::string& userId) const;

    /**
     * @brief checks whether the user is in this meeting
     * @param user the user itself
     * @return bool indicating result
     */
    bool hasParticipant(const User* user) const;

    /**
     * @brief gets the amount of participants in this meeting.
     * @return the amount of participants
     */
    size_t getParticipantCount() const;

    /**
     * @brief Gets all participants in the meeting.
     * @return a map containing the participants
     */
    const Users& getParticipants() const;

private:

    /**
     *
     * @pre REQUIRE(user != nullptr, "User can not be null")
     * @pre REQUIRE(user->isProperlyInitialized(), "User needs to be properly initialized.")
     * @pre REQUIRE(!hasParticipant(user), "User '%s' can't already participate in meeting", user->getId().c_str())
     * @pre REQUIRE(getParticipant(user->getId()) == nullptr, "User id '%s' must be unique", user->getId().c_str())
     * @pre REQUIRE(!user->isExternal() || this->externalsAllowed(), "Can't add external user %s to meeting %s which doesn't allow external users.", user->getId().c_str(), this->getId().c_str())
     * @pre REQUIRE(getParticipant(user->getId()) == user, "User must be added.")
     *
     */
    void _addParticipant(User* user);

    App* app;

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
