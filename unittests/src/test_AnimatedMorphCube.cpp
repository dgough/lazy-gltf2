#include <lazy_gltf2.hpp>
#include <gtest/gtest.h>

#include <array>

#include "common.hpp"

using namespace gltf2;

static const char* LANTERN_PATH = LAZY_GLTF2_BASE_SAMPLE_DIR "/2.0/AnimatedMorphCube/glTF/AnimatedMorphCube.gltf";

TEST(gltf, AnimatedMorphCube) {
    Gltf gltf(LANTERN_PATH);
    EXPECT_TRUE(gltf);

    auto material = gltf.material(0);
    EXPECT_TRUE(material);
    auto roughness = material.pbrMetallicRoughness();
    EXPECT_TRUE(roughness);
    EXPECT_EQ(0.0f, roughness.metallicFactor());
    EXPECT_EQ(0.5f, roughness.roughnessFactor());
    {
        std::array<float, 4> expected = { 0.6038274f, 0.6038274f, 0.6038274f, 1.0f };
        std::array<float, 4> actual;
        roughness.baseColorFactor(actual.data());
        EXPECT_EQ(expected, actual);
    }
    EXPECT_FALSE(roughness.baseColorTexture());
    EXPECT_FALSE(roughness.metallicRoughnessTexture());

    // animations
    EXPECT_EQ(1, gltf.animationCount());
    EXPECT_FALSE(gltf.animation(1));
    auto animation = gltf.animation(0);
    EXPECT_TRUE(animation);
    EXPECT_STREQ("Square", animation.name());

    {
        EXPECT_EQ(1, animation.channelCount());
        EXPECT_FALSE(animation.channel(1));
        auto channel = animation.channel(0);
        EXPECT_TRUE(channel);
        // channel.target
        auto target = channel.target();
        EXPECT_TRUE(target);
        EXPECT_EQ(TargetPath::WEIGHTS, target.path());
        size_t nodeIndex;
        EXPECT_TRUE(target.node(nodeIndex));
        EXPECT_EQ(0, nodeIndex);
        EXPECT_TRUE(target.node());

        // channel.sampler
        EXPECT_TRUE(channel.sampler());
        size_t samplerIndex;
        EXPECT_TRUE(channel.sampler(samplerIndex));
        EXPECT_EQ(0, samplerIndex);
    }
    {
        EXPECT_EQ(1, animation.samplerCount());
        EXPECT_FALSE(animation.sampler(1));
        auto sampler = animation.sampler(0);
        EXPECT_TRUE(sampler);

        EXPECT_EQ(Interpolation::LINEAR, sampler.interpolation());
        EXPECT_STREQ("LINEAR", sampler.interpolationStr());

        auto input = sampler.input();
        EXPECT_TRUE(input);
        size_t inputIndex;
        EXPECT_TRUE(sampler.input(inputIndex));
        EXPECT_EQ(10, inputIndex);

        auto output = sampler.output();
        EXPECT_TRUE(output);
        size_t outputIndex;
        EXPECT_TRUE(sampler.output(outputIndex));
        EXPECT_EQ(11, outputIndex);
    }

    // morph targets
    {
        auto prim = gltf.mesh(0).primitive(0);
        EXPECT_TRUE(prim);
        EXPECT_EQ(2, prim.targetCount());
        {
            auto morphTarget = prim.target(0);
            EXPECT_TRUE(morphTarget);
            EXPECT_EQ(3, morphTarget.normal());
            EXPECT_EQ(4, morphTarget.position());
            EXPECT_EQ(5, morphTarget.tangent());
        }
        {
            auto morphTarget = prim.target(1);
            EXPECT_TRUE(morphTarget);
            EXPECT_EQ(6, morphTarget.normal());
            EXPECT_EQ(7, morphTarget.position());
            EXPECT_EQ(8, morphTarget.tangent());
        }
    }
}
