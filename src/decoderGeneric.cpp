#include "decoderGeneric.h"

enum direction { xy, xz, yz, xyz};

void dataError(std::vector<int> &qubits, double p, std::mt19937& engine, std::uniform_real_distribution<double>& dist, std::vector<int>& qubitIndices)
{
    for (int i : qubitIndices)
    {
        if (dist(engine) < p)
        {
            qubits[i] = (qubits[i] + 1) % 2;
        }
    }
}

void measError(std::vector<int> &syndrome, double q, std::mt19937& engine, std::uniform_real_distribution<double>& dist, std::vector<int> &syndromeIndices)
{
    for (int i : syndromeIndices)
    {
        if (dist(engine) < q)
        {
            syndrome[i] = (syndrome[i] + 1) % 2;
        }
    }
}

void calcSynd(std::vector<int> &syndrome, std::vector<int> &qubits, std::vector<int> &lowerQubitIndices, std::vector<int> &middleQubitIndices, std::vector<int> &upperQubitIndices, std::vector<std::vector<int>> &faceToEdges, std::vector<int>& syndromeIndices)
{
    std::fill(syndrome.begin(), syndrome.end(), 0);
    for (std::vector<int> qubitIndices : {lowerQubitIndices, middleQubitIndices, upperQubitIndices})
    {
        for (int i : qubitIndices)
        {
            if (qubits[i] == 1)
            {
                std::vector<int> &edges = faceToEdges[i];
                for (auto edgeIndex : edges)
                {
                    if (std::find(syndromeIndices.begin(), syndromeIndices.end(), edgeIndex)
                            == syndromeIndices.end()) 
                    {
                        continue;
                    }

                    syndrome[edgeIndex] = (syndrome[edgeIndex] + 1) % 2;
                }
            }
        }
    }
}

void findDefects(std::vector<int> &syndrome, std::vector<std::vector<int>> &vertexToEdges, std::vector<std::pair<int, int>>& edgeToVertices, std::vector<int>& defects, std::vector<int>& defectIndices)
{
    defects.clear();
    for (auto v : defectIndices) 
    {
        std::vector<int>& edges = vertexToEdges[v];
        int count = 0;
        for (auto const& e : edges)
        {
            if (syndrome[e] == 1)
            {
                count += 1;
            }
        }
        if (count % 2 == 1)
        {
            defects.push_back(v);
        }
    }
}

bool checkCorrection(std::vector<int> &qubits, std::vector<int> &zLogical)
{
    int parity = 0;
    for (int i : zLogical)
    {
        if (qubits[i] == 1)
        {
            parity = (parity + 1) % 2;
        }
    }
    if (parity) return true;
    else return false;
}

int scalarProduct(std::vector<float> vec, int dir)
{
    //scalar product of a vector with one of the edge directions
    //but these directions can be negative
    int sign = 1;
    if (dir < 0) 
    {
        dir = -1*dir;
        sign = -1;
    }
    if (dir == 1) return sign*vec[0] + sign*vec[1] - sign*vec[2];   //{1,1,-1}
    else if (dir == 2) return sign*vec[0] - sign*vec[1] + sign*vec[2];  //{1,-1,1}
    else if (dir == 3) return -1*sign*vec[0] + sign*vec[1] + sign*vec[2];   //{-1,1,1}
    else if (dir == 4) return sign*vec[0] + sign*vec[1] + sign*vec[2];  //{1,1,1}
    else throw std::invalid_argument("Invalid direction");
}
