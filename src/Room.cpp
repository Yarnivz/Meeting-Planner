//
// Created by User on 2/26/2026.
//

#include "Room.h"

Room::Room(const std::string &name, const std::string &id, unsigned int capacity): name(name), id(id), capacity(capacity) {}

const std::string& Room::getId() { return id; }

Room::~Room() {}
