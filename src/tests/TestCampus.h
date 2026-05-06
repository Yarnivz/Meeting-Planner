//
// Created by tishaak on 5/2/26.
//

#ifndef MEETING_PLANNER_TESTCAMPUS_H
#define MEETING_PLANNER_TESTCAMPUS_H
#include <gtest/gtest.h>
#include "objects/Campus.h"
class TestCampus : public ::testing::Test
{
protected:
};
TEST_F(TestCampus, HappyDay)
{
    Campus* campus = new Campus ("Middelheim", "M");

    EXPECT_TRUE(campus->isProperlyInitialized());
    EXPECT_EQ("Middelheim", campus->toString());
    EXPECT_EQ("M", campus->getId());

    delete campus;
}
TEST_F(TestCampus, CopyConstructor)
{
    Campus* campus1 = new Campus ("Middelheim", "M");
    Campus* campus2 = campus1;

    EXPECT_TRUE(campus1->isProperlyInitialized());
    EXPECT_TRUE(campus2->isProperlyInitialized());
    EXPECT_EQ("Middelheim", campus1->toString());
    EXPECT_EQ("Middelheim", campus2->toString());
    EXPECT_EQ("M", campus1->getId());
    EXPECT_EQ("M", campus2->getId());

    delete campus1;
}
TEST_F(TestCampus, ContractViolation)
{
    EXPECT_DEATH(Campus("", "M"), "");
    EXPECT_DEATH(Campus("Middelheim", ""), "");
}
#endif //MEETING_PLANNER_TESTCAMPUS_H