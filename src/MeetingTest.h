//
// Created by lucas on 2026-03-12.
//
#pragma once

#include "gtest/gtest.h"

#include "Meeting.h"

class MeetingTest : public ::testing::Test {
protected:
    //Meeting meeting_;
};


TEST_F(MeetingTest, HappyDay) {
    Meeting meeting = Meeting("My Meeting", "meeting0", "some_room", Date(2025,2,2));

    EXPECT_TRUE(meeting.isProperlyInitialized());
    EXPECT_EQ("My Meeting", meeting.toString());
    EXPECT_EQ("meeting0", meeting.getId());
    EXPECT_EQ("some_room", meeting.getRoomId());

    Date d = meeting.getDate();

    EXPECT_TRUE(d.isProperlyInitialized());

    EXPECT_EQ(2025, d.getYear());
    EXPECT_EQ(2, d.getMonth());
    EXPECT_EQ(2, d.getDay());
}

TEST_F(MeetingTest, ContractViolation) {
    EXPECT_DEATH(Meeting("hello", "", "my_room", Date()), "");
    EXPECT_DEATH(Meeting("world", "some_id", "", Date()), "");
}