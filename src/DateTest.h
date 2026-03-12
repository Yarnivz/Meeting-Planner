//
// Created by lucas on 2026-03-05.
//
#pragma once

#include <gtest/gtest.h>

#include "Date.h"

class DateTest : public ::testing::Test {
protected:
    Date date_;
};

TEST_F(DateTest, HappyDay) {

    int data[][3] = {
        {2025, 10, 2}, {2027, 1, 12},
        {2023, 12, 30}, {2031, 7, 23}
    };

    for (const int* t : data)
    {
        Date d = Date(t[0], t[1], t[2]);
        EXPECT_TRUE(d.isProperlyInitialized());
        EXPECT_EQ(d.getYear(), t[0]);
        EXPECT_EQ(d.getMonth(), t[1]);
        EXPECT_EQ(d.getDay(), t[2]);
    }

}

TEST_F(DateTest, CopyConstructor) {
    Date d1 = Date();
    Date d2 = d1;

    EXPECT_TRUE(d2.isProperlyInitialized());
    EXPECT_EQ(d1.getDay(), d2.getDay());
    EXPECT_EQ(d1.getYear(), d2.getYear());
    EXPECT_EQ(d1.getMonth(), d2.getMonth());

}

TEST_F(DateTest, FailConstructor) {
    EXPECT_DEATH(Date(1,-1,-23), "");
    EXPECT_DEATH(Date(0,0,0), "");
    EXPECT_DEATH(Date(2025, 12, 0), "");
    EXPECT_DEATH(Date(2025, 12, 0), "");
    EXPECT_DEATH(Date(2025, 11, 31), "");
    EXPECT_DEATH(Date(2025, 12, 32), "");
    EXPECT_DEATH(Date(2025, 0, 31), "");
    EXPECT_DEATH(Date(2025, 2, 29), "");
    EXPECT_DEATH(Date(2024, 2, 30), "");
}

TEST_F(DateTest, EdgeCases) {
    // Schrikkeljaren
    Date correct = Date(2024, 2, 29);
    EXPECT_TRUE(correct.isProperlyInitialized());
    EXPECT_EQ(correct.getYear(), 2024);
    EXPECT_EQ(correct.getMonth(), 2);
    EXPECT_EQ(correct.getDay(), 29);

    EXPECT_DEATH(Date(2025, 2, 29), "");

    //Verschillende maanden
    Date correct2 = Date(2024, 3, 31);
    EXPECT_TRUE(correct2.isProperlyInitialized());
    EXPECT_EQ(correct2.getYear(), 2024);
    EXPECT_EQ(correct2.getMonth(), 3);
    EXPECT_EQ(correct2.getDay(), 31);

    Date correct3 = Date(2024, 4, 30);
    EXPECT_TRUE(correct3.isProperlyInitialized());
    EXPECT_EQ(correct3.getYear(), 2024);
    EXPECT_EQ(correct3.getMonth(), 4);
    EXPECT_EQ(correct3.getDay(), 30);

    EXPECT_DEATH(Date(2025, 4, 31), "");

    //Juli en Augustus beide 31 dagen lang
    Date juli = Date(2024, 7, 31);
    EXPECT_TRUE(juli.isProperlyInitialized());
    EXPECT_EQ(juli.getYear(), 2024);
    EXPECT_EQ(juli.getMonth(), 7);
    EXPECT_EQ(juli.getDay(), 31);

    Date aug = Date(2024, 8, 31);
    EXPECT_TRUE(aug.isProperlyInitialized());
    EXPECT_EQ(aug.getYear(), 2024);
    EXPECT_EQ(aug.getMonth(), 8);
    EXPECT_EQ(aug.getDay(), 31);
}