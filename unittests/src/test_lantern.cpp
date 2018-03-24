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
    EXPECT_EQ(expectedMeshCount, gltf.meshCount());
    EXPECT_FALSE(gltf.mesh(expectedMeshCount));

    auto body = gltf.mesh(0);
    EXPECT_TRUE(body);
    EXPECT_STREQ("LanternPole_Body", body.name());
    EXPECT_EQ(1, body.primitiveCount());
    auto bodyPrim = body.primitive(0);
    EXPECT_FALSE(bodyPrim.texcoord(5));

    EXPECT_EQ(4, bodyPrim.attributeCount());

    for (const auto& key : bodyPrim.attributeStrings()) {
        EXPECT_TRUE(bodyPrim.attribute(key));
    }
    auto attributes = bodyPrim.attributes();
    EXPECT_EQ(4, attributes.size());
    if (attributes.size() >= 4) {
        EXPECT_EQ(0, attributes[0].second);
        EXPECT_EQ(1, attributes[1].second);
        EXPECT_EQ(2, attributes[2].second);
        EXPECT_EQ(3, attributes[3].second);
    }

    EXPECT_TRUE(bodyPrim.position());
    EXPECT_TRUE(bodyPrim.normal());
    EXPECT_TRUE(bodyPrim.tangent());
    EXPECT_TRUE(bodyPrim.texcoord(0));

    {
        auto& prim = bodyPrim;
        auto position = prim.position();
        EXPECT_EQ(Accessor::ComponentType::FLOAT, position.componentType());
        EXPECT_EQ(926, position.count());
        EXPECT_EQ(Accessor::Type::VEC3, position.type());

        std::array<float, 3> max;
        position.max(max.data(), max.size());
        EXPECT_FLOAT_EQ(7.74559927f, max[0]);
        EXPECT_FLOAT_EQ(12.8321095f, max[1]);
        EXPECT_FLOAT_EQ(2.31570983f, max[2]);

        std::array<float, 3> min;
        position.min(min.data(), min.size());
        EXPECT_FLOAT_EQ(-7.74559927f, min[0]);
        EXPECT_FLOAT_EQ(-12.8321095f, min[1]);
        EXPECT_FLOAT_EQ(-2.31570983f, min[2]);
    }

    // indices
    size_t indicesIndex;
    EXPECT_TRUE(gltf.mesh(0).primitive(0).indices(indicesIndex));
    EXPECT_EQ(4, indicesIndex);

    // material
    const size_t expectedMaterialCount = 1;
    EXPECT_EQ(expectedMaterialCount, gltf.materialCount());
    EXPECT_FALSE(gltf.material(expectedMaterialCount));
    auto material = gltf.material(0);
    EXPECT_TRUE(material);
    auto normalTexture = material.normalTexture();
    EXPECT_TRUE(normalTexture);
    EXPECT_EQ(2, normalTexture.index());
    EXPECT_TRUE(normalTexture.texture());

    std::array<float, 3> emissiveFactorActual;
    std::array<float, 3> emissiveFactorExpected = { 1.0f, 1.0f, 1.0f };
    material.emissiveFactor(emissiveFactorActual.data());
    EXPECT_EQ(emissiveFactorExpected, emissiveFactorActual);
    EXPECT_TRUE(gltf.defaultScene()[0][0].mesh().primitive(0).material());

    // pbrMetallicRoughness
    auto roughness = material.pbrMetallicRoughness();
    EXPECT_TRUE(roughness);
    EXPECT_EQ(1.0f, roughness.metallicFactor());
    EXPECT_EQ(1.0f, roughness.roughnessFactor());
    {
        auto baseColorTextureInfo = roughness.baseColorTexture();
        EXPECT_TRUE(baseColorTextureInfo);
        EXPECT_EQ(0, baseColorTextureInfo.index());
        EXPECT_TRUE(baseColorTextureInfo.texture());
        EXPECT_EQ(baseColorTextureInfo.texture(), gltf.texture(baseColorTextureInfo.index()));
    }
    {
        auto metallicInfo = roughness.metallicRoughnessTexture();
        EXPECT_TRUE(metallicInfo);
        EXPECT_EQ(1, metallicInfo.index());
        EXPECT_TRUE(metallicInfo.texture());
        EXPECT_EQ(metallicInfo.texture(), gltf.texture(metallicInfo.index()));
    }

    // child nodes
    {
        auto node = gltf.defaultScene()[0];
        auto children = node.children();
        std::vector<size_t> expected{ 0, 1, 2 };
        EXPECT_EQ(expected, children);
        EXPECT_EQ(node.childCount(), children.size());
        EXPECT_EQ(node.nodes(), node.children());
    }

    // find node by name
    const char* nodeName = "LanternPole_Lantern";
    auto poleNode = gltf.findNode(nodeName);
    EXPECT_TRUE(poleNode);
    EXPECT_STREQ(nodeName, poleNode.name());
    EXPECT_STRNE("asdf", poleNode.name());

    // object comparison
    EXPECT_EQ(poleNode, gltf.node(2));
    EXPECT_NE(poleNode, gltf.node(0));
    EXPECT_NE(poleNode, gltf.node(1));
    EXPECT_NE(poleNode, gltf.node(500));
    EXPECT_NE(poleNode, Node());

    // find mesh by name
    {
        const char* meshName = "LanternPole_Lantern";
        auto mesh = gltf.findMesh(meshName);
        EXPECT_TRUE(mesh);
        EXPECT_EQ(mesh, gltf.mesh(2));
        EXPECT_STREQ(mesh.name(), meshName);
        EXPECT_STRNE(mesh.name(), "asdf");
        EXPECT_NE(mesh, gltf.mesh(0));
        EXPECT_NE(Mesh(), mesh);
    }
    // find material by name
    const char* matName = "LanternPost_Mat";
    material = gltf.findMaterial(matName);
    EXPECT_TRUE(material);
    EXPECT_EQ(gltf.material(0), material);
    EXPECT_FALSE(gltf.findMaterial(nullptr));
    EXPECT_EQ(Material(), gltf.findMaterial(nullptr));

    // vectors
    EXPECT_EQ(gltf.scenes().size(), gltf.sceneCount());
    EXPECT_EQ(gltf.nodes().size(), gltf.nodeCount());
    EXPECT_EQ(gltf.meshes().size(), gltf.meshCount());
    EXPECT_EQ(gltf.cameras().size(), gltf.cameraCount());
    EXPECT_EQ(gltf.accessors().size(), gltf.accessorCount());
    EXPECT_EQ(gltf.buffers().size(), gltf.bufferCount());
    EXPECT_EQ(gltf.bufferViews().size(), gltf.bufferViewCount());
    EXPECT_EQ(gltf.animations().size(), gltf.animationCount());
    EXPECT_EQ(gltf.images().size(), gltf.imageCount());
    EXPECT_EQ(gltf.textures().size(), gltf.textureCount());
    EXPECT_EQ(gltf.samplers().size(), gltf.samplerCount());
    EXPECT_EQ(gltf.materials().size(), gltf.materialCount());
    EXPECT_EQ(gltf.skins().size(), gltf.skinCount());
}
