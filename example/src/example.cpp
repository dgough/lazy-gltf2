// The purpose of this file is to make sure the example in README.md actually compiles.
#include <lazy_gltf2.hpp>
#include <iostream>
int main() {
    static const char* path = "../../glTF-Sample-Models/2.0/Lantern/glTF/Lantern.gltf";
    gltf2::Gltf gltf;
    if (!gltf.load(path)) {
        std::cout << "Failed to load " << path << std::endl;
        return 1;
    }
    // get the 3rd mesh from the gltf array of meshes
    auto lanternMesh = gltf.mesh(2);

    // get the 2nd node in the default scene
    auto node = gltf.scene().node(1);

    // all objects can be used in an if statement to test if they are valid
    if (node) {
        // get this node's first child
        auto c = node.child(0);
        // or use operator[]
        c = node[0];
        // loop through each child index of the node
        for (const auto& index : node.children()) {
            // look up the child node using this index
            auto child = gltf.node(index);
            if (auto camera = child.camera()) {
                // this child has a camera
                if (camera.type() == gltf2::Camera::Type::PERSPECTIVE) {
                    auto yfov = camera.perspective().yfov();
                    std::cout << yfov << std::endl;
                }
            }
        }
    }

    // load a buffer's data regardless of whether it is an external bin file, base64 text or in a GLB chunk
    std::vector<unsigned char> data;
    if (auto buffer = gltf.buffer(0).load(data)) {
        std::cout << data.size() << std::endl;
    }

    // method calls can be chained and you don't have to worry about null pointers
    auto image = gltf.mesh(2).primitive(0).material().pbrMetallicRoughness().baseColorTexture().texture().image();
    // the resulting image object will return false when used in an if statement if it wasn't found
    if (image) {
        if (const char* uri = image.uri()) {
            // image is in an external file
            std::string imagePath = gltf.baseDir() + image.uri();
            // image loading API not included
            // Image::createFromFile(imagePath.c_str());
        }
        else if (auto bufferView = image.bufferView()) {
            // image is in the GLB chunk
            std::vector<unsigned char> imageData;
            if (bufferView.buffer().load(imageData)) {
                // Image::createFromFileMemory(imageData.data(), imageData.size());
            }
        }
    }

    // loop through all meshes in this gltf file
    const size_t meshCount = gltf.meshCount();
    for (size_t i = 0; i < meshCount; ++i) {
        auto mesh = gltf.mesh(i);
    }
    // or get the meshes as an std::vector<Mesh> and use a range-based for loop
    for (const auto& mesh : gltf.meshes()) {
        std::cout << mesh.primitiveCount() << std::endl;
    }
    // find a node by name
    if (auto chainNode = gltf.findNode("LanternPole_Chain")) {
        // read the translation if it is set
        std::array<float, 3> t{ 0.0f, 0.0f, 0.0f };
        chainNode.translation(t.data());
        std::cout << t[0] << ',' << t[1] << ',' << t[2] << std::endl;
        // or if you are using glm
        // glm::vec3 trans;
        // chainNode.translation(glm::value_ptr(trans));
    }

    if (auto material = gltf.scene(0).node(3).mesh().primitive(0).material()) {

    }
    return 0;
}
