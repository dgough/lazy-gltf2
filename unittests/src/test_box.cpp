#include <lazy_gltf2.hpp>
#include <gtest/gtest.h>
#include <array>

#include "common.hpp"

using namespace gltf2;

static const char* BOX_PATH = LAZY_GLTF2_BASE_SAMPLE_DIR "/2.0/Box/glTF/Box.gltf";
static const char* BINARY_BOX_PATH = LAZY_GLTF2_BASE_SAMPLE_DIR "/2.0/Box/glTF-Binary/Box.glb";
static const char* BASE64_BOX_PATH = LAZY_GLTF2_BASE_SAMPLE_DIR "/2.0/Box/glTF-Embedded/Box.gltf";
static const char* PBR_BOX_PATH = LAZY_GLTF2_BASE_SAMPLE_DIR "/2.0/Box/glTF-pbrSpecularGlossiness/Box.gltf";
static const char* DARCO_BOX_PATH = LAZY_GLTF2_BASE_SAMPLE_DIR "/2.0/Box/glTF-Draco/Box.gltf";

void testBoxCommon(Gltf& gltf) {
    // counts
    EXPECT_EQ(2, gltf.nodeCount());
    EXPECT_EQ(1, gltf.meshCount());
    EXPECT_EQ(1, gltf.sceneCount());

    EXPECT_TRUE(gltf.node(0));
    EXPECT_TRUE(gltf.node(1));
    EXPECT_TRUE(gltf.scene(0));
    EXPECT_TRUE(gltf.defaultScene());

    // nodes


    // scene
    auto scene = gltf.defaultScene();
    EXPECT_EQ(1, scene.nodeCount());
    EXPECT_TRUE(scene.node(0));
    EXPECT_TRUE(scene[0]);

    // asset
    auto asset = gltf.asset();
    EXPECT_TRUE(asset);
    EXPECT_STREQ("2.0", asset.version());
    EXPECT_STREQ("COLLADA2GLTF", asset.generator());
    EXPECT_EQ(nullptr, asset.copyright());
    EXPECT_EQ(nullptr, asset.minVersion());


    auto mesh = gltf.node(0).child(0).mesh();
    EXPECT_TRUE(mesh);
    EXPECT_STREQ("Mesh", mesh.name());
    const size_t expectedPrimitiveCount = 1;
    EXPECT_EQ(expectedPrimitiveCount, mesh.primitiveCount());
    EXPECT_FALSE(mesh.primitive(expectedPrimitiveCount));
    auto prim = mesh.primitive(0);
    EXPECT_EQ(2, prim.attributeCount());
    EXPECT_TRUE(prim.attribute("NORMAL"));
    EXPECT_TRUE(prim.attribute("POSITION"));
    EXPECT_TRUE(prim.normal());
    EXPECT_TRUE(prim.position());
    EXPECT_TRUE(prim.material());
    size_t materialIndex;
    EXPECT_TRUE(prim.material(materialIndex));
    EXPECT_EQ(0, materialIndex);
    {
        // indices
        auto indices = prim.indices();
        EXPECT_TRUE(indices);
        EXPECT_EQ(Accessor::ComponentType::UNSIGNED_SHORT, indices.componentType());
        EXPECT_EQ(Accessor::Type::SCALAR, indices.type());
        EXPECT_EQ(36, indices.count());
    }
    // primitives
    auto primitives = mesh.primitives();
    EXPECT_EQ(1, primitives.size());

    auto c1 = gltf.node(0).child(0);
    auto c2 = gltf.node(1);
    EXPECT_EQ(c1, c2);


    std::array<float, 16> mat1;
    std::array<float, 16> mat2{ 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, -1.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0 };
    EXPECT_TRUE(gltf.node(0).matrix(mat1.data()));
    EXPECT_EQ(mat1, mat2);
    EXPECT_FALSE(gltf.node(1).matrix(mat1.data()));

    // accessors
    EXPECT_FALSE(gltf.accessor(324));
    EXPECT_EQ(3, gltf.accessorCount());

    {
        auto accessor = gltf.accessor(0);
        EXPECT_TRUE(accessor);
        EXPECT_EQ(Accessor::Type::SCALAR, accessor.type());
        EXPECT_EQ(0, accessor.byteOffset());
        EXPECT_FALSE(accessor.normalized());
        EXPECT_EQ(36, accessor.count());
        EXPECT_EQ(23, accessor.max(0));
        EXPECT_EQ(1, accessor.maxCount());
        auto bufferView = accessor.bufferView();
        EXPECT_TRUE(bufferView);
        size_t index;
        EXPECT_TRUE(accessor.bufferView(index));
        EXPECT_EQ(0, index);
    }
    {
        auto accessor = gltf.accessor(1);
        EXPECT_TRUE(accessor);
        EXPECT_EQ(Accessor::Type::VEC3, accessor.type());
        EXPECT_EQ(0, accessor.byteOffset());
        EXPECT_FALSE(accessor.normalized());
        EXPECT_EQ(24, accessor.count());
        // max
        EXPECT_EQ(3, accessor.maxCount());
        EXPECT_EQ(1.0f, accessor.max(0));
        std::vector<float> expectedMax{ 1.0f, 1.0f, 1.0f };
        EXPECT_EQ(expectedMax, accessor.max());
        size_t index;
        EXPECT_TRUE(accessor.bufferView(index));
        EXPECT_EQ(1, index);
        // min
        std::vector<float> expectedMin{ -1.0f, -1.0f, -1.0f };
        EXPECT_EQ(expectedMin, accessor.min());
    }
    {
        auto accessor = gltf.accessor(2);
        EXPECT_TRUE(accessor);
        EXPECT_EQ(Accessor::Type::VEC3, accessor.type());
        EXPECT_EQ(288, accessor.byteOffset());
        EXPECT_FALSE(accessor.normalized());
        EXPECT_EQ(24, accessor.count());
        EXPECT_EQ(3, accessor.maxCount());
        EXPECT_EQ(0.5f, accessor.max(0));
        size_t index;
        EXPECT_TRUE(accessor.bufferView(index));
        EXPECT_EQ(1, index);
    }

    // bufferViews
    {
        EXPECT_EQ(2, gltf.bufferViewCount());
        EXPECT_FALSE(gltf.bufferView(2));
        auto bufferView = gltf.bufferView(0);
        EXPECT_TRUE(bufferView);
        EXPECT_TRUE(bufferView.buffer());
        EXPECT_EQ(576, bufferView.byteOffset());
        EXPECT_EQ(72, bufferView.byteLength());
        EXPECT_EQ(BufferView::Target::ELEMENT_ARRAY_BUFFER, bufferView.target());

        size_t index;
        EXPECT_TRUE(bufferView.buffer(index));
        EXPECT_EQ(0, index);
    }
    {
        auto bufferView = gltf.bufferView(1);
        EXPECT_TRUE(bufferView);
        EXPECT_TRUE(bufferView.buffer());
        EXPECT_EQ(0, bufferView.byteOffset());
        EXPECT_EQ(576, bufferView.byteLength());
        EXPECT_EQ(12, bufferView.byteStride());
        EXPECT_EQ(BufferView::Target::ARRAY_BUFFER, bufferView.target());
    }
    // animations
    EXPECT_EQ(0, gltf.animationCount());

    auto material = gltf.findMaterial("Red");
    EXPECT_TRUE(material);
    std::array<float, 3> expectedEmissiveFactor{ 0.0f, 0.0f, 0.0f };
    EXPECT_EQ(expectedEmissiveFactor, material.emissiveFactor());
    auto roughness = material.pbrMetallicRoughness();
    EXPECT_TRUE(roughness);
    std::array<float, 4> expectedBaseColorFactor{ 0.800000011920929f, 0.0f, 0.0f, 1.0f };
    std::array<float, 4> actualBaseColorFactor;
    EXPECT_TRUE(roughness.baseColorFactor(actualBaseColorFactor.data()));
    EXPECT_EQ(expectedBaseColorFactor, actualBaseColorFactor);
    EXPECT_EQ(actualBaseColorFactor, roughness.baseColorFactor());
    EXPECT_EQ(0.0f, roughness.metallicFactor());

    // numberOfComponents
    EXPECT_EQ(1, numberOfComponents(Accessor::Type::SCALAR));
    EXPECT_EQ(2, numberOfComponents(Accessor::Type::VEC2));
    EXPECT_EQ(3, numberOfComponents(Accessor::Type::VEC3));
    EXPECT_EQ(4, numberOfComponents(Accessor::Type::VEC4));
    EXPECT_EQ(4, numberOfComponents(Accessor::Type::MAT2));
    EXPECT_EQ(9, numberOfComponents(Accessor::Type::MAT3));
    EXPECT_EQ(16, numberOfComponents(Accessor::Type::MAT4));
}

TEST(gltf, box) {
    Gltf gltf;
    ASSERT_TRUE(gltf.load(BOX_PATH));
    EXPECT_TRUE(gltf);
    testBoxCommon(gltf);

    // buffers
    {
        EXPECT_EQ(1, gltf.bufferCount());
        EXPECT_FALSE(gltf.buffer(1));
        auto buffer = gltf.buffer(0);
        EXPECT_TRUE(buffer);
        EXPECT_EQ(648, buffer.byteLength());
        EXPECT_STREQ("Box0.bin", buffer.uri());
    }

    EXPECT_EQ(gltf.baseDir(), std::string(BOX_PATH, strlen(BOX_PATH) - 8));
}

static Gltf createGltf(const char* path) {
    return Gltf(path);
}
static void testMove(Gltf&& gltf) {
    EXPECT_TRUE(gltf);
}

TEST(gltf, box_binary) {
    Gltf gltf = createGltf(BINARY_BOX_PATH);
    EXPECT_TRUE(gltf);
    testBoxCommon(gltf);

    auto buffer = gltf.buffer(0);
    std::vector<unsigned char> data;
    EXPECT_TRUE(buffer.load(data));
    EXPECT_TRUE(data.size() > 0);
    EXPECT_EQ(data.size(), buffer.byteLength());

    testMove(std::move(gltf));
}

TEST(gltf, box_base64) {
    Gltf gltf = createGltf(BASE64_BOX_PATH);
    EXPECT_TRUE(gltf);
    testBoxCommon(gltf);
}

TEST(gltf, compare_buffers) {
    Gltf g1(BOX_PATH);
    ASSERT_TRUE(g1);
    Gltf g2(BINARY_BOX_PATH);
    ASSERT_TRUE(g2);
    Gltf g3(BASE64_BOX_PATH);
    ASSERT_TRUE(g3);

    auto b1 = g1.buffer(0);
    EXPECT_TRUE(b1);
    auto b2 = g2.buffer(0);
    EXPECT_TRUE(b2);
    auto b3 = g3.buffer(0);
    EXPECT_TRUE(b3);

    std::vector<unsigned char> v1;
    std::vector<unsigned char> v2;
    std::vector<unsigned char> v3;
    EXPECT_TRUE(b1.load(v1));
    EXPECT_TRUE(b2.load(v2));
    EXPECT_TRUE(b3.load(v3));
    EXPECT_EQ(v1, v2);
    EXPECT_EQ(v1, v3);

    // compare gltf objects
    EXPECT_EQ(g1, g1);
    EXPECT_NE(g1, g2);
    EXPECT_NE(g1, g3);
    EXPECT_NE(g1, Gltf());
}

TEST(gltf, box_pbr) {
    Gltf gltf(PBR_BOX_PATH);
    ASSERT_TRUE(gltf);
    auto extensionsUsed = gltf.extensionsUsed();
    EXPECT_EQ(1, extensionsUsed.size());
    EXPECT_STREQ("KHR_materials_pbrSpecularGlossiness", extensionsUsed[0]);
}

TEST(gltf, box_darco) {
    Gltf gltf;
    ASSERT_TRUE(gltf.load(DARCO_BOX_PATH));
    auto extensionsUsed = gltf.extensionsUsed();
    ASSERT_EQ(1, extensionsUsed.size());
    EXPECT_STREQ("KHR_draco_mesh_compression", extensionsUsed[0]);

    auto extensionsRequired = gltf.extensionsRequired();
    EXPECT_EQ(1, extensionsRequired.size());
    EXPECT_STREQ("KHR_draco_mesh_compression", extensionsRequired[0]);
}
