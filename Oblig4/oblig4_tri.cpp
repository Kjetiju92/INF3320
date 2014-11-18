/* $Id: oblig2.cpp, v1.1 2008/09/22$
 *
 * Author: Atgeirr F Rasmussen <atgeirr@sintef.no>
 *
 * Distributed under the GNU GPL.
 */

#include "GLUTWrapper.hpp"
#include "Oblig4App.hpp"

int main(int argc, char** argv) {
    std::string filename = "meshes/cube.msh";
    if (argc == 2) {
        //Hint: Use <program> <path-to-mesh> to load arbitrary mesh
        filename = std::string(argv[1]);
    }
    Oblig4App app(filename);
    return GLUTWrapper::run(argc, argv, app);
}
