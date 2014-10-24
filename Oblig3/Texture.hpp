/* $Id: Texture.hpp, v1.1 2011/09/20$
 *
 * Author: Bartlomiej Siwek, <bartloms@ifi.uio.no>
 *
 * Distributed under the GNU GPL.
 */

#ifndef TEXTURE_H_
#define TEXTURE_H_

#include <string>
#include <GL/glew.h>

namespace GfxUtil {
  
class Texture {
 public:
  Texture();
  ~Texture();
  void Load(const std::string &filename);
  GLuint GetTextureId();
  
 protected:
  GLuint m_texture_id_;
};

}  // GfxUtil

#endif  // TEXTURE_H_
