//
// Created by Lucas on 2026-03-12.
//
#pragma once

#include "gtest/gtest.h"

#include "objects/Meeting.h"
#include "objects/Room.h"

class TestMeeting : public ::testing::Test
{
protected:
};


TEST_F(TestMeeting, HappyDay)
{
    Room r = Room("label", "some_room", 123);
    Meeting meeting = Meeting("My Meeting", "meeting0", &r, Date(2025, 2, 2));

    EXPECT_TRUE(meeting.isProperlyInitialized());
    EXPECT_EQ("My Meeting", meeting.toString());
    EXPECT_EQ("meeting0", meeting.getId());
    EXPECT_EQ(&r, meeting.getRoom());

    Date d = meeting.getDate();

    EXPECT_TRUE(d.isProperlyInitialized());

    EXPECT_EQ(2025, d.getYear());
    EXPECT_EQ(2u, d.getMonth());
    EXPECT_EQ(2u, d.getDay());
}

TEST_F(TestMeeting, ContractViolation)
{
    EXPECT_DEATH(Meeting("hello", "", nullptr, Date()), "");
    EXPECT_DEATH(Meeting("world", "some_id", nullptr, Date()), "");
}