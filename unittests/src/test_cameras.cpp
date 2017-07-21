#include <lazy_gltf2.hpp>
#include <gtest/gtest.h>
#include <array>

#include "common.hpp"

using namespace gltf2;

static const char* CAMERAS_PATH = LAZY_GLTF2_BASE_SAMPLE_DIR "/2.0/Cameras/glTF/Cameras.gltf";

TEST(gltf, cameras) {
    Gltf gltf(CAMERAS_PATH);
    EXPECT_TRUE(gltf);

    EXPECT_EQ(2, gltf.cameraCount());

    auto camera1 = gltf.camera(0);
    EXPECT_TRUE(camera1);
    EXPECT_EQ(Camera::Type::PERSPECTIVE, camera1.type());
    EXPECT_FALSE(camera1.orthographic());
    auto pers = camera1.perspective();
    EXPECT_EQ(1.0f, pers.aspectRatio());
    EXPECT_EQ(0.7f, pers.yfov());
    EXPECT_EQ(100.0f, pers.zfar());
    EXPECT_EQ(0.01f, pers.znear());

    auto camera2 = gltf.camera(1);
    EXPECT_TRUE(camera2);
    EXPECT_EQ(Camera::Type::ORTHOGRAPHIC, camera2.type());
    EXPECT_FALSE(camera2.perspective());
    auto ortho = camera2.orthographic();
    EXPECT_EQ(1.0f, ortho.xmag());
    EXPECT_EQ(1.0f, ortho.ymag());
    EXPECT_EQ(100.0f, ortho.zfar());
    EXPECT_EQ(0.01f, ortho.znear());

    EXPECT_FALSE(gltf.camera(2));

    EXPECT_TRUE(gltf.node(1).camera());
    EXPECT_TRUE(gltf.node(2).camera());

    size_t cameraIndex = 100;
    EXPECT_TRUE(gltf.node(1).camera(cameraIndex));
    EXPECT_EQ(0, cameraIndex);
    EXPECT_TRUE(gltf.node(2).camera(cameraIndex));
    EXPECT_EQ(1, cameraIndex);
    EXPECT_FALSE(gltf.node(0).camera(cameraIndex));
    EXPECT_FALSE(gltf.node(3).camera(cameraIndex));

    // scene.nodes
    auto nodes = gltf.scene(0).nodes();
    std::vector<size_t> expectedNodes{ 0,1,2 };
    EXPECT_EQ(expectedNodes, nodes);

    auto cameras = gltf.cameras();
    EXPECT_EQ(2, cameras.size());
    EXPECT_EQ(Camera::Type::PERSPECTIVE, cameras[0].type());
    EXPECT_EQ(Camera::Type::ORTHOGRAPHIC, cameras[1].type());
}
