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
    EXPECT_EQ(roughness.metallicFactor(), 0.0f);
    EXPECT_EQ(roughness.roughnessFactor(), 0.5f);
    {
        std::array<float, 4> expected = { 0.6038274f, 0.6038274f, 0.6038274f, 1.0f };
        std::array<float, 4> actual;
        roughness.baseColorFactor(actual.data());
        EXPECT_EQ(actual, expected);
    }
    EXPECT_FALSE(roughness.baseColorTexture());
    EXPECT_FALSE(roughness.metallicRoughnessTexture());

    // animations
    EXPECT_EQ(gltf.animationCount(), 1);
    EXPECT_FALSE(gltf.animation(1));
    auto animation = gltf.animation(0);
    EXPECT_TRUE(animation);
    EXPECT_STREQ(animation.name(), "Square");

    {
        EXPECT_EQ(animation.channelCount(), 1);
        EXPECT_FALSE(animation.channel(1));
        auto channel = animation.channel(0);
        EXPECT_TRUE(channel);
        // channel.target
        auto target = channel.target();
        EXPECT_TRUE(target);
        EXPECT_EQ(target.path(), TargetPath::WEIGHTS);
        size_t nodeIndex;
        EXPECT_TRUE(target.node(nodeIndex));
        EXPECT_EQ(nodeIndex, 0);
        EXPECT_TRUE(target.node());

        // channel.sampler
        EXPECT_TRUE(channel.sampler());
        size_t samplerIndex;
        EXPECT_TRUE(channel.sampler(samplerIndex));
        EXPECT_EQ(samplerIndex, 0);
    }
    {
        EXPECT_EQ(animation.samplerCount(), 1);
        EXPECT_FALSE(animation.sampler(1));
        auto sampler = animation.sampler(0);
        EXPECT_TRUE(sampler);

        EXPECT_EQ(sampler.interpolation(), Interpolation::LINEAR);
        EXPECT_STREQ(sampler.interpolationStr(), "LINEAR");

        auto input = sampler.input();
        EXPECT_TRUE(input);
        size_t inputIndex;
        EXPECT_TRUE(sampler.input(inputIndex));
        EXPECT_EQ(inputIndex, 10);

        auto output = sampler.output();
        EXPECT_TRUE(output);
        size_t outputIndex;
        EXPECT_TRUE(sampler.output(outputIndex));
        EXPECT_EQ(outputIndex, 11);
    }

    // morph targets
    {
        auto prim = gltf.mesh(0).primitive(0);
        EXPECT_TRUE(prim);
        EXPECT_EQ(prim.targetCount(), 2);
        {
            auto morphTarget = prim.target(0);
            EXPECT_TRUE(morphTarget);
            EXPECT_EQ(morphTarget.normal(), 3);
            EXPECT_EQ(morphTarget.position(), 4);
            EXPECT_EQ(morphTarget.tangent(), 5);
        }
        {
            auto morphTarget = prim.target(1);
            EXPECT_TRUE(morphTarget);
            EXPECT_EQ(morphTarget.normal(), 6);
            EXPECT_EQ(morphTarget.position(), 7);
            EXPECT_EQ(morphTarget.tangent(), 8);
        }
    }
}
