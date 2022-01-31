#include <iostream>
#include "rhombic1.h"
#include "decoderRhombic1.h"
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
 
    vvint vertexToEdges = buildVertexToEdges(L);
    vpint edgeToVertices = buildEdgeToVertices(L);
    
    vvint faceToVertices;
    vvint faceToEdges;
    vvint vertexToFaces;
    vvint edgeToFaces; 
    buildFaces(faceToVertices, faceToEdges, vertexToFaces, edgeToFaces, L);

    vint lowerQubitIndices;
    vint middleQubitIndices;
    vint upperQubitIndices;
    vint syndIndices;
    vint defectIndices;
    vint bulkSweepVertices;
    vint middleBoundarySweepVertices;
    vint upperBoundarySweepVertices1;
    vint upperBoundarySweepVertices2;
    vint zLogical;

    buildSliceOne(lowerQubitIndices, middleQubitIndices, upperQubitIndices, syndIndices, defectIndices, bulkSweepVertices, middleBoundarySweepVertices, upperBoundarySweepVertices1, upperBoundarySweepVertices2, zLogical, vertexToFaces, faceToVertices, faceToEdges, edgeToVertices, L);
    
    int failure;
    double failures = 0;

    for (int i = 0; i < runs; i++)
    {
        failure = oneRun(L, p, q, faceToEdges, edgeToFaces, faceToVertices, vertexToFaces, vertexToEdges, edgeToVertices, lowerQubitIndices, middleQubitIndices, upperQubitIndices, syndIndices, defectIndices, bulkSweepVertices, middleBoundarySweepVertices, upperBoundarySweepVertices1, upperBoundarySweepVertices2, zLogical, i, cutoff, reduction);
        failures += failure;
    }
    
    std::cout << L << ',' << p << ',' << q << ',' << runs << ',' << failures << '\n'; 
    return 0;
}

