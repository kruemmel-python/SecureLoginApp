#include "Database.h"
#include <gtest/gtest.h>

TEST(DatabaseTest, CanRegisterAndLogin) {
    ASSERT_TRUE(Database::Register("testuser", "testpassword"));
    ASSERT_TRUE(Database::Login("testuser", "testpassword"));
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
