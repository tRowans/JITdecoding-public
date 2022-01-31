#ifndef DECODERGENERIC_H
#define DECODERGENERIC_H

#include <vector>
#include <stdexcept>
#include <algorithm>
#include <random>

void dataError(std::vector<int> &qubits, double p, std::mt19937& engine, std::uniform_real_distribution<double>& dist, std::vector<int>& qubitIndices);

void measError(std::vector<int> &syndrome, double q, std::mt19937& engine, std::uniform_real_distribution<double>& dist, std::vector<int> &syndromeIndices);

void calcSynd(std::vector<int> &syndrome, std::vector<int> &qubits, std::vector<int> &lowerQubitIndices, std::vector<int> &middleQubitIndices, std::vector<int> &upperQubitIndices, std::vector<std::vector<int>> &faceToEdges, std::vector<int>& syndromeIndices); 

void findDefects(std::vector<int> &syndrome, std::vector<std::vector<int>> &vertexToEdges, std::vector<std::pair<int, int>>& edgeToVertices, std::vector<int>& defects, std::vector<int>& defectIndices);

bool checkCorrection(std::vector<int> &qubits, std::vector<int> &zLogical);

int scalarProduct(std::vector<float> vec, int dir);

#endif
