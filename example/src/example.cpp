#include <lazy_gltf2.hpp>
#include <iostream>
static const char* path = "../../glTF-Sample-Models/2.0/Lantern/glTF/Lantern.gltf";
int main() {
    gltf2::Gltf gltf;
    if (gltf.load(path)) {
        const char* name = gltf.scene(0).node(0).name();
        if (name) {
            std::cout << name << std::endl;
        }
        auto image = gltf.mesh(2).primitive(0).material().pbrMetallicRoughness().baseColorTexture().texture().source();
        const char* url = image.uri();
        if (url) {
            std::cout << url << std::endl;
        }
    }
    else {
        std::cout << "Failed to load " << path << std::endl;
    }
#ifdef _MSC_VER
    std::getchar(); // keep console window open until Return keystroke
#endif
    return 0;
}
