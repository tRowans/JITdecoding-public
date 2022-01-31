#include "rhombic2.h"

vint lowerQubitIndices;
vint middleQubitIndices;
vint upperQubitIndices;
vint syndIndices;
vint defectIndices;
vint bulkSweepVertices;
vint middleBoundarySweepVertices;
vint zLogical;

vvint faceToVertices;
vvint faceToEdges;
vvint vertexToFaces;
vvint edgeToFaces;

vvint vertexToEdges = buildVertexToEdges(6);
vpint edgeToVertices = buildEdgeToVertices(6);
