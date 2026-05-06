//
// Created by lucas on 5/1/26.
//

#ifndef MEETING_PLANNER_TESTMEETINGREGISTRY_H
#define MEETING_PLANNER_TESTMEETINGREGISTRY_H
#include "gtest/gtest.h"
#include "objects/MeetingRegistry.h"
#include "objects/Room.h"

class TestMeetingRegistry : public ::testing::Test
{
};

TEST_F(TestMeetingRegistry, HappyDay)
{
    Campus c = Campus("c", "c");
    Building b = Building("b", "b", &c);
    Room r = Room("r", "r", 99, &b);
    ASSERT_TRUE(c.isProperlyInitialized());
    ASSERT_TRUE(b.isProperlyInitialized());
    ASSERT_TRUE(r.isProperlyInitialized());

    Meeting m1 = Meeting("meeting1", "m1", &r, DateTime(2026, 1, 1, 1), false, false, false);
    Meeting m2 = Meeting("meeting2", "m2", &r, DateTime(2026, 1, 1, 2), false, false, false);
    Meeting m3 = Meeting("meeting3", "m3", &r, DateTime(2026, 1, 1, 3), false, false, false);
    ASSERT_TRUE(m1.isProperlyInitialized());
    ASSERT_TRUE(m2.isProperlyInitialized());
    ASSERT_TRUE(m3.isProperlyInitialized());

    MeetingRegistry m = MeetingRegistry();
    m.add(&m1);
    m.add(&m2);
    m.add(&m3);

    EXPECT_EQ(&m1, m.getById("m1"));
    EXPECT_EQ(&m2, m.getById("m2"));
    EXPECT_EQ(&m3, m.getById("m3"));

    const std::list<Meeting*>& dt1 = m.getByDateTime(DateTime(2026, 1, 1, 1));
    const std::list<Meeting*>& dt2 = m.getByDateTime(DateTime(2026, 1, 1, 2));
    const std::list<Meeting*>& dt3 = m.getByDateTime(DateTime(2026, 1, 1, 3));
    EXPECT_EQ(size_t(1), dt1.size());
    EXPECT_EQ(size_t(1), dt2.size());
    EXPECT_EQ(size_t(1), dt3.size());
    EXPECT_EQ(&m1, dt1.front());
    EXPECT_EQ(&m2, dt2.front());
    EXPECT_EQ(&m3, dt3.front());

    m.removeById("m1");
    EXPECT_EQ(nullptr, m.getById("m1"));
    EXPECT_EQ(&m2, m.getById("m2"));
    EXPECT_EQ(&m3, m.getById("m3"));

    m.removeById("m2");
    EXPECT_EQ(nullptr, m.getById("m1"));
    EXPECT_EQ(nullptr, m.getById("m2"));
    EXPECT_EQ(&m3, m.getById("m3"));

    m.removeById("m3");
    EXPECT_EQ(nullptr, m.getById("m1"));
    EXPECT_EQ(nullptr, m.getById("m2"));
    EXPECT_EQ(nullptr, m.getById("m3"));
}

TEST_F(TestMeetingRegistry, Invalid)
{
    Campus c = Campus("c", "c");
    Building b = Building("b", "b", &c);
    Room r = Room("r", "r", 99, &b);
    ASSERT_TRUE(c.isProperlyInitialized());
    ASSERT_TRUE(b.isProperlyInitialized());
    ASSERT_TRUE(r.isProperlyInitialized());

    Meeting m1 = Meeting("meeting1", "m1", &r, DateTime(2026, 1, 1, 1), false, false, false);
    Meeting m2 = Meeting("meeting2", "m2", &r, DateTime(2026, 1, 1, 2), false, false, false);
    ASSERT_TRUE(m1.isProperlyInitialized());
    ASSERT_TRUE(m2.isProperlyInitialized());

    Meeting m1_dup = Meeting("another meeting1", "m1", &r, DateTime(2026, 1, 1, 1), false, false, false);
    ASSERT_TRUE(m1_dup.isProperlyInitialized());

    MeetingRegistry m = MeetingRegistry();
    m.add(&m1);
    m.add(&m2);
    EXPECT_EQ(&m1, m.getById("m1"));
    EXPECT_EQ(&m2, m.getById("m2"));

    EXPECT_DEATH(m.add(&m1_dup), "");
    EXPECT_DEATH(m.add(nullptr), "");
}

#endif //MEETING_PLANNER_TESTMEETINGREGISTRY_H
