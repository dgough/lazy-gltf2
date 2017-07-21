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
    EXPECT_EQ("", dirName(nullptr));
    EXPECT_EQ("./", dirName("./asdf.txt"));
    EXPECT_EQ("a/b/c/", dirName("a/b/c/asdf.txt"));
    EXPECT_EQ("glTF-Sample-Models/2.0/Box/glTF/", dirName("glTF-Sample-Models/2.0/Box/glTF/box.gltf"));
    EXPECT_EQ("../../../", dirName("../../../box.gltf"));
}

TEST(base64, readBsae64) {
    std::vector<unsigned char> data;
    std::vector<unsigned char> expected{'1','2','3','4','5','6'};
    EXPECT_TRUE(readBase64("MTIzNDU2", 6, data));
    EXPECT_EQ(6, data.size());
    EXPECT_EQ(expected, data);
}