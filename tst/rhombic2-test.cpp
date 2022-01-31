#include "gtest/gtest.h"
#include "rhombic2.h"

TEST(coordToIndexTest, HandlesCorrectInput)
{
    coord c0;
    coord c1;
    coord c2;

    c0.xi[0] = 4;
    c0.xi[1] = 4;
    c0.xi[2] = 1;
    c0.xi[3] = 0;

    c1.xi[0] = 4;
    c1.xi[1] = 4;
    c1.xi[2] = 1;
    c1.xi[3] = 1;

    c2.xi[0] = 4;
    c2.xi[1] = 15;
    c2.xi[2] = 0;
    c2.xi[3] = 0;
    
    EXPECT_EQ(coordToIndex(c0, 6), 208);
    EXPECT_EQ(coordToIndex(c1, 6), 1288);
    EXPECT_EQ(coordToIndex(c2, 6), 94);
} 
TEST(coordToIndexTest, HandlesOutOfRangeInput)
{
    coord c0;
    coord c1;
    coord c2;
    coord c3;

    c0.xi[0] = -1;
    c0.xi[1] = 0;
    c0.xi[2] = 0;
    c0.xi[3] = 0;
    
    c1.xi[0] = 0;
    c1.xi[1] = 50;
    c1.xi[2] = 0;
    c1.xi[3] = 0;

    c2.xi[0] = 6;
    c2.xi[1] = 0;
    c2.xi[2] = 0;
    c2.xi[3] = 0;

    c3.xi[0] = 0;
    c3.xi[1] = 0;
    c3.xi[2] = 0;
    c3.xi[3] = 3;

    EXPECT_THROW(coordToIndex(c0, 6), std::invalid_argument);
    EXPECT_THROW(coordToIndex(c1, 6), std::invalid_argument);
    EXPECT_THROW(coordToIndex(c2, 6), std::invalid_argument);
    EXPECT_THROW(coordToIndex(c3, 6), std::invalid_argument);
}

//------------------------------------------------------------

TEST(indexToCoordTest, HandlesExpectedInput)
{
    coord c0 = indexToCoord(208, 6);
    coord c1 = indexToCoord(1288, 6);
    coord c2 = indexToCoord(94, 6);

    EXPECT_EQ(c0.xi[0], 4);
    EXPECT_EQ(c0.xi[1], 4);
    EXPECT_EQ(c0.xi[2], 1);
    EXPECT_EQ(c0.xi[3], 0);

    EXPECT_EQ(c1.xi[0], 4);
    EXPECT_EQ(c1.xi[1], 4);
    EXPECT_EQ(c1.xi[2], 1);
    EXPECT_EQ(c1.xi[3], 1);

    EXPECT_EQ(c2.xi[0], 4);
    EXPECT_EQ(c2.xi[1], 15);
    EXPECT_EQ(c2.xi[2], 0);
    EXPECT_EQ(c2.xi[3], 0);
}
TEST(indexToCoordTest, HandlesOutOfRangeInput)
{
    EXPECT_THROW(indexToCoord(-1, 6), std::invalid_argument);
    EXPECT_THROW(indexToCoord(2160, 6), std::invalid_argument);
}

//------------------------------------------------------------

TEST(neighTest, CorrectOutputW0)
{
    //v=188 is (2,1,1,0)
    EXPECT_EQ(neigh(188, xy, 1, 6), 1088);
    EXPECT_EQ(neigh(188, xz, 1, 6), 1262);
    EXPECT_EQ(neigh(188, yz, 1, 6), 1267);
    EXPECT_EQ(neigh(188, xyz, 1, 6), 1268);
    EXPECT_EQ(neigh(188, xy, -1, 6), 1261);
    EXPECT_EQ(neigh(188, xz, -1, 6), 1087);
    EXPECT_EQ(neigh(188, yz, -1, 6), 1082);
    EXPECT_EQ(neigh(188, xyz, -1, 6), 1081);
}
TEST(neighTest, CorrectOutputW1)
{
    //v=1268 is (2,1,1,1)
    EXPECT_EQ(neigh(1268, xy, 1, 6), 195);
    EXPECT_EQ(neigh(1268, xz, 1, 6), 369);
    EXPECT_EQ(neigh(1268, yz, 1, 6), 374);
    EXPECT_EQ(neigh(1268, xyz, 1, 6), 375);
    EXPECT_EQ(neigh(1268, xy, -1, 6), 368);
    EXPECT_EQ(neigh(1268, xz, -1, 6), 194);
    EXPECT_EQ(neigh(1268, yz, -1, 6), 189);
    EXPECT_EQ(neigh(1268, xyz, -1, 6), 188);
}
TEST(neighTest, xGreaterThanL)
{
    //v=196 is (10,1,1,0)
    EXPECT_EQ(neigh(196, xy, 1, 6), 1096);
    EXPECT_EQ(neigh(196, xz, 1, 6), 1270);
    EXPECT_EQ(neigh(196, yz, 1, 6), 1275);
    EXPECT_EQ(neigh(196, xyz, 1, 6), 1276);
    EXPECT_EQ(neigh(196, xy, -1, 6), 1269);
    EXPECT_EQ(neigh(196, xz, -1, 6), 1095);
    EXPECT_EQ(neigh(196, yz, -1, 6), 1090);
    EXPECT_EQ(neigh(196, xyz, -1, 6), 1089);

    //v=1276 is (10,1,1,1)
    EXPECT_EQ(neigh(1276, xy, 1, 6), 203);
    EXPECT_EQ(neigh(1276, xz, 1, 6), 377);
    EXPECT_EQ(neigh(1276, yz, 1, 6), 382);
    EXPECT_EQ(neigh(1276, xyz, 1, 6), 383);
    EXPECT_EQ(neigh(1276, xy, -1, 6), 376);
    EXPECT_EQ(neigh(1276, xz, -1, 6), 202);
    EXPECT_EQ(neigh(1276, yz, -1, 6), 197);
    EXPECT_EQ(neigh(1276, xyz, -1, 6), 196);
}
TEST(neighTest, HandlesIncorrectArugments)
{
    EXPECT_THROW(neigh(188, -1, 1, 6), std::invalid_argument);
    EXPECT_THROW(neigh(188, 4, 1, 6), std::invalid_argument);
    EXPECT_THROW(neigh(188, 0, 0, 6), std::invalid_argument);
}

//------------------------------------------------------------

TEST(edgeIndexTest, CorrectOutput)
{
    EXPECT_EQ(edgeIndex(188, xy, 1, 6), 752);
    EXPECT_EQ(edgeIndex(188, xz, 1, 6), 753);
    EXPECT_EQ(edgeIndex(188, yz, 1, 6), 754);
    EXPECT_EQ(edgeIndex(188, xyz, 1, 6), 755);
    EXPECT_EQ(edgeIndex(188, xy, -1, 6), 5044);
    EXPECT_EQ(edgeIndex(188, xz, -1, 6), 4349);
    EXPECT_EQ(edgeIndex(188, yz, -1, 6), 4330);
    EXPECT_EQ(edgeIndex(188, xyz, -1, 6), 4327);
}
//Incorrect inputs will be detected by neigh inside the function 
//so don't need to test for them again here

//------------------------------------------------------------

TEST(addFaceTest, CorrectOutput)
{
    vvint faceToVertices;
    vvint faceToEdges;
    vvint vertexToFaces;
    vvint edgeToFaces;

    faceToVertices.assign(3 * 5 * 6 * 6 * 6, {});
    faceToEdges.assign(3 * 5 * 6 * 6 * 6,  {});
    vertexToFaces.assign(2 * 5 * 6 * 6 * 6, {});
    edgeToFaces.assign(8 * 5 * 6 * 6 * 6, {});

    addFace(188, 0, {xyz, xz, xz, xyz}, {1, 1, 1, 1}, faceToVertices, faceToEdges, vertexToFaces, edgeToFaces, 6);
    addFace(188, 1, {xy, xz, xz, xy}, {1, -1, -1, 1}, faceToVertices, faceToEdges, vertexToFaces, edgeToFaces, 6);

    vint vertices1 = {188, 369, 1262, 1268};
    vint edges1 = {753, 755, 5051, 5073};
    vint vertices2 = {14, 188, 1087, 1088};
    vint edges2 = {57, 752, 4348, 4349};
    vint faces1 = {0, 1};
    vint faces2 = {0};
    vint faces3 = {1};

    EXPECT_EQ(faceToVertices[0], vertices1);
    EXPECT_EQ(faceToEdges[0], edges1);
    EXPECT_EQ(faceToVertices[1], vertices2);
    EXPECT_EQ(faceToEdges[1], edges2);
    EXPECT_EQ(vertexToFaces[188], faces1);
    EXPECT_EQ(vertexToFaces[369], faces2);
    EXPECT_EQ(vertexToFaces[14], faces3);
    EXPECT_EQ(edgeToFaces[753], faces2);
    EXPECT_EQ(edgeToFaces[752], faces3);
}

//------------------------------------------------------------

TEST(buildFacesTest, CorrectOutput)
{   
    vvint faceToVertices;
    vvint faceToEdges;
    vvint vertexToFaces;
    vvint edgeToFaces;

    buildFaces(faceToVertices, faceToEdges, vertexToFaces, edgeToFaces, 6);

    vint vertices1 = {0, 181, 1080, 1254};
    vint edges1 = {1, 3, 4321, 5019};

    EXPECT_EQ(faceToVertices[1], vertices1);
    EXPECT_EQ(faceToEdges[1], edges1);
    EXPECT_EQ(vertexToFaces[0][1], 1);
    EXPECT_EQ(vertexToFaces[181][0], 1);
    EXPECT_EQ(vertexToFaces[1080][1], 1);
    EXPECT_EQ(vertexToFaces[1254][0], 1);
    EXPECT_EQ(edgeToFaces[3][1], 1);
    EXPECT_EQ(edgeToFaces[1][0], 1);
    EXPECT_EQ(edgeToFaces[4321][0], 1);
    EXPECT_EQ(edgeToFaces[5019][0], 1);
}

//------------------------------------------------------------
    
TEST(buildVertexToEdgesTest, CorrectOutput)
{
    vvint vertexToEdges = buildVertexToEdges(6);
    
    vint edges1 = {752, 753, 754, 755, 4327, 4330, 4349, 5044};
    vint edges2 = {};
    vint edges3 = {755, 5072, 5073, 5074};
    vint edges4 = {754, 773, 1468, 5071};
    
    EXPECT_EQ(vertexToEdges[188], edges1);
    EXPECT_EQ(vertexToEdges[187], edges2);
    EXPECT_EQ(vertexToEdges[1268], edges3);
    EXPECT_EQ(vertexToEdges[1267], edges4);
}

//------------------------------------------------------------

TEST(buildEdgeToVerticesTest, CorrectOutput)
{
    vpint edgeToVertices = buildEdgeToVertices(6);

    std::pair<int, int> vertices1 = {188, 1088};
    std::pair<int, int> vertices2 = {188, 1262};
    std::pair<int, int> vertices3 = {188, 1267};
    std::pair<int, int> vertices4 = {188, 1268};
    
    EXPECT_EQ(edgeToVertices[752], vertices1);
    EXPECT_EQ(edgeToVertices[753], vertices2);
    EXPECT_EQ(edgeToVertices[754], vertices3);
    EXPECT_EQ(edgeToVertices[755], vertices4);
}

//------------------------------------------------------------

TEST(findFaceTest, CorrectOutput)
{
    vvint faceToVertices;
    vvint faceToEdges;
    vvint vertexToFaces;
    vvint edgeToFaces;

    buildFaces(faceToVertices, faceToEdges, vertexToFaces, edgeToFaces, 6);
    vint vertices = {0, 181, 1080, 1254};
    EXPECT_EQ(findFace(vertices, vertexToFaces, faceToVertices), 1);
}

//------------------------------------------------------------

TEST(buildSliceTwoTest, CorrectOutput)
{
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
    buildFaces(faceToVertices, faceToEdges, vertexToFaces, edgeToFaces, 6);

    vvint vertexToEdges = buildVertexToEdges(6);
    vpint edgeToVertices = buildEdgeToVertices(6);

    buildSliceTwo(lowerQubitIndices, middleQubitIndices, upperQubitIndices, syndIndices, defectIndices, bulkSweepVertices, middleBoundarySweepVertices, zLogical, vertexToFaces, faceToVertices, faceToEdges, edgeToVertices, 6);

    vint expectedLowerQubitIndices = {636, 637, 618, 619, 1158, 1159, 1160, 606, 1140, 1141,
                                 1142, 1680, 1681, 1682, 1128, 1662, 1663, 1664, 1650};
    vint expectedMiddleQubitIndices = {1197, 1179, 1199, 1198, 1719, 1167, 1181, 1180, 1701, 
                                  1721, 1720, 2241, 1689, 1703, 1702, 2223, 2242, 2211, 
                                  2224};
    vint expectedUpperQubitIndices = {672, 673, 654, 655, 1194, 1195, 1196, 642, 1176, 1177,
                                      1178, 1716, 1717, 1718, 1164, 1698, 1699, 1700, 1686};
    vint expectedSyndIndices = {897, 899, 877, 879, 5168, 878, 5148, 858, 2984, 6562, 
                           6587, 2964, 6542, 6567, 6561, 6541, 1554, 1572, 1573,
                           1574, 1575, 1592, 1593, 1595, 2250, 2268, 2269, 2270, 2271, 2288, 
                           2289, 2291, 5149, 5150, 5169, 5170, 5175, 5195, 5844, 5845, 5846, 
                           5864, 5865, 5866, 5871, 5891, 6540, 6560};
    vint expectedDefectIndices = {1292, 1287, 741, 219, 393, 567, 1293, 1298, 1461, 1466, 1467, 
                            1472, 1635, 1640, 1641, 1646};
    vint expectedBulkSweepVertices = {214, 219, 224, 388, 393, 398, 562, 567, 572, 1287, 1292,
                                 1297, 1461, 1466, 1471, 1635, 1640, 1645};
    vint expectedMiddleBoundarySweepVertices = {736, 741, 746};
    vint expectedZLogical = {672, 1194, 1716};
    
    EXPECT_EQ(lowerQubitIndices, expectedLowerQubitIndices);
    EXPECT_EQ(middleQubitIndices, expectedMiddleQubitIndices);
    EXPECT_EQ(upperQubitIndices, expectedUpperQubitIndices);
    EXPECT_EQ(syndIndices, expectedSyndIndices);
    EXPECT_EQ(defectIndices, expectedDefectIndices);
    EXPECT_EQ(bulkSweepVertices, expectedBulkSweepVertices);
    EXPECT_EQ(middleBoundarySweepVertices, expectedMiddleBoundarySweepVertices);
    EXPECT_EQ(zLogical, expectedZLogical);
}

//------------------------------------------------------------

TEST(moveFacesTest, CorrectOutput)
{
    vvint faceToVertices;
    vvint faceToEdges;
    vvint vertexToFaces;
    vvint edgeToFaces;
    buildFaces(faceToVertices, faceToEdges, vertexToFaces, edgeToFaces, 6);
    vint lowerQubitIndices = {636, 637, 618, 619, 1158, 1159, 1160, 606, 1140, 1141,
                                 1142, 1680, 1681, 1682, 1128, 1662, 1663, 1664, 1650};
    vint upperQubitIndices = {672, 673, 654, 655, 1194, 1195, 1196, 642, 1176, 1177,
                                      1178, 1716, 1717, 1718, 1164, 1698, 1699, 1700, 1686};

    EXPECT_EQ(moveFaces(lowerQubitIndices, vertexToFaces, faceToVertices, 6), upperQubitIndices);
}

//------------------------------------------------------------

TEST(moveIndicesTest, CorrectOutput)
{   
    vvint faceToVertices;
    vvint faceToEdges;
    vvint vertexToFaces;
    vvint edgeToFaces;
    buildFaces(faceToVertices, faceToEdges, vertexToFaces, edgeToFaces, 6);
    vint lowerQubitIndices = {636, 637, 618, 619, 1158, 1159, 1160, 606, 1140, 1141,
                                 1142, 1680, 1681, 1682, 1128, 1662, 1663, 1664, 1650};
    vint middleQubitIndices = {1197, 1179, 1199, 1198, 1719, 1167, 1181, 1180, 1701, 
                                 1721, 1720, 2241, 1689, 1703, 1702, 2223, 2242, 2211, 
                                 2224};
    vint upperQubitIndices = {672, 673, 654, 655, 1194, 1195, 1196, 642, 1176, 1177,
                                 1178, 1716, 1717, 1718, 1164, 1698, 1699, 1700, 1686};
    vint syndIndices = {897, 899, 877, 879, 5168, 878, 851, 5148, 858, 831, 2984, 6562, 
                           6587, 2964, 6542, 6567, 6561, 6539, 6541, 6519, 1554, 1572, 1573,
                           1574, 1575, 1592, 1593, 1595, 2250, 2268, 2269, 2270, 2271, 2288, 
                           2289, 2291, 5149, 5150, 5169, 5170, 5175, 5195, 5844, 5845, 5846, 
                           5864, 5865, 5866, 5871, 5891, 6540, 6560};
    vint defectIndices = {219, 393, 567, 741, 1287, 1292, 1293, 1298, 1461, 1466, 1467, 
                            1472, 1635, 1640, 1641, 1646};
    vint bulkSweepVertices = {214, 219, 224, 388, 393, 398, 562, 567, 572, 1287, 1292,
                                 1297, 1461, 1466, 1471, 1635, 1640, 1645};
    vint middleBoundarySweepVertices = {736, 741, 746};
    vint zLogical = {672, 1194, 1716};
    vint movedLowerQubitIndices = {672, 673, 654, 655, 1194, 1195, 1196, 642, 1176, 1177,
                                 1178, 1716, 1717, 1718, 1164, 1698, 1699, 1700, 1686};
    vint movedMiddleQubitIndices = {1233, 1215, 1235, 1234, 1755, 1203, 1217, 1216, 1737, 
                                 1757, 1756, 2277, 1725, 1739, 1738, 2259, 2278, 2247, 
                                 2260};
    vint movedUpperQubitIndices = {708, 709, 690, 691, 1230, 1231, 1232, 678, 1212, 1213,
                                 1214, 1752, 1753, 1754, 1200, 1734, 1735, 1736, 1722};
    vint movedSyndIndices = {945, 947, 925, 927, 5216, 926, 899, 5196, 906, 879, 3032, 6610, 
                           6635, 3012, 6590, 6615, 6609, 6587, 6589, 6567, 1602, 1620, 1621,
                           1622, 1623, 1640, 1641, 1643, 2298, 2316, 2317, 2318, 2319, 2336, 
                           2337, 2339, 5197, 5198, 5217, 5218, 5223, 5243, 5892, 5893, 5894, 
                           5912, 5913, 5914, 5919, 5939, 6588, 6608};
    vint movedDefectIndices = {231, 405, 579, 753, 1299, 1304, 1305, 1310, 1473, 1478, 1479, 
                            1484, 1647, 1652, 1653, 1658};
    vint movedBulkSweepVertices = {226, 231, 236, 400, 405, 410, 574, 579, 584, 1299, 1304,
                                 1309, 1473, 1478, 1483, 1647, 1652, 1657};
    vint movedMiddleBoundarySweepVertices = {748, 753, 758};
    vint movedZLogical = {708, 1230, 1752};

    moveIndices(lowerQubitIndices, middleQubitIndices, upperQubitIndices, syndIndices, defectIndices, bulkSweepVertices, middleBoundarySweepVertices, zLogical, vertexToFaces, faceToVertices, 6);

    EXPECT_EQ(lowerQubitIndices, movedLowerQubitIndices);
    EXPECT_EQ(middleQubitIndices, movedMiddleQubitIndices);
    EXPECT_EQ(upperQubitIndices, movedUpperQubitIndices);
    EXPECT_EQ(syndIndices, movedSyndIndices);
    EXPECT_EQ(defectIndices, movedDefectIndices);
    EXPECT_EQ(bulkSweepVertices, movedBulkSweepVertices);
    EXPECT_EQ(middleBoundarySweepVertices, movedMiddleBoundarySweepVertices);
    EXPECT_EQ(zLogical, movedZLogical);
}
