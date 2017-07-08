#include <lazy_gltf2.hpp>
#include <gtest/gtest.h>
#include <array>

#include "common.hpp"

using namespace gltf2;

static const char* LANTERN_PATH = LAZY_GLTF2_BASE_SAMPLE_DIR "/2.0/Lantern/glTF/Lantern.gltf";

TEST(gltf, lantern) {
    Gltf gltf(LANTERN_PATH);
    EXPECT_TRUE(gltf);

    // meshes
    const size_t expectedMeshCount = 3;
    EXPECT_EQ(gltf.meshCount(), expectedMeshCount);
    EXPECT_FALSE(gltf.mesh(expectedMeshCount));

    auto body = gltf.mesh(0);
    EXPECT_TRUE(body);
    EXPECT_STREQ(body.name(), "LanternPole_Body");
    EXPECT_EQ(body.primitiveCount(), 1);
    auto bodyPrim = body.primitive(0);
    EXPECT_FALSE(bodyPrim.texcoord(5));

    EXPECT_EQ(bodyPrim.attributeCount(), 4);

    for (const auto& key : bodyPrim.attributeStrings()) {
        EXPECT_TRUE(bodyPrim.attribute(key));
    }
    auto attributes = bodyPrim.attributes();
    EXPECT_EQ(attributes.size(), 4);
    EXPECT_EQ(attributes[0].second, 0);
    EXPECT_EQ(attributes[1].second, 1);
    EXPECT_EQ(attributes[2].second, 2);
    EXPECT_EQ(attributes[3].second, 3);

    EXPECT_TRUE(bodyPrim.position());
    EXPECT_TRUE(bodyPrim.normal());
    EXPECT_TRUE(bodyPrim.tangent());
    EXPECT_TRUE(bodyPrim.texcoord(0));

    {
        auto& prim = bodyPrim;
        auto position = prim.position();
        EXPECT_EQ(position.componentType(), Accessor::ComponentType::FLOAT);
        EXPECT_EQ(position.count(), 926);
        EXPECT_EQ(position.type(), Accessor::Type::VEC3);

        std::array<float, 3> max;
        position.max(max.data(), max.size());
        EXPECT_FLOAT_EQ(max[0], 7.74559927f);
        EXPECT_FLOAT_EQ(max[1], 12.8321095f);
        EXPECT_FLOAT_EQ(max[2], 2.31570983f);

        std::array<float, 3> min;
        position.min(min.data(), min.size());
        EXPECT_FLOAT_EQ(min[0], 7.74559927f);
        EXPECT_FLOAT_EQ(min[1], 12.8321095f);
        EXPECT_FLOAT_EQ(min[2], 2.31570983f);
    }

    // indices
    size_t indicesIndex;
    EXPECT_TRUE(gltf.mesh(0).primitive(0).indices(indicesIndex));
    EXPECT_EQ(indicesIndex, 4);

    // material
    const size_t expectedMaterialCount = 1;
    EXPECT_EQ(gltf.materialCount(), expectedMaterialCount);
    EXPECT_FALSE(gltf.material(expectedMaterialCount));
    auto material = gltf.material(0);
    EXPECT_TRUE(material);
    auto normalTexture = material.normalTexture();
    EXPECT_TRUE(normalTexture);
    EXPECT_EQ(normalTexture.index(), 2);
    EXPECT_TRUE(normalTexture.texture());

    std::array<float, 3> emissiveFactorActual;
    std::array<float, 3> emissiveFactorExpected = { 1.0f, 1.0f, 1.0f };
    material.emissiveFactor(emissiveFactorActual.data());
    EXPECT_EQ(emissiveFactorActual, emissiveFactorExpected);
    EXPECT_TRUE(gltf.defaultScene()[0][0].mesh().primitive(0).material());

    // pbrMetallicRoughness
    auto roughness = material.pbrMetallicRoughness();
    EXPECT_TRUE(roughness);
    EXPECT_EQ(roughness.metallicFactor(), 1.0f);
    EXPECT_EQ(roughness.roughnessFactor(), 1.0f);

    {
        std::array<float, 4> expected = { 0.214041144f, 0.214041144f, 0.214041144f, 1.0f };
        std::array<float, 4> actual;
        roughness.baseColorFactor(actual.data());
        EXPECT_EQ(actual, expected);
    }
    {
        auto baseColorTextureInfo = roughness.baseColorTexture();
        EXPECT_TRUE(baseColorTextureInfo);
        EXPECT_EQ(baseColorTextureInfo.index(), 0);
        EXPECT_TRUE(baseColorTextureInfo.texture());
        EXPECT_EQ(baseColorTextureInfo.texture(), gltf.texture(baseColorTextureInfo.index()));
    }
    {
        auto metallicInfo = roughness.metallicRoughnessTexture();
        EXPECT_TRUE(metallicInfo);
        EXPECT_EQ(metallicInfo.index(), 1);
        EXPECT_TRUE(metallicInfo.texture());
        EXPECT_EQ(metallicInfo.texture(), gltf.texture(metallicInfo.index()));
    }

    // child nodes
    {
        auto node = gltf.defaultScene()[0];
        auto children = node.children();
        std::vector<size_t> expected{ 0, 1, 2 };
        EXPECT_EQ(children, expected);
        EXPECT_EQ(children.size(), node.childCount());
        EXPECT_EQ(node.nodes(), node.children());
    }

}
