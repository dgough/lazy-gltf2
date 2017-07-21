# lazy-gltf2
Header only C++11 glTF 2.0 lazy loader

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

- copy the header files in `include/`
- copy [rapidjson's header files](https://github.com/miloyip/rapidjson/tree/master/include)

## Example

```c++
// TODO example
```

## Dependencies

- [rapidjson](https://github.com/miloyip/rapidjson)
- [libb64](http://libb64.sourceforge.net)

## References

- [glTf 2.0 specification](https://github.com/KhronosGroup/glTF/blob/master/specification/2.0/README.md)