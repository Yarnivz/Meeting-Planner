#include <gtest/gtest.h>
#include "tests/TestDate.h"
#include "tests/TestDateTime.h"
#include "tests/TestMeeting.h"
#include "tests/TestRoom.h"
#include "tests/TestParseFile.h"
#include "tests/TestWriteToStream.h"
#include "tests/TestApp.h"
#include "tests/TestProcessMeetings.h"
#include "tests/TestCatering.h"
#include "tests/TestUser.h"
#include "tests/TestMeetingRegistry.h"

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}