//
// Created by lucas on 5/1/26.
//

#ifndef MEETING_PLANNER_TESTUSER_H
#define MEETING_PLANNER_TESTUSER_H

#include "objects/User.h"
#include "gtest/gtest.h"

class TestUser : public ::testing::Test
{
};

TEST_F(TestUser, HappyDay)
{
    // Regular constructor
    User u = User("User100", false);
    ASSERT_TRUE(u.isProperlyInitialized());
    EXPECT_EQ("User100", u.getId());
    EXPECT_FALSE(u.isExternal());

    // Copy constructor
    User u_copy = u;
    EXPECT_TRUE(u_copy.isProperlyInitialized());
    EXPECT_EQ("User100", u_copy.getId());
    EXPECT_FALSE(u_copy.isExternal());

    // External
    User external = User("External User", true);
    ASSERT_TRUE(external.isProperlyInitialized());
    EXPECT_EQ("External User", external.getId());
    EXPECT_TRUE(external.isExternal());
}

#endif //MEETING_PLANNER_TESTUSER_H
