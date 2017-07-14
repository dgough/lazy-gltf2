#include <lazy_gltf2.hpp>
#include <gtest/gtest.h>
#include <array>

#include "common.hpp"

using namespace gltf2;

static const char* CAMERAS_PATH = LAZY_GLTF2_BASE_SAMPLE_DIR "/2.0/Cameras/glTF/Cameras.gltf";

TEST(gltf, cameras) {
    Gltf gltf(CAMERAS_PATH);
    EXPECT_TRUE(gltf);

    EXPECT_EQ(gltf.cameraCount(), 2);

    auto camera1 = gltf.camera(0);
    EXPECT_TRUE(camera1);
    EXPECT_EQ(camera1.type(), Camera::Type::PERSPECTIVE);
    EXPECT_FALSE(camera1.orthographic());
    auto pers = camera1.perspective();
    EXPECT_EQ(pers.aspectRatio(), 1.0f);
    EXPECT_EQ(pers.yfov(), 0.7f);
    EXPECT_EQ(pers.zfar(), 100.0f);
    EXPECT_EQ(pers.znear(), 0.01f);

    auto camera2 = gltf.camera(1);
    EXPECT_TRUE(camera2);
    EXPECT_EQ(camera2.type(), Camera::Type::ORTHOGRAPHIC);
    EXPECT_FALSE(camera2.perspective());
    auto ortho = camera2.orthographic();
    EXPECT_EQ(ortho.xmag(), 1.0f);
    EXPECT_EQ(ortho.ymag(), 1.0f);
    EXPECT_EQ(ortho.zfar(), 100.0f);
    EXPECT_EQ(ortho.znear(), 0.01f);

    EXPECT_FALSE(gltf.camera(2));

    EXPECT_TRUE(gltf.node(1).camera());
    EXPECT_TRUE(gltf.node(2).camera());

    size_t cameraIndex = 100;
    EXPECT_TRUE(gltf.node(1).camera(cameraIndex));
    EXPECT_EQ(cameraIndex, 0);
    EXPECT_TRUE(gltf.node(2).camera(cameraIndex));
    EXPECT_EQ(cameraIndex, 1);
    EXPECT_FALSE(gltf.node(0).camera(cameraIndex));
    EXPECT_FALSE(gltf.node(3).camera(cameraIndex));

    // scene.nodes
    auto nodes = gltf.scene(0).nodes();
    std::vector<size_t> expectedNodes{ 0,1,2 };
    EXPECT_EQ(nodes, expectedNodes);

    auto cameras = gltf.cameras();
    EXPECT_EQ(cameras.size(), 2);
    EXPECT_EQ(cameras[0].type(), Camera::Type::PERSPECTIVE);
    EXPECT_EQ(cameras[1].type(), Camera::Type::ORTHOGRAPHIC);
}
