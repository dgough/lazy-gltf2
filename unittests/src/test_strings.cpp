#include <lazy_gltf2.hpp>
#include <gtest/gtest.h>
#include <array>

#include "common.hpp"

using namespace gltf2;

TEST(gltf, startsWith) {
    char blank[1] = { '\0' };
    EXPECT_TRUE(startsWith(nullptr, nullptr));
    EXPECT_TRUE(startsWith("", ""));
    EXPECT_TRUE(startsWith("", blank));
    EXPECT_TRUE(startsWith(blank, ""));
    EXPECT_TRUE(startsWith(blank, blank));

    EXPECT_TRUE(startsWith("The quick", "The"));
    EXPECT_TRUE(startsWith("abc", "a"));
    EXPECT_TRUE(startsWith("abc", "ab"));
    EXPECT_TRUE(startsWith("abc", "abc"));
    EXPECT_TRUE(startsWith("abc", ""));

    EXPECT_FALSE(startsWith("", nullptr));
    EXPECT_FALSE(startsWith(nullptr, ""));
    EXPECT_FALSE(startsWith("The", "The quick"));
}
