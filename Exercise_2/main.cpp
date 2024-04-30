#include "Utils.hpp"
#include "PolygonalMesh.hpp"
using namespace MeshLibrary;
using namespace std;

int main()
{
    PolygonalMesh mesh;
    string path_to_folder = "./PolygonalMesh";
    if (!importMesh(path_to_folder, mesh, true)){
        return 1;
    }
    testMesh(mesh, true);
    return 0;
}

