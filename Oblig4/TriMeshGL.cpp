/* -*- mode: C++; tab-width:4; c-basic-offset: 4; indent-tabs-mode:nil -*-
 *
 * Time-stamp: <2005-07-13 12:01:05 dyken>
 *
 * (C) 2005 Christopher Dyken, <dyken@cma.uio.no>
 *
 * Distributed under the GNU GPL.
 */

#include "TriMesh.hpp"

#include <stdexcept>
#include <vector>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>

using std::string;
using std::vector;


namespace GfxUtil {
    void TriMesh::render() {  
        // Bind VBO's
        glBindBuffer(GL_ARRAY_BUFFER, m_vertices_vbo_id);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indices_vbo_id);
        
        // Setup the strides, offsets etc.
        glVertexPointer(3, GL_FLOAT, sizeof(Node), BUFFER_OFFSET(0));
        glNormalPointer(GL_FLOAT, sizeof(Node), BUFFER_OFFSET(12));
        
        // Enable the above arrays
        glEnableClientState(GL_VERTEX_ARRAY);
        glEnableClientState(GL_NORMAL_ARRAY);
        
        // Call the drawing function
        glDrawElements(GL_TRIANGLES, 3*m_triangles.size(), GL_UNSIGNED_INT, BUFFER_OFFSET(0));
        
        // Unbind VBOs
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        
        // Disable arrays
        glDisableClientState(GL_VERTEX_ARRAY);
        glDisableClientState(GL_NORMAL_ARRAY);
    }
    
    void TriMesh::prepareBufferObjects(const vector<int>& indices) { 
        glGenBuffers(1, &m_vertices_vbo_id);
        glGenBuffers(1, &m_indices_vbo_id);
        
        // Bind VBOs and provide data to them  
        glBindBuffer(GL_ARRAY_BUFFER, m_vertices_vbo_id);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indices_vbo_id);
        
        glBufferData(GL_ARRAY_BUFFER, m_nodes.size() * sizeof(Node), &m_nodes[0], GL_STATIC_DRAW);    
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
        
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }

    void TriMesh::computeNormals() {
        // skip
        // Calculate triangle normals
        // Calculate vertex normals by averaging triangle normals
        // unskip
        
        for (size_t i = 0; i < m_triangles.size(); i++) {
            Node *n_i = m_triangles[i]->getNode(0);
            Node *n_j = m_triangles[i]->getNode(1);
            Node *n_k = m_triangles[i]->getNode(2);
            
            m_triangles[i]->m_N_ = glm::cross(n_j->m_pos_ - n_i->m_pos_, n_k->m_pos_ - n_i->m_pos_) / (float)glm::cross(n_j->m_pos_ - n_i->m_pos_, n_k->m_pos_ - n_i->m_pos_).length();
        }

        for (size_t i = 0; i < m_nodes.size(); i++) {
            float divider = 1.0f;
            glm::vec3 normalSum;
            HalfEdge *edge = m_nodes[i].getLeadingHalfEdge();
            
            do {
                normalSum += edge->getTriangle()->m_N_;
                divider++;
                
                edge = edge->getVtxRingNext();
            } while (edge != m_nodes[i].getLeadingHalfEdge());
  
            m_nodes[i].m_N_ = glm::normalize(normalSum / divider);
        }
    }
}  // GfxUtil
