#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <list>
#include <unordered_map>
#include <map>
#include <algorithm>
#include <queue>
#include <cmath>
#include "DCEL.hpp"

using namespace std;


// forward declarations; these functions are given below main()
void DemoDCEL();
void printDCEL(DCEL & D);

typedef std::unordered_map<int,Vertex*> vertexmap;
typedef std::map<vector<int>,vector<HalfEdge*>> edgemap;

/* 
  Example functions that you could implement. But you are 
  free to organise/modify the code however you want.
  After each function you should have a DCEL without invalid elements!
*/
// 0.

void importJSON(DCEL &D, const char *file_in) {
    vertexmap id2vertex;
    edgemap ver2halfedge;

}
// 1.
void importOBJ(DCEL & D, const char *file_in) {
    vertexmap id2vertex;
    edgemap ver2halfedge;
    string line;
    char c;
    unsigned int i, j, k;
    float x, y, z;
    string si, sj, sk;
    ifstream in;
    in.open(file_in);
    int n = 0;
    while (getline(in, line))                           // read whole line
    {
        if (line.find_first_of("vf") == std::string::npos) continue;     // skip pointless lines
        istringstream ss(line);                            // put line into a stream for input
        ss >> c;                                             // get first character
        switch (c) {
            case 'v':                                         // VERTICES
            {
                ++n;
                ss >> x >> y >> z;
                id2vertex[n] = D.createVertex(x, y, z);
                D.vermap()[id2vertex[n]] = n - 1;
                break;
            }
            case 'f':                                         // FACES
            {
                Face *f0 = D.createFace();
                ss >> si >> sj >> sk;
                i = std::stoi(si);
                j = std::stoi(sj);
                k = std::stoi(sk);
                HalfEdge *e0 = D.createHalfEdge();
                HalfEdge *e1 = D.createHalfEdge();
                HalfEdge *e2 = D.createHalfEdge();
                e0->origin = id2vertex[i];
                e0->destination = id2vertex[j];
                vector<int> l0;
                l0.push_back(i);
                l0.push_back(j);
                sort(l0.begin(),l0.end());
                ver2halfedge[l0].push_back(e0);
                e0->prev = e2;
                e0->next = e1;
                f0->exteriorEdge = e0;
                e0->incidentFace = f0;

                e1->origin = id2vertex[j];
                e1->destination = id2vertex[k];
                vector<int> l1;
                l1.push_back(j);
                l1.push_back(k);
                sort(l1.begin(),l1.end());
                ver2halfedge[l1].push_back(e1);
                e1->prev = e0;
                e1->next = e2;
                e1->incidentFace = f0;

                e2->origin = id2vertex[k];
                e2->destination = id2vertex[i];
                vector<int> l2;
                l2.push_back(k);
                l2.push_back(i);
                sort(l2.begin(),l2.end());
                ver2halfedge[l2].push_back(e2);
                e2->prev = e1;
                e2->next = e0;
                e2->incidentFace = f0;
                break;
            }
        }
    }
    in.close();

    edgemap::iterator iter;
    iter = ver2halfedge.begin();
    for (iter = ver2halfedge.begin(); iter != ver2halfedge.end(); iter++) {
        vector ver_list = iter->first;
        vector<HalfEdge*> edge_list = iter->second;
        if (edge_list.size() != 2) throw std::invalid_argument("Non-manifold!!!");
        HalfEdge* edge1 = edge_list[0];
        HalfEdge* edge2 = edge_list[1];
        edge1->twin = edge2;
        edge2->twin = edge1;
    }
    //printDCEL(D);
}

// 2.
void groupTriangles(DCEL & D) {
  // to do
}
// 3.
void orientMeshes(DCEL & D) {
  // to do
}
// 4.
void mergeCoPlanarFaces(DCEL & D) {
  // to do
}
// 5.
void exportCityJSON(DCEL & D, const char *file_out) {
  // to do
}


int main(int argc, const char * argv[]) {
  const char *file_in = "bk_soup.obj";
  const char *file_out = "bk.json";

  // Demonstrate how to use the DCEL to get you started (see function implementation below)
  // you can remove this from the final code
  DemoDCEL();

  // create an empty DCEL
  DCEL D;

  // 1. read the triangle soup from the OBJ input file and convert it to the DCEL,
  
  // 2. group the triangles into meshes,
  
  // 3. determine the correct orientation for each mesh and ensure all its triangles 
  //    are consistent with this correct orientation (ie. all the triangle normals 
  //    are pointing outwards).
  
  // 4. merge adjacent triangles that are co-planar into larger polygonal faces.
  
  // 5. write the meshes with their faces to a valid CityJSON output file.

  return 0;
}


void printDCEL(DCEL & D) {

  // Quick check if there is an invalid element
  auto element = D.findInValid();
  if ( element == nullptr ) {
    // Beware that a 'valid' DCEL here only means there are no dangling links and no elimated elements.
    // There could still be problems like links that point to the wrong element.
    std::cout << "DCEL is valid\n";
  } else {
    std::cout << "DCEL is NOT valid ---> ";
    std::cout << *element << "\n";
  }

  // iterate all elements of the DCEL and print the info for each element
  const auto & vertices = D.vertices();
  const auto & halfEdges = D.halfEdges();
  const auto & faces = D.faces();
  std::cout << "DCEL has:\n";
  std::cout << " " << vertices.size() << " vertices:\n";
  for ( const auto & v : vertices ) {
    std::cout << "  * " << *v << "\n";
  }
  std::cout << " " << halfEdges.size() << " half-edges:\n";
  for ( const auto & e : halfEdges ) {
    std::cout << "  * " << *e << "\n";
  }
  std::cout << " " << faces.size() << " faces:\n";
  for ( const auto & f : faces ) {
    std::cout << "  * " << *f << "\n";
  }

}


void DemoDCEL() {

  std::cout << "/// STEP 1 Creating empty DCEL...\n";
  DCEL D;
  printDCEL(D);

  /*

  v2 (0,1,0)
   o
   |\
   | \
   |  \
   o---o v1 (1,0,0)
  v0
  (0,0,0)

  We will construct the DCEL of a single triangle 
  in the plane z=0 (as shown above).

  This will require:
    3 vertices
    6 halfedges (2 for each edge)
    1 face

  */
  std::cout << "\n/// STEP 2 Adding triangle vertices...\n";
  Vertex* v0 = D.createVertex(0,0,0);
  Vertex* v1 = D.createVertex(1,0,0);
  Vertex* v2 = D.createVertex(0,1,0);
  printDCEL(D);

  std::cout << "\n/// STEP 3 Adding triangle half-edges...\n";
  HalfEdge* e0 = D.createHalfEdge();
  HalfEdge* e1 = D.createHalfEdge();
  HalfEdge* e2 = D.createHalfEdge();
  HalfEdge* e3 = D.createHalfEdge();
  HalfEdge* e4 = D.createHalfEdge();
  HalfEdge* e5 = D.createHalfEdge();
  printDCEL(D);

  std::cout << "\n/// STEP 4 Adding triangle face...\n";
  Face* f0 = D.createFace();
  printDCEL(D);

  std::cout << "\n/// STEP 5 Setting links...\n";
  e0->origin = v0;
  e0->destination = v1;
  e0->twin = e3;
  e0->next = e1;
  e0->prev = e2;
  e0->incidentFace = f0;

  e3->origin = v1;
  e3->destination = v0;
  e3->twin = e0;
  e3->next = e5;
  e3->prev = e4;

  /* 
  If a half-edge is incident to 'open space' (ie not an actual face with an exterior boundary), 
  we use the infiniteFace which is predifined in the DCEL class
  */
  e3->incidentFace = D.infiniteFace();

  e1->origin = v1;
  e1->destination = v2;
  e1->twin = e4;
  e1->next = e2;
  e1->prev = e0;
  e1->incidentFace = f0;

  e4->origin = v2;
  e4->destination = v1;
  e4->twin = e1;
  e4->next = e3;
  e4->prev = e5;
  e4->incidentFace = D.infiniteFace();

  e2->origin = v2;
  e2->destination = v0;
  e2->twin = e5;
  e2->next = e0;
  e2->prev = e1;
  e2->incidentFace = f0;

  e5->origin = v0;
  e5->destination = v2;
  e5->twin = e2;
  e5->next = e4;
  e5->prev = e3;
  e5->incidentFace = D.infiniteFace();

  f0->exteriorEdge = e0;
  printDCEL(D);


  std::cout << "\n/// STEP 6 Traversing exterior vertices of f0...\n";
  /* 
  if all is well in the DCEL, following a chain of half-edges (ie keep going to e.next)
  should lead us back the the half-edge where we started.
  */
  HalfEdge* e = f0->exteriorEdge;
  const HalfEdge* e_start = e;
  do {
    std::cout << " -> " << *e->origin << "\n";
    e = e->next;
  } while ( e_start!=e) ; // we stop the loop when e_start==e (ie. we are back where we started)
  
  
  std::cout << "\n/// STEP 7 eliminating v0...\n";
  v0->eliminate();
  printDCEL(D);
  
  /* 
  We just eliminated v0. At the same time we know there are elements that still 
  pointers to v0 (ie the edges e0, e2, e3, e5). This means we can NOT call D.cleanup()!
  If you do this anyways, the program may crash. 
  
  Eg. if you uncomment the following there could be a crash/stall of the program.
  */
  // D.cleanup(); // this will remove v0 from memory (because we just eliminated v0 and the cleanup() function simply removes all the eliminated elements)
  // std::cout << *v0 << "\n"; // we try to access that memory, but v0 is gone -> undefined behaviour 
  // std::cout << *e0->origin << "\n"; // this equivalent to the previous line (both point to the same memory address)


  std::cout << "\n/// STEP 8 eliminating all the remaining DCEL elements\n";
  for ( const auto & v : D.vertices() ) {
    v->eliminate();
  }
  for ( const auto & e : D.halfEdges() ) {
    e->eliminate();
  }
  for ( const auto & f : D.faces() ) {
    f->eliminate();
  }
  printDCEL(D);

  std::cout << "\n/// STEP 9 cleaning up the DCEL\n";
  D.cleanup();
  printDCEL(D);

}