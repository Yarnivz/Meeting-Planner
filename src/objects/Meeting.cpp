//
// Created by Yarni on 2/26/2026.
//

#include "Meeting.h"

#include "Catering.h"
#include "Room.h"
#include "helper/DesignByContract.h"
#include "objects/User.h"


Meeting::Meeting(const std::string& label, const std::string& id, Room* room, const DateTime& date_time, const bool& online, bool externals_allowed, bool catering_needed)
    : label(label), id(id), room(room), date_time(date_time), externals_allowed(externals_allowed), online(online), catering_needed(catering_needed)
{
    REQUIRE(!id.empty(), "Failed to construct meeting. 'id' can not be empty.");
    REQUIRE(room != nullptr, "Failed to construct meeting. 'room' can not be empty.");
    REQUIRE(date_time.isProperlyInitialized(), "Failed to construct meeting. 'date' has to be properly initialized with the constructor.");

    this->state = UNPROCESSED;
    this->order = 0;
    init_check_this_ptr = this;

    ENSURE(isProperlyInitialized(), "Meeting creation failed. Object was not properly initialized.");
    ENSURE(getOrder() == this->order, "Meeting creation failed. Order was not correctly set.");
    ENSURE(isUnProcessed() && !isProcessed() && !isCancelled(), "Meeting creation failed, state was not correctly set.");
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

bool Meeting::isOnline() const
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
    REQUIRE(orderAdded >= 0, "cannot set order to a negative value %d", orderAdded);
    order = orderAdded;
    ENSURE(orderAdded == getOrder(), "OrderAdded does not match GetOrder return value");
}

void Meeting::process()
{
    ENSURE(state == UNPROCESSED, "Meeting was already processed or canceled.");
    state = PROCESSED;
    REQUIRE(isProcessed(), "Meeting must be processed.");
}

void Meeting::cancel(const std::string& reason)
{
    ENSURE(state == UNPROCESSED, "Meeting was already processed or canceled");
    state = CANCELLED;
    cancellation_reason = reason;
    REQUIRE(isCancelled(), "Meeting must be cancelled.");
}

bool Meeting::isUnProcessed() const { return state == UNPROCESSED; }

bool Meeting::isProcessed() const { return state == PROCESSED; }

bool Meeting::isCancelled() const { return state == CANCELLED; }

bool Meeting::externalsAllowed() const { return externals_allowed; }

bool Meeting::cateringNeeded() const { return catering_needed; }

const std::string& Meeting::getCancellationReason() const
{
    ENSURE(isCancelled(), "Meeting was not cancelled.");
    return cancellation_reason;
}

float Meeting::getCateringCosts() const
{
    float cost = 0.0f;
    if (!cateringNeeded()) return cost;

    for (Users::const_iterator it = getParticipants().begin(); it != getParticipants().end(); ++it)
    {
        if (it->second->isExternal()) cost += 20.79;
        else cost += 10.59;
    }

    return cost;
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

    ENSURE(it->second->getId() == userId, "User must have a correct id.");
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
    REQUIRE(!user->isExternal() || this->externalsAllowed(), "Can't add external user %s to meeting %s which doesn't allow external users.", user->getId().c_str(), this->getId().c_str());

    participants.insert({user->getId(), user});

    REQUIRE(getParticipant(user->getId()) == user, "User must be added.");
}

float Meeting::getEmissions() const
{
    REQUIRE(room != nullptr, "Room cannot be null");
    REQUIRE(room->getCampus() != nullptr, "Campus cannot be null");
    REQUIRE(catering_needed && online != true, "Catering and online cannot be true at the same time.");
    float addedEmissions = 0;

    for (const std::pair<std::string, User*> u : participants)
    {
        if (online)
        {
            addedEmissions += 30;
        }
        else
        {
            if (u.second->isExternal())
            {
                addedEmissions += 1200;
            }
            else
            {
                addedEmissions += 120;
            }

            if (!room->getCampus()->getCaterings().empty() && catering_needed)
            {
                addedEmissions = static_cast<float>(getParticipantCount()) * room->getCampus()->getCaterings().front()->getEmissions();
            }
        }
    }
    return addedEmissions;
}


