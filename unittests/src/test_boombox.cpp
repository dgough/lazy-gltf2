#include <lazy_gltf2.hpp>
#include <gtest/gtest.h>
#include <array>

#include "common.hpp"

using namespace gltf2;

static const char* BOOMBOX_PATH = LAZY_GLTF2_BASE_SAMPLE_DIR "/2.0/BoomBox/glTF/BoomBox.gltf";

TEST(gltf, boombox) {
    Gltf gltf(BOOMBOX_PATH);
    EXPECT_TRUE(gltf);

    auto occ = gltf.material(0).occlusionTexture();
    EXPECT_TRUE(occ);
    EXPECT_EQ(occ.strength(), 1.0f);
    EXPECT_EQ(occ.index(), 1);
    EXPECT_TRUE(occ.texture());
    EXPECT_TRUE(occ.texture().image());
    EXPECT_EQ(occ.texture().image(), occ.texture().source());
    size_t sourceIndex;
    EXPECT_TRUE(occ.texture().source(sourceIndex));
    EXPECT_EQ(sourceIndex, 1);

    auto material = gltf.material(0);
    EXPECT_TRUE(material);
    auto emissiveTexture = material.emissiveTexture();
    EXPECT_TRUE(emissiveTexture);
    EXPECT_EQ(emissiveTexture.index(), 3);
    EXPECT_TRUE(emissiveTexture.texture());
}