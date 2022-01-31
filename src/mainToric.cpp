#include <iostream>
#include <fstream>
#include "decoderToric.h"
#include <string>

int main(int argc, char *argv[])
{
    if (argc != 7)
    {
        std::cout << "Invalid number of arguments." << '\n';
        return 1;
    }

    int L = std::atoi(argv[1]);
    double p = std::atof(argv[2]);
    double q = std::atof(argv[3]);
    int runs = std::atoi(argv[4]);
    int cutoff = std::atoi(argv[5]);
    int reduction = std::atoi(argv[6]);

    vvint faceToEdges = buildFaceToEdges(L);
    vvint edgeToFaces = buildEdgeToFaces(L);
    vvint vertexToEdges = buildVertexToEdges(L);
    vpint edgeToVertices = buildEdgeToVertices(L);

    vint lowerQubitIndices;
    vint middleQubitIndices;
    vint upperQubitIndices;
    vint syndIndices;
    vint defectIndices;
    vint zLogical;
    vint sweepVertices;

    buildIndexVectors(lowerQubitIndices, middleQubitIndices, upperQubitIndices, syndIndices, defectIndices, zLogical, sweepVertices, vertexToEdges, faceToEdges, edgeToVertices, L);
    
    int failure;
    double failures = 0;

    for (int i = 0; i < runs; i++)
    {
        failure = oneRun(L, p, q, faceToEdges, edgeToFaces, vertexToEdges, edgeToVertices, lowerQubitIndices, middleQubitIndices, upperQubitIndices, syndIndices, defectIndices, zLogical, sweepVertices, i, cutoff, reduction);
        failures += failure;
    }
    
    std::cout << L << ',' << p << ',' << q << ',' << runs << ',' << failures << '\n'; 
    return 0;
}

