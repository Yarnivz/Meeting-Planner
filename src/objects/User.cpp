//
// Created by lucas on 4/5/26.
//

#include "User.h"

User::User(const std::string& name, bool external)
: name(name), external(external)
{
    init_check_this_ptr = this;
}

bool User::isProperlyInitialized() const
{
    return init_check_this_ptr == this;
}

const std::string& User::getName() const
{
    return name;
}

bool User::isExternal() const
{
    return external;
}
