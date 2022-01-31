#ifndef DECODER_H
#define DECODER_H

#include "toricCodeSlice.h"
#include "decoderGeneric.h"
#include "PerfectMatching.h"
#include <map>
#include <iostream>

vint taxi(int v1, int v2, int L);

vvint prePath(int v1, int v2, int L);

vint shortestPath(int v1, int v2, int L, vint &syndIndices);

void sweepRule(vint &qubits, vint &layerQubitIndices, vint &syndrome, vint &syndIndices, vint &sweepVertices, vvint &faceToEdges, int L, std::mt19937& engine, std::uniform_real_distribution<double>& dist);

vint distanceToClosestBoundary(int v, int L);

vint shortestPathToB(int v, int L);

vpint mwpm(vint &defects, std::map<std::pair<int, int>, int> &defectPairDistances, int L);

void updatePairing(std::map<std::pair<int, int>, int> &defectPairDistances, vint &defects, vint &syndIndices, int L);

void joinPair(int v1, int v2, vint &syndIndices, vint &syndrome, int L);

vint pathToTop(int v, vint &syndIndices, int L);

void syndromeRepair(vint &syndrome, vint &syndIndices, std::map<std::pair<int, int>, int> &defectPairDistances, vpint &edgeToVertices, int L, int cutoff, int reduction);

int oneRun(int L, double p, double q, vvint &faceToEdges, vvint &edgeToFaces, vvint &vertexToEdges, vpint &edgeToVertices, vint lowerQubitIndices, vint middleQubitIndices, vint upperQubitIndices, vint syndIndices, vint defectIndices, vint zLogical, vint sweepVertices, int run, int cutoff, int reduction);

#endif
