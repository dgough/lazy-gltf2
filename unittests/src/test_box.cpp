#include <lazy_gltf2.hpp>
#include <gtest/gtest.h>
#include <array>

#include "common.hpp"

using namespace gltf2;

static const char* BOX_PATH = LAZY_GLTF2_BASE_SAMPLE_DIR "/2.0/Box/glTF/Box.gltf";
static const char* BINARY_BOX_PATH = LAZY_GLTF2_BASE_SAMPLE_DIR "/2.0/Box/glTF-Binary/Box.glb";
static const char* BASE64_BOX_PATH = LAZY_GLTF2_BASE_SAMPLE_DIR "/2.0/Box/glTF-Embedded/Box.gltf";

void testBoxCommon(Gltf& gltf) {
    // counts
    EXPECT_EQ(gltf.nodeCount(), 2);
    EXPECT_EQ(gltf.meshCount(), 1);
    EXPECT_EQ(gltf.sceneCount(), 1);

    EXPECT_TRUE(gltf.node(0));
    EXPECT_TRUE(gltf.node(1));
    EXPECT_TRUE(gltf.scene(0));
    EXPECT_TRUE(gltf.defaultScene());

    // nodes


    // scene
    auto scene = gltf.defaultScene();
    EXPECT_EQ(scene.nodeCount(), 1);
    EXPECT_TRUE(scene.node(0));
    EXPECT_TRUE(scene[0]);

    // asset
    auto asset = gltf.asset();
    EXPECT_TRUE(asset);
    EXPECT_STREQ(asset.version(), "2.0");
    EXPECT_STREQ(asset.generator(), "COLLADA2GLTF");
    EXPECT_EQ(asset.copyright(), nullptr);
    EXPECT_EQ(asset.minVersion(), nullptr);


    auto mesh = gltf.node(0).child(0).mesh();
    EXPECT_TRUE(mesh);
    EXPECT_STREQ(mesh.name(), "Mesh");
    const size_t expectedPrimitiveCount = 1;
    EXPECT_EQ(mesh.primitiveCount(), expectedPrimitiveCount);
    EXPECT_FALSE(mesh.primitive(expectedPrimitiveCount));
    auto prim = mesh.primitive(0);
    EXPECT_EQ(prim.attributeCount(), 2);
    EXPECT_TRUE(prim.attribute("NORMAL"));
    EXPECT_TRUE(prim.attribute("POSITION"));
    EXPECT_TRUE(prim.normal());
    EXPECT_TRUE(prim.position());
    EXPECT_TRUE(prim.material());
    size_t materialIndex;
    EXPECT_TRUE(prim.material(materialIndex));
    EXPECT_EQ(materialIndex, 0);
    {
        // indices
        auto indices = prim.indices();
        EXPECT_TRUE(indices);
        EXPECT_EQ(indices.componentType(), Accessor::ComponentType::UNSIGNED_SHORT);
        EXPECT_EQ(indices.type(), Accessor::Type::SCALAR);
        EXPECT_EQ(indices.count(), 36);
    }

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
    EXPECT_EQ(gltf.accessorCount(), 3);

    {
        auto accessor = gltf.accessor(0);
        EXPECT_TRUE(accessor);
        EXPECT_EQ(accessor.type(), Accessor::Type::SCALAR);
        EXPECT_EQ(accessor.byteOffset(), 0);
        EXPECT_FALSE(accessor.normalized());
        EXPECT_EQ(accessor.count(), 36);
        EXPECT_EQ(accessor.max(0), 23);
        EXPECT_EQ(accessor.maxCount(), 1);
        auto bufferView = accessor.bufferView();
        EXPECT_TRUE(bufferView);
        size_t index;
        EXPECT_TRUE(accessor.bufferView(index));
        EXPECT_EQ(index, 0);
    }
    {
        auto accessor = gltf.accessor(1);
        EXPECT_TRUE(accessor);
        EXPECT_EQ(accessor.type(), Accessor::Type::VEC3);
        EXPECT_EQ(accessor.byteOffset(), 0);
        EXPECT_FALSE(accessor.normalized());
        EXPECT_EQ(accessor.count(), 24);
        EXPECT_EQ(accessor.maxCount(), 3);
        EXPECT_EQ(accessor.max(0), 1.0f);
        size_t index;
        EXPECT_TRUE(accessor.bufferView(index));
        EXPECT_EQ(index, 1);
    }
    {
        auto accessor = gltf.accessor(2);
        EXPECT_TRUE(accessor);
        EXPECT_EQ(accessor.type(), Accessor::Type::VEC3);
        EXPECT_EQ(accessor.byteOffset(), 288);
        EXPECT_FALSE(accessor.normalized());
        EXPECT_EQ(accessor.count(), 24);
        EXPECT_EQ(accessor.maxCount(), 3);
        EXPECT_EQ(accessor.max(0), 0.5f);
        size_t index;
        EXPECT_TRUE(accessor.bufferView(index));
        EXPECT_EQ(index, 1);
    }

    // bufferViews
    {
        EXPECT_EQ(gltf.bufferViewCount(), 2);
        EXPECT_FALSE(gltf.bufferView(2));
        auto bufferView = gltf.bufferView(0);
        EXPECT_TRUE(bufferView);
        EXPECT_TRUE(bufferView.buffer());
        EXPECT_EQ(bufferView.byteOffset(), 576);
        EXPECT_EQ(bufferView.byteLength(), 72);
        EXPECT_TRUE(bufferView.hasTarget());
        EXPECT_EQ(bufferView.target(), BufferView::Target::ELEMENT_ARRAY_BUFFER);

        size_t index;
        EXPECT_TRUE(bufferView.buffer(index));
        EXPECT_EQ(index, 0);
    }
    {
        auto bufferView = gltf.bufferView(1);
        EXPECT_TRUE(bufferView);
        EXPECT_TRUE(bufferView.buffer());
        EXPECT_EQ(bufferView.byteOffset(), 0);
        EXPECT_EQ(bufferView.byteLength(), 576);
        EXPECT_EQ(bufferView.byteStride(), 12);
        EXPECT_TRUE(bufferView.hasTarget());
        EXPECT_EQ(bufferView.target(), BufferView::Target::ARRAY_BUFFER);
    }
    // animations
    EXPECT_EQ(gltf.animationCount(), 0);

    // 
    EXPECT_EQ(numberOfComponents(Accessor::Type::SCALAR), 1);
    EXPECT_EQ(numberOfComponents(Accessor::Type::VEC2), 2);
    EXPECT_EQ(numberOfComponents(Accessor::Type::VEC3), 3);
    EXPECT_EQ(numberOfComponents(Accessor::Type::VEC4), 4);
    EXPECT_EQ(numberOfComponents(Accessor::Type::MAT2), 4);
    EXPECT_EQ(numberOfComponents(Accessor::Type::MAT3), 9);
    EXPECT_EQ(numberOfComponents(Accessor::Type::MAT4), 16);
}

TEST(gltf, box) {
    Gltf gltf;
    ASSERT_TRUE(gltf.load(BOX_PATH));
    EXPECT_TRUE(gltf);
    testBoxCommon(gltf);

    // buffers
    {
        EXPECT_EQ(gltf.bufferCount(), 1);
        EXPECT_FALSE(gltf.buffer(1));
        auto buffer = gltf.buffer(0);
        EXPECT_TRUE(buffer);
        EXPECT_EQ(buffer.byteLength(), 648);
        EXPECT_STREQ(buffer.uri(), "Box0.bin");
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
}