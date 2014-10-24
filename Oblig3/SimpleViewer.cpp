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

namespace GfxUtil {
SimpleViewer::SimpleViewer() { 
    
    setViewVolume(glm::vec3(0.0f, 0.0f, 0.0f), 1.74f);
  }

  void SimpleViewer::setWindowSize(int w, int h) {
    m_window_width = w;
    m_window_height = h;
    m_window_aspect = (float)w / (float)h;
  }
  
  void SimpleViewer::setViewVolume(glm::vec3 origin, float radius) {
    m_viewvolume_origin = origin;
    m_viewvolume_radius = radius;
    m_camera_distance = 4.0f;
    // puts the camera on origo just to be sure so it dosent break the program
    m_camera_position = origin;
  }
  
  void SimpleViewer::rotationBegin(int x, int y) {
    SimpleViewer::m_rotation_orientation_i = SimpleViewer::m_camera_orientation;
    SimpleViewer::m_rotation_mousepos_i = getPointOnUnitSphere(getNormalizedCoords(x, y));
    m_state = ROTATING;
  }

  void SimpleViewer::panBegin(int x, int y) {
    // skip
    // handle beginning of panning... (OPTIONAL)
    // unskip
  }
  
  void SimpleViewer::zoomBegin(int x, int y) {
    // skip
    // handle beginning of zoom... (OPTIONAL)
    // unskip
  }
  
  void SimpleViewer::resetState(int /*x*/, int /*y*/) {
    m_state = NONE;
  }

  void SimpleViewer::motion(int x, int y) {
    /** If the m_state is rotating we start rotating to the point/angle where the 
  mouse position is. We set the m_rotation_mousepos_i in startRotating. This
  is the point where mouse was initially clicked then we set the m_rotation_mousepos_c
  here which is the current position of the mouse. Then we get the rotation quaternion
  which represents the rotation between the two mouse positions. We set the rotation angle
  to be the angle parameter of the quaternion (w) and the axis to be the points represented 
  in the rotation quaternion. Lastly we set the camera orientation to be the rotation quaternion
  times the current camera orientation, this is so we dont reset the camera all the time. 
    */
    if(m_state == ROTATING) {
      m_rotation_mousepos_c = getPointOnUnitSphere(getNormalizedCoords(x, y));
      // Rotation beetween the click point and current point
      glm::fquat t = getGreatCircleRotation(m_rotation_mousepos_i, m_rotation_mousepos_c);
      m_rotation_angle = t.w; // Get angle from t
      m_rotation_axis = glm::vec3(t.x, t.y, t.z); // Get the axis from t
      m_camera_orientation = t * m_rotation_orientation_i; // Set camera orientation
    }
  }

  glm::mat4x4 SimpleViewer::getProjectionMatrix() {
    glm::mat4x4 result;
    // Init the result with identity matrix
    result = glm::mat4x4(1.0f);
    
    /** Here we set the perspective view frustum 
  Here I have choosen to set the left plane be the negative window aspect and
  the right plane to be the window aspect. As of the bottom and top I have
        choosen to use -1 as bottom and 1 as top so that the box dont scale to a rectangular shape 
    */
    glm::mat4x4 perspective = glm::frustum(-m_window_aspect, m_window_aspect, -1.0f, 1.0f, 1.0f, -1.0f);
    
    return result * perspective;
  }

  /** This returns the model view matrix (the matrix that describes how the objects should be
      rendered. Basicly only the camera moves so first we move the camera so it points on origo
      where the cube is and we move it a little bit backwards. Then apply the rotation to the 
      matrix.
  */
  glm::mat4x4 SimpleViewer::getModelViewMatrix() {
    glm::mat4x4 result;

    result = glm::mat4x4(1.0f);
    // Sets the camera in position to watch straight forward on the cube
    result = result * glm::translate(result, glm::vec3(0.0f, 0.0f, -m_camera_distance)); 
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

      glColor3f(0.4f, 0.4f, 0.4f);
      glBegin(GL_LINE_LOOP);
      for(size_t i=0; i<50; i++)
        glVertex2f(glm::cos(2.0f*M_PI*i/(float)50), glm::sin(2.0f*M_PI*i/(float)50));
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

  /** Method that returns normalized coords based on the 
      definitions in the oblig text. Basically we return the
      counterparts of x and y in a new point vector.
  */
  glm::vec2 SimpleViewer::getNormalizedCoords(int x, int y) {
    glm::vec2 normalized_coords = glm::vec2();
    float xf = (float) x;
    float yf = (float) y; 
    normalized_coords.x = (xf/m_window_width)-0.5f;
    normalized_coords.y = 0.5f-(yf/m_window_height);
    
    return normalized_coords; 
  }
  
  /** This method checks wheter the normalized point is on the unit sphere or not.
      We define r to be the length of the vector p.
      If r < 0.5 (which is the radius of unit sphere) then the point is within the 
      unit sphere else it is considered to be outside.
  */
  glm::vec3 SimpleViewer::getPointOnUnitSphere(glm::vec2 p) {
    float r = glm::sqrt((p.x * p.x) + (p.y * p.y));
    glm::vec3 point = glm::vec3();
    
    if(r < 0.5f) // Point is in sphere 
      point = glm::vec3(2.0f * p.x, 2.0f * p.y, glm::sqrt(1.0f - 4 * (r * r)));
    else 
      point = glm::vec3(p.x / r, p.y / r, 0);
    return point;
  }

  // This function computes the quaternion corresponding to the great circle rotation between two vectors
  glm::fquat SimpleViewer::getGreatCircleRotation(glm::vec3 a, glm::vec3 b) {
    glm::vec3 axis = glm::cross(a, b);
    float length = glm::length(axis);
    if(length > std::numeric_limits<float>::epsilon()) {
      axis = axis / length;

      float angle = glm::acos(glm::dot(a,b));
      float c = glm::cos(0.5f*angle);
      float s = glm::sin(0.5f*angle);
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