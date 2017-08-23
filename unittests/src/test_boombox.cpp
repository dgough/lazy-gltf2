#include <lazy_gltf2.hpp>
#include <gtest/gtest.h>
#include <array>

#include "common.hpp"

using namespace gltf2;

static const char* BOOMBOX_PATH = LAZY_GLTF2_BASE_SAMPLE_DIR "/2.0/BoomBox/glTF/BoomBox.gltf";
static const char* BOOMBOX_BINARY_PATH = LAZY_GLTF2_BASE_SAMPLE_DIR "/2.0/BoomBox/glTF-Binary/BoomBox.glb";

TEST(gltf, emptyGltf) {
    Gltf gltf;
    EXPECT_TRUE(gltf == nullptr);
    EXPECT_TRUE(nullptr == gltf);
    EXPECT_FALSE(gltf != nullptr);
    EXPECT_FALSE(nullptr != gltf);

    Node node;
    EXPECT_TRUE(node == nullptr);
    EXPECT_TRUE(nullptr == node);
    EXPECT_FALSE(node != nullptr);
    EXPECT_FALSE(nullptr != node);

    EXPECT_TRUE(node == Node());
    EXPECT_FALSE(node != Node());
}

TEST(gltf, boombox) {
    Gltf gltf(BOOMBOX_PATH);
    EXPECT_TRUE(gltf);
    EXPECT_FALSE(gltf == nullptr);
    EXPECT_FALSE(nullptr == gltf);
    EXPECT_TRUE(gltf != nullptr);
    EXPECT_TRUE(nullptr != gltf);

    auto occ = gltf.material(0).occlusionTexture();
    EXPECT_TRUE(occ);
    EXPECT_FALSE(occ == nullptr);
    EXPECT_FALSE(nullptr == occ);
    EXPECT_TRUE(occ != nullptr);
    EXPECT_TRUE(nullptr != occ);
    EXPECT_FALSE(occ == OcclusionTextureInfo());
    EXPECT_TRUE(occ != OcclusionTextureInfo());

    EXPECT_EQ(1.0f, occ.strength());
    EXPECT_EQ(1, occ.index());
    EXPECT_TRUE(occ.texture());
    EXPECT_TRUE(occ.texture().image());
    EXPECT_EQ(occ.texture().image(), occ.texture().source());
    size_t sourceIndex;
    EXPECT_TRUE(occ.texture().source(sourceIndex));
    EXPECT_EQ(1, sourceIndex);

    // material
    auto material = gltf.findMaterial("BoomBox_Mat");
    EXPECT_TRUE(material);
    auto emissiveTexture = material.emissiveTexture();
    EXPECT_TRUE(emissiveTexture);
    EXPECT_EQ(3, emissiveTexture.index());
    EXPECT_TRUE(emissiveTexture.texture());

    {
        const std::array<float, 3> expectedEmissiveFactor{1.0f, 1.0f, 1.0f};
        std::array<float, 3> actualEmissiveFactor;
        material.emissiveFactor(actualEmissiveFactor.data());
        EXPECT_EQ(expectedEmissiveFactor, actualEmissiveFactor);
        EXPECT_EQ(expectedEmissiveFactor, material.emissiveFactor());
    }
}

TEST(gltf, boombox_multiple) {
    Gltf binGltf(BOOMBOX_BINARY_PATH);
    EXPECT_TRUE(binGltf);

    {
        // the images in this glb use BufferView instead of uri
        auto image = binGltf.image(0);
        EXPECT_TRUE(image);
        EXPECT_EQ(nullptr, image.uri());
        size_t index = 500;
        EXPECT_TRUE(image.bufferView(index));
        EXPECT_EQ(0, index);
        auto bufferView = image.bufferView();
        EXPECT_TRUE(bufferView);
    }

    Gltf gltf(BOOMBOX_PATH);
    {
        // compare buffer values
        auto b1 = gltf.buffer(0);
        auto b2 = gltf.buffer(0);
        std::vector<unsigned char> v1;
        std::vector<unsigned char> v2;
        EXPECT_TRUE(b1.load(v1));
        EXPECT_TRUE(b2.load(v2));
        EXPECT_EQ(v1, v2);
    }
}
