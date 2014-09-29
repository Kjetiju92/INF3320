/* $Id: SimpleViewer.cpp, v1.1 2011/09/20$
 *
 * Author: Christopher Dyken, <dyken@cma.uio.no>
 * Reviewed by: Bartlomiej Siwek, <bartloms@ifi.uio.no>
 *
 * Distributed under the GNU GPL.
 */

#include "SimpleViewer.hpp"

#include <cmath>
#include <stdexcept>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace GfxUtil {
  SimpleViewer::SimpleViewer() {
    setViewVolume(glm::vec3(0.0f, 0.0f, 0.0f), 1.74);
  }

  void SimpleViewer::setWindowSize(int w, int h) {
    m_window_width = w;
    m_window_height = h;
    m_window_aspect = (float)w / (float)h;
  }

  void SimpleViewer::setViewVolume(glm::vec3 origin, float radius) {
    // skip
    // set viewvolume related variables...
    // unskip
    m_viewvolume_origin = origin;
    m_viewvolume_radius = radius;

    m_camera_position = origin;


    m_camera_distance = 4.0f;
  }

  void SimpleViewer::rotationBegin(int x, int y) {
    // skip
    // handle beginning of the rotation...
    // unskip
    SimpleViewer::m_rotation_orientation_i = SimpleViewer::m_camera_orientation;
    // Get the current coords when the mouse is initially clicked on the unit sphere
    SimpleViewer::m_rotation_mousepos_i = getPointOnUnitSphere(getNormalizedCoords(x, y));

    // Last set the state to be rotating
    m_state = ROTATING;
  }

  void SimpleViewer::panBegin(int x, int y) {
    // skip
    // handle beginning of panning... (OPTIONAL)
    // unskip
  }

  void SimpleViewer::zoomBegin(int x, int y, char dir) {
    // skip
    // handle beginning of zoom... (OPTIONAL)
    // unskip
  }

  void SimpleViewer::resetState(int /*x*/, int /*y*/) {
    m_state = NONE;
  }

  void SimpleViewer::motion(int x, int y) {
    // skip
    // handle mouse motion while one of the mouse buttons is down...
    // unskip
    if (m_state == ROTATING) {
      // Get the current mouse position on the unit sphere
      m_rotation_mousepos_c = getPointOnUnitSphere(getNormalizedCoords(x, y));
      glm::fquat t = getGreatCircleRotation(m_rotation_mousepos_i, m_rotation_mousepos_c);
      // Sets the rotation angle and axis (The red line)
      m_rotation_angle = t.w;
      m_rotation_axis = glm::vec3(t.x, t.y, t.z);
      
      m_camera_orientation = t * m_rotation_orientation_i;
    }
  }

  /**
   * Returns a prjoction matrix that starts as a identity matrix which is then 
   * multiplied with a frustum matrix with the parameters:
   * left: -m_window_aspect
   * right: m_window_aspect
   * bottom: -1.0f,
   * top: 1.0f,
   * near: 1.0f
   * far: -1.0f
   *
   * @return - A projection matrix
   **/
  glm::mat4x4 SimpleViewer::getProjectionMatrix() {
    glm::mat4x4 result;

    result = glm::mat4x4(1.0f) * glm::frustum(-m_window_aspect, m_window_aspect, -1.0f, 1.0f, 1.0f, -1.0f);
      
    return result;
  }

  glm::mat4x4 SimpleViewer::getModelViewMatrix() {
    glm::mat4x4 result;

      // Moves the cube so it's in the center of the projection/camera view
      result = glm::mat4x4(1.0f) * glm::translate(result, glm::vec3(0.0f, 0.0f, -m_camera_distance)); 
      // Cast the quaternion to a matrix that fits the modelview matrix
      result = result * glm::mat4_cast(m_camera_orientation);

      return result;
  }

  void SimpleViewer::renderDebugGraphics() {
    // This is a debug method that uses the deprecated API
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-m_window_aspect, m_window_aspect, -1.0f, 1.0f, -1.0f, 1.0f);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    switch(m_state) {
      case ROTATING:
        glLineWidth(3.0);

        glColor3f(0.4, 0.4, 0.4);
        glBegin(GL_LINE_LOOP);
        for(size_t i=0; i<50; i++)
          glVertex2f(glm::cos(2.0*M_PI*i/(float)50), glm::sin(2.0*M_PI*i/(float)50));
        glEnd();

        glBegin(GL_LINES);
        glColor3f(1.0, 0.5, 0.5);
        glVertex3f(0.0f, 0.0f, 0.0f);
        glVertex3fv(&m_rotation_axis[0]);

        glColor3f(0.5, 1.0, 0.5);
        glVertex3f(0.0f, 0.0f, 0.0f);
        glVertex3fv(&m_rotation_mousepos_i[0]);

        glColor3f(0.5, 0.5, 1.0);
        glVertex3f(0.0f, 0.0f, 0.0f);
        glVertex3fv(&m_rotation_mousepos_c[0]);

        glEnd();
        break;
      default:
        // Do nothing.
        break;
    }
  }

  glm::vec2 SimpleViewer::getNormalizedCoords(int x, int y) {
    // skip
    // compute the position of mouse in a coordinate system convinet for us...
    // unskip
    float tmpX = (float)x;
    float tmpY = (float)y;

    float fX = (tmpX/m_window_width) - 0.5f;
    float fY = 0.5f - (tmpY/m_window_height);

    return glm::vec2(fX, fY);
  }

  glm::vec3 SimpleViewer::getPointOnUnitSphere(glm::vec2 p) {
    // skip
    // project mouse position in a convinient coordinate system to a unit sphere...
    // unskip
    float r = glm::sqrt((p.x * p.x) + (p.y * p.y));

    if (r < 0.5f) 
      return glm::vec3(2.0f * p.x, 2.0f * p.y, glm::sqrt(1.0f - 4.0f * (r * r)));
    return glm::vec3(p.x / r, p.y / r, 0.0f);
  }

  // This function computes the quaternion corresponding to the great circle rotation between two vectors
  glm::fquat SimpleViewer::getGreatCircleRotation(glm::vec3 a, glm::vec3 b) {
    glm::vec3 axis = glm::cross(a, b);
    float length = glm::length(axis);
    if(length > std::numeric_limits<float>::epsilon()) {
      axis = axis / length;

      float angle = glm::acos(glm::dot(a,b));
      float c = glm::cos(0.5*angle);
      float s = glm::sin(0.5*angle);
      return glm::fquat(c, s*axis[0], s*axis[1], s*axis[2]);
    } else {
      return glm::fquat(1.0f, 0.0f, 0.0f, 0.0f);
    }
  }

  // This function computes the quaternion corresponding to the rotation by a give angle around given axis
  glm::fquat SimpleViewer::getAxisAngleRotation(glm::vec3& axis, float angle) {
    float c = glm::cos(0.5f * angle);
    float s = glm::sin(0.5f * angle);
    return glm::fquat(c, s*axis[0], s*axis[1], s*axis[2]);
  }
} // namespace GfxUtil
