#include "gtest/gtest.h"
#include "decoderRhombic2.h"
#include "prepIndexVectorsRhombic2.h"
#include <iostream>

//------------------------------------------------------------

TEST(indexBuilding, building)
{
    buildFaces(faceToVertices, faceToEdges, vertexToFaces, edgeToFaces, 6);
    buildSliceTwo(lowerQubitIndices, middleQubitIndices, upperQubitIndices, syndIndices, defectIndices, bulkSweepVertices, middleBoundarySweepVertices, zLogical, vertexToFaces, faceToVertices, faceToEdges, edgeToVertices, 6);
    ASSERT_TRUE(true);
}

//------------------------------------------------------------

TEST(shortestPathLengthTest, CorrectOutput)
{
    EXPECT_EQ(shortestPathLength(393, 1467, 6), 1);
    EXPECT_EQ(shortestPathLength(393, 567, 6), 2);
    EXPECT_EQ(shortestPathLength(393, 1641, 6), 3);
    EXPECT_EQ(shortestPathLength(393, 1461, 6), 3);
    vint path = shortestPath(393, 1461, syndIndices, vertexToEdges, 6);
    for (int i : path) std::cout << i << ',';
    std::cout << '\n';
}

//------------------------------------------------------------

TEST(shortestPathTest, CorrectOutput)
{
    vint path1 = {1573};
    vint path2 = {5864, 5865};
    vint path3 = {1573, 2268, 2269};
    EXPECT_EQ(shortestPath(393, 1467, syndIndices, vertexToEdges, 6), path1);
    EXPECT_EQ(shortestPath(393, 567, syndIndices, vertexToEdges, 6), path2);
    EXPECT_EQ(shortestPath(393, 1641, syndIndices, vertexToEdges, 6), path3);
}

//------------------------------------------------------------

TEST(middleSweepRuleTest, CorrectOutput)
{
    vint qubits(3*5*6*6*6, 0);
    vint syndrome(8*5*6*6*6, 0);
    vint blankQubits(3*5*6*6*6, 0);
    vint nonBlankQubits(3*5*6*6*6, 0);
    vint blankSyndrome(8*5*6*6*6, 0);
    vint nonBlankSyndrome(8*5*6*6*6, 0);

    qubits[1701] = 1;
    syndrome[5864] = 1;
    syndrome[5865] = 1;
    syndrome[1573] = 1;
    syndrome[2268] = 1;
    middleSweepRule(qubits, middleQubitIndices, syndrome, syndIndices, bulkSweepVertices, 
                        faceToEdges, faceToVertices, vertexToFaces, 6);
    EXPECT_EQ(qubits, blankQubits);
    EXPECT_EQ(syndrome, blankSyndrome);

    qubits[1701] = 1;
    qubits[1720] = 1;
    syndrome[5865] = 1;
    syndrome[5866] = 1;
    syndrome[1573] = 1;
    syndrome[1574] = 1;
    syndrome[2268] = 1;
    syndrome[2288] = 1;
    nonBlankQubits[1701] = 1;
    nonBlankQubits[1720] = 1;
    nonBlankQubits[1721] = 1;
    nonBlankSyndrome[1573] = 1;
    nonBlankSyndrome[1574] = 1;
    nonBlankSyndrome[2268] = 1;
    nonBlankSyndrome[2270] = 1;
    nonBlankSyndrome[2288] = 1;
    nonBlankSyndrome[2289] = 1;
    middleSweepRule(qubits, middleQubitIndices, syndrome, syndIndices, bulkSweepVertices, 
                        faceToEdges, faceToVertices, vertexToFaces, 6);
    EXPECT_EQ(qubits, nonBlankQubits);
    EXPECT_EQ(syndrome, nonBlankSyndrome);
}
 
//------------------------------------------------------------

TEST(middleBoundarySweepRuleTest, CorrectOutput)
{
    vint qubits(3*5*6*6*6, 0);
    vint syndrome(8*5*6*6*6, 0);
    vint blankQubits(3*5*6*6*6, 0);
    vint blankSyndrome(8*5*6*6*6, 0);
    
    qubits[2223] = 1;
    syndrome[2964] = 1;
    syndrome[6560] = 1;
    syndrome[6561] = 1;
    syndrome[2269] = 1;
    middleBoundarySweepRule(qubits, middleQubitIndices, syndrome, syndIndices, 
                                middleBoundarySweepVertices, faceToEdges, faceToVertices, 
                                vertexToFaces, 6);
    EXPECT_EQ(qubits, blankQubits);
    EXPECT_EQ(syndrome, blankSyndrome);

    qubits[2223] = 1;
    qubits[2224] = 1;
    syndrome[6560] = 1;
    syndrome[6561] = 1;
    syndrome[2269] = 1;
    syndrome[6540] = 1;
    syndrome[6542] = 1;
    syndrome[2250] = 1;
    middleBoundarySweepRule(qubits, middleQubitIndices, syndrome, syndIndices,
                                middleBoundarySweepVertices, faceToEdges, faceToVertices, 
                                vertexToFaces, 6);
    EXPECT_EQ(qubits, blankQubits);
    EXPECT_EQ(syndrome, blankSyndrome);
    
    qubits[2211] = 1;
    qubits[2241] = 1;
    syndrome[6540] = 1;
    syndrome[6541] = 1;
    syndrome[2984] = 1;
    syndrome[2289] = 1;
    middleBoundarySweepRule(qubits, middleQubitIndices, syndrome, syndIndices,
                                middleBoundarySweepVertices, faceToEdges, faceToVertices, 
                                vertexToFaces, 6);
    EXPECT_EQ(qubits, blankQubits);
    EXPECT_EQ(syndrome, blankSyndrome);

    qubits[2241] = 1;
    qubits[2242] = 1;
    syndrome[2270] = 1;
    syndrome[2289] = 1;
    syndrome[6560] = 1;
    syndrome[6562] = 1;
    middleBoundarySweepRule(qubits, middleQubitIndices, syndrome, syndIndices,
                                middleBoundarySweepVertices, faceToEdges, faceToVertices, 
                                vertexToFaces, 6);
    EXPECT_EQ(qubits, blankQubits);
    EXPECT_EQ(syndrome, blankSyndrome);
}

//------------------------------------------------------------

TEST(upperSweepRuleTest, CorrectOutput)
{
    vint qubits(3*5*6*6*6, 0);
    vint syndrome(8*5*6*6*6, 0);
    vint blankQubits(3*5*6*6*6, 0);
    vint blankSyndrome(8*5*6*6*6, 0);
    
    qubits[1698] = 1;
    syndrome[2270] = 1;
    syndrome[2271] = 1;
    syndrome[6587] = 1;
    upperSweepRule(qubits, upperQubitIndices, syndrome, syndIndices, bulkSweepVertices, 
                    faceToEdges, faceToVertices, vertexToFaces, 6);
    EXPECT_EQ(qubits, blankQubits);
    EXPECT_EQ(syndrome, blankSyndrome);
    
    qubits[1698] = 1;
    qubits[1700] = 1;
    syndrome[2268] = 1;
    syndrome[2270] = 1;
    syndrome[5871] = 1;
    syndrome[6587] = 1;
    upperSweepRule(qubits, upperQubitIndices, syndrome, syndIndices, bulkSweepVertices, 
                    faceToEdges, faceToVertices, vertexToFaces, 6);
    EXPECT_EQ(qubits, blankQubits);
    EXPECT_EQ(syndrome, blankSyndrome);

    qubits[1698] = 1;
    qubits[1699] = 1;
    qubits[1700] = 1;
    syndrome[2268] = 1;
    syndrome[2269] = 1;
    syndrome[2270] = 1;
    syndrome[2271] = 1;
    syndrome[5871] = 1;
    syndrome[6587] = 1;
    syndrome[6567] = 1;
    upperSweepRule(qubits, upperQubitIndices, syndrome, syndIndices, bulkSweepVertices, 
                    faceToEdges, faceToVertices, vertexToFaces, 6);
    EXPECT_EQ(qubits, blankQubits);
    EXPECT_EQ(syndrome, blankSyndrome);

    qubits[1716] = 1;
    qubits[1718] = 1;
    syndrome[2288] = 1;
    syndrome[5891] = 1;
    upperSweepRule(qubits, upperQubitIndices, syndrome, syndIndices, bulkSweepVertices, 
                    faceToEdges, faceToVertices, vertexToFaces, 6);
    EXPECT_EQ(qubits, blankQubits);
    EXPECT_EQ(syndrome, blankSyndrome);
}

//------------------------------------------------------------

TEST(distanceToClosestBoundaryTest, CorrectOutput)
{
    vint distInfo1 = {-1, 1};
    vint distInfo2 = {1, 2};
    vint distInfo3 = {-1, 1};
    EXPECT_EQ(distanceToClosestBoundary(1466, 6), distInfo1);
    EXPECT_EQ(distanceToClosestBoundary(567, 6), distInfo2);
    EXPECT_EQ(distanceToClosestBoundary(1472, 6), distInfo3);
}

//------------------------------------------------------------

TEST(shortestPathToBTest, CorrectOutput)
{
    vint path1 = {5866};
    vint path2 = {2269, 2250};
    vint path3 = {1593};
    EXPECT_EQ(shortestPathToB(1466, 6), path1);
    EXPECT_EQ(shortestPathToB(567, 6), path2);
    EXPECT_EQ(shortestPathToB(1472, 6), path3);
}

//------------------------------------------------------------

TEST(mwpmTest, CorrectOutput)
{
    vint defects;
    vpint expectedPairing;
    std::map<std::pair<int, int>, int> defectPairDistances;

    defects = {393, 1466};
    expectedPairing = {{393, 1466}};
    EXPECT_EQ(mwpm(defects, defectPairDistances, 6), expectedPairing);

    defects = {393, 1292, 567, 1641};
    expectedPairing = {{393, 1292}, {567, 1641}};
    EXPECT_EQ(mwpm(defects, defectPairDistances, 6), expectedPairing);

    defects = {1292, 1641};
    expectedPairing = {{1292, -1}, {1641, -1}};
    EXPECT_EQ(mwpm(defects, defectPairDistances, 6), expectedPairing);

    defectPairDistances[{1292, 1641}] = 1;
    expectedPairing = {{1292, 1641}};
    EXPECT_EQ(mwpm(defects, defectPairDistances, 6), expectedPairing);
}

//------------------------------------------------------------

TEST(updatePairingTest, CorrectOutput)
{
    vint defects;
    std::map<std::pair<int, int>, int> defectPairDistances;
    std::map<std::pair<int, int>, int> expectedDefectPairDistances;
    
    defects = {393, 1292, 567, 1641};
    defectPairDistances[{393, 1292}] = 1;
    defectPairDistances[{741, 1641}] = 1;
    expectedDefectPairDistances[{393, 1292}] = 1;
    expectedDefectPairDistances[{567, 1641}] = 1;
    updatePairing(defectPairDistances, defects, 6);
    EXPECT_EQ(defectPairDistances, expectedDefectPairDistances);
}

//------------------------------------------------------------

TEST(joinPairTest, CorrectOutput)
{
    vint syndrome(8*5*6*6*6, 0);
    vint blankSyndrome(8*5*6*6*6, 0);

    syndrome[5864] = 1;
    joinPair(393, 1466, syndIndices, vertexToEdges, syndrome, 6);
    EXPECT_EQ(syndrome, blankSyndrome);

    syndrome[5866] = 1;
    joinPair(1466, -1, syndIndices, vertexToEdges, syndrome, 6);
    EXPECT_EQ(syndrome, blankSyndrome);
}

//------------------------------------------------------------

TEST(pathToTopTest, CorrectOutput)
{
    vint path1 = {1573, 1575};
    vint path2 = {5865, 5866, 2288, 5891};
    vint path3 = {5846, 2268, 1573, 1575};
    vint path4 = {};
    EXPECT_EQ(pathToTop(1467, syndIndices, vertexToEdges, 6), path1);
    EXPECT_EQ(pathToTop(567, syndIndices, vertexToEdges, 6), path2);
    //I actually think this one will fail and it will defer upwards too early and get stuck
    //Can either hard-code deferral paths or make pathfinding stuff cleverer
    //i.e. when it loops/turns back you delete those edges from the path
    //and then force it to choose other edges 
    EXPECT_EQ(pathToTop(1461, syndIndices, vertexToEdges, 6), path3);
    EXPECT_EQ(pathToTop(-1, syndIndices, vertexToEdges, 6), path4);
}

//------------------------------------------------------------

TEST(syndromeRepairTest, CorrectOutput)
{
    vint qubits(3*5*6*6*6, 0);
    vint syndrome(8*5*6*6*6, 0);
    vint blankSyndrome(8*5*6*6*6, 0);
    vint nonBlankSyndrome(8*5*6*6*6, 0);

    syndrome[5864] = 1;
    syndrome[5865] = 1;
    syndrome[5846] = 1;
    nonBlankSyndrome[5864] = 1;
    nonBlankSyndrome[5865] = 1;
    nonBlankSyndrome[2268] = 1;
    nonBlankSyndrome[1573] = 1;
    nonBlankSyndrome[1575] = 1;
    nonBlankSyndrome[5169] = 1;
    nonBlankSyndrome[5170] = 1;
    nonBlankSyndrome[1592] = 1;
    nonBlankSyndrome[5195] = 1;
    std::map<std::pair<int, int>, int> defectPairDistances;
    defectPairDistances[{393, 1461}] = 3;
    std::map<std::pair<int, int>, int> updatedDefectPairDistances;
    updatedDefectPairDistances[{405, 1473}] = 2;
    syndromeRepair(syndrome, syndIndices, vertexToEdges, edgeToVertices, defectPairDistances, 6,2,1);
    EXPECT_EQ(syndrome, nonBlankSyndrome);
    EXPECT_EQ(defectPairDistances, updatedDefectPairDistances);
    
    for (int i=0; i < syndrome.size(); i++) syndrome[i] = 0;
    syndrome[5864] = 1;
    syndrome[5865] = 1;
    syndrome[5846] = 1;
    
    std::map<std::pair<int, int>, int> defectPairDistances2;
    defectPairDistances2[{393, 1461}] = 1;
    syndromeRepair(syndrome, syndIndices, vertexToEdges, edgeToVertices, defectPairDistances2,6,2,1);
    middleBoundarySweepRule(qubits, middleQubitIndices, syndrome, syndIndices, 
                                middleBoundarySweepVertices, faceToEdges, faceToVertices, 
                                vertexToFaces, 6);
    middleSweepRule(qubits, middleQubitIndices, syndrome, syndIndices, bulkSweepVertices,
                     faceToEdges, faceToVertices, vertexToFaces, 6);
    upperSweepRule(qubits, upperQubitIndices, syndrome, syndIndices, bulkSweepVertices,
                    faceToEdges, faceToVertices, vertexToFaces, 6);
    EXPECT_EQ(syndrome, blankSyndrome);
}
