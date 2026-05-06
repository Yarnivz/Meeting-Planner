//
// Created by lucas on 5/1/26.
//

#ifndef MEETING_PLANNER_TESTCATERING_H
#define MEETING_PLANNER_TESTCATERING_H
#include "gtest/gtest.h"
#include "objects/Campus.h"
#include "objects/Catering.h"

class TestCatering : public ::testing::Test
{
};

TEST_F(TestCatering, HappyDay)
{
    Campus c = Campus("Campus","campus");
    Catering cat = Catering(&c, 10.0);
    ASSERT_TRUE(cat.isProperlyInitialized());
    EXPECT_EQ(&c, cat.getCampus());
    EXPECT_EQ(10.0, cat.getEmissions());

    const std::list<Catering*>& caterings = c.getCaterings();
    EXPECT_EQ(size_t(1), caterings.size());
    EXPECT_EQ(&cat, caterings.front());
}

TEST_F(TestCatering, Invalid)
{
    Campus c = Campus("c", "c");
    EXPECT_DEATH(Catering(nullptr, 10.0), "");
    EXPECT_DEATH(Catering(&c, -10.0), "");
}

#endif //MEETING_PLANNER_TESTCATERING_H
