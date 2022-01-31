#ifndef RHOMBIC2_H
#define RHOMBIC2_H

#include <vector>
#include <set>
#include <string>
#include <stdexcept>
#include <algorithm>
#include <math.h>
#include <map>

struct coord
{
    int xi[4];
};

typedef std::vector<int> vint;
typedef std::vector<std::vector<int>> vvint;
typedef std::vector<std::pair<int, int>> vpint;

enum direction {xy, xz, yz, xyz};

int coordToIndex(coord c, int L);

coord indexToCoord(int i, int L);

int neigh(int v, int dir, int sign, int L);

int edgeIndex(int v, int dir, int sign, int L);

void addFace(int v, int f, const vint &dirs, const vint &signs, vvint &faceToVertices, vvint &faceToEdges, vvint &vertexToFaces, vvint &edgeToFaces, int L);

void buildFaces(vvint &faceToVertices, vvint &faceToEdges, vvint &vertexToFaces, vvint &edgeToFaces, int L);

//vvint listToBin(vvint& in, int L);

vvint buildVertexToEdges(int L);

vpint buildEdgeToVertices(int L);

int findFace(vint &vertices, vvint &vertexToFaces, vvint &faceToVertices);

//std::vector<std::vector<int>> buildZLogicals(int L);

void buildSliceTwo(vint &lowerQubitIndices, vint &middleQubitIndices, vint &upperQubitIndices, vint &syndIndices, vint &defectIndices, vint &bulkSweepVertices, vint &middleBoundarySweepVertices, vint &zLogical, vvint &vertexToFaces, vvint &faceToVertices, vvint &faceToEdges, vpint &edgeToVertices, int L);

vint moveFaces(vint &indexVector, vvint &vertexToFaces, vvint &faceToVertices, int L);

void moveIndices(vint &lowerQubitIndices, vint &middleQubitIndices, vint &upperQubitIndices, vint &syndIndices, vint &defectIndices, vint &bulkSweepVertices, vint &middleBoundarySweepVertices, vint &zLogical, vvint &vertexToFaces, vvint &faceToVertices, int L);

#endif
