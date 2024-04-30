#pragma once

#include <iostream>
#include <vector>
#include <Eigen/Eigen>
using namespace std;
using namespace Eigen;

struct PolygonalMesh {
    vector<unsigned int> IdCell0Ds;
    vector<unsigned int> MarkerCell0Ds;
    vector<Vector2d> CoordinateCell0Ds;

    vector<unsigned int> IdCell1Ds;
    vector<unsigned int> MarkerCell1Ds;
    vector<array<unsigned int, 2>> VerticesCell1Ds;

    vector<unsigned int> IdCell2Ds;
    vector<unsigned int> MarkerCell2Ds;
    vector<vector<unsigned int>> VerticesCell2Ds;
    vector<vector<unsigned int>> EdgesCell2Ds;

    map<unsigned int, list<unsigned int>> VerticesMarker;
    map<unsigned int, list<unsigned int>> EdgesMarker;
    map<unsigned int, list<unsigned int>> CellsMarker;
};
