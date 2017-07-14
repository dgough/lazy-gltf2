#include <lazy_gltf2.hpp>
#include <gtest/gtest.h>
#include <array>

#include "common.hpp"

using namespace gltf2;

static const char* MONSTER_PATH = LAZY_GLTF2_BASE_SAMPLE_DIR "/2.0/Monster/glTF/Monster.gltf";

TEST(gltf, monster) {
    Gltf gltf;
    ASSERT_TRUE(gltf.load(MONSTER_PATH));

    auto scene = gltf.defaultScene();
    EXPECT_TRUE(scene);
    auto root = scene[0];
    EXPECT_TRUE(root);

    auto node = gltf.node(2);
    EXPECT_TRUE(node);

    std::array<float, 3> transExpected{ -1.1628570556640626f, 99.4341278076172f,-44.13312911987305f };
    std::array<float, 3> transActual;
    node.translation(transActual.data());
    for (size_t i = 0; i < transActual.size(); ++i) {
        EXPECT_FLOAT_EQ(transActual[i], transExpected[i]);
    }

    std::array<float, 4> rotExpected{ 0.6116809844970703f, 0.354727566242218f, 0.6117032766342163f, 0.3547307252883911f };
    std::array<float, 4> rotActual;
    node.rotation(rotActual.data());
    for (size_t i = 0; i < rotActual.size(); ++i) {
        EXPECT_FLOAT_EQ(rotActual[i], rotExpected[i]);
    }

    std::array<float, 3> scaleExpected{ 0.9999999403953552f, 1.0000001192092896f, 1.0f };
    std::array<float, 3> scaleActual;
    node.scale(scaleActual.data());
    for (size_t i = 0; i < scaleActual.size(); ++i) {
        EXPECT_FLOAT_EQ(scaleActual[i], scaleExpected[i]);
    }

    auto node2 = scene[0][1];
    EXPECT_TRUE(node2);
    auto mesh = node2.mesh();
    EXPECT_TRUE(mesh);
    EXPECT_STREQ(mesh.name(), "monster");
    EXPECT_EQ(mesh.primitiveCount(), 1);
    auto prim = mesh.primitive(0);
    EXPECT_TRUE(prim);
    EXPECT_EQ(prim.mode(), Primitive::Mode::TRIANGLES);

    EXPECT_TRUE(prim.normal());
    EXPECT_TRUE(prim.position());
    EXPECT_TRUE(prim.joints(0));
    EXPECT_TRUE(prim.texcoord(0));
    EXPECT_TRUE(prim.weights(0));

    EXPECT_EQ(prim.normal(), prim.attribute("NORMAL"));
    EXPECT_EQ(prim.position(), prim.attribute("POSITION"));
    EXPECT_EQ(prim.joints(0), prim.attribute("JOINTS_0"));
    EXPECT_EQ(prim.texcoord(0), prim.attribute("TEXCOORD_0"));
    EXPECT_EQ(prim.weights(0), prim.attribute("WEIGHTS_0"));

    // animations
    {
        EXPECT_EQ(gltf.animationCount(), 32);
    }

    // images
    {
        EXPECT_EQ(gltf.imageCount(), 1);
        auto image = gltf.image(0);
        EXPECT_TRUE(image);
        EXPECT_STREQ(image.uri(), "Monster.jpg");
    }

    // textures
    EXPECT_EQ(gltf.textureCount(), 1);
    EXPECT_FALSE(gltf.texture(1));
    auto texture = gltf.texture(0);
    EXPECT_TRUE(texture);
    auto image = texture.image();
    EXPECT_TRUE(image);
    EXPECT_STREQ(image.uri(), "Monster.jpg");
    auto sampler = texture.sampler();
    EXPECT_TRUE(sampler);
    EXPECT_EQ(sampler.magFilter(), Sampler::MagFilter::LINEAR);
    EXPECT_EQ(sampler.minFilter(), Sampler::MinFilter::NEAREST_MIPMAP_LINEAR);
    size_t samplerIndex;
    EXPECT_TRUE(texture.sampler(samplerIndex));
    EXPECT_EQ(samplerIndex, 0);

    // skins
    EXPECT_EQ(gltf.skinCount(), 1);
    EXPECT_FALSE(gltf.skin(1));

    size_t skinIndex = 255;
    EXPECT_TRUE(gltf.node(1).skin(skinIndex));
    EXPECT_EQ(skinIndex, 0);

    size_t meshIndex = 255;
    EXPECT_TRUE(gltf.node(1).mesh(meshIndex));
    EXPECT_EQ(meshIndex, 0);

    auto skin = gltf.skin(0);
    // TODO
    auto inverseBindMat = skin.inverseBindMatrices();
    EXPECT_TRUE(inverseBindMat);
    size_t inverseIndex;
    EXPECT_TRUE(skin.inverseBindMatrices(inverseIndex));
    EXPECT_EQ(inverseIndex, 134);

    auto skeleton = skin.skeleton();
    EXPECT_TRUE(skeleton);
    EXPECT_EQ(skeleton.childCount(), 1);
    EXPECT_EQ(skeleton.child(0).childCount(), 4);
    size_t skeletonIndex;
    EXPECT_TRUE(skin.skeleton(skeletonIndex));
    EXPECT_EQ(skeletonIndex, 2);

    std::vector<size_t> joints{ 2, 3, 18, 19, 32, 33, 26, 27, 28, 29, 30, 31, 20, 21, 22, 23, 24, 25, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 4, 5, 6, 7 };
    EXPECT_EQ(skin.jointCount(), joints.size());
    for (size_t i = 0; i < joints.size(); ++i) {
        EXPECT_EQ(skin.joint(i), joints[i]);
    }

    std::vector<size_t> jointCopy;
    jointCopy.resize(joints.size());
    skin.joints(jointCopy.data(), jointCopy.size());
    EXPECT_EQ(joints, jointCopy);

    EXPECT_EQ(joints, skin.joints());

    EXPECT_TRUE(gltf.node(1).skin());

    // find skin by name
    const char* name = "Armature";
    skin = gltf.findSkin(name);
    EXPECT_TRUE(skin);
    EXPECT_FALSE(Skin());
    EXPECT_STREQ(name, skin.name());
    EXPECT_EQ(skin, gltf.skin(0));
    EXPECT_EQ(gltf.findSkin(nullptr), Skin());
}
