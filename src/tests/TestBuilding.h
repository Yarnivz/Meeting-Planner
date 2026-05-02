//
// Created by tishaak on 5/2/26.
//

#ifndef MEETING_PLANNER_TESTBUILDING_H
#define MEETING_PLANNER_TESTBUILDING_H
#include <gtest/gtest.h>
#include <objects/Building.h>
class TestBuilding : public ::testing::Test
{
protected:
};
TEST_F(TestBuilding, HappyDay)
{
    Campus* campus = new Campus ("Middelheim", "M");
    Building* building = new Building("Bib", "G", campus);

    EXPECT_TRUE(building->isProperlyInitialized());
    EXPECT_EQ("Bib", building->toString());
    EXPECT_EQ("G", building->getId());
    EXPECT_EQ(campus, building->getCampus());
}
TEST_F(TestBuilding, CopyConstructor)
{
    Campus* campus = new Campus ("Middelheim", "M");

    Building* building1 = new Building("Bib", "G", campus);
    Building* building2 = building1;

    EXPECT_TRUE(building1->isProperlyInitialized());
    EXPECT_TRUE(building2->isProperlyInitialized());
    EXPECT_EQ("Bib", building1->toString());
    EXPECT_EQ("Bib", building2->toString());
    EXPECT_EQ("G", building1->getId());
    EXPECT_EQ("G", building2->getId());
    EXPECT_EQ(campus, building1->getCampus());
    EXPECT_EQ(campus, building2->getCampus());
}
TEST_F(TestBuilding, ContractViolation)
{
    Campus* campus = new Campus ("Middelheim", "M");

    EXPECT_DEATH(Building(nullptr, "G", campus), "");
    EXPECT_DEATH(Building("Bib", nullptr, campus), "");
    EXPECT_DEATH(Building("Bib", "G", nullptr), "");
}
#endif //MEETING_PLANNER_TESTBUILDING_H