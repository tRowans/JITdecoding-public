#include "decoderToric.h"

vint taxi(int v1, int v2, int L)
{
    coord c1 = indexToCoord(v1, L);
    coord c2 = indexToCoord(v2, L);
    int dx, dy, dz;
    dx = c2.xi[0] - c1.xi[0];
    dy = c2.xi[1] - c1.xi[1];
    dz = c2.xi[2] - c1.xi[2];
    vint dists = {dx, dy, dz};
    return dists;
}

vvint prePath(int v1, int v2, int L)
{
    vint dists = taxi(v1, v2, L);
    int largest;
    if (abs(dists[0]) > abs(dists[1]))
    {
        if (abs(dists[0]) > abs(dists[2])) largest = 0;
        else largest = 2;
    }
    else
    {
        if (abs(dists[1]) > abs(dists[2])) largest = 1;
        else largest = 2;
    }

    vvint info; 
    for (int i=0; i < 3; i++)
    {
        int j = (i + largest) % 3;
        vint info1D = {j, (0 < dists[j]) - (0 > dists[j]), abs(dists[j])};
        info.push_back(info1D);
    }

    return info;
}

vint shortestPath(int v1, int v2, int L, vint &syndIndices)
{
    //paths are constructed only on intermediate edges
    vint path = {};
    
    if (v1 == v2)
    {
        return path;
    }

    vvint info = prePath(v1, v2, L);
    //info for each direction has structure (direction, sign, abs(distance))
    vint &infoLargest = info[0];
    vint &infoOtherA = info[1];
    vint &infoOtherB = info[2];

    if (std::find(syndIndices.begin(), syndIndices.end(), edgeIndex(v1, infoLargest[0], infoLargest[1], L)) == syndIndices.end())
    {
        if (infoOtherA[2] != 0)
        {
            path.push_back(edgeIndex(v1, infoOtherA[0], infoOtherA[1], L));
            v1 = neigh(v1, infoOtherA[0], infoOtherA[1], L);
            infoOtherA[2] -= 1;
        }
        else
        {
            path.push_back(edgeIndex(v1, infoOtherB[0], infoOtherB[1], L));
            v1 = neigh(v1, infoOtherB[0], infoOtherB[1], L);
            infoOtherB[2] -= 1;
        }
    }

    while (infoOtherA[2] > 0)
    {
        path.push_back(edgeIndex(v1, infoLargest[0], infoLargest[1], L));
        v1 = neigh(v1, infoLargest[0], infoLargest[1], L);
        infoLargest[2] -= 1;
        path.push_back(edgeIndex(v1, infoOtherA[0], infoOtherA[1], L));
        v1 = neigh(v1, infoOtherA[0], infoOtherA[1], L);
        infoOtherA[2] -= 1;
    }

    while (infoLargest[2] > 0 && infoOtherB[2] > 0)
    {
        path.push_back(edgeIndex(v1, infoLargest[0], infoLargest[1], L));
        v1 = neigh(v1, infoLargest[0], infoLargest[1], L);
        infoLargest[2] -= 1;
        path.push_back(edgeIndex(v1, infoOtherB[0], infoOtherB[1], L));
        v1 = neigh(v1, infoOtherB[0], infoOtherB[1], L);
        infoOtherB[2] -= 1;
    }

    if (infoLargest[2] == 0)
    {
        while (infoOtherB[2] > 0)
        {
            path.push_back(edgeIndex(v1, infoOtherB[0], infoOtherB[1], L));
            v1 = neigh(v1, infoOtherB[0], infoOtherB[1], L);
            infoOtherB[2] -= 1;
        }
    }
    else
    {   
        while (infoLargest[2] > 0)
        {
            path.push_back(edgeIndex(v1, infoLargest[0], infoLargest[1], L));
            v1 = neigh(v1, infoLargest[0], infoLargest[1], L);
            infoLargest[2] -= 1;
        }
    }

    return path;
}

void sweepRule(vint &qubits, vint &layerQubitIndices, vint &syndrome, vint &syndIndices, vint &sweepVertices, vvint &faceToEdges, int L, std::mt19937& engine, std::uniform_real_distribution<double>& dist)
{
    std::set<int> flipBits;
    for (int v : sweepVertices)
    {
         int xi = edgeIndex(v, x, 1, L);
         int yi = edgeIndex(v, y, 1, L);
         int zi = edgeIndex(v, z, 1, L);

        coord c = indexToCoord(v, L);
        //Modified boundary sweep rule
        if (c.xi[1] == 1)   
        {
            if (syndrome[yi] == 1)  
            {
                if (c.xi[0] == (L - 2)) flipBits.insert(3*v + 2);
                else
                {
                    if (dist(engine) < 0.5) flipBits.insert(3* v);
                    else flipBits.insert(3*v + 2);
                }
            }
        }
        //Bulk Sweep Rule
        else    
        {
            if (syndrome[xi] == 1 && syndrome[yi] == 1) flipBits.insert(3*v);
            else if (syndrome[xi] == 1 && syndrome[zi] == 1) flipBits.insert(3*v + 1);
            else if (syndrome[yi] == 1 && syndrome[zi] == 1) flipBits.insert(3*v + 2);
        }
    }
    for (int i : layerQubitIndices)
    {
        if (std::find(flipBits.begin(), flipBits.end(), i) != flipBits.end())
        {
            qubits[i] = (qubits[i] + 1) % 2;
            vint edges = faceToEdges[i];
            for (int j : edges)
            {
                if (std::find(syndIndices.begin(), syndIndices.end(), j) != syndIndices.end())
                {
                    syndrome[j] = (syndrome[j] + 1) % 2;
                }
            }
        }
    }
}

vint distanceToClosestBoundary(int v, int L)
{
    coord c = indexToCoord(v, L);
    int sign;
    int dist;
   
    if (c.xi[1] >= L/2)
    {
        sign = 1;
        dist = 2*((L-2) - c.xi[1]) - 1;
    }
    else 
    {
        sign = -1;
        dist = 2*(c.xi[1] -1);
    }

    vint distInfo = {sign, dist};
    return distInfo;
}

vint shortestPathToB(int v, int L)   //B = boundary
{
    vint distInfo = distanceToClosestBoundary(v, L);
    int &sign = distInfo[0];
    int &dist = distInfo[1];
    vint path;
    if (sign == 1)
    {
        path.push_back(edgeIndex(v, y, sign, L));
        v = neigh(v, y, sign, L);
        dist -= 1;
    }
    while (dist > 0)
    {
        path.push_back(edgeIndex(v, z, -1*sign, L));
        v = neigh(v, z, -1*sign, L);
        dist -= 1;
        
        path.push_back(edgeIndex(v, y, sign, L));
        v = neigh(v, y, sign, L);
        dist -= 1;
    }
    return path;
}

vpint mwpm(vint &defects, std::map<std::pair<int, int>, int> &defectPairDistances, int L)
{
    vint edges;
    vint weights;
    int nodeNum = defects.size();
    for (int i = 0; i < nodeNum; ++i)
    {
        for (int j = i + 1; j < nodeNum; ++j)
        {
            edges.push_back(i);
            edges.push_back(j);
            if (defectPairDistances.count({defects[i],defects[j]}))
            {
                weights.push_back(defectPairDistances[{defects[i],defects[j]}]);
            }
            else
            {
                vint dists = taxi(defects[i], defects[j], L); 
                weights.push_back(abs(dists[0]) + abs(dists[1]) + abs(dists[2]));
            }
            // Add boundary node edges with wt = 0 so they can be matched for no cost
            edges.push_back(nodeNum + i);
            edges.push_back(nodeNum + j);
            weights.push_back(0);
        }
        // Add edge to boundary node
        edges.push_back(i);
        edges.push_back(nodeNum + i);
        if (defectPairDistances.count({defects[i], -1}))
        {
            weights.push_back(defectPairDistances[{defects[i], -1}]);
        }
        else
        {
            weights.push_back(distanceToClosestBoundary(defects[i], L)[1]);
        }
    }
    int edgeNum = edges.size() / 2;
    struct PerfectMatching::Options options;
    options.verbose = false;
    PerfectMatching *pm;
    pm = new PerfectMatching(2 * nodeNum, edgeNum);
    
    for (int e = 0; e < edgeNum; ++e)
    {
        pm->AddEdge(edges[2*e], edges[2*e+1], weights[e]);
    }
    pm->options = options;
    pm->Solve();
    
    vpint defectPairs;
    for (int i = 0; i < nodeNum; ++i)
    {
        int j = pm->GetMatch(i);
        if (i < j)
        {
            if (j >= nodeNum)
            {
                defectPairs.push_back({defects[i], -1});  //-1 means matched to boundary
            }
            else
            {
                defectPairs.push_back({defects[i], defects[j]});
            }
        }
    }
    delete pm;
    return defectPairs;
}

void updatePairing(std::map<std::pair<int, int>, int> &defectPairDistances, vint &defects, vint &syndIndices, int L)
{
    std::sort(defects.begin(), defects.end()); //need a standard ordering for pairs
    vpint defectPairs = mwpm(defects, defectPairDistances, L); 
    vpint removeThese;
    for (auto &p : defectPairDistances)
    {
        if (std::find(defectPairs.begin(), defectPairs.end(), p.first) == defectPairs.end())
        {
            removeThese.push_back(p.first);
        }
    }
    for (std::pair<int, int> p : removeThese) defectPairDistances.erase(p);
    
    vint dists;
    int distance;

    for (std::pair<int, int> &q : defectPairs)
    {
        if (!defectPairDistances.count(q))
        {
            if (q.second == -1)
            {
                distance = distanceToClosestBoundary(q.first, L)[1];
            }
            else
            {
                dists = taxi(q.first, q.second, L);
                distance = abs(dists[0]) + abs(dists[1]) + abs(dists[2]);
            }
            defectPairDistances[q] = distance; 
        }
    }
}

void joinPair(int v1, int v2, vint &syndIndices, vint &syndrome, int L)
{
    vint path;
    if (v2 == -1)  //If matched to boundary
    {
        path = shortestPathToB(v1, L);
    }
    else
    {
        path = shortestPath(v1, v2, L, syndIndices);
    }
    for (int i : path)
    {
        syndrome[i] = (syndrome[i] + 1) % 2;
    }
}

vint pathToTop(int v, vint &syndIndices, int L)
{
    vint path = {};
    if (v == -1) return path;
    while (std::find(syndIndices.begin(), syndIndices.end(), edgeIndex(v, z, 1, L)) != syndIndices.end())
    {
        path.push_back(edgeIndex(v, z, 1, L));
        v = neigh(v, z, 1, L);
    }
    return path;
}

void syndromeRepair(vint &syndrome, vint &syndIndices, std::map<std::pair<int, int>, int> &defectPairDistances, vpint &edgeToVertices, int L, int cutoff, int reduction)
{
    std::map<std::pair<int, int>, int> newDefectPairDistances;
    for (std::pair<const std::pair<int, int>, int> &p : defectPairDistances)
    {
        std::pair<int, int> newPair;
        vint path1 = pathToTop(p.first.first, syndIndices, L);
        vint path2 = pathToTop(p.first.second, syndIndices, L);
        int joinRange;
        if (cutoff == 0) joinRange = (path1.size() + path2.size());
        else joinRange = cutoff;
        int reduceBy;
        if (reduction == 0) reduceBy = (path1.size() + path2.size());
        else reduceBy = reduction;

        if (p.second <= joinRange)
        {
            joinPair(p.first.first, p.first.second, syndIndices, syndrome, L);
        }
        else
        {
            for (int i : path1) syndrome[i] = (syndrome[i] + 1) % 2;
            //second vertex in edgeToVertices is always the bigger one
            //and hence the one we end on
            newPair.first = edgeToVertices[path1.back()].second;
            if (p.first.second != -1)
            {
                for (int i : path2) syndrome[i] = (syndrome[i] + 1) % 2;
                newPair.second = edgeToVertices[path2.back()].second;
            }
            else newPair.second = -1;
            newDefectPairDistances[newPair] =  p.second - reduceBy;
        }
    }
    defectPairDistances = newDefectPairDistances;
}

//No references on lattice index vectors because we want copies of these
//so that the positions are reset at the start of each run
int oneRun(int L, double p, double q, vvint &faceToEdges, vvint &edgeToFaces, vvint &vertexToEdges, vpint &edgeToVertices, vint lowerQubitIndices, vint middleQubitIndices, vint upperQubitIndices, vint syndIndices, vint defectIndices, vint zLogical, vint sweepVertices, int run, int cutoff, int reduction)
{
    vint qubits(3*5*L*L*L, 0);
    vint syndrome(3*5*L*L*L, 0);
    vint defects = {};

    std::map<std::pair<int, int>, int> defectPairDistances;
    
    std::random_device rd{};
    std::mt19937 engine{rd()};
    std::uniform_real_distribution<double> dist(0,1);
    
    for (int i=0; i < (3*L)/2; i++)
    {
        calcSynd(syndrome, qubits, lowerQubitIndices, middleQubitIndices, upperQubitIndices, 
                    faceToEdges, syndIndices);
        measError(syndrome, q, engine, dist, syndIndices);
        findDefects(syndrome, vertexToEdges, edgeToVertices, defects, defectIndices);
        updatePairing(defectPairDistances, defects, syndIndices, L);
        syndromeRepair(syndrome, syndIndices, defectPairDistances, edgeToVertices, L, 
                        cutoff, reduction);
        findDefects(syndrome, vertexToEdges, edgeToVertices, defects, defectIndices);
        if (defects.size() != 0)
        {
            std::cout << "syndrome repair failed on run " << run << '\n';
            return 2;
        }
        sweepRule(qubits, middleQubitIndices, syndrome, syndIndices, sweepVertices, 
                    faceToEdges, L, engine, dist);
        sweepRule(qubits, upperQubitIndices, syndrome, syndIndices, sweepVertices, 
                    faceToEdges, L, engine, dist);
        //applying data errors only to top face and only at the end is fine for now
        //because we don't care if other qubits get X errors after we measure stabilisers
        //but when we are applying CCZs this will matter
        //because X errors on lower qubits between stab measurement and CCZ application
        //cannot be detected/corrected and will map to CZ errors
        dataError(qubits, p, engine, dist, upperQubitIndices);
        moveIndices(lowerQubitIndices, middleQubitIndices, upperQubitIndices, syndIndices, 
                        defectIndices, zLogical, sweepVertices, L);
    }

    calcSynd(syndrome, qubits, lowerQubitIndices, middleQubitIndices, upperQubitIndices, faceToEdges, syndIndices);
    findDefects(syndrome, vertexToEdges, edgeToVertices, defects, defectIndices);
    updatePairing(defectPairDistances, defects, syndIndices, L);
    for (auto &p : defectPairDistances)
    {
        joinPair(p.first.first, p.first.second, syndIndices, syndrome, L);
    }
    findDefects(syndrome, vertexToEdges, edgeToVertices, defects, defectIndices);
    sweepRule(qubits, middleQubitIndices, syndrome, syndIndices, sweepVertices,
                faceToEdges, L, engine, dist);
    sweepRule(qubits, upperQubitIndices, syndrome, syndIndices, sweepVertices, 
                faceToEdges, L, engine, dist);
    
    //Check return to codespace
    /*
    std::set<int> upperStabs = {};
    for (int f : upperQubitIndices)
    {
        vint edges = faceToEdges[f];
        for (int e : edges)
        {
            if (std::find(syndIndices.begin(), syndIndices.end(), e) != syndIndices.end())
            {
                upperStabs.insert(e);
            }
        }
    }
    calcSynd(syndrome, qubits, lowerQubitIndices, middleQubitIndices, upperQubitIndices, faceToEdges, syndIndices);
    int notEmpty = 0;
    for (int stab : upperStabs)
    {
        if (syndrome[stab] == 1) notEmpty += 1;
    }
    if (notEmpty != 0) std::cout << "Return to codespace failed" << '\n';
    */
    if (checkCorrection(qubits, zLogical))
    {
       return 1;
    }
    else
    {
       return 0;
    } 
}
