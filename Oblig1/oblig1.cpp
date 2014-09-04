/* $Id: oblig1_gasket.cpp, v1.0 2011/09/07$
 *
 * Author: Bartlomiej Siwek, <bartloms@ifi.uio.no>
 * 
 * Distributed under the GNU GPL.
 */

#include <cstdlib>
#include <cmath>
#include <iostream>
#include <vector>

#include <GL/glew.h>
#include <GL/freeglut.h>
#include <GL/gl.h>

#include <glm/glm.hpp>

#include "OpenGLError.hpp"

#define BUFFER_OFFSET(i) ((char *)NULL + (i))

// Vertex structure
struct Vertex { 
  Vertex(float x, float y)
      : position(x, y) {
  }
  
  Vertex(const glm::vec2 &p) 
      : position(p) {
  }
  
  glm::vec2 position;
};

// Global variables
static std::vector<Vertex> vertices;
static std::vector<unsigned int> indices;
static int level = 0;

// Vertex buffer object IDs
GLuint verticesVertexBufferObjectId;
GLuint indicesVertexBufferObjectId;

void sierpinskiGasket(unsigned int a, unsigned int b, unsigned int c, unsigned int level) {
  if(level == 0) {
    // Insert trinagle indices into indices array
    // ... insert your code here ...
    indices.push_back(a);
    indices.push_back(b);
    indices.push_back(c);
    // ... end of your code ... 
  } else {
    // Generate new vertices and call this function recursively
    // ... insert your code here ...
    vertices.push_back(Vertex((vertices[a].position + vertices[b].position) / 2.0f));
    vertices.push_back(Vertex((vertices[b].position + vertices[c].position) / 2.0f));
    vertices.push_back(Vertex((vertices[a].position + vertices[c].position) / 2.0f));
    
    unsigned int size = vertices.size();
    sierpinskiGasket(a, size-3, size-1, level-1);
    sierpinskiGasket(size-3, b, size-2, level-1);
    sierpinskiGasket(size-1, size-2, c, level-1);
    // ... end of your code ...
  }
}

void rebuildGasket() {
  // Clear the data arrays
  vertices.clear();
  indices.clear();

  // Insert basic vertices
  vertices.push_back(Vertex(-1.0f, -1.0f));
  vertices.push_back(Vertex( 1.0f, -1.0f));
  vertices.push_back(Vertex( 0.0f,  1.0f));
  
  // Start the recursion
  sierpinskiGasket(0, 1, 2, level);
}

void bindDataToBuffers() {
  // Bind VBOs and provide data to them
  // ... insert your code here ...
  glBindBuffer(GL_ARRAY_BUFFER, verticesVertexBufferObjectId);
  glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);
  
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indicesVertexBufferObjectId);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
  // ... end of your code ... 
  
  // Unbind the VBO's after we are done
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  
  CHECK_OPENGL;
}

void myInit()
{
  // Initialize the gasket
  rebuildGasket();
  
  // Generate VBO ids
  // ... insert your code here ...
  glGenBuffers(1, &verticesVertexBufferObjectId);
  glGenBuffers(1, &indicesVertexBufferObjectId);
  // ... end of your code ... 

  // Bind data to buffers
  bindDataToBuffers();
    
  // Initialize OpenGL specific part
  glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
  
  CHECK_OPENGL;
}

void myKeyboard(unsigned char key, int /* x */, int /* y */)
{
  switch(key) {
    case '+':
      level++;
      // Rebuild gasket and bind data to buffers
      // ... insert your code here ...
      rebuildGasket();
      bindDataToBuffers();
      // ... end of your code ...
      break;
    case '-':
      level = level > 0 ? level-1 : 0;
      // Rebuild gasket and bind data to buffers
      // ... insert your code here ...
      rebuildGasket();
      bindDataToBuffers();
      // ... end of your code ... 
      break;
  }
  
  glutPostRedisplay();
}

void myDisplay()
{
  // Clear the backgound
  glClear(GL_COLOR_BUFFER_BIT);
 
  // Bind VBO's and call a drawing function
  // ... insert your code here ...
  glBindBuffer(GL_ARRAY_BUFFER, verticesVertexBufferObjectId);
  
  glVertexPointer(2, GL_FLOAT, sizeof(Vertex), BUFFER_OFFSET(0));
  glEnableClientState(GL_VERTEX_ARRAY);
  
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indicesVertexBufferObjectId);

  glColor3f(1.0f, 1.0f, 1.0f);
  glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, (void*)0);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  // ... end of your code ... 
  
  // Switch the buffer
  glFlush();
  glutSwapBuffers();
  
  CHECK_OPENGL;
}

void myShutdown() {
  // Delete VBOs
  // ... insert your code here ...
  glDeleteBuffers(1, &verticesVertexBufferObjectId);
  glDeleteBuffers(1, &indicesVertexBufferObjectId);
  // ... end of your code ... 
}

int main(int argc, char **argv)
{
  // Initialization of GLUT
  glutInit(&argc, argv);
  glutInitContextVersion(3, 1);
  glutInitContextFlags(GLUT_DEBUG);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
  glutInitWindowSize(512, 512);
  glutCreateWindow( __FILE__ );

  // Init GLEW
  GLenum err = glewInit();
  if (GLEW_OK != err) {
    std::cerr << "Error: " << glewGetErrorString(err) << std::endl;
    return 1;
  } 

  if(!GLEW_VERSION_3_1) {
    std::cerr << "Driver does not support OpenGL 3.1" << std::endl;  
    return 1;
  }
  
  // Attach handlers
  glutDisplayFunc(myDisplay);
  glutKeyboardFunc(myKeyboard);
  
  // A nasty trick to get a shutdown handler
  atexit(myShutdown);
  
  // Application specific initialization
  myInit();
  
  // Run the GLUT main loop
  glutMainLoop();
  return 0;
}
