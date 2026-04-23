//
// Created by Yarni on 2/26/2026.
//

#include "Meeting.h"

#include "helper/DesignByContract.h"
#include "objects/User.h"


Meeting::Meeting(const std::string& label, const std::string& id, Room* room,const bool& online, const DateTime& date_time)
    : label(label), id(id), room(room), date_time(date_time)
{
    REQUIRE(!id.empty(), "Failed to construct meeting. 'id' can not be empty.");
    REQUIRE(room != nullptr, "Failed to construct meeting. 'room' can not be empty.");
    REQUIRE(date_time.isProperlyInitialized(), "Failed to construct meeting. 'date' has to be properly initialized with the constructor.");

    this->state = UNPROCESSED;
    this->order = 0;
    init_check_this_ptr = this;
}

bool Meeting::isProperlyInitialized() const
{
    return init_check_this_ptr == this;
}


const std::string& Meeting::getId() const
{
    REQUIRE(isProperlyInitialized(), "Failed to get id. Meeting has to be properly initialized with the constructor.");
    return id;
}

Room* Meeting::getRoom() const
{
    REQUIRE(isProperlyInitialized(), "Failed to get room. Meeting has to be properly initialized with the constructor.");
    return room;
}

const bool& Meeting::getOnline() const
{
    REQUIRE(isProperlyInitialized(), "Failed to get online status. Meeting has to be properly initialized with the constructor.");
    return online;
}

const DateTime& Meeting::getDateTime() const
{
    REQUIRE(isProperlyInitialized(), "Failed to get date. Meeting has to be properly initialized with the constructor.");
    return date_time;
}


const std::string& Meeting::toString() const
{
    REQUIRE(isProperlyInitialized(), "Failed to convert to string. Meeting has to be properly initialized with the constructor.");
    return label;
}

//TODO: add contracts
int Meeting::getOrder() const
{
    ENSURE(order >= 0, "Order can not be negative");
    return order;
}

void Meeting::setOrder(const int orderAdded)
{
    /*REQUIRE(orderAdded >= 0, "cannot set order to as negative value %d", orderAdded);
    order = orderAdded;
    ENSURE(orderAdded == getOrder(), "OrderAdded does not match GetOrder return value"); */
}

void Meeting::process()
{
    ENSURE(state == UNPROCESSED, "Meeting was already processed or canceled.");
    state = PROCESSED;
    REQUIRE(isProcessed(), "Something went wrong. Meeting was not processed.");
}

void Meeting::cancel(const std::string& reason)
{
    ENSURE(state == UNPROCESSED, "Meeting was already processed or canceled");
    state = CANCELLED;
    cancellation_reason = reason;
    REQUIRE(isCancelled(), "Something went wrong. Meeting was not cancelled.");
}

bool Meeting::isUnProcessed() const { return state == UNPROCESSED; }

bool Meeting::isProcessed() const { return state == PROCESSED; }

bool Meeting::isCancelled() const { return state == CANCELLED; }

const std::string& Meeting::getCancellationReason() const
{
    ENSURE(isCancelled(), "Meeting was not cancelled.");
    return cancellation_reason;
}


std::ostream& operator<<(std::ostream& os, const Meeting& meeting)
{
    REQUIRE(meeting.isProperlyInitialized(), "meeting must be properly initialized");
    os << meeting.toString();
    ENSURE(os, "ostream variable is not usable");
    return os;
}

Meeting::~Meeting() = default;


void Meeting::addParticipant(User* user)
{
    REQUIRE(user != nullptr, "User can not be null");
    this->_addParticipant(user);
    user->_addMeeting(this);
}

User* Meeting::getParticipant(const std::string& userId)
{
    REQUIRE(!userId.empty(), "UserId cannot be empty");
    const Users::iterator it = participants.find(userId);

    if (it == participants.end()) return nullptr;

    ENSURE(it->second->getId() == userId, "Something went wrong, The user which was found did not have the correct id.");
    return it->second;
}

size_t Meeting::getParticipantCount() const
{
    return participants.size();
}

const Users& Meeting::getParticipants() const
{
    return participants;
}

void Meeting::_addParticipant(User* user)
{
    REQUIRE(user != nullptr, "User can not be null");
    REQUIRE(user->isProperlyInitialized(), "User needs to be properly initialized.");

    participants.insert({user->getId(), user});

    REQUIRE(getParticipant(user->getId()) == user, "Something went wrong. User was not added.");
}
