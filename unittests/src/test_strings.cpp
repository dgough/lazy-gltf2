#include <lazy_gltf2.hpp>
#include <gtest/gtest.h>
#include <array>

#include "common.hpp"

using namespace gltf2;

TEST(strings, startsWith) {
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

TEST(strings, dirName) {
    EXPECT_EQ(dirName(nullptr), "");
    EXPECT_EQ(dirName("./asdf.txt"), "./");
    EXPECT_EQ(dirName("a/b/c/asdf.txt"), "a/b/c/");
    EXPECT_EQ(dirName("glTF-Sample-Models/2.0/Box/glTF/box.gltf"), "glTF-Sample-Models/2.0/Box/glTF/");
    EXPECT_EQ(dirName("../../../box.gltf"), "../../../");
}
