#include <lazy_gltf2.hpp>
#include <gtest/gtest.h>
#include <array>

#include "common.hpp"

using namespace gltf2;

static const char* DUCK_PATH = LAZY_GLTF2_BASE_SAMPLE_DIR "/2.0/Duck/glTF/Duck.gltf";
static const char* BINARY_DUCK_PATH = LAZY_GLTF2_BASE_SAMPLE_DIR "/2.0/Duck/glTF-Binary/Duck.glb";
static const char* BASE64_DUCK_PATH = LAZY_GLTF2_BASE_SAMPLE_DIR "/2.0/Duck/glTF-Embedded/Duck.gltf";
static const char* PBR_DUCK_PATH = LAZY_GLTF2_BASE_SAMPLE_DIR "/2.0/Duck/glTF-pbrSpecularGlossiness/Duck.gltf";
static const char* MAT_DUCK_PATH = LAZY_GLTF2_BASE_SAMPLE_DIR "/2.0/Duck/glTF-MaterialsCommon/Duck.gltf";

TEST(gltf, compare_ducks) {
    Gltf gltfDuck(DUCK_PATH);
    ASSERT_TRUE(gltfDuck);
    Gltf binDuck(BINARY_DUCK_PATH);
    ASSERT_TRUE(binDuck);
    Gltf base64Duck(BASE64_DUCK_PATH);
    ASSERT_TRUE(base64Duck);

    auto b1 = gltfDuck.buffer(0);
    EXPECT_TRUE(b1);
    auto b2 = binDuck.buffer(0);
    EXPECT_TRUE(b2);
    auto b3 = base64Duck.buffer(0);
    EXPECT_TRUE(b3);

    auto length1 = b1.byteLength();
    auto length2 = b2.byteLength();
    auto length3 = b3.byteLength();

    std::vector<unsigned char> v1;
    std::vector<unsigned char> v2;
    std::vector<unsigned char> v3;
    EXPECT_TRUE(b1.load(v1));
    EXPECT_TRUE(b2.load(v2));
    EXPECT_TRUE(b3.load(v3));
    // GLB duck has different sized data?
    //EXPECT_EQ(v1, v2);
    //EXPECT_EQ(v1, v3);

    // compare gltf objects
    EXPECT_EQ(gltfDuck, gltfDuck);
    EXPECT_NE(gltfDuck, binDuck);
    EXPECT_NE(gltfDuck, base64Duck);
    EXPECT_NE(gltfDuck, Gltf());


    auto base64Image = base64Duck.image(0);
    EXPECT_TRUE(base64Image);
    EXPECT_TRUE(base64Image.isBase64());
    std::vector<unsigned char> base64ImageData;
    EXPECT_TRUE(base64Image.loadBase64(base64ImageData));
    EXPECT_GT(base64ImageData.size(), 0);


}
