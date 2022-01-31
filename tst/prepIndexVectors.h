#include "toricCodeSlice.h"

//Index vectors for various lattice sizes to be used in decoder-test.cpp
//Each of these functions has been tested individually in toricCodeSlice-test.cpp
//Maybe not best practise but saves building these every time and makes tests a lot cleaner
//

vvint faceToEdges6 = buildFaceToEdges(6);
vvint edgeToFaces6 = buildEdgeToFaces(6);
vvint vertexToEdges6 = buildVertexToEdges(6);
vpint edgeToVertices6 = buildEdgeToVertices(6);
//vvint adjacentFaces6 = buildAdjacentFaces(faceToEdges6, edgeToVertices6, vertexToEdges6, edgeToFaces6, 6);

vint lowerQubitIndices6;
vint middleQubitIndices6;
vint upperQubitIndices6;
vint syndIndices6;
vint defectIndices6;
vint zLogical6;
vint sweepVertices6;
