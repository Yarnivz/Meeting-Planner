//
// Created by lucas on 2026-03-05.
//

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