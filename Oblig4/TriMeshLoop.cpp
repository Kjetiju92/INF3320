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


	TriMesh* TriMesh::subdivideLoop() {
		printf("triangles: %d\n", m_triangles.size());
		printf("nodes: %d\n", m_nodes.size());
        printf("edges: %d\n", m_halfedges.size());
		vector<glm::vec3> points;
		vector<int> indices;

		//skip
		// Generate a new set of points and indices using the Loop subdivision scheme
		// unskip
        /*
		for (size_t i = 0; i < m_triangles.size(); i++) {
			printf("i: %d\n", i);
			points.push_back((m_triangles[i]->getNode(0)->m_pos_, m_triangles[i]->getNode(1)->m_pos_, m_triangles[i]->getNode(2)->m_pos_) / 3.0f);
		}

		for (size_t i = 0; i < m_nodes.size(); i++) {
			int n = 0;
			HalfEdge *edge = m_nodes[i].getLeadingHalfEdge();

			do {
				n++;
				edge = edge->getVtxRingNext();
			} while (edge != m_nodes[i].getLeadingHalfEdge());

			if (n != 6) {
				float beta = (4.0f - 2.0f * glm::cos((2.0f * 3.14f) / n)) / (9 * n);
				points.push_back((1.0f - n * beta) * m_nodes[i].m_pos_ + beta);
			} else {
				points.push_back(m_nodes[i].m_pos_);
			}
			//indices.push_back(m_nodes[i].m_ix_);
		}

		for (size_t i = 0; i < m_triangles.size(); i++) 
		{
            
		}*/

        int index_counter = 0;
        
        for (size_t i = 0; i < m_triangles.size(); i++) {
            glm::vec3 pos_1 = m_triangles[i]->getNode(0)->m_pos_;
            glm::vec3 pos_2 = m_triangles[i]->getNode(1)->m_pos_;
            glm::vec3 pos_3 = m_triangles[i]->getNode(2)->m_pos_;

            glm::vec3 new_point = (pos_1 + pos_2 + pos_3) / 3.0f;
            points.push_back(new_point);
            
            m_triangles[i]->m_ix_ = index_counter++;
        }

        for (size_t i = 0; i < m_nodes.size(); i++) {
            if (m_nodes[i].isBoundary()) {
                printf("Boundary\n");
            }
            int n = 0;
            HalfEdge *edge = m_nodes[i].getLeadingHalfEdge();

            do {
                n++;
                edge = edge->getVtxRingNext();
            } while(edge != m_nodes[i].getLeadingHalfEdge());
                        
            float beta = (4.0f - 2.0f * glm::cos((2.0f * 3.14f) / n)) / (9 * n);
            points.push_back((1.0f - n * beta) * m_nodes[i].m_pos_ + beta);
 
            m_nodes[i].m_ix_ = index_counter++;
        }
		
        
        for (size_t i = 0; i < m_halfedges.size(); i++) {
            int tri_1_index = m_halfedges[i]->getTriangle()->m_ix_;
            int tri_2_index = m_halfedges[i]->getTwin()->getTriangle()->m_ix_;
            int node_1_index = m_halfedges[i]->getSourceNode()->m_ix_;
            int node_2_index = m_halfedges[i]->getDestinationNode()->m_ix_;
            
            indices.push_back(tri_1_index);
            indices.push_back(tri_2_index);
            indices.push_back(node_1_index);

            indices.push_back(node_2_index);
            indices.push_back(tri_1_index);
            indices.push_back(tri_2_index);
        }

        printf("Size points: %d\nSize indices: %d\n", points.size(), indices.size());
		return new TriMesh(points, indices);
	}
}  // GfxUtil
