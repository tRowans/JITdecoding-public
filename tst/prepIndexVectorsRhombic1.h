#include "rhombic1.h"

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

vvint faceToVertices;
vvint faceToEdges;
vvint vertexToFaces;
vvint edgeToFaces;

vvint vertexToEdges = buildVertexToEdges(6);
vpint edgeToVertices = buildEdgeToVertices(6);
