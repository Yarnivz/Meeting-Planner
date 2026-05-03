//
// Created by lucas on 4/5/26.
//

#include "objects/MeetingRegistry.h"

#include "helper/DesignByContract.h"


void MeetingRegistry::add(Meeting* meeting)
{
    REQUIRE(meeting, "Meeting can not be null.");
    REQUIRE(meeting->isProperlyInitialized(), "Meeting needs to be properly initialized.");
    REQUIRE(getById(meeting->getId()) == nullptr, "Meeting Id needs to be unique!");

    by_id.insert({meeting->getId(), meeting});

    if (!by_date.contains(meeting->getDateTime())) by_date.insert({meeting->getDateTime(), {}});
    by_date[meeting->getDateTime()].push_back(meeting);

    ENSURE(getById(meeting->getId()) == meeting, "Something went wrong. The meeting was not added to the Registry");
    //ENSURE(getByDate(meeting->getDate()) == meeting, "Something went wrong. The meeting was not added to the Registry");
}

std::list<Meeting*>& MeetingRegistry::getByDateTime(const DateTime& date)
{
    if (!by_date.contains(date)) by_date.insert({date, {}});

    //ENSURE(it->second->getDate() == date, "Something went wrong, The meeting which was found did not have the correct date.");
    return by_date.at(date);
}


Meeting* MeetingRegistry::getById(const std::string& id) const
{
    REQUIRE(!id.empty(), "Meeting id cannot be empty.");
    const std::unordered_map<std::string, Meeting*>::const_iterator it = by_id.find(id);

    if (it == by_id.end()) return nullptr;

    ENSURE(it->second->getId() == id, "Something went wrong. The meeting which was found did not have the correct id");
    return it->second;
}

// void MeetingRegistry::removeByDate(const Date& date)
// {
//     const std::unordered_map<Date, Meeting*, DateHash>::iterator it = by_date.find(date);
//
//     REQUIRE(it != by_date.end(), "A meeting with that date does not exist in the registry.");
//     ENSURE(it->second->getDate() == date, "Something went wrong. The meeting which was found did not have the correct date.");
//
//
//     by_date.erase(it);
//     by_id.erase(it->second->getId());
//     ENSURE(getByDate(date) == nullptr, "Something went wrong. The meeting was not removed from the registry.");
// }

void MeetingRegistry::removeById(const std::string& id)
{
    const std::unordered_map<std::string, Meeting*>::iterator it = by_id.find(id);

    REQUIRE(it != by_id.end(), "A meeting with that id does not exist in the registry.");
    ENSURE(it->second->getId() == id, "Something went wrong. The meeting which was found did not have the correct id.");

    by_id.erase(it);
    by_date.erase(it->second->getDateTime());
    ENSURE(getById(id) == nullptr, "Something went wrong. The meeting was not removed from the registry.");
}


const std::unordered_map<std::string, Meeting*>& MeetingRegistry::getRawIdMap() const
{
    return by_id;
}

const std::unordered_map<DateTime, std::list<Meeting*>, DateTimeHash>& MeetingRegistry::getRawDateMap() const
{
    return by_date;
}


inline void hash_combine(std::size_t& acc, std::size_t value)
{
    acc ^= value + 0x9e3779b9 + (acc << 6) + (acc >> 2);
}

std::size_t DateTimeHash::operator()(const DateTime& d) const
{
    size_t acc = 0;
    hash_combine(acc, d.getDay());
    hash_combine(acc, d.getMonth());
    hash_combine(acc, d.getYear());
    hash_combine(acc, d.getHour());
    return acc;
}