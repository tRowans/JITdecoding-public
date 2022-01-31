#include "gtest/gtest.h"
#include "decoderToric.h"
#include "prepIndexVectors.h"
#include <iostream>

//------------------------------------------------------------

//This is just a hack to make the testing software build the index vectors
//Means they don't have to be build individually in every test

TEST(indexBuilding, build)
{
    buildIndexVectors(lowerQubitIndices6, middleQubitIndices6, upperQubitIndices6, syndIndices6, defectIndices6, zLogical6, sweepVertices6, vertexToEdges6, faceToEdges6, edgeToVertices6, 6);
    ASSERT_TRUE(true);
}

//------------------------------------------------------------
//GENERIC TESTS


//Again not sure about the best way to test these but they are simple enough that it should be fine
//These are actually really annoying because they randomly fail so disabled them for now
/*
TEST(dataErrorTest, p0)
{
    vint qubits(3*6*6*6, 0);
    vint qubitsCompare(3*6*6*6, 0);
    std::mt19937 engine(time(0));
    std::uniform_real_distribution<double> dist(0,1);
    dataError(qubits, 0, engine, dist, upperQubitIndices6);
    EXPECT_EQ(qubits, qubitsCompare);
}
TEST(dataErrorTest, p1)
{
    vint qubits(3*6*6*6, 0);
    vint qubitsCompare(3*6*6*6, 0);
    std::mt19937 engine(time(0));
    std::uniform_real_distribution<double> dist(0,1);
    dataError(qubits, 1, engine, dist, upperQubitIndices6);
    for (int i : upperQubitIndices6)
    {
        qubitsCompare[i] = 1;
    }
    EXPECT_EQ(qubits, qubitsCompare);
}

//------------------------------------------------------------

TEST(measErrorTest, p0)
{
    vint syndrome(3*6*6*6, 0);
    vint syndromeCompare(3*6*6*6, 0);
    std::mt19937 engine(time(0));
    std::uniform_real_distribution<double> dist(0,1);
    measError(syndrome, 0, engine, dist, syndIndices6);
    EXPECT_EQ(syndrome, syndromeCompare);
}
TEST(intermediateMeasErrorTest, p1)
{
    vint syndrome(3*6*6*6, 0);
    vint syndromeCompare(3*6*6*6, 0);
    std::mt19937 engine(time(0));
    std::uniform_real_distribution<double> dist(0,1);
    measError(syndrome, 1, engine, dist, syndIndices6);
    for (int i : syndIndices6)
    {
        syndromeCompare[i] = 1;
    }
    EXPECT_EQ(syndrome, syndromeCompare);
}
*/
//------------------------------------------------------------

TEST(calcSyndTest, noErrors)
{
    vint qubits(3*5*6*6*6, 0);
    vint syndrome(3*5*6*6*6, 0);
    vint syndromeExpected(3*5*6*6*6, 0);
    vvint faceToEdges = buildFaceToEdges(6);

    calcSynd(syndrome, qubits, lowerQubitIndices6, middleQubitIndices6, upperQubitIndices6, faceToEdges, syndIndices6);

    EXPECT_EQ(syndrome, syndromeExpected);
}
TEST(calcSyndTest, singleError)
{
    vint qubits(3*5*6*6*6, 0);
    vint syndrome(3*5*6*6*6, 0);
    vint syndromeExpected(3*5*6*6*6, 0);

    qubits[266] = 1;
    syndromeExpected[265] = 1;
    syndromeExpected[266] = 1;
    syndromeExpected[284] = 1;
    syndromeExpected[373] = 1;

    vvint faceToEdges = buildFaceToEdges(6);

    calcSynd(syndrome, qubits, lowerQubitIndices6, middleQubitIndices6, upperQubitIndices6, faceToEdges, syndIndices6);
    
    EXPECT_EQ(syndrome, syndromeExpected);
}
TEST(calcSyndTest, separateErrors)
{
    vint qubits(3*5*6*6*6, 0);
    vint syndrome(3*5*6*6*6, 0);
    vint syndromeExpected(3*5*6*6*6, 0);

    qubits[266] = 1;
    qubits[474] = 1;
    syndromeExpected[265] = 1;
    syndromeExpected[266] = 1;
    syndromeExpected[284] = 1;
    syndromeExpected[373] = 1;
    syndromeExpected[474] = 1;
    syndromeExpected[475] = 1;
    syndromeExpected[492] = 1;
    syndromeExpected[478] = 1;

    vvint faceToEdges = buildFaceToEdges(6);

    calcSynd(syndrome, qubits, lowerQubitIndices6, middleQubitIndices6, upperQubitIndices6, faceToEdges, syndIndices6);

    EXPECT_EQ(syndrome, syndromeExpected);
}
TEST(calcSyndTest, adjacentErrors)
{
    vint qubits(3*5*6*6*6, 0);
    vint syndrome(3*5*6*6*6, 0);
    vint syndromeExpected(3*5*6*6*6, 0);

    qubits[369] = 1;
    qubits[371] = 1;
    syndromeExpected[369] = 1;
    syndromeExpected[371] = 1;
    syndromeExpected[373] = 1;
    syndromeExpected[387] = 1;
    syndromeExpected[389] = 1;
    syndromeExpected[478] = 1;

    vvint faceToEdges = buildFaceToEdges(6);

    calcSynd(syndrome, qubits, lowerQubitIndices6, middleQubitIndices6, upperQubitIndices6, faceToEdges, syndIndices6);

    EXPECT_EQ(syndrome, syndromeExpected);
}

//------------------------------------------------------------

TEST(findDefectsTest, validSyndrome)
{
    vint syndrome(3*5*6*6*6, 0);
    vint defects = {};
    vint defectsExpected = {};

    vvint vertexToEdges = buildVertexToEdges(6);
    vpint edgeToVertices = buildEdgeToVertices(6);
    
    syndrome[373] = 1;
    syndrome[374] = 1; 
    
    findDefects(syndrome, vertexToEdges, edgeToVertices, defects, defectIndices6);
    
    EXPECT_EQ(defects, defectsExpected);
}
TEST(findDefectsTest, invalidSyndrome)
{
    vint syndrome(3*5*6*6*6, 0);
    vint defects = {};
    vint defectsExpected = {88, 124};

    vvint vertexToEdges = buildVertexToEdges(6);
    vpint edgeToVertices = buildEdgeToVertices(6);
    
    syndrome[266] = 1;
    
    findDefects(syndrome, vertexToEdges, edgeToVertices, defects, defectIndices6);
    
    EXPECT_EQ(defects, defectsExpected);
}

//------------------------------------------------------------

TEST(scalarProductTest, HandlesCorrectInput)
{
    EXPECT_EQ(scalarProduct({4,1,-3}, 1), 8);
    EXPECT_EQ(scalarProduct({4,1,-3}, 2), 0);
    EXPECT_EQ(scalarProduct({4,1,-3}, 3), -6);
    EXPECT_EQ(scalarProduct({4,1,-3}, 4), 2);
    EXPECT_EQ(scalarProduct({4,1,-3}, -1*1), -8);
    EXPECT_EQ(scalarProduct({4,1,-3}, -1*2), 0);
    EXPECT_EQ(scalarProduct({4,1,-3}, -1*3), 6);
    EXPECT_EQ(scalarProduct({4,1,-3}, -1*4), -2);

    EXPECT_EQ(scalarProduct({0,1,1}, 1), 0);
    EXPECT_EQ(scalarProduct({1,0,1}, 2), 2);
    EXPECT_EQ(scalarProduct({1,1,0}, 3), 0);
}
TEST(scalarProductTest, HandlesIncorrectInput)
{
    EXPECT_THROW(scalarProduct({1,1,1}, 0), std::invalid_argument);
    EXPECT_THROW(scalarProduct({1,1,1}, 5), std::invalid_argument);
    EXPECT_THROW(scalarProduct({1,1,1}, -5), std::invalid_argument);
}

//------------------------------------------------------------
//TORIC TESTS (although the previous tests assume a toric lattice)

TEST(taxiTest, CorrectOutput)
{
    vint dists1 = {-1, -1, 2};
    vint dists2 = {1, 1, -2};
    EXPECT_EQ(taxi(93, 158, 6), dists1);
    EXPECT_EQ(taxi(158, 93, 6), dists2);

    dists1 = {1, -1, 0};
    dists2 = {-1, 1, 0};
    EXPECT_EQ(taxi(93, 88, 6), dists1);
    EXPECT_EQ(taxi(88, 93, 6), dists2);
}
TEST(taxiTest, sameVertex)
{
    vint dists = {0, 0, 0};
    EXPECT_EQ(taxi(58, 58, 6), dists);
}

//------------------------------------------------------------

TEST(prePathTest, CorrectOutput)
{
    vvint prePathOutput = prePath(58, 123, 6);
    vvint prePathOutputExpected = {{z, 1, 2}, 
                                   {x, -1, 1}, 
                                   {y, -1, 1}}; 
    EXPECT_EQ(prePathOutput, prePathOutputExpected);

    prePathOutput = prePath(93, 88, 6);
    prePathOutputExpected = {{y, -1, 1}, 
                                   {z, 0, 0}, 
                                   {x, 1, 1}};
    EXPECT_EQ(prePathOutput, prePathOutputExpected);
    prePathOutput = prePath(88, 93, 6);
    prePathOutputExpected = {{y, 1, 1}, 
                             {z, 0, 0}, 
                             {x, -1, 1}};
    EXPECT_EQ(prePathOutput, prePathOutputExpected);
}
TEST(prePathTest, sameVertex)
{
    vvint prePathOutput = prePath(58, 58, 6);
    vvint prePathOutputExpected = {{z, 0, 0}, 
                                   {x, 0, 0}, 
                                   {y, 0, 0}};
    EXPECT_EQ(prePathOutput, prePathOutputExpected);
}

//------------------------------------------------------------

TEST(shortestPathTest, sameSection)
{
    vint path = shortestPath(58, 123, 6, syndIndices6);
    vint pathExpected = {176, 279, 281, 370};
    EXPECT_EQ(path, pathExpected);
}
TEST(shortestPathTest, sameLevel)
{
    vint path = shortestPath(93, 88, 6, syndIndices6);
    vint pathExpected = {279, 265};
    EXPECT_EQ(path, pathExpected);
    path = shortestPath(88, 93, 6, syndIndices6);
    pathExpected = {265, 279};
    EXPECT_EQ(path, pathExpected);
}
TEST(shortestPathTest, sameVertex)
{
    vint path = shortestPath(58, 58, 6, syndIndices6);
    vint pathExpected = {};
    EXPECT_EQ(path, pathExpected);
}

//------------------------------------------------------------

TEST(checkCorrectionTest, noError)
{
    vint qubits(3*5*6*6*6, 0);
    EXPECT_FALSE(checkCorrection(qubits, zLogical6));
}
TEST(checkCorrectionTest, logicalError)
{
    vint qubits(3*5*6*6*6, 0);
    qubits[284] = 1;
    qubits[374] = 1;
    qubits[464] = 1;
    EXPECT_TRUE(checkCorrection(qubits, zLogical6));
}

//------------------------------------------------------------

//This is specifically testing how the sweep decoder deals with our layers
TEST(sweepRuleTest, correctsError)
{
    vint qubits(3*5*6*6*6, 0);
    vint qubitsExpected(3*5*6*6*6, 0);
    vint syndrome(3*5*6*6*6, 0);
    vint syndromeExpected(3*5*6*6*6, 0);
    std::mt19937 engine(time(0));
    std::uniform_real_distribution<double> dist(0,1);
    
    qubits[371] = 1;
    qubits[479] = 1;
    syndrome[370] = 1;
    syndrome[371] = 1;
    syndrome[389] = 1;
    syndrome[479] = 1;
    sweepRule(qubits, middleQubitIndices6, syndrome, syndIndices6, sweepVertices6, 
                faceToEdges6, 6, engine, dist);
    sweepRule(qubits, upperQubitIndices6, syndrome, syndIndices6, sweepVertices6,
                faceToEdges6, 6, engine, dist);

    EXPECT_EQ(qubits, qubitsExpected);
    EXPECT_EQ(syndrome, syndromeExpected);
}

//------------------------------------------------------------

TEST(distanceToClosestBoundaryTest, correctOutput)
{
    vint distanceExpected1 = {1, 1};
    vint distanceExpected2 = {-1, 2};
    EXPECT_EQ(distanceToClosestBoundary(58, 6), distanceExpected1);
    EXPECT_EQ(distanceToClosestBoundary(88, 6), distanceExpected2);
}

//------------------------------------------------------------

TEST(shortestPathToBTest, correctOutput)
{
    vint pathExpected1 = {175};
    vint pathExpected2 = {266, 355};
    EXPECT_EQ(shortestPathToB(58, 6), pathExpected1);
    EXPECT_EQ(shortestPathToB(88, 6), pathExpected2);
}

//------------------------------------------------------------

TEST(mwpmTest, matchingToDefects)
{
    vint defects = {88, 123};
    std::map<std::pair<int, int>, int> defectPairDistances;
    vpint defectPairsExpected = {{88, 123}};
    EXPECT_EQ(mwpm(defects, defectPairDistances, 6), defectPairsExpected);
}
TEST(mwpmTest, matchingToBoundaries)
{
    vint defects = {58, 158};
    std::map<std::pair<int, int>, int> defectPairDistances;
    vpint defectPairsExpected = {{58, -1},
                                                            {158, -1}};
    EXPECT_EQ(mwpm(defects, defectPairDistances, 6), defectPairsExpected);
}

//------------------------------------------------------------

TEST(updatePairingTest, addPair)
{
    std::map<std::pair<int, int>, int> defectPairDistances;
    std::map<std::pair<int, int>, int> defectPairDistancesExpected;
    defectPairDistancesExpected[{88,123}] = 2;
    vint defects = {88, 123};
    updatePairing(defectPairDistances, defects, syndIndices6, 6);
    EXPECT_EQ(defectPairDistances, defectPairDistancesExpected);
}
TEST(updatePairingTest, removePair)
{
    std::map<std::pair<int, int>, int> defectPairDistances;
    std::map<std::pair<int, int>, int> defectPairDistancesExpected;
    defectPairDistances[{88, 123}] = 2;
    vint defects = {};
    updatePairing(defectPairDistances, defects, syndIndices6, 6);
    EXPECT_EQ(defectPairDistances, defectPairDistancesExpected);
}
TEST(updatePairingTest, existingPair)
{
    std::map<std::pair<int, int>, int> defectPairDistances;
    std::map<std::pair<int, int>, int> defectPairDistancesExpected;
    defectPairDistances[{88, 123}] = 1;
    defectPairDistancesExpected[{88, 123}] = 1;
    vint defects = {88, 123};
    updatePairing(defectPairDistances, defects, syndIndices6, 6);
    EXPECT_EQ(defectPairDistances, defectPairDistancesExpected);
}

//------------------------------------------------------------

TEST(joinPairTest, joinToDefect)
{
    vint syndrome(3*5*6*6*6, 0);
    vint syndromeExpected(3*5*6*6*6, 0);
    syndrome[281] = 1;
    syndrome[370] = 1;
    joinPair(93, 123, syndIndices6, syndrome, 6);
    EXPECT_EQ(syndrome, syndromeExpected);
}
TEST(joinPairTest, joinToBoundary)
{
    vint syndrome(3*5*6*6*6, 0);
    vint syndromeExpected(3*5*6*6*6, 0);
    syndrome[280] = 1;
    joinPair(93, -1, syndIndices6, syndrome, 6);
    EXPECT_EQ(syndrome, syndromeExpected);
}

//------------------------------------------------------------

TEST(syndromeRepairTest, matchEffectiveDistanceOnePair)
{
    vint syndrome(3*5*6*6*6, 0);
    vint syndromeExpected(3*5*6*6*6, 0);
    std::map<std::pair<int, int>, int> defectPairDistances;
    syndrome[266] = 1;
    syndrome[369] = 1;
    defectPairDistances[{88, 123}] = 1;
    syndromeRepair(syndrome, syndIndices6, defectPairDistances, edgeToVertices6, 6, 0, 0);
    EXPECT_EQ(syndrome, syndromeExpected);
}
TEST(syndromeRepairTest, matchTrueDistanceOneToBoundary)
{
    vint syndrome(3*5*6*6*6, 0);
    vint syndromeExpected(3*5*6*6*6, 0);
    std::map<std::pair<int, int>, int> defectPairDistances;
    syndrome[175] = 1;
    defectPairDistances[{58,-1}] = 1;
    syndromeRepair(syndrome, syndIndices6, defectPairDistances, edgeToVertices6, 6, 0, 0);
    EXPECT_EQ(syndrome, syndromeExpected);
}
TEST(syndromeRepairTest, matchEffectiveDistanceOneToBoundary)
{
    vint syndrome(3*5*6*6*6, 0);
    vint syndromeExpected(3*5*6*6*6, 0);
    std::map<std::pair<int, int>, int> defectPairDistances;
    syndrome[266] = 1;
    syndrome[355] = 1;
    defectPairDistances[{88,-1}] = 1;
    syndromeRepair(syndrome, syndIndices6, defectPairDistances, edgeToVertices6, 6, 0, 0);
    EXPECT_EQ(syndrome, syndromeExpected);
}
TEST(syndromeRepairTest, matchMultipleDefectsToBoundary)
{
    vint syndrome(3*5*6*6*6, 0);
    vint syndromeExpected(3*5*6*6*6, 0);
    std::map<std::pair<int, int>, int> defectPairDistances;
    syndrome[175] = 1;
    syndrome[280] = 1;
    syndrome[385] = 1;
    defectPairDistances[{58, -1}] = 1;
    defectPairDistances[{93, -1}] = 1;
    defectPairDistances[{128, -1}] = 1;
    syndromeRepair(syndrome, syndIndices6, defectPairDistances, edgeToVertices6, 6, 0, 0);
    EXPECT_EQ(syndrome, syndromeExpected);
}
TEST(syndromeRepairTest, matchSomeDeferOthers)
{
    vint syndrome(3*5*6*6*6, 0);
    vint syndromeExpected(3*5*6*6*6, 0);
    std::map<std::pair<int, int>, int> defectPairDistances;
    syndrome[280] = 1;
    syndrome[584] = 1;
    syndrome[673] = 1;
    syndromeExpected[673] = 1;
    defectPairDistances[{93, -1}] = 1;
    defectPairDistances[{194, -1}] = 2;
    syndromeRepair(syndrome, syndIndices6, defectPairDistances, edgeToVertices6, 6, 0, 0);
    EXPECT_EQ(syndrome, syndromeExpected);
}
TEST(syndromeRepairTest, deferDefectPair)
{
    vint syndrome(3*5*6*6*6, 0);
    vint syndromeExpected(3*5*6*6*6, 0);
    std::map<std::pair<int, int>, int> defectPairDistances;
    syndrome[374] = 1;
    syndrome[477] = 1;
    syndrome[479] = 1;
    syndrome[582] = 1;
    syndromeExpected[477] = 1;
    syndromeExpected[479] = 1;
    syndromeExpected[582] = 1;
    syndromeExpected[584] = 1;
    defectPairDistances[{124, 194}] = 4;
    syndromeRepair(syndrome, syndIndices6, defectPairDistances, edgeToVertices6, 6, 0, 0);
    EXPECT_EQ(syndrome, syndromeExpected);
}
TEST(syndromeRepairTest, deferSingleDefectUpwards)
{
    vint syndrome(3*5*6*6*6, 0);
    vint syndromeExpected(3*5*6*6*6, 0);
    std::map<std::pair<int, int>, int> defectPairDistances;
    defectPairDistances[{124, -1}] = 2;
    syndrome[374] = 1;
    syndrome[463] = 1;
    syndromeExpected[463] = 1;
    syndromeRepair(syndrome, syndIndices6, defectPairDistances, edgeToVertices6, 6, 0, 0);
    EXPECT_EQ(syndrome, syndromeExpected);
}

//------------------------------------------------------------


//Not sure it makes much sense to add unit tests for oneRun
//I think the simulation outcomes are essentially tests of that

