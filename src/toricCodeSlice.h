#ifndef TORICCODESLICE_H
#define TORICCODESLICE_H

#include <vector>
#include <set>
#include <random>
#include <stdexcept>
#include <algorithm>
#include <iostream>

struct coord
{
    int xi[3];
};

enum direction { x, y, z, };

typedef std::vector<int> vint;
typedef std::vector<std::vector<int>> vvint;
typedef std::vector<std::pair<int, int>> vpint;

int coordToIndex(coord c, int L);

coord indexToCoord(int i, int L);

int neigh(int v, int dir, int sign, int L);

int edgeIndex(int v, int dir, int sign, int L);

vvint buildFaceToEdges(int L);

//vvint listToBin(vvint& in, int L);

vvint buildEdgeToFaces(int L);

vvint buildVertexToEdges(int L);

vpint buildEdgeToVertices(int L);

vvint buildAdjacentFaces(vvint &faceToEdges, vpint &edgeToVertices, vvint &vertexToEdges, vvint &edgeToFaces, int L);

//void printIndex(vint &bin);

//vvint buildZLogicals(int L);

void buildIndexVectors (vint &lowerQubitIndices, vint &middleQubitIndices, vint &upperQubitIndices, vint &syndIndices, vint &defectIndices, vint &zLogical, vint &sweepVertices, vvint &vertexToEdges, vvint &faceToEdges, vpint &edgeToVertices, int L);

//void collapseExpand(vint &qubits, std::mt19937& engine, std::uniform_real_distribution<double>& dist, vint &qubitIndices);

void moveIndices(vint &lowerQubitIndices, vint &middleQubitIndices, vint &upperQubitIndices, vint &syndIndices, vint &defectIndices, vint &zLogical, vint &sweepVertices, int L);

//void removeStabilisers(vint &qubits, vvint &upperXStabIndices, vvint &faceToEdges);

#endif
