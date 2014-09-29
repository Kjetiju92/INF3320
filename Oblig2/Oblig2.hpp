/* $Id: Oblig2.hpp, v1.1 2011/09/20$
 *
 * Author: Atgeirr F Rasmussen <atgeirr@sintef.no>
 * Reviewed by: Bartlomiej Siwek, <bartloms@ifi.uio.no>
 *
 * Distributed under the GNU GPL.
 */

#ifndef OBLIG2_HEADER_H_
#define OBLIG2_HEADER_H_

#include <cstdlib>
#include <stdexcept>
#include <glm/gtc/type_ptr.hpp>

#include "OpenGLError.hpp"
#include "GLApp.hpp"
#include "SimpleViewer.hpp"
#include "ColoredCube.hpp"

class Oblig2 : public GLApp {
 public:
  void initGL() {
    glClearColor(0.0f, 0.0f, 0.5f, 0.0f);
    glEnable(GL_CULL_FACE);
    
    m_cube_.init();
  }

  void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    // Set up projection matrix
    glMatrixMode(GL_PROJECTION);
    glm::mat4x4 projection_matrix = m_viewer_.getProjectionMatrix();
    glLoadMatrixf(glm::value_ptr(projection_matrix));
  
    // Setup model view matrix
    glMatrixMode(GL_MODELVIEW);
    glm::mat4x4 model_view_matrix = m_viewer_.getModelViewMatrix();
    glLoadMatrixf(glm::value_ptr(model_view_matrix));  
    
    // Render the cube
    m_cube_.render();

    // Use this for debugging only
    m_viewer_.renderDebugGraphics();
    
    glutSwapBuffers();
  }

  void reshape(int w, int h) {
    // skip
    // setup viewport and update window size variable in different palces...
    // unskip
    glViewport(0, 0, (GLsizei)w, (GLsizei)h);
    m_viewer_.setWindowSize(w, h);
  }

  void keyboard(unsigned char key, int /*x*/, int /*y*/) {
    if (key == 'q') {
      std::exit(0);
    }
  }

  void mouse(int b, int s, int x, int y) {
    // skip
    // handle mouse button events...
    // unskip
    if (s == GLUT_DOWN && b == GLUT_LEFT_BUTTON)
      m_viewer_.rotationBegin(x, y);
    else 
      m_viewer_.resetState(x, y);

    glutPostRedisplay();
  }

  void motion(int x, int y) {
    // skip
    // handle mouse motion while a button is pressed events...
    // unskip
    m_viewer_.motion(x, y);
    glutPostRedisplay();
  }


  int displayMode() {
    return GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH;
  }

  std::string windowName() {
    return "Oblig 2";
  }

 protected:
  GfxUtil::SimpleViewer m_viewer_;
  GfxUtil::ColoredCube m_cube_;
};

#endif // OBLIG2_HEADER_H_
