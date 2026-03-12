//
// Created by lucas on 2026-03-12.
//

#ifndef MEETING_PLANNER_PARTICIPATIONTEST_H
#define MEETING_PLANNER_PARTICIPATIONTEST_H

#include "gtest/gtest.h"
#include "Participation.h"

class ParticipationTest : public ::testing::Test {
protected:
};

TEST_F(ParticipationTest, HappyDay) {
    Participation p("John Doe", "my_meeting1");
    Participation articipation("Peter Selie", "my_meeting_9000_pro_max++");

    EXPECT_TRUE(p.isProperlyInitialized());
    EXPECT_EQ("John Doe", p.getUser());
    EXPECT_EQ("my_meeting1", p.getMeetingId());

    EXPECT_TRUE(articipation.isProperlyInitialized());
    EXPECT_EQ("Peter Selie", articipation.getUser());
    EXPECT_EQ("my_meeting_9000_pro_max++", articipation.getMeetingId());
}

TEST_F(ParticipationTest, ContractViolation) {
    EXPECT_DEATH(Participation p("Evil Meeting User With No Meeting Id", ""), "");
    EXPECT_DEATH(Participation p("", "no user :("), "");
}

TEST_F(ParticipationTest, CopyConstructor) {
    Participation p1("me", "some_meeting");
    Participation p2 = p1;

    EXPECT_TRUE(p1.isProperlyInitialized());
    EXPECT_TRUE(p2.isProperlyInitialized());
    EXPECT_EQ("me", p1.getUser());
    EXPECT_EQ("me", p2.getUser());
    EXPECT_EQ("some_meeting", p1.getMeetingId());
    EXPECT_EQ("some_meeting", p2.getMeetingId());
}

TEST_F(ParticipationTest, ProperlyInitialized) {
    Participation p1("user", "meeting");
    Participation* p2 = (Participation*) malloc(sizeof(Participation)); //Unsafe malloc

    memcpy((void*)p2, &p1, sizeof(Participation)); //Unsafe memcpy

    EXPECT_TRUE(p1.isProperlyInitialized());
    EXPECT_FALSE(p2->isProperlyInitialized());
}

#endif //MEETING_PLANNER_PARTICIPATIONTEST_H