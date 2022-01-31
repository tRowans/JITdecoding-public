#include "gtest/gtest.h"
#include "decoderRhombic1.h"
#include "prepIndexVectorsRhombic1.h"
#include <iostream>

//------------------------------------------------------------

//This is just a hack to make the testing software build the index vectors
//Means they don't have to be build individually in every test
TEST(indexBuilding, building)
{
    buildFaces(faceToVertices, faceToEdges, vertexToFaces, edgeToFaces, 6);
    buildSliceOne(lowerQubitIndices, middleQubitIndices, upperQubitIndices, syndIndices, defectIndices, bulkSweepVertices, middleBoundarySweepVertices, upperBoundarySweepVertices1, upperBoundarySweepVertices2, zLogical, vertexToFaces, faceToVertices, faceToEdges, edgeToVertices, 6);
    ASSERT_TRUE(true);
}

//------------------------------------------------------------

TEST(shortestPathLengthTest, CorrectOutput)
{
    EXPECT_EQ(shortestPathLength(424, 1503, 6), 1);
    EXPECT_EQ(shortestPathLength(424, 603, 6), 2);
    EXPECT_EQ(shortestPathLength(424, 1682, 6), 3);
}
    
//------------------------------------------------------------

TEST(shortestPathTest, CorrectOutput)
{
    vint path1 = {1698};
    vint path2 = {1698, 2412};
    vint path3 = {1698, 2412, 2414};
    vint path4 = {1699};
    EXPECT_EQ(shortestPath(424, 1503, syndIndices, vertexToEdges, 6), path1);
    EXPECT_EQ(shortestPath(424, 603, syndIndices, vertexToEdges, 6), path2);
    EXPECT_EQ(shortestPath(424, 1682, syndIndices, vertexToEdges, 6), path3);
    EXPECT_EQ(shortestPath(424, 1504, syndIndices, vertexToEdges, 6), path4);
}

//------------------------------------------------------------

TEST(middleSweepRuleTest, CorrectOutput)
{
    vint qubits(3*5*6*6*6, 0);
    vint syndrome(8*5*6*6*6, 0);
    vint blankQubits(3*5*6*6*6, 0);
    vint blankSyndrome(8*5*6*6*6, 0);
   
    qubits[1808] = 1;
    syndrome[2412] = 1;
    syndrome[2415] = 1;
    syndrome[6015] = 1;
    syndrome[6732] = 1;
    middleSweepRule(qubits, middleQubitIndices, syndrome, syndIndices, bulkSweepVertices, 
                        faceToEdges, faceToVertices, vertexToFaces, 6);
    EXPECT_EQ(qubits, blankQubits);
    EXPECT_EQ(syndrome, blankSyndrome);

    qubits[1807] = 1;
    qubits[1808] = 1;
    syndrome[2412] = 1;
    syndrome[2413] = 1;
    syndrome[6015] = 1;
    syndrome[6732] = 1;
    syndrome[6615] = 1;
    syndrome[6733] = 1;
    middleSweepRule(qubits, middleQubitIndices, syndrome, syndIndices, bulkSweepVertices,
                        faceToEdges, faceToVertices, vertexToFaces, 6);
    EXPECT_EQ(qubits, blankQubits);
    EXPECT_EQ(syndrome, blankSyndrome);

    qubits[1806] = 1;
    qubits[1807] = 1;
    qubits[1808] = 1;
    syndrome[2412] = 1;
    syndrome[2413] = 1;
    syndrome[2414] = 1;
    syndrome[2415] = 1;
    syndrome[6732] = 1;
    syndrome[6733] = 1;
    syndrome[6734] = 1;
    syndrome[6015] = 1;
    syndrome[6615] = 1;
    syndrome[6731] = 1;
    middleSweepRule(qubits, middleQubitIndices, syndrome, syndIndices, bulkSweepVertices,
                        faceToEdges, faceToVertices, vertexToFaces, 6);
    EXPECT_EQ(qubits, blankQubits);
    EXPECT_EQ(syndrome, blankSyndrome);
}

//------------------------------------------------------------

TEST(middleBoundarySweepRuleTest, CorrectOutput)
{
    vint qubits(3*5*6*6*6, 0);
    vint syndrome(8*5*6*6*6, 0);
    vint blankQubits(3*5*6*6*6, 0);
    vint nonBlankQubits(3*5*6*6*6, 0);
    vint blankSyndrome(8*5*6*6*6, 0);
    vint nonBlankSyndrome(8*5*6*6*6, 0);

    std::random_device rd{};
    std::mt19937 engine{rd()};
    std::uniform_real_distribution<double> dist(0,1);

    qubits[1722] = 1;
    syndrome[6615] = 1;
    syndrome[6618] = 1;
    middleBoundarySweepRule(qubits, middleQubitIndices, syndrome, syndIndices, 
                                middleBoundarySweepVertices, faceToEdges, faceToVertices, 
                                vertexToFaces, 6, engine, dist);
    EXPECT_EQ(qubits, blankQubits);
    EXPECT_EQ(syndrome, blankSyndrome);

    qubits[1724] = 1;
    syndrome[5899] = 1;
    syndrome[6616] = 1;
    nonBlankQubits[1724] = 1;
    nonBlankQubits[1182] = 1;
    nonBlankSyndrome[6616] = 1;
    nonBlankSyndrome[5902] = 1;
    middleBoundarySweepRule(qubits, middleQubitIndices, syndrome, syndIndices, 
                                middleBoundarySweepVertices, faceToEdges, faceToVertices, 
                                vertexToFaces, 6, engine, dist);
    if (qubits == blankQubits || qubits == nonBlankQubits) SUCCEED();
    else FAIL();
    if (syndrome == blankSyndrome || syndrome == nonBlankSyndrome) SUCCEED();
    else FAIL();
}

//------------------------------------------------------------

TEST(upperSweepRuleTest, CorrectOutput)
{
    vint qubits(3*5*6*6*6, 0);
    vint syndrome(8*5*6*6*6, 0);
    vint blankQubits(3*5*6*6*6, 0);
    vint nonBlankQubits(3*5*6*6*6, 0);
    vint blankSyndrome(8*5*6*6*6, 0);

    qubits[1815] = 1;
    syndrome[6016] = 1;
    syndrome[6017] = 1;
    upperSweepRule(qubits, upperQubitIndices, syndrome, syndIndices, bulkSweepVertices, 
                    faceToEdges, faceToVertices, vertexToFaces, 6);
    EXPECT_EQ(qubits, blankQubits);
    EXPECT_EQ(syndrome, blankSyndrome);

    qubits[1815] = 1;
    qubits[1906] = 1;
    syndrome[6017] = 1;
    syndrome[6018] = 1;
    nonBlankQubits[1815] = 1;
    nonBlankQubits[1906] = 1;
    nonBlankQubits[1907] = 1;
    upperSweepRule(qubits, upperQubitIndices, syndrome, syndIndices, bulkSweepVertices, 
                    faceToEdges, faceToVertices, vertexToFaces, 6);
    EXPECT_EQ(qubits, nonBlankQubits);
    EXPECT_EQ(syndrome, blankSyndrome);
}

//------------------------------------------------------------

TEST(upperBoundarySweepRule1Test, CorrectOutput)
{
    vint qubits(3*5*6*6*6, 0);
    vint syndrome(8*5*6*6*6, 0);
    vint blankQubits(3*5*6*6*6, 0);
    vint blankSyndrome(8*5*6*6*6, 0);

    qubits[2439] = 1;
    syndrome[3252] = 1;
    syndrome[6849] = 1;
    upperBoundarySweepRule1(qubits, upperQubitIndices, syndrome, syndIndices,
                                upperBoundarySweepVertices1, faceToEdges, faceToVertices,
                                vertexToFaces, 6);
    EXPECT_EQ(qubits, blankQubits);
    EXPECT_EQ(syndrome, blankSyndrome);

    qubits[2439] = 1;
    qubits[2440] = 1;
    syndrome[6849] = 1;
    syndrome[6732] = 1;
    syndrome[6734] = 1;
    upperBoundarySweepRule1(qubits, upperQubitIndices, syndrome, syndIndices,
                                upperBoundarySweepVertices1, faceToEdges, faceToVertices,
                                vertexToFaces, 6);
    EXPECT_EQ(qubits, blankQubits);
    EXPECT_EQ(syndrome, blankSyndrome);

    qubits[2440] = 1;
    qubits[2355] = 1;
    syndrome[3136] = 1;
    syndrome[3252] = 1;
    syndrome[6733] = 1;
    syndrome[6734] = 1;
    upperBoundarySweepRule1(qubits, upperQubitIndices, syndrome, syndIndices,
                                upperBoundarySweepVertices1, faceToEdges, faceToVertices,
                                vertexToFaces, 6);
    
    EXPECT_EQ(qubits, blankQubits);
    EXPECT_EQ(syndrome, blankSyndrome);
}

//------------------------------------------------------------

TEST(upperBoundarySweepRule2Test, CorrectOutput)
{
    vint qubits(3*5*6*6*6, 0);
    vint syndrome(8*5*6*6*6, 0);
    vint blankQubits(3*5*6*6*6, 0);
    vint nonBlankQubits(3*5*6*6*6, 0);
    vint blankSyndrome(8*5*6*6*6, 0);
    
    std::random_device rd{};
    std::mt19937 engine{rd()};
    std::uniform_real_distribution<double> dist(0,1);

    qubits[1731] = 1;
    syndrome[5900] = 1;
    upperBoundarySweepRule2(qubits, upperQubitIndices, syndrome, syndIndices, 
                                upperBoundarySweepVertices2, faceToEdges, faceToVertices, 
                                vertexToFaces, 6, engine, dist);
    EXPECT_EQ(qubits, blankQubits);
    EXPECT_EQ(syndrome, blankSyndrome);

    qubits[1905] = 1;
    syndrome[6133] = 1;
    nonBlankQubits[1905] = 1;
    nonBlankQubits[1991] = 1;
    upperBoundarySweepRule2(qubits, upperQubitIndices, syndrome, syndIndices, 
                                upperBoundarySweepVertices2, faceToEdges, faceToVertices, 
                                vertexToFaces, 6, engine, dist);
    if (qubits == blankQubits || qubits == nonBlankQubits) SUCCEED();
    else FAIL();
    EXPECT_EQ(syndrome, blankSyndrome);
}

//------------------------------------------------------------

TEST(distanceToClosestBoundaryTest, CorrectOutput)
{
    vint distInfo1 = {1, 2};
    vint distInfo2 = {1, 1};
    vint distInfo3 = {1, 2};
    vint distInfo4 = {1, 1};
    EXPECT_EQ(distanceToClosestBoundary(1504, 6), distInfo1);
    EXPECT_EQ(distanceToClosestBoundary(634, 6), distInfo2);
    EXPECT_EQ(distanceToClosestBoundary(1503, 6), distInfo3);
    EXPECT_EQ(distanceToClosestBoundary(453, 6), distInfo4);
}

//------------------------------------------------------------

TEST(shortestPathToBTest, CorrectOutput)
{
    vint path1 = {6018, 6133};
    vint path2 = {6133};
    vint path3 = {1813, 1814};
    vint path4 = {1814};
    EXPECT_EQ(shortestPathToB(1504, syndIndices, 6), path1);
    EXPECT_EQ(shortestPathToB(634, syndIndices, 6), path2);
    EXPECT_EQ(shortestPathToB(1503, syndIndices, 6), path3);
    EXPECT_EQ(shortestPathToB(453, syndIndices, 6), path4);
}

//------------------------------------------------------------

TEST(mwpmTest, CorrectOutput)
{
    vint defects;
    vpint expectedMatching;
    std::map<std::pair<int, int>, int> defectPairDistances;

    defects = {1503, 603};
    expectedMatching = {{1503, 603}};
    EXPECT_EQ(mwpm(defects, defectPairDistances, 6), expectedMatching);

    defects = {1503, 603, 245, 1325};
    expectedMatching = {{1503, 603}, {245, 1325}};
    EXPECT_EQ(mwpm(defects, defectPairDistances, 6), expectedMatching);

    defects = {424, 632};
    expectedMatching = {{424, -1}, {632, -1}};
    EXPECT_EQ(mwpm(defects, defectPairDistances, 6), expectedMatching);

    defectPairDistances[{424, 632}] = 1;
    expectedMatching = {{424, 632}};
    EXPECT_EQ(mwpm(defects, defectPairDistances, 6), expectedMatching);
}

//------------------------------------------------------------

TEST(updatePairingTest, CorrectOutput)
{
    vint defects;
    std::map<std::pair<int, int>, int> defectPairDistances;
    std::map<std::pair<int, int>, int> expectedDefectPairDistances;
    
    defects = {603, 1503, 245, 1325};
    defectPairDistances[{603, 1503}] = 1;
    defectPairDistances[{424, 1504}] = 1;
    expectedDefectPairDistances[{603, 1503}] = 1;
    expectedDefectPairDistances[{245, 1325}] = 1;
    updatePairing(defectPairDistances, defects, 6);
    EXPECT_EQ(defectPairDistances, expectedDefectPairDistances);
}

//------------------------------------------------------------

TEST(joinPairTest, CorrectOutput)
{
    vint syndrome(8*5*6*6*6, 0);
    vint blankSyndrome(8*5*6*6*6, 0);

    syndrome[2412] = 1;
    joinPair(1503, 603, syndIndices, vertexToEdges, syndrome, 6);
    EXPECT_EQ(syndrome, blankSyndrome);

    syndrome[2530] = 1;
    joinPair(632, -1, syndIndices, vertexToEdges, syndrome, 6);
    EXPECT_EQ(syndrome, blankSyndrome);
}

//------------------------------------------------------------

TEST(pathToTopTest, CorrectOutput)
{
    vint path1 = {2413, 6615, 6618, 6616};
    vint path2 = {6615, 6618, 6616};
    vint path3 = {6616};
    vint path4 = {};
    EXPECT_EQ(pathToTop(603, 6), path1);
    EXPECT_EQ(pathToTop(1653, 6), path2);
    EXPECT_EQ(pathToTop(1654, 6), path3);
    EXPECT_EQ(pathToTop(-1, 6), path4);
}

//------------------------------------------------------------

TEST(syndromeRepairTest, CorrectOutput)
{
    vint qubits(3*5*6*6*6, 0);
    vint syndrome(8*5*6*6*6, 0);
    vint blankSyndrome(8*5*6*6*6, 0);
    vint nonBlankSyndrome(8*5*6*6*6, 0);

    std::random_device rd{};
    std::mt19937 engine{rd()};
    std::uniform_real_distribution<double> dist(0,1);

    syndrome[5302] = 1;
    syndrome[6016] = 1;
    syndrome[6018] = 1;
    syndrome[6732] = 1;
    nonBlankSyndrome[5300] = 1;
    nonBlankSyndrome[5302] = 1;
    nonBlankSyndrome[6016] = 1;
    nonBlankSyndrome[6018] = 1;

    std::map<std::pair<int, int>, int> defectPairDistances;
    defectPairDistances[{1325, 1683}] = 4;
    syndromeRepair(syndrome, syndIndices, vertexToEdges, edgeToVertices, 
                    defectPairDistances, 6, 2, 0);
    EXPECT_EQ(syndrome, nonBlankSyndrome);
    int distance = defectPairDistances[{276, 634}];
    EXPECT_EQ(distance, 2);
    
    syndrome[5300] = 0;
    syndrome[6732] = 1;
    std::map<std::pair<int, int>, int> defectPairDistances2;
    defectPairDistances2[{1325, 1683}] = 2;
    syndromeRepair(syndrome, syndIndices, vertexToEdges, edgeToVertices, 
                    defectPairDistances2, 6, 2, 0);
    middleSweepRule(qubits, middleQubitIndices, syndrome, syndIndices, bulkSweepVertices, 
                            faceToEdges, faceToVertices, vertexToFaces, 6);
    middleBoundarySweepRule(qubits, middleQubitIndices, syndrome, syndIndices, 
                                middleBoundarySweepVertices, faceToEdges, faceToVertices, 
                                vertexToFaces, 6, engine, dist);
    upperSweepRule(qubits, upperQubitIndices, syndrome, syndIndices, bulkSweepVertices, 
                        faceToEdges, faceToVertices, vertexToFaces, 6);
    upperBoundarySweepRule1(qubits, upperQubitIndices, syndrome, syndIndices, 
                                upperBoundarySweepVertices1, faceToEdges, faceToVertices, 
                                vertexToFaces, 6);
    upperBoundarySweepRule2(qubits, upperQubitIndices, syndrome, syndIndices, 
                                upperBoundarySweepVertices2, faceToEdges, faceToVertices, 
                                vertexToFaces, 6, engine, dist);

    EXPECT_EQ(syndrome, blankSyndrome);
}
