#include "gtest/gtest.h"
#include "toricCodeSlice.h"

TEST(coordToIndexTest, HandlesExpectedInput)
{
    coord c;
    c.xi[0] = 4;
    c.xi[1] = 4;
    c.xi[2] = 1;
    EXPECT_EQ(coordToIndex(c, 5), 49);
} 
TEST(coordToIndexTest, HandlesOutOfRangeInput)
{
    coord c;
    c.xi[0] = -1;
    c.xi[1] = 0;
    c.xi[2] = 0;
    EXPECT_THROW(coordToIndex(c, 5), std::invalid_argument);
    c.xi[0] = 5;
    EXPECT_THROW(coordToIndex(c, 5), std::invalid_argument);
}

//------------------------------------------------------------

TEST(indexToCoordTest, HandlesExpectedInput)
{
    coord c = indexToCoord(49, 5);
    EXPECT_EQ(c.xi[0], 4);
    EXPECT_EQ(c.xi[1], 4);
    EXPECT_EQ(c.xi[2], 1);
}
TEST(indexToCoordTest, HandlesOutOfRangeInput)
{
    EXPECT_THROW(indexToCoord(-1, 5), std::invalid_argument);
    EXPECT_THROW(indexToCoord(625, 5), std::invalid_argument);
}

//------------------------------------------------------------

TEST(neighTest, CorrectOutput)
{
    EXPECT_EQ(neigh(129, x, 1, 6), 130);
    EXPECT_EQ(neigh(129, y, 1, 6), 135);
    EXPECT_EQ(neigh(129, z, 1, 6), 165);
    EXPECT_EQ(neigh(129, x, -1, 6), 128);
    EXPECT_EQ(neigh(129, y, -1, 6), 123);
    EXPECT_EQ(neigh(129, z, -1, 6), 93);
}

//------------------------------------------------------------

TEST(edgeIndexTest, HandlesExpectedInput)
{
    EXPECT_EQ(edgeIndex(129, x, 1, 6), 387);
    EXPECT_EQ(edgeIndex(129, y, 1, 6), 388);
    EXPECT_EQ(edgeIndex(129, z, 1, 6), 389);
    EXPECT_EQ(edgeIndex(129, x, -1, 6), 384);
    EXPECT_EQ(edgeIndex(129, y, -1, 6), 370);
    EXPECT_EQ(edgeIndex(129, z, -1, 6), 281);
}
TEST(edgeIndexTest, HandlesInvalidInput)
{
    EXPECT_THROW(edgeIndex(0, 3, 1, 5), std::invalid_argument);
}

//------------------------------------------------------------

TEST(buildFaceToEdgesTest, CorrectOutput)
{   
    //Not worth checking the whole thing
    //Just check this for the three directions of face and check size is right
    //Same for other functions of this type
    vvint faceToEdges = buildFaceToEdges(6);
    vint edges1 = {387, 388, 391, 405};
    vint edges2 = {387, 389, 392, 495};
    vint edges3 = {388, 389, 407, 496};
    EXPECT_EQ(faceToEdges[387], edges1);
    EXPECT_EQ(faceToEdges[388], edges2);
    EXPECT_EQ(faceToEdges[389], edges3);
}

//------------------------------------------------------------

TEST(buildEdgeToFaces, CorrectOutput)
{
    vvint edgeToFaces = buildEdgeToFaces(6);
    vint faces1 = {387, 388, 369, 280};
    vint faces2 = {387, 389, 384, 281};
    vint faces3 = {389, 388, 371, 385};
    EXPECT_EQ(edgeToFaces[387], faces1);
    EXPECT_EQ(edgeToFaces[388], faces2);
    EXPECT_EQ(edgeToFaces[389], faces3);  
}

//------------------------------------------------------------

TEST(buildVertexToEdgesTest, CorrectOutput)
{
    vvint vertexToEdges = buildVertexToEdges(6);
    vint edges = {387, 388, 389, 384, 370, 281};
    EXPECT_EQ(vertexToEdges[129], edges);
}

//------------------------------------------------------------

TEST(buildEdgeToVerticesTest, CorrectOutput)
{
    vpint edgeToVertices = buildEdgeToVertices(6);
    std::pair<int,int> vertices1 = {129, 130};
    std::pair<int,int> vertices2 = {129, 135};
    std::pair<int,int> vertices3 = {129, 165};
    EXPECT_EQ(edgeToVertices[387], vertices1);
    EXPECT_EQ(edgeToVertices[388], vertices2);
    EXPECT_EQ(edgeToVertices[389], vertices3);
}

//------------------------------------------------------------

//NOT IN USE
/*
TEST(buildAdjacentFacesTest, CorrectOutput)
{
    vvint faceToEdges = buildFaceToEdges(2);
    vvint edgeToFaces = buildEdgeToFaces(2);
    vvint vertexToEdges = buildVertexToEdges(2);
    vpint edgeToVertices = buildEdgeToVertices(2);
    vvint adjacentFaces = buildAdjacentFaces(faceToEdges, edgeToVertices, vertexToEdges, edgeToFaces, 2);

    vint element0 = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 13, 14, 16, 17, 19, 20, 22, 23};
    EXPECT_EQ(adjacentFaces[0], element0);
    EXPECT_EQ(adjacentFaces.size(), 24);
}
*/

//------------------------------------------------------------

//NOT IN USE
/*
TEST(buildZLogicalsTest, CorrectOutput)
{
    vvint zLogicals = buildZLogicals(6);
    vint zLower = {68, 171, 173, 276, 278};
    vint zUpper = {176, 279, 281, 384, 386};
    EXPECT_EQ(zLogicals[0], zLower);
    EXPECT_EQ(zLogicals[1], zUpper);
}
*/

//------------------------------------------------------------

TEST(buildIndexVectorsTest, CorrectOutput)
{
    vint lowerQubitIndices;
    vint middleQubitIndices;
    vint upperQubitIndices;
    vint syndIndices;
    vint defectIndices;
    vint zLogical;
    vint sweepVertices;

    vvint vertexToEdges = buildVertexToEdges(6);  
    vvint faceToEdges = buildFaceToEdges(6);
    vpint edgeToVertices = buildEdgeToVertices(6);

    buildIndexVectors(lowerQubitIndices, middleQubitIndices, upperQubitIndices, syndIndices, defectIndices, zLogical, sweepVertices, vertexToEdges, faceToEdges, edgeToVertices, 6);
    
    vint lowerQubitIndicesExpected = {68, 173, 171, 172, 158, 278, 276, 277, 263, 261, 262, 
                                      248, 368, 366, 367, 353, 351, 458, 456};
    vint middleQubitIndicesExpected = {176, 281, 279, 280, 266, 386, 384, 385, 371, 369, 370, 
                                       356, 476, 474, 475, 461, 459, 566, 564};
    vint upperQubitIndicesExpected = {284, 389, 387, 388, 374, 494, 492, 493, 479, 477, 478,
                                      464, 584, 582, 583, 569, 567, 674, 672};
    vint syndIndicesExpected = {175, 176, 280, 281, 279, 265, 266, 385, 386, 384, 370, 371, 369, 
                                355, 475, 476, 474, 460, 565, 283, 284, 388, 389, 387, 373, 374,  
                                493, 494, 492, 478, 479, 477, 463, 583, 584, 582, 568, 673};
    vint defectIndicesExpected = {58, 93, 88, 128, 123, 158, 94, 129, 124, 164, 159, 194};
    vint zLogicalExpected = {284, 389, 387, 494, 492};
    vint sweepVerticesExpected = {58, 64, 88, 93, 94, 99, 100, 118, 123, 124, 128, 129, 130, 134, 
                                  135, 136, 153, 154, 158, 159, 160, 164, 165, 166, 170, 171, 188, 
                                  189, 190, 194, 195, 196, 200, 201, 206, 224, 225, 230, 231, 236, 
                                  260, 266};

    EXPECT_EQ(lowerQubitIndices, lowerQubitIndicesExpected);
    EXPECT_EQ(middleQubitIndices, middleQubitIndicesExpected);
    EXPECT_EQ(upperQubitIndices, upperQubitIndicesExpected);
    EXPECT_EQ(syndIndices, syndIndicesExpected);
    EXPECT_EQ(defectIndices, defectIndicesExpected);
    EXPECT_EQ(zLogical, zLogicalExpected);
    EXPECT_EQ(sweepVertices, sweepVerticesExpected);
}

//------------------------------------------------------------

//NOT IN USE
/*

//Not sure what the best way is to test something like this.
//For now just checking something changes
//(Small prob this fails anyway, but run again and it should pass)
TEST(collapseExpandTest, changesState)
{
    vint qubits(3*5*5*5, 0);
    vint qubitsCompare(3*5*5*5, 0);
    std::mt19937 engine(time(0));
    std::uniform_real_distribution<double> dist(0,1);
    vint upperQubitIndices= {111, 113, 112, 171, 173, 183, 184, 243};
    collapseExpand(qubits, engine, dist, upperQubitIndices);
    EXPECT_NE(qubits, qubitsCompare);
}
*/

//------------------------------------------------------------

TEST(moveIndicesTest, correctOutput)
{
    vint lowerQubitIndices = {68, 158, 173, 171, 172, 248, 263, 261, 262, 
                              278, 276, 277, 353, 351, 368, 366, 367, 458, 456};
    vint middleQubitIndices = {176, 266, 281, 279, 280, 356, 371, 369, 370, 
                               386, 384, 385, 461, 459, 476, 474, 475, 566, 564};
    vint upperQubitIndices = {284, 374, 389, 387, 388, 464, 479, 477, 478, 
                              494, 492, 493, 569, 567, 584, 582, 583, 674, 672};
    vint syndIndices = {175, 176, 265, 266, 280, 281, 279, 355, 370, 371, 369, 385, 386, 
                        384, 460, 475, 476, 474, 565, 283, 284, 373, 374, 388, 389, 387, 
                        463, 478, 479, 477, 493, 494, 492, 568, 583, 584, 582, 673};
    vint defectIndices = {58, 88, 93, 123, 128, 158, 94, 124, 129, 159, 164, 194};
    vint zLogical = {284, 387, 389, 492, 494};
    vint sweepVertices = {58, 64, 88, 93, 94, 99, 100, 118, 123, 124, 128, 129, 130, 134, 135, 
                          136, 153, 154, 158, 159, 160, 164, 165, 166, 170, 171, 188, 189, 190, 
                          194, 195, 196, 200, 201, 206, 224, 225, 230, 231, 236, 260, 266};
   
    vint lowerQubitIndicesMoved = {284, 374, 389, 387, 388, 464, 479, 477, 478, 
                                   494, 492, 493, 569, 567, 584, 582, 583, 674, 672};
    vint middleQubitIndicesMoved = {392, 482, 497, 495, 496, 572, 587, 585, 586, 
                                    602, 600, 601, 677, 675, 692, 690, 691, 782, 780};
    vint upperQubitIndicesMoved = {500, 590, 605, 603, 604, 680, 695, 693, 694, 
                                   710, 708, 709, 785, 783, 800, 798, 799, 890, 888};
    vint syndIndicesMoved = {391, 392, 481, 482, 496, 497, 495, 571, 586, 587, 585, 601, 602, 
                             600, 676, 691, 692, 690, 781, 499, 500, 589, 590, 604, 605, 603, 
                             679, 694, 695, 693, 709, 710, 708, 784, 799, 800, 798, 889};
    vint defectIndicesMoved = {130, 160, 165, 195, 200, 230, 166, 196, 201, 231, 236, 266};
    vint zLogicalMoved = {500, 603, 605, 708, 710};
    vint sweepVerticesMoved = {130, 136, 160, 165, 166, 171, 172, 190, 195, 196, 200, 201, 202, 
                               206, 207, 208, 225, 226, 230, 231, 232, 236, 237, 238, 242, 243, 
                               260, 261, 262, 266, 267, 268, 272, 273, 278, 296, 297, 302, 303, 
                               308, 332, 338};

    moveIndices(lowerQubitIndices, middleQubitIndices, upperQubitIndices, syndIndices, 
                defectIndices, zLogical, sweepVertices, 6);

    EXPECT_EQ(lowerQubitIndices, lowerQubitIndicesMoved);
    EXPECT_EQ(middleQubitIndices, middleQubitIndicesMoved);
    EXPECT_EQ(upperQubitIndices, upperQubitIndicesMoved);
    EXPECT_EQ(syndIndices, syndIndicesMoved);
    EXPECT_EQ(defectIndices, defectIndicesMoved);
    EXPECT_EQ(zLogical, zLogicalMoved);
    EXPECT_EQ(sweepVertices, sweepVerticesMoved);
}

//------------------------------------------------------------

//NOT IN USE
/*
TEST(removeStabilisersTest, removesStabiliser)
{
    vint qubits(3*6*6*6, 0);
    vint qubitsExpected(3*6*6*6);
    vvint upperXStabIndices = {{279,281,280}, {384, 386, 385}, {369, 371, 370}, {474, 476, 475}, {459, 461}, {564, 566}};
    vvint faceToEdges = buildFaceToEdges(6);

    qubits[279] = 1;
    qubits[280] = 1;
    qubits[281] = 1;
    qubits[459] = 1;
    qubits[461] = 1;

    removeStabilisers(qubits, upperXStabIndices, faceToEdges);

    EXPECT_EQ(qubits, qubitsExpected);
}
TEST(removeStabilisersTest, doesntRemoveNonStabiliser)
{
    vint qubits(3*6*6*6, 0);
    vint qubitsExpected(3*6*6*6);
    vvint upperXStabIndices = {{279,281,280}, {384, 386, 385}, {369, 371, 370}, {474, 476, 475}, {459, 461}, {564, 566}};
    vvint faceToEdges = buildFaceToEdges(6);

    qubits[279] = 1;
    qubits[384] = 1;
    qubits[385] = 1;

    qubitsExpected[279] = 1;
    qubitsExpected[384] = 1;
    qubitsExpected[385] = 1;

    removeStabilisers(qubits, upperXStabIndices, faceToEdges);

    EXPECT_EQ(qubits, qubitsExpected);
}
*/
