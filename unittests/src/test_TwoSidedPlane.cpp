#include <lazy_gltf2.hpp>
#include <gtest/gtest.h>
#include <array>

#include "common.hpp"

using namespace gltf2;

static const char* PATH = LAZY_GLTF2_BASE_SAMPLE_DIR "/2.0/TwoSidedPlane/glTF/TwoSidedPlane.gltf";

TEST(gltf, twosided) {
    Gltf gltf(PATH);
    EXPECT_TRUE(gltf);

    auto material = gltf.material(0);
    EXPECT_TRUE(material);
    EXPECT_TRUE(material.doubleSided());
}