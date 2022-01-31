#include "gtest/gtest.h"
#include "rhombic1.h"

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

    c2.xi[0] = 15;
    c2.xi[1] = 3;
    c2.xi[2] = 0;
    c2.xi[3] = 0;
    
    EXPECT_EQ(coordToIndex(c0, 6), 304);
    EXPECT_EQ(coordToIndex(c1, 6), 1384);
    EXPECT_EQ(coordToIndex(c2, 6), 105);
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
    
    c1.xi[0] = 50;
    c1.xi[1] = 0;
    c1.xi[2] = 0;
    c1.xi[3] = 0;

    c2.xi[0] = 0;
    c2.xi[1] = 6;
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
    coord c0 = indexToCoord(304, 6);
    coord c1 = indexToCoord(1384, 6);
    coord c2 = indexToCoord(105, 6);

    EXPECT_EQ(c0.xi[0], 4);
    EXPECT_EQ(c0.xi[1], 4);
    EXPECT_EQ(c0.xi[2], 1);
    EXPECT_EQ(c0.xi[3], 0);

    EXPECT_EQ(c1.xi[0], 4);
    EXPECT_EQ(c1.xi[1], 4);
    EXPECT_EQ(c1.xi[2], 1);
    EXPECT_EQ(c1.xi[3], 1);

    EXPECT_EQ(c2.xi[0], 15);
    EXPECT_EQ(c2.xi[1], 3);
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
    //v=212 is (2,1,1,0)
    EXPECT_EQ(neigh(212, xy, 1, 6), 1112);
    EXPECT_EQ(neigh(212, xz, 1, 6), 1262);
    EXPECT_EQ(neigh(212, yz, 1, 6), 1291);
    EXPECT_EQ(neigh(212, xyz, 1, 6), 1292);
    EXPECT_EQ(neigh(212, xy, -1, 6), 1261);
    EXPECT_EQ(neigh(212, xz, -1, 6), 1111);
    EXPECT_EQ(neigh(212, yz, -1, 6), 1082);
    EXPECT_EQ(neigh(212, xyz, -1, 6), 1081);
}
TEST(neighTest, CorrectOutputW1)
{
    //v=1292 is (2,1,1,1)
    EXPECT_EQ(neigh(1292, xy, 1, 6), 243);
    EXPECT_EQ(neigh(1292, xz, 1, 6), 393);
    EXPECT_EQ(neigh(1292, yz, 1, 6), 422);
    EXPECT_EQ(neigh(1292, xyz, 1, 6), 423);
    EXPECT_EQ(neigh(1292, xy, -1, 6), 392);
    EXPECT_EQ(neigh(1292, xz, -1, 6), 242);
    EXPECT_EQ(neigh(1292, yz, -1, 6), 213);
    EXPECT_EQ(neigh(1292, xyz, -1, 6), 212);
}
TEST(neighTest, xGreaterThanL)
{
    //v=220 is (10,1,1,0)
    EXPECT_EQ(neigh(220, xy, 1, 6), 1120);
    EXPECT_EQ(neigh(220, xz, 1, 6), 1270);
    EXPECT_EQ(neigh(220, yz, 1, 6), 1299);
    EXPECT_EQ(neigh(220, xyz, 1, 6), 1300);
    EXPECT_EQ(neigh(220, xy, -1, 6), 1269);
    EXPECT_EQ(neigh(220, xz, -1, 6), 1119);
    EXPECT_EQ(neigh(220, yz, -1, 6), 1090);
    EXPECT_EQ(neigh(220, xyz, -1, 6), 1089);

    //v=1300 is (10,1,1,1)
    EXPECT_EQ(neigh(1300, xy, 1, 6), 251);
    EXPECT_EQ(neigh(1300, xz, 1, 6), 401);
    EXPECT_EQ(neigh(1300, yz, 1, 6), 430);
    EXPECT_EQ(neigh(1300, xyz, 1, 6), 431);
    EXPECT_EQ(neigh(1300, xy, -1, 6), 400);
    EXPECT_EQ(neigh(1300, xz, -1, 6), 250);
    EXPECT_EQ(neigh(1300, yz, -1, 6), 221);
    EXPECT_EQ(neigh(1300, xyz, -1, 6), 220);
}
TEST(neighTest, HandlesIncorrectArugments)
{
    EXPECT_THROW(neigh(212, -1, 1, 6), std::invalid_argument);
    EXPECT_THROW(neigh(212, 4, 1, 6), std::invalid_argument);
    EXPECT_THROW(neigh(212, 0, 0, 6), std::invalid_argument);
}

//------------------------------------------------------------

TEST(edgeIndexTest, CorrectOutput)
{
    EXPECT_EQ(edgeIndex(212, xy, 1, 6), 848);
    EXPECT_EQ(edgeIndex(212, xz, 1, 6), 849);
    EXPECT_EQ(edgeIndex(212, yz, 1, 6), 850);
    EXPECT_EQ(edgeIndex(212, xyz, 1, 6), 851);
    EXPECT_EQ(edgeIndex(212, xy, -1, 6), 5044);
    EXPECT_EQ(edgeIndex(212, xz, -1, 6), 4445);
    EXPECT_EQ(edgeIndex(212, yz, -1, 6), 4330);
    EXPECT_EQ(edgeIndex(212, xyz, -1, 6), 4327);
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

    addFace(212, 0, {xyz, xz, xz, xyz}, {1, 1, 1, 1}, faceToVertices, faceToEdges, vertexToFaces, edgeToFaces, 6);
    addFace(212, 1, {xy, xz, xz, xy}, {1, -1, -1, 1}, faceToVertices, faceToEdges, vertexToFaces, edgeToFaces, 6);

    vint vertices1 = {212, 393, 1262, 1292};
    vint edges1 = {849, 851, 5051, 5169};
    vint vertices2 = {62, 212, 1111, 1112};
    vint edges2 = {249, 848, 4444, 4445};
    vint faces1 = {0,1};
    vint faces2 = {0};
    vint faces3 = {1};

    EXPECT_EQ(faceToVertices[0], vertices1);
    EXPECT_EQ(faceToEdges[0], edges1);
    EXPECT_EQ(faceToVertices[1], vertices2);
    EXPECT_EQ(faceToEdges[1], edges2);
    EXPECT_EQ(vertexToFaces[212], faces1);
    EXPECT_EQ(vertexToFaces[393], faces2);
    EXPECT_EQ(vertexToFaces[62], faces3);
    EXPECT_EQ(edgeToFaces[849], faces2);
    EXPECT_EQ(edgeToFaces[848], faces3);
}

//------------------------------------------------------------

TEST(buildFacesTest, CorrectOutput)
{   
    vvint faceToVertices;
    vvint faceToEdges;
    vvint vertexToFaces;
    vvint edgeToFaces;

    buildFaces(faceToVertices, faceToEdges, vertexToFaces, edgeToFaces, 6);
    
    vint vertices = {0, 181, 1080, 1230};
    vint edges = {1, 3, 4321, 4923};

    EXPECT_EQ(faceToVertices[1], vertices);
    EXPECT_EQ(faceToEdges[1], edges);
    EXPECT_EQ(vertexToFaces[0][1], 1);
    EXPECT_EQ(vertexToFaces[181][0], 1);
    EXPECT_EQ(vertexToFaces[1080][1], 1);
    EXPECT_EQ(vertexToFaces[1230][0], 1);
    EXPECT_EQ(edgeToFaces[3][1], 1);
    EXPECT_EQ(edgeToFaces[1][0], 1);
    EXPECT_EQ(edgeToFaces[4321][0], 1);
    EXPECT_EQ(edgeToFaces[4923][0], 1);
}

//------------------------------------------------------------

TEST(buildVertexToEdgesTest, CorrectOutput)
{
    vvint vertexToEdges = buildVertexToEdges(6);
    
    vint edges1 = {848, 849, 850, 851, 4327, 4330, 4445, 5044};
    vint edges2 = {};
    vint edges3 = {851, 5168, 5169, 5170};
    vint edges4 = {850, 965, 1564, 5167};
    
    EXPECT_EQ(vertexToEdges[212], edges1);
    EXPECT_EQ(vertexToEdges[211], edges2);
    EXPECT_EQ(vertexToEdges[1292], edges3);
    EXPECT_EQ(vertexToEdges[1291], edges4);
}

//------------------------------------------------------------

TEST(buildEdgeToVerticesTest, CorrectOutput)
{
    vpint edgeToVertices = buildEdgeToVertices(6);
    
    std::pair<int, int> vertices1 = {212, 1112};
    std::pair<int, int> vertices2 = {212, 1262};
    std::pair<int, int> vertices3 = {212, 1291};
    std::pair<int, int> vertices4 = {212, 1292};

    EXPECT_EQ(edgeToVertices[848], vertices1);
    EXPECT_EQ(edgeToVertices[849], vertices2);
    EXPECT_EQ(edgeToVertices[850], vertices3);
    EXPECT_EQ(edgeToVertices[851], vertices4);
}

//------------------------------------------------------------

TEST(findFaceTest, CorrectOutput)
{
    vvint faceToVertices;
    vvint faceToEdges;
    vvint vertexToFaces;
    vvint edgeToFaces;

    buildFaces(faceToVertices, faceToEdges, vertexToFaces, edgeToFaces, 6);
    vint vertices = {0, 181, 1080, 1230};
    EXPECT_EQ(findFace(vertices, vertexToFaces, faceToVertices), 1);
}

//------------------------------------------------------------

TEST(buildSliceOneTest, CorrectOutput)
{
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
    buildFaces(faceToVertices, faceToEdges, vertexToFaces, edgeToFaces, 6);

    vvint vertexToEdges = buildVertexToEdges(6);
    vpint edgeToVertices = buildEdgeToVertices(6);

    buildSliceOne(lowerQubitIndices, middleQubitIndices, upperQubitIndices, syndIndices, defectIndices, bulkSweepVertices, middleBoundarySweepVertices, upperBoundarySweepVertices1, upperBoundarySweepVertices2, zLogical, vertexToFaces, faceToVertices, faceToEdges, edgeToVertices, 6);

    vint qubits1 = {2433, 2434, 1985, 2349, 2350, 1901, 1899, 1900, 1451,
                    2259, 1811, 1809, 1810, 1361, 1359, 1360, 1725, 1277,
                    1275, 1276, 1185};
    vint qubits2 = {1896, 1806, 1897, 1356, 1898, 1722, 1807, 1272, 1808,
                    1357, 822, 1358, 1182, 1724, 1273, 732, 1274, 823, 
                    648, 1184, 733};
    vint qubits3 = {2439, 2440, 1991, 2355, 2356, 1907, 1905, 1906, 1457,
                    2265, 1817, 1815, 1816, 1367, 1365, 1366, 1731, 1283,
                    1281, 1282, 1191};
    vint stabs = {6731, 6849, 6734, 3252, 6615, 6733, 6618, 3136, 1097, 1098, 
                  1099, 981, 982, 983, 5300, 5184, 1696, 1697, 1698, 1699, 
                  1812, 1813, 1814, 1815, 2412, 2413, 2414, 2415, 2528, 2529, 
                  2530, 2531, 5183, 5186, 5299, 5301, 5302, 5417, 5899, 5900, 
                  5902, 6015, 6016, 6017, 6018, 6133, 6616, 6732};
    vint defects = {1682, 274, 245, 424, 453, 603, 632, 784, 813, 1324, 1325, 1503, 
                    1504, 1683};
    vint vertices1 = {245, 274, 303, 395, 424, 453, 482, 574, 603, 632, 661,
                      753, 782, 811, 1296, 1325, 1354, 1475, 1504, 1533, 1654,
                      1683, 1712};
    vint vertices2 = {1295, 1474, 1653};
    vint vertices3 = {755, 784, 813};
    vint vertices4 = {1296, 1354, 1475, 1533, 1654, 1712};
    vint logical = {2265, 1817, 1731, 1283, 1191};
    
    EXPECT_EQ(lowerQubitIndices, qubits1);
    EXPECT_EQ(middleQubitIndices, qubits2);
    EXPECT_EQ(upperQubitIndices, qubits3);
    EXPECT_EQ(syndIndices, stabs);
    EXPECT_EQ(defectIndices, defects);
    EXPECT_EQ(bulkSweepVertices, vertices1);
    EXPECT_EQ(middleBoundarySweepVertices, vertices2);
    EXPECT_EQ(upperBoundarySweepVertices1, vertices3);
    EXPECT_EQ(upperBoundarySweepVertices2, vertices4);
    EXPECT_EQ(zLogical, logical);
}

//------------------------------------------------------------

TEST(moveFacesTest, CorrectOutput)
{
    vvint faceToVertices;
    vvint faceToEdges;
    vvint vertexToFaces;
    vvint edgeToFaces;
    buildFaces(faceToVertices, faceToEdges, vertexToFaces, edgeToFaces, 6);

    vint lowerQubitIndices = {2433, 2434, 1985, 2349, 2350, 1901, 1899, 1900, 1451,
                              2259, 1811, 1809, 1810, 1361, 1359, 1360, 1725, 1277,
                              1275, 1276, 1185};
    vint upperQubitIndices = {2439, 2440, 1991, 2355, 2356, 1907, 1905, 1906, 1457,
                              2265, 1817, 1815, 1816, 1367, 1365, 1366, 1731, 1283,
                              1281, 1282, 1191};
    EXPECT_EQ(moveFaces(lowerQubitIndices, vertexToFaces, faceToVertices), upperQubitIndices);
}

//------------------------------------------------------------

TEST(moveIndicesTest, CorrectOutput)
{   
    vvint faceToVertices;
    vvint faceToEdges;
    vvint vertexToFaces;
    vvint edgeToFaces;
    buildFaces(faceToVertices, faceToEdges, vertexToFaces, edgeToFaces, 6);

    vint lowerQubitIndices = {2433, 2434, 1985, 2349, 2350, 1901, 1899, 1900, 1451,
                              2259, 1811, 1809, 1810, 1361, 1359, 1360, 1725, 1277,
                              1275, 1276, 1185};
    vint middleQubitIndices = {1896, 1806, 1897, 1356, 1898, 1722, 1807, 1272, 1808,
                               1357, 822, 1358, 1182, 1724, 1273, 732, 1274, 823, 
                               648, 1184, 733};
    vint upperQubitIndices = {2439, 2440, 1991, 2355, 2356, 1907, 1905, 1906, 1457,
                              2265, 1817, 1815, 1816, 1367, 1365, 1366, 1731, 1283,
                              1281, 1282, 1191};
    vint syndIndices = {3244, 3128, 6731, 6849, 6734, 3252, 6615, 6733, 6618, 3136,
                        5292, 1097, 1098, 1099, 5176, 981, 982, 983, 5300, 5184,
                        1696, 1697, 1698, 1699, 1812, 1813, 1814, 1815, 2412, 2413,
                        2414, 2415, 2528, 2529, 2530, 2531, 5186, 5301, 5302, 5417,
                        5902, 6016, 6017, 6018, 6133, 6616, 6732};
    vint defectIndices = {245, 274, 424, 453, 603, 632, 784, 813, 1324, 1325, 1503, 
                          1504, 1682, 1683};
    vint bulkSweepVertices = {245, 274, 303, 395, 424, 453, 482, 574, 603, 632, 661,
                              753, 782, 811, 1296, 1325, 1354, 1475, 1504, 1533, 1654,
                              1683, 1712};
    vint middleBoundarySweepVertices = {1295, 1474, 1653};
    vint upperBoundarySweepVertices1 = {755, 784, 813};
    vint upperBoundarySweepVertices2 = {1296, 1354, 1475, 1533, 1654, 1712};
    vint zLogical = {1191, 1283, 1731, 1817, 2265};

    vint movedLowerQubitIndices = {2439, 2440, 1991, 2355, 2356, 1907, 1905, 1906, 1457,
                                   2265, 1817, 1815, 1816, 1367, 1365, 1366, 1731, 1283,
                                   1281, 1282, 1191};
    vint movedMiddleQubitIndices = {1902, 1812, 1903, 1362, 1904, 1728, 1813, 1278, 1814,
                                    1363, 828, 1364, 1188, 1730, 1279, 738, 1280, 829, 
                                    654, 1190, 739};
    vint movedUpperQubitIndices = {2445, 2446, 1997, 2361, 2362, 1913, 1911, 1912, 1463,
                                   2271, 1823, 1821, 1822, 1373, 1371, 1372, 1737, 1289,
                                   1287, 1288, 1197};
    vint movedSyndIndices = {3252, 3136, 6739, 6857, 6742, 3260, 6623, 6741, 6626, 3144,
                             5300, 1105, 1106, 1107, 5184, 989, 990, 991, 5308, 5192,
                             1704, 1705, 1706, 1707, 1820, 1821, 1822, 1823, 2420, 2421,
                             2422, 2423, 2536, 2537, 2538, 2539, 5194, 5309, 5310, 5425,
                             5910, 6024, 6025, 6026, 6141, 6624, 6740};
    vint movedDefectIndices = {247, 276, 426, 455, 605, 634, 786, 815, 1326, 1327, 1505,
                               1506, 1684, 1685};
    vint movedBulkSweepVertices = {247, 276, 305, 397, 426, 455, 484, 576, 605, 634, 663,
                                   755, 784, 813, 1298, 1327, 1356, 1477, 1506, 1535, 1656,
                                   1685, 1714};
    vint movedMiddleBoundarySweepVertices = {1297, 1476, 1655};
    vint movedUpperBoundarySweepVertices1 = {757, 786, 815};
    vint movedUpperBoundarySweepVertices2 = {1298, 1356, 1477, 1535, 1656, 1714};
    vint movedZLogical = {1197, 1289, 1737, 1823, 2271};

    moveIndices(lowerQubitIndices, middleQubitIndices, upperQubitIndices, syndIndices, defectIndices, bulkSweepVertices, middleBoundarySweepVertices, upperBoundarySweepVertices1, upperBoundarySweepVertices2, zLogical, vertexToFaces, faceToVertices, 6);

    EXPECT_EQ(lowerQubitIndices, movedLowerQubitIndices);
    EXPECT_EQ(middleQubitIndices, movedMiddleQubitIndices);
    EXPECT_EQ(upperQubitIndices, movedUpperQubitIndices);
    EXPECT_EQ(syndIndices, movedSyndIndices);
    EXPECT_EQ(defectIndices, movedDefectIndices);
    EXPECT_EQ(bulkSweepVertices, movedBulkSweepVertices);
    EXPECT_EQ(middleBoundarySweepVertices, movedMiddleBoundarySweepVertices);
    EXPECT_EQ(upperBoundarySweepVertices1, movedUpperBoundarySweepVertices1);
    EXPECT_EQ(upperBoundarySweepVertices2, movedUpperBoundarySweepVertices2);
    EXPECT_EQ(zLogical, movedZLogical);
}
