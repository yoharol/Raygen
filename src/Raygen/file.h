#ifndef RAYGEN_FILE_H_
#define RAYGEN_FILE_H_

#include <raylib.h>

namespace Raygen {

Texture2D load_texture2d(std::string filepath) {
  Image img = LoadImage(filepath.c_str());
  Texture2D tex = LoadTextureFromImage(img);
  UnloadImage(img);
  return tex;
}

}  // namespace Raygen

#endif  // RAYGEN_FILE_H_
