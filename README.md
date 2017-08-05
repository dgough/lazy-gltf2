# lazy-gltf2
Header only C++11 glTF 2.0 loader

## Design

### Goals

- Provide an alternative way to read a glTF 2.0 file without having to use a json API directly
- Don't automatically allocate lots of objects for things you don't care about (like animations)
- Don't automatically load large files like images or buffers until told to
- Support loading glTF 2.0 files with external bins, embedded base64 text or GLB (binary blob)

### Limitations

- Only for loading glTF 2.0 (no writing)
- The objects returned by the Gltf class become invalid once that Gltf object loads a new file or is destroyed. Think of them like iterators.

## Setup

- copy the header files from `include/`
- copy [rapidjson's header files](https://github.com/miloyip/rapidjson/tree/master/include)

## Example

```c++
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
        // image data can be stored in external file, an embedded base64 encoding or in the GLB chunk
        if (const char* uri = image.uri()) {
            if (image.isBase64()) {
                // image is embedded base64
                std::vector<unsigned char> imageData;
                if (image.loadBase64(imageData)) {
                    // Image::createFromFileMemory(imageData.data(), static_cast<int>(imageData.size()));
                }
            }
            else {
                // image is in an external file
                std::string imagePath = gltf.baseDir() + image.uri();
                // image loading API not included
                // Image::createFromFile(imagePath.c_str());
            }
        }
        else if (auto bufferView = image.bufferView()) {
            // image is in the GLB chunk. You might want to cache the buffer because you're going to read from it multiple times.
            std::vector<unsigned char> buffer;
            if (bufferView.buffer().load(buffer)) {
                // Image::createFromFileMemory(imageData.data() + bufferView.byteOffset(), bufferView.byteLength());
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
    return 0;
}
```

### Example in practice

I use this API to load glTF 2.0 files in my engine:

- [GLTF2Loader.cpp](https://github.com/dgough/kepler3d/blob/master/kepler/src/GLTF2Loader.cpp)

## License

- [Apache License 2.0](LICENSE)

## Dependencies

- [rapidjson](https://github.com/miloyip/rapidjson)
- [libb64](http://libb64.sourceforge.net) (modified and included)

## References

- [glTf 2.0 specification](https://github.com/KhronosGroup/glTF/blob/master/specification/2.0/README.md)
- [glTF 2.0 sample models](https://github.com/KhronosGroup/glTF-Sample-Models/tree/master/2.0)
