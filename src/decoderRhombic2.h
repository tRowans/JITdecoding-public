#ifndef DECODERRHOMBIC2_H
#define DECODERRHOMBIC2_H

#include "rhombic2.h"
#include "decoderGeneric.h"
#include "PerfectMatching.h"
#include <iostream>

int shortestPathLength(int v1, int v2, int L);

vint shortestPath(int v1, int v2, vint &syndIndices, vvint &vertexToEdges, int L);

void middleSweepRule(vint &qubits, vint &middleQubitIndices, vint &syndrome, vint &syndIndices, vint &bulkSweepVertices, vvint &faceToEdges, vvint &faceToVertices, vvint &vertexToFaces, int L);

void middleBoundarySweepRule(vint &qubits, vint &middleQubitIndices, vint &syndrome, vint &syndIndices, vint &middleBoundarySweepVertices, vvint &faceToEdges, vvint &faceToVertices, vvint &vertexToFaces, int L);

void upperSweepRule(vint &qubits, vint &upperQubitIndices, vint &syndrome, vint &syndIndices, vint &bulkSweepVertices, vvint &faceToEdges, vvint &faceToVertices, vvint &vertexToFaces, int L);

std::vector<int> distanceToClosestBoundary(int v, int L);

std::vector<int> shortestPathToB(int v, int L);

vpint mwpm(std::vector<int> &defects, std::map<std::pair<int, int>, int> &defectPairDistances, int L);

void updatePairing(std::map<std::pair<int, int>, int> &defectPairDistances, vint &defects, int L);

void joinPair(int v1, int v2, vint &syndIndices, vvint &vertexToEdges, vint &syndrome, int L);

vint pathToTop(int v, vint &syndIndices, vvint &vertexToEdges, int L);

void syndromeRepair(vint &syndrome, vint &syndIndices, vvint &vertexToEdges, vpint &edgeToVertices, std::map<std::pair<int, int>, int> &defectPairDistances, int L, int cutoff, int reduction);

int oneRun(int L, double p, double q, vvint &faceToEdges, vvint &edgeToFaces, vvint &faceToVertices, vvint &vertexToFaces, vvint &vertexToEdges, vpint &edgeToVertices, vint lowerQubitIndices, vint middleQubitIndices, vint upperQubitIndices, vint syndIndices, vint defectIndices, vint bulkSweepVertices, vint middleBoundarySweepVertices, vint zLogical, int run, int cutoff, int reduction);

#endif
