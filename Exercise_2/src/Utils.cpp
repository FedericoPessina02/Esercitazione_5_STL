#include <iostream>
#include <sstream>
#include <fstream>
#include <cmath>
#include <Eigen/Eigen>
#include "PolygonalMesh.hpp"
#include "Utils.hpp"
using namespace std;
using namespace Eigen;


namespace Utils {

double get_edge_length(const double x1, const double y1, const double x2, const double y2) {
    return sqrt(pow(x1-x2, 2) + pow(y1-y2, 2));
}

double get_angle(Eigen::Vector2d a, Eigen::Vector2d b) {
    double dot_product = a.dot(b);
    return acos((dot_product)/(a.norm()*b.norm()));
}

}


namespace MeshLibrary {

bool importMesh(const string& path, PolygonalMesh& mesh, const bool verbose_log){
    cout << "Loading data from " << path << " ..." << endl << endl;
    if (!importCell0Ds(path+"/Cell0Ds.csv", mesh, verbose_log)) {return false;}
    if (!importCell1Ds(path+"/Cell1Ds.csv", mesh, verbose_log)) {return false;}
    if (!importCell2Ds(path+"/Cell2Ds.csv", mesh, verbose_log)) {return false;}
    cout << endl;
    return true;
}

bool importCell0Ds(const string& path, PolygonalMesh& mesh, const bool verbose_log){
    if (verbose_log) {cout << "Cell0Ds data" << endl;}
    ifstream file(path);
    if (file.fail()) {
        return false;
    }
    string header;
    getline(file,header);
    list<string> lines;
    string line;
    while(getline(file, line))
        lines.push_back(line);
    unsigned int n = lines.size();
    mesh.CoordinateCell0Ds.reserve(n);
    mesh.IdCell0Ds.reserve(n);
    mesh.MarkerCell0Ds.reserve(n);

    for(const string& line: lines) {
        istringstream s(line);
        string id, marker, coordinate_0, coordinate_1;
        Vector2d coordinates;
        getline(s, id, ';');
        getline(s, marker, ';');
        getline(s, coordinate_0, ';');
        getline(s, coordinate_1, ';');
        if (verbose_log) {cout << "  " <<  id << " " << marker << " " << coordinate_0 << " " << coordinate_1 << endl;}
        mesh.CoordinateCell0Ds.push_back(Vector2d (stod(coordinate_0), stod(coordinate_1)));
        mesh.IdCell0Ds.push_back(stoul(id));
        mesh.MarkerCell0Ds.push_back(stoul(marker));
        mesh.VerticesMarker.insert({stoul(marker), {stoul(id)}});
    }
    if (verbose_log) {cout << endl;}
    file.close();
    return true;
}

bool importCell1Ds(const string& path, PolygonalMesh& mesh, const bool verbose_log){
    if (verbose_log) {cout << "Cell1Ds data" << endl;}
    ifstream file(path);
    if (file.fail()) {
        return false;
    }
    string header;
    getline(file,header);
    list<string> lines;
    string line;
    while(getline(file, line))
        lines.push_back(line);
    unsigned int n = lines.size();
    mesh.VerticesCell1Ds.reserve(n);
    mesh.IdCell1Ds.reserve(n);
    mesh.MarkerCell1Ds.reserve(n);

    for(const string& line: lines) {
        istringstream s(line);
        string id, marker, edge_1, edge_2;
        getline(s, id, ';');
        getline(s, marker, ';');
        getline(s, edge_1, ';');
        getline(s, edge_2, ';');
        if (verbose_log) {cout << "  " << id << " " << marker << " " << edge_1 << " " << edge_2 << endl;}
        mesh.VerticesCell1Ds.push_back(array<unsigned, 2> {stoul(edge_1), stoul(edge_2)});
        mesh.IdCell1Ds.push_back(stoul(id));
        mesh.MarkerCell1Ds.push_back(stoul(marker));
        mesh.EdgesMarker.insert({stoul(marker), {stoul(id)}});
    }
    if (verbose_log) {cout << endl;}
    file.close();
    return true;
}

bool importCell2Ds(const string& path, PolygonalMesh& mesh, const bool verbose_log){
    if (verbose_log) {cout << "Cell2Ds data" << endl;}
    ifstream file(path);
    if (file.fail()) {
        return false;
    }
    string header;
    getline(file,header);
    list<string> lines;
    string line;
    while(getline(file, line))
        lines.push_back(line);
    unsigned int n = lines.size();
    mesh.IdCell2Ds.reserve(n);
    mesh.MarkerCell2Ds.reserve(n);
    mesh.EdgesCell2Ds.reserve(n);
    mesh.VerticesCell2Ds.reserve(n);

    for(const string& line: lines) {
        istringstream s(line);
        string id, marker;
        getline(s, id, ';');
        getline(s, marker, ';');
        mesh.IdCell2Ds.push_back(stoul(id));
        mesh.MarkerCell2Ds.push_back(stoul(marker));
        mesh.CellsMarker.insert({stoul(marker), {stoul(id)}});
        if (verbose_log) {cout << "  " <<  id << " " << marker << " ";}

        string num_vertices;
        getline(s, num_vertices, ';');
        vector<unsigned int> vertices_list;
        vertices_list.reserve(stoi(num_vertices));
        if (verbose_log) {cout << stoi(num_vertices) << " ";}
        for (int i = 0; i<stoi(num_vertices); i++) {
            string value;
            getline(s, value, ';');
            vertices_list.push_back(stoul(value));
            if (verbose_log) {cout << value << " ";}
        }
        mesh.VerticesCell2Ds.push_back(vertices_list);

        string num_edges;
        getline(s, num_edges, ';');
        vector<unsigned int> edges_list;
        edges_list.reserve(stoi(num_edges));
        if (verbose_log) {cout << stoi(num_edges) << " ";}
        for (int i = 0; i<stoi(num_edges); i++) {
            string value;
            getline(s, value, ';');
            edges_list.push_back(stoul(value));
            if (verbose_log) {cout << value << " ";}
        }
        mesh.EdgesCell2Ds.push_back(edges_list);
        if (verbose_log) {cout << endl;}
    }
    if (verbose_log) {cout << endl;}
    return true;
}

void testMesh(PolygonalMesh& mesh, const bool verbose_log) {
    bool final_result = true;
    cout << "Controlling loaded mesh quality..." << endl;
    cout << endl;
    final_result = check_edges(mesh, verbose_log);
    final_result = check_polygons(mesh, verbose_log);
    if (final_result) {cout << "All tests were passed" << endl;}
    else {cout << "There are some problems with the loaded mesh" << endl;}
    cout << endl;
}

bool check_edges(PolygonalMesh& mesh, const bool verbose_log) {
    bool result = true;
    if (verbose_log) {cout << "Testing edges:" << endl;}
    for(long long unsigned int i = 0; i<mesh.VerticesCell1Ds.size(); i++) {
        // controlla le lunghezze
        double length;
        length = Utils::get_edge_length(
            mesh.CoordinateCell0Ds[mesh.VerticesCell1Ds[i][0]](0),
            mesh.CoordinateCell0Ds[mesh.VerticesCell1Ds[i][0]](1),
            mesh.CoordinateCell0Ds[mesh.VerticesCell1Ds[i][1]](0),
            mesh.CoordinateCell0Ds[mesh.VerticesCell1Ds[i][1]](1)
            );
        if (length < numeric_limits<double>::epsilon()) {
            result = false;
            cout << "  (#)Cell1D id: " << i << " has length 0 (up to a tolerance)" << endl;
        }
        else {
            if (verbose_log) {cout << "  (-)Cell1D id: " << i << " has positive length (" << length << ")" << endl;}
        }

        // controlla che i marker siano correttamente assegnati
        unsigned int edge_marker = mesh.MarkerCell1Ds[i];
        unsigned int vert_1_marker = mesh.MarkerCell0Ds[mesh.VerticesCell1Ds[i][0]];
        unsigned int vert_2_marker = mesh.MarkerCell0Ds[mesh.VerticesCell1Ds[i][1]];
        if (edge_marker != 0 && vert_1_marker + vert_2_marker == 0) {
            cout << "  (#)Cell1D id: " << i << " is on the border but not the point(s)" << endl;
            result = false;
        }
        else if (edge_marker == 0 && vert_1_marker != 0 && vert_2_marker != 0) {
            cout << "  (#)Cell1D id: " << i << " is inside the area but edges are on the border" << endl;
            result = false;
        }
        else {
            if (verbose_log) {cout << "  (-)Cell1D id: " << i << " is of the right type according to its points" << endl;}
        }
    }
    if (verbose_log) {cout << endl;}
    return result;
}

bool check_polygons(PolygonalMesh& mesh, const bool verbose_log) {
    bool result = true;
    if (verbose_log) {cout << "Testing polygons:" << endl;}
    for(long long unsigned int i = 0; i<mesh.EdgesCell2Ds.size(); i++) {
        double cell_area = 0;
        bool check = true;
        vector<unsigned int> edges = mesh.EdgesCell2Ds[i];
        vector<array<unsigned int, 2>> checked_pairs;
        for(long long unsigned int j0 = 0; j0<edges.size(); j0++) {
            for(long long unsigned int j1 = 0; j1<edges.size(); j1++) {
                if (j0 == j1) {
                    // sto considerando lo stesso lato due volte
                    continue;
                }
                array<unsigned int, 2> edge_0 = mesh.VerticesCell1Ds[j0];
                array<unsigned int, 2> edge_1 = mesh.VerticesCell1Ds[j1];
                if (!(edge_0[0] == edge_1[0] || edge_0[0] == edge_1[1] || edge_0[1] == edge_1[0] || edge_0[1] == edge_1[1])) {
                    // i lati che sto considerando non sono contigui
                    continue;
                }

                bool already_checked = false;
                for (const std::array<unsigned int, 2>& pair: checked_pairs) {
                    if (pair[0] == j1 && pair[1] == j0) {
                        already_checked = true;
                    }
                }
                if (already_checked) {
                    continue;
                }

                checked_pairs.push_back(std::array<unsigned int, 2> {static_cast<unsigned>(j0), static_cast<unsigned>(j1)}); // memorizza la coppia così evito di rivederla al contrario
                Vector2d a = mesh.CoordinateCell0Ds[edge_0[0]] - mesh.CoordinateCell0Ds[edge_0[1]];
                Vector2d b = mesh.CoordinateCell0Ds[edge_1[0]] - mesh.CoordinateCell0Ds[edge_1[1]];
                if (abs(a(0)*b(1) - a(1)*b(0)) >= numeric_limits<double>::epsilon()) {
                    cell_area += a(0)*b(1) - a(1)*b(0);
                } else {
                    check = false;
                }
                double angle = Utils::get_angle(a, b);
                if (abs(angle) < numeric_limits<double>::epsilon()) {
                    // se due lati hanno angolo 0 tra di loro il triangolo elementare a loro associato è degenere
                    check = false;
                    result = false;
                }
            }
        }
        if (!check) {
            cout << "  (#)Cell2Ds id: " << i << " has an internal triangle of area 0 (up to tolerance)" << endl;
        } else {
            if (verbose_log) {cout << "  (-)Cell2Ds id: " << i << " is ok" << endl;}
        }
        if (cell_area == 0) {
            cout << " (#)Cell2Ds id: " << i << " has a null total area" << endl;
        } else {
            if (verbose_log) {cout << "  (-)Cell2Ds id: " << i << " has  a positive area (" << 0.5*abs(cell_area) << ")" << endl;}
        }
    }
    if (verbose_log) {cout << endl;}
    return result;
}

}
