#pragma once

#include <iostream>
#include "PolygonalMesh.hpp"
using namespace std;


namespace Utils {

double get_edge_length(const double x1, const double y1, const double x2, const double y2);

double get_angle(Eigen::Vector2d a, Eigen::Vector2d b);

}


namespace MeshLibrary {

bool importMesh(const string& path, PolygonalMesh& mesh, const bool verbose_log);

bool importCell0Ds(const string& path, PolygonalMesh& mesh, const bool verbose_log);

bool importCell1Ds(const string& path, PolygonalMesh& mesh, const bool verbose_log);

bool importCell2Ds(const string& path, PolygonalMesh& mesh, const bool verbose_log);

void testMesh(PolygonalMesh& mesh, const bool verbose_log);

bool check_edges(PolygonalMesh& mesh, const bool verbose_log);

bool check_polygons(PolygonalMesh& mesh, const bool verbose_log);

}
