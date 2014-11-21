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
#include <glm/glm.hpp>

using std::string;
using std::vector;

namespace GfxUtil {
    TriMesh* TriMesh::subdivideSqrt3(void) {
        vector<glm::vec3> points;
        vector<int> indices;
        
        //skip
        // Generate a new set of points and indices using the Loop subdivision scheme
        // unskip
        printf("Hello\n");
        int index_counter = 0;
        size_t i;
        for (i = 0; i < m_triangles.size(); i++) {
            glm::vec3 pos_1 = m_triangles[i]->getNode(0)->m_pos_;
            glm::vec3 pos_2 = m_triangles[i]->getNode(1)->m_pos_;
            glm::vec3 pos_3 = m_triangles[i]->getNode(2)->m_pos_;
            
            glm::vec3 new_point = (pos_1 + pos_2 + pos_3) / 3.0f;
            points.push_back(new_point);
            
            m_triangles[i]->m_ix_ = index_counter++;
            printf("m_ix_ %d : %d\n", i, m_triangles[i]->m_ix_);
        }

        for (size_t i = 0; i < m_halfedges.size(); i++) {
            if (m_halfedges[i]->isBoundary()) {
                points.push_back((m_halfedges[i]->getSourceNode()->m_pos_ + m_halfedges[i]->getDestinationNode()->m_pos_) / 2.0f);
                m_halfedges[i]->m_ix_ = index_counter++;
            }
        }

        bool isRegular = false;
        for (size_t i = 0; i < m_nodes.size(); i++) {
            if (isRegular) {
                isRegular = false;
                continue;
            }
            if (m_nodes[i].isBoundary()) {
                glm::vec3 pos1;
                HalfEdge *edge = m_nodes[i].getLeadingHalfEdge();

                do {
                    if (edge->getVtxRingNext() == NULL) 
                        pos1 = edge->getDestinationNode()->m_pos_;
                    edge = edge->getVtxRingNext();
                } while (edge != NULL);

                points.push_back((1.0f/8.0f) * pos1 + (3.0f/4.0f) * m_nodes[i].m_pos_ + (1.0f/8.0f) * m_nodes[i].getLeadingHalfEdge()->getDestinationNode()->m_pos_);
                m_nodes[i].m_ix_ = index_counter++;
            } else {
                int n = 0;
                HalfEdge *edge = m_nodes[i].getLeadingHalfEdge();
                glm::vec3 pos_c = glm::vec3(0, 0, 0);
                printf("Hello\n");
                do {
                    n++;
                    pos_c += (edge->getDestinationNode()->m_pos_ == m_nodes[i].m_pos_) ? edge->getSourceNode()->m_pos_ : edge->getDestinationNode()->m_pos_;
                    edge = edge->getVtxRingPrev();
                } while(edge != m_nodes[i].getLeadingHalfEdge() && edge != NULL);
                
                if (n != 6) {
                    //glm::vec3 pos = (m_nodes[i].m_pos_ - (m_nodes[i+1].m_pos_ - m_nodes[i].m_pos_)) / (2.0f * m_nodes[i].m_pos_);
                    //                    points.push_back((1.0f/4.0f) * m_nodes[i].m_pos_ + (3.0f/4.0f) * pos);
                    points.push_back((1.0f/2.0f) * (m_nodes[i].m_pos_ + m_nodes[i].getLeadingHalfEdge()->getVtxRingPrev()->getDestinationNode()->m_pos_));
                    m_nodes[i].m_ix_ = index_counter++;
                } else {
                    float beta = (4.0f - 2.0f * glm::cos((2.0f * 3.14f) / n)) / (9 * n);
                    points.push_back((1.0f - (float)n * beta) * m_nodes[i].m_pos_ + beta * pos_c);
                    m_nodes[i].m_ix_ = index_counter++;
                    printf("m_ixlol_ %d : %d\n", i, m_nodes[i].m_ix_);
            
                }
            }
        }
        
        for (size_t i = 0; i < m_halfedges.size(); i++) {

            if (m_halfedges[i]->getSourceNode()->isBoundary() || m_halfedges[i]->getDestinationNode()->isBoundary()) {
                int index_1 = m_halfedges[i]->getTriangle()->m_ix_;
                int index_2 = m_halfedges[i]->getSourceNode()->m_ix_;
                int index_3 = m_halfedges[i]->getDestinationNode()->m_ix_;
                int index_4 = m_halfedges[i]->m_ix_;

                indices.push_back(index_2);
                indices.push_back(index_1);
                indices.push_back(index_4);
            } else {
            int tri_1_index = m_halfedges[i]->getTriangle()->m_ix_;
            int tri_2_index = m_halfedges[i]->getTwin()->getTriangle()->m_ix_;

            if (!tri_2_index)
                printf("FAIL!\n");
            int node_1_index = m_halfedges[i]->getSourceNode()->m_ix_;
            int node_2_index = m_halfedges[i]->getDestinationNode()->m_ix_;
            
            printf("------ %d --------\n", i);

            /*
            indices.push_back(node_1_index); 
            indices.push_back(tri_1_index);
            indices.push_back(tri_2_index);indices.push_back(node_2_index);indices.push_back(tri_1_index);
            indices.push_back(tri_2_index);
            */
            indices.push_back(tri_2_index); indices.push_back(tri_1_index); indices.push_back(node_1_index); 
            
            }

        }
        


        return new TriMesh(points, indices);
    }
}  // GfxUtil
