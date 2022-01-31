#include "decoderRhombic1.h"

int shortestPathLength(int v1, int v2, int L)
{
    coord c1 = indexToCoord(v1, L);
    coord c2 = indexToCoord(v2, L);
    int dist = 0;
    vint diff = {c2.xi[0] - c1.xi[0],
                 c2.xi[1] - c1.xi[1],
                 c2.xi[2] - c1.xi[2]};
    if (c1.xi[3] == 1)
    {
        if ((diff[0] + diff[1] + diff[2]) > 0) dist -= 1;
        else dist += 1;
    }
    if (c2.xi[3] == 1)
    {
        if((diff[0] + diff[1] + diff[2]) > 0) dist += 1;
        else dist -= 1;
    }
    for (int &i : diff) i = abs(i);
    std::sort(diff.begin(), diff.end());
    if ((diff[0] + diff[1]) > diff[2]) dist += 2*(diff[0] + diff[1]);
    else dist += 2*diff[2];
    return abs(dist); //can be the same if both vertices have the same first 3 coords
}

vint shortestPath(int v1, int v2, vint &syndIndices, vvint &vertexToEdges, int L)
{
    int originalVertex = v1;
    coord c1 = indexToCoord(v1, L);
    coord c2 = indexToCoord(v2, L);
    std::vector<float> diff = {c2.xi[0] - c1.xi[0] + (static_cast<float>(c2.xi[3] - c1.xi[3])/2), 
                               c2.xi[1] - c1.xi[1] + (static_cast<float>(c2.xi[3] - c1.xi[3])/2),
                               c2.xi[2] - c1.xi[2] + (static_cast<float>(c2.xi[3] - c1.xi[3])/2)};
    vint path = {};
    //Need to add 1 to each or xy and -xy look the same
    vint dirs1 = {xy+1, xz+1, yz+1, xyz+1, -1*(xy+1), -1*(xz+1), -1*(yz+1), -1*(xyz+1)};
    vint dirs2 = {xy+1, xz+1, yz+1, -1*(xyz+1)};
    vint dirs3 = {xyz+1, -1*(xy+1), -1*(xz+1), -1*(yz+1)};
    int turnAround = 0;
    while (abs(diff[0]) + abs(diff[1]) + abs(diff[2]) > 0)
    {
        int bestEdge;
        int bestProduct;
        vpint products;
        vint dirs;
        if (c1.xi[3] == 0) dirs = dirs1;
        else 
        {
            if ((c1.xi[0] + c1.xi[1] + c1.xi[2]) % 2 == 0) dirs = dirs2;
            else dirs = dirs3;
        }
        vint edges;
        for (int i = 0; i < dirs.size(); i++)
        {
            int edge;
            if (dirs[i] > 0) edge = edgeIndex(v1, dirs[i]-1, 1, L);
            else edge = edgeIndex(v1, -1*dirs[i]-1, -1, L);
            edges.push_back(edge);
            if (std::find(syndIndices.begin(), syndIndices.end(), edge) != syndIndices.end())
            {
                products.push_back({scalarProduct(diff, dirs[i]), i});
            }
        }
        std::sort(products.begin(), products.end());
        if (turnAround == 1) 
        {
            products.pop_back();
            turnAround = 0;
        }
        bestEdge = products.back().second;
        
        if (std::find(path.begin(), path.end(), edges[bestEdge]) != path.end())
        {
            //Sometimes gets stuck at a dead end and needs to turn around
            //we go back to the previous vertex and rerun then choose the second best edge 
            if (path.back() == edges[bestEdge]) turnAround = 1;
            //If we have gone in a larger loop just abort
            //but I don't think this should happen
            else
            {
                std::string errorMsg = "Loop occurred while building path between vertices " + std::to_string(v1) + " and " + std::to_string(v2) + '\n'+ " (vertices passed to function were " + std::to_string(originalVertex) + " and " + std::to_string(v2) + ")\n";
                throw std::runtime_error(errorMsg);
            }
        }
        if (turnAround == 0) path.push_back(edges[bestEdge]);
        else path.pop_back();
        if (dirs[bestEdge] > 0) v1 = neigh(v1, dirs[bestEdge]-1, 1, L);
        else v1 = neigh(v1, -1*dirs[bestEdge]-1, -1, L);
        c1 = indexToCoord(v1, L);
        diff = {c2.xi[0] - c1.xi[0] + (static_cast<float>(c2.xi[3] - c1.xi[3])/2), 
                c2.xi[1] - c1.xi[1] + (static_cast<float>(c2.xi[3] - c1.xi[3])/2),
                c2.xi[2] - c1.xi[2] + (static_cast<float>(c2.xi[3] - c1.xi[3])/2)};
    }

    return path;
}

void middleSweepRule(vint &qubits, vint &middleQubitIndices, vint &syndrome, vint &syndIndices, vint &bulkSweepVertices, vvint &faceToEdges, vvint &faceToVertices, vvint &vertexToFaces, int L)
{
    //Sweeping in the +xyz direction only
    std::set<int> flipBits;
    vint vertices;
    //Bulk sweep in XYZ
    for (int v : bulkSweepVertices)
    {
        int xyi = 4*v + xy;
        int xzi = 4*v + xz;
        int yzi = 4*v + yz;
        int xyzi = 4*v + xyz;
        coord c = indexToCoord(v, L);
        
        if (c.xi[3] == 0)
        {
            if (syndrome[xyi] == 1 && syndrome[xzi] == 1 && 
                syndrome[yzi] == 1 && syndrome[xyzi] == 1)
            {  
                vertices = {v, neigh(v, xy, 1, L), neigh(v, xyz, 1, L), v + 5*L + 1}; 
                flipBits.insert(findFace(vertices, vertexToFaces, faceToVertices));
                vertices = {v, neigh(v, xz, 1, L), neigh(v, xyz, 1, L), v + 5*L*L + 1};
                flipBits.insert(findFace(vertices, vertexToFaces, faceToVertices));
                vertices = {v, neigh(v, yz, 1, L), neigh(v, xyz, 1, L), v + 5*L*L + 5*L};
                flipBits.insert(findFace(vertices, vertexToFaces, faceToVertices));
            }
            else if (syndrome[xyi] == 1 && syndrome[xyzi] == 1)
            {
                vertices = {v, neigh(v, xy, 1, L), neigh(v, xyz, 1, L), v + 5*L + 1};
                flipBits.insert(findFace(vertices, vertexToFaces, faceToVertices));
            }
            else if (syndrome[xzi] == 1 && syndrome[xyzi] == 1)
            {
                vertices = {v, neigh(v, xz, 1, L), neigh(v, xyz, 1, L), v + 5*L*L + 1};
                flipBits.insert(findFace(vertices, vertexToFaces, faceToVertices));
            }
            else if (syndrome[yzi] == 1 && syndrome[xyzi] == 1)
            {
                vertices = {v, neigh(v, yz, 1, L), neigh(v, xyz, 1, L), v + 5*L*L + 5*L};
                flipBits.insert(findFace(vertices, vertexToFaces, faceToVertices));
            }
            else if (syndrome[xyi] == 1 && syndrome[xzi] == 1)
            {
                vertices = {v, neigh(v, xy, 1, L), neigh(v, xyz, 1, L), v + 5*L + 1};
                flipBits.insert(findFace(vertices, vertexToFaces, faceToVertices));
                vertices = {v, neigh(v, xz, 1, L), neigh(v, xyz, 1, L), v + 5*L*L + 1};
                flipBits.insert(findFace(vertices, vertexToFaces, faceToVertices));
            }
            else if (syndrome[xyi] == 1 && syndrome[yzi] == 1)
            {   
                vertices = {v, neigh(v, xy, 1, L), neigh(v, xyz, 1, L), v + 5*L + 1};
                flipBits.insert(findFace(vertices, vertexToFaces, faceToVertices));
                vertices = {v, neigh(v, yz, 1, L), neigh(v, xyz, 1, L), v + 5*L*L + 5*L};
                flipBits.insert(findFace(vertices, vertexToFaces, faceToVertices));
            }
            else if (syndrome[xzi] == 1 && syndrome[yzi] == 1)
            {
                vertices = {v, neigh(v, xz, 1, L), neigh(v, xyz, 1, L), v + 5*L*L + 1};
                flipBits.insert(findFace(vertices, vertexToFaces, faceToVertices));
                vertices = {v, neigh(v, yz, 1, L), neigh(v, xyz, 1, L), v + 5*L*L + 5*L};
                flipBits.insert(findFace(vertices, vertexToFaces, faceToVertices));
            }
        }
    }
        
    for (int i : middleQubitIndices)
    {
        if (std::find(flipBits.begin(), flipBits.end(), i) != flipBits.end())
        {
            qubits[i] = (qubits[i] + 1) % 2;
            std::vector<int> edges = faceToEdges[i];
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

void middleBoundarySweepRule(vint &qubits, vint &middleQubitIndices, vint &syndrome, vint &syndIndices, vint &middleBoundarySweepVertices, vvint &faceToEdges, vvint &faceToVertices, vvint &vertexToFaces, int L, std::mt19937 &engine, std::uniform_real_distribution<double> &dist)
{
    //special boundary sweep for y=1 intermediate qubits which cannot be flipped by the normal rule
    //only valid if called after normal middle sweep rule
    std::set<int> flipBits;
    vint vertices;
    for (int v : middleBoundarySweepVertices)
    {
        coord c = indexToCoord(v, L);
        if (syndrome[4*v + xyz] == 1)
        {
            if (c.xi[2] == L-3)
            {
                vertices = {v, neigh(v, yz, -1, L), neigh(v, xyz, 1, L), v + 1};
                flipBits.insert(findFace(vertices, vertexToFaces, faceToVertices));
            }
            else
            {
                if (dist(engine) < 0.5) 
                {
                    vertices = {v, neigh(v, yz, -1, L), neigh(v, xyz, 1, L), v + 1};
                    flipBits.insert(findFace(vertices, vertexToFaces, faceToVertices));
                }
                else
                {
                    vertices = {v, neigh(v, xy, -1, L), neigh(v, xyz, 1, L), v + 5*L*L};
                    flipBits.insert(findFace(vertices, vertexToFaces, faceToVertices));
                }
            }
        }
    }
    
    for (int i : middleQubitIndices)
    {
        if (std::find(flipBits.begin(), flipBits.end(), i) != flipBits.end())
        {
            qubits[i] = (qubits[i] + 1) % 2;
            std::vector<int> edges = faceToEdges[i];
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

void upperSweepRule(vint &qubits, vint &upperQubitIndices, vint &syndrome, vint &syndIndices, vint &bulkSweepVertices, vvint &faceToEdges, vvint &faceToVertices, vvint &vertexToFaces, int L)
{
    //Sweeping in the +xyz direction only
    //Requires both middle sweep rule functions to have been run to be valid
    std::set<int> flipBits;
    vint vertices;
    for (int v : bulkSweepVertices)
    {
        int xyi = 4*v + xy;
        int xzi = 4*v + xz;
        int yzi = 4*v + yz;
        int xyzi = 4*v + xyz;
        coord c = indexToCoord(v, L);
        
        if ((c.xi[0] + c.xi[1] + c.xi[2]) % 2 == 0)
        {
            if (syndrome[xyi] == 1 && syndrome[xzi] == 1)
            {
                vertices = {v, neigh(v, xy, 1, L), neigh(v, xz, 1, L), v + 1};
                flipBits.insert(findFace(vertices, vertexToFaces, faceToVertices));
            }
            else if (syndrome[xyi] == 1 && syndrome[yzi] == 1)
            {
                vertices = {v, neigh(v, xy, 1, L), neigh(v, yz, 1, L), v + 5*L};
                flipBits.insert(findFace(vertices, vertexToFaces, faceToVertices));
            }
            else if (syndrome[xzi] == 1 && syndrome[yzi] == 1)
            {
                vertices = {v, neigh(v, xz, 1, L), neigh(v, yz, 1, L), v + 5*L*L};
                flipBits.insert(findFace(vertices, vertexToFaces, faceToVertices));
            }
        }
    }
   
    for (int i : upperQubitIndices)
    {
        if (std::find(flipBits.begin(), flipBits.end(), i) != flipBits.end())
        {
            qubits[i] = (qubits[i] + 1) % 2;
            std::vector<int> edges = faceToEdges[i];
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

void upperBoundarySweepRule1(vint &qubits, vint &upperQubitIndices, vint &syndrome, vint &syndIndices, vint &upperBoundarySweepVertices1, vvint &faceToEdges, vvint &faceToVertices, vvint &vertexToFaces, int L)
{
    //z=L-2 boundary sweep in XY 
    //Only valid if called after all middle + normal top sweep rule
    std::set<int> flipBits;
    vint vertices;
    for (int v : upperBoundarySweepVertices1)
    {
        int xyi = 4*v + xy;
        int xzi = 4*neigh(v, xz, -1, L) + xz;
        int yzi = 4*neigh(v, yz, -1, L) + yz;
        coord c = indexToCoord(v, L);
        if (syndrome[xyi] == 1 && syndrome[xzi] == 1)
        {
            vertices = {v, neigh(v, xy, 1, L), neigh(v, xz, -1, L), v + 5*L - 5*L*L};
            flipBits.insert(findFace(vertices, vertexToFaces, faceToVertices));
        }
        if (syndrome[xyi] == 1 && syndrome[yzi] == 1)
        {
            vertices = {v, neigh(v, xy, 1, L), neigh(v, yz, -1, L), v + 1 - 5*L*L};
            flipBits.insert(findFace(vertices, vertexToFaces, faceToVertices));
        }
        if (syndrome[xzi] == 1 && syndrome[yzi] == 1)
        {
            vertices = {v, neigh(v, xy, 1, L), neigh(v, xz, -1, L), v + 5*L - 5*L*L};
            flipBits.insert(findFace(vertices, vertexToFaces, faceToVertices));
            vertices = {v, neigh(v, xy, 1, L), neigh(v, yz, -1, L), v + 1 - 5*L*L};
            flipBits.insert(findFace(vertices, vertexToFaces, faceToVertices));
        }
    }

    for (int i : upperQubitIndices)
    {
        if (std::find(flipBits.begin(), flipBits.end(), i) != flipBits.end())
        {
            qubits[i] = (qubits[i] + 1) % 2;
            std::vector<int> edges = faceToEdges[i];
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

void upperBoundarySweepRule2(vint &qubits, vint &upperQubitIndices, vint &syndrome, vint &syndIndices, vint &upperBoundarySweepVertices2, vvint &faceToEdges, vvint &faceToVertices, vvint &vertexToFaces, int L, std::mt19937 &engine, std::uniform_real_distribution<double> &dist)
{
    //y=1 and y=L-3 boundary special sweep rules
    //Only valid if called after all other sweep rules
    std::set<int> flipBits;
    vint vertices;
    for (int v : upperBoundarySweepVertices2)
    {
        int xyi = 4*v + xy;
        int xzi = 4*v + xz;
        int yzi = 4*v + yz;
        coord c = indexToCoord(v, L);
        if (c.xi[1] == 1 && syndrome[xyi] == 1)
        {
            vertices = {v, neigh(v, xy, 1, L), neigh(v, xz, 1, L), v + 1};
            flipBits.insert(findFace(vertices, vertexToFaces, faceToVertices));
        }
        else if (c.xi[1] == 1 && syndrome[yzi] == 1)
        {
            vertices = {v, neigh(v, xz, 1, L), neigh(v, yz, 1, L), v + 5*L*L};
            flipBits.insert(findFace(vertices, vertexToFaces, faceToVertices));
        }
        else if (c.xi[1] == L-3 && syndrome[xzi] == 1)
        {
            if (dist(engine) < 0.5 )
            {
                vertices = {v, neigh(v, xy, 1, L), neigh(v, xz, 1, L), v + 1};
                flipBits.insert(findFace(vertices, vertexToFaces, faceToVertices));
            }
            else
            {
                vertices = {v, neigh(v, xz, 1, L), neigh(v, yz, 1, L), v + 5*L*L};
                flipBits.insert(findFace(vertices, vertexToFaces, faceToVertices));
            }
        }
    }
    
    for (int i : upperQubitIndices)
    {
        if (std::find(flipBits.begin(), flipBits.end(), i) != flipBits.end())
        {
            qubits[i] = (qubits[i] + 1) % 2;
            std::vector<int> edges = faceToEdges[i];
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

std::vector<int> distanceToClosestBoundary(int v, int L)
{
    coord c = indexToCoord(v, L);
    vint distInfo;
    int dPlus;  //distance to +y boundary
    int dMinus;  //distance to -y boundary
    
    if (c.xi[3] == 0)
    {
        dPlus = 2*(L - 3 - c.xi[1]) + 1;
        dMinus = 2*(c.xi[1] - 1) - 1;
    }
    else 
    {
        dPlus = 2*(L - 3 - c.xi[1]);
        dMinus = 2*(c.xi[1] - 1);
    }

    if (dPlus > dMinus) distInfo = {-1, dMinus};
    //goes to +y boundary if distances are equal
    //but this choice is not important
    else distInfo = {1, dPlus};

    return distInfo;
}

std::vector<int> shortestPathToB(int v, vint &syndIndices, int L)   //B = boundary
{
    std::vector<int> distInfo = distanceToClosestBoundary(v, L);
    int &sign = distInfo[0];
    int &dist = distInfo[1];
    std::vector<int> path;
    coord c = indexToCoord(v, L);

    if (c.xi[3] == 0)
    {
        //if upper defect
        if (std::find(syndIndices.begin(), syndIndices.end(), 4*v + xyz) == syndIndices.end())
        {
            if (sign == 1)
            {
                v = neigh(v, xz, -1, L);
                path.push_back(4*v + xz);
            }
            else
            {
                v = neigh(v, yz, -1, L);
                path.push_back(4*v + yz);
            }
        }
        //if lower defect
        else
        {
            if (sign == 1)
            {
                path.push_back(4*v + yz);
                v = neigh(v, yz, 1, L);
            }
            else 
            {
                path.push_back(4*v + xz);
                v = neigh(v, xz, 1, L);
            }
        }
        dist -= 1;
        c = indexToCoord(v, L);
    }

    if ((c.xi[0] + c.xi[1] + c.xi[2]) % 2 == 0 && sign == 1)
    {
        while (dist > 0)
        {
            path.push_back(4*v + yz);
            v = neigh(v, yz, 1, L);
            v = neigh(v, xz, -1, L);
            path.push_back(4*v + xz);
            dist -= 2;
        }
    }
    else if ((c.xi[0] + c.xi[1] + c.xi[2]) % 2 == 0 && sign == -1)
    {
        while (dist > 0)
        {
            path.push_back(4*v + xz);
            v = neigh(v, xz, 1, L);
            v = neigh(v, yz, -1, L);
            path.push_back(4*v + yz);
            dist -= 2;
        }
    }
    else if ((c.xi[0] + c.xi[1] + c.xi[2]) % 2 == 1 && sign == 1)
    {
        while (dist > 0)
        {
            v = neigh(v, xz, -1, L);
            path.push_back(4*v + xz);
            path.push_back(4*v + yz);
            v = neigh(v, yz, 1, L);
            dist -= 2;
        }
    }
    else if ((c.xi[0] + c.xi[1] + c.xi[2]) % 2 == 1 && sign == -1)
    {
        while (dist > 0)
        {
            v = neigh(v, yz, -1, L);
            path.push_back(4*v + yz);
            path.push_back(4*v + xz);
            v = neigh(v, xz, 1, L);
            dist -= 2;
        }
    }

    return path;
}

vpint mwpm(std::vector<int> &defects, std::map<std::pair<int, int>, int> &defectPairDistances, int L)
{
    std::vector<int> edges;
    std::vector<int> weights;
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
                weights.push_back(shortestPathLength(defects[i], defects[j], L));
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
    
    std::vector<std::pair<int, int>> defectPairs;
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

void updatePairing(std::map<std::pair<int, int>, int> &defectPairDistances, std::vector<int> &defects, int L)
{
    std::sort(defects.begin(), defects.end()); //need a standard ordering for pairs
    std::vector<std::pair<int, int>> defectPairs = mwpm(defects, defectPairDistances, L); 
    std::vector<std::pair<int, int>> removeThese;
    for (auto &p : defectPairDistances)
    {
        if (std::find(defectPairs.begin(), defectPairs.end(), p.first) == defectPairs.end())
        {
            removeThese.push_back(p.first);
        }
    }
    for (std::pair<int, int> p : removeThese) defectPairDistances.erase(p);
    
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
                distance = shortestPathLength(q.first, q.second, L);
            }
            defectPairDistances[q] = distance; 
        }
    }
}

void joinPair(int v1, int v2, std::vector<int> &syndIndices, vvint &vertexToEdges, std::vector<int> &syndrome, int L)
{
    std::vector<int> path;
    if (v2 == -1)  //If matched to boundary
    {
        path = shortestPathToB(v1, syndIndices, L);
    }
    else
    {
        path = shortestPath(v1, v2, syndIndices, vertexToEdges, L);
    }
    for (int i : path)
    {
        syndrome[i] = (syndrome[i] + 1) % 2;
    }
}

vint pathToTop(int v, int L)
{
    vint path;
    if (v == -1)
    {
        //no deferral for boundaries
        return path;
    }
    coord c = indexToCoord(v, L);
    if (c.xi[3] == 0)
    {
        if (c.xi[2] == L-2)
        {
            v = neigh(v, yz, -1, L);
            path.push_back(4*v + yz);
        }
        else
        {
            path.push_back(4*v + xz);
            v = neigh(v, xz, 1, L);
        }
        c = indexToCoord(v, L);
    }
    if ((c.xi[0] + c.xi[1] + c.xi[2]) % 2 == 1)
    {
        path.push_back(4*v + xyz);
        v = neigh(v, xyz, 1, L);
        v = neigh(v, yz, -1, L);
        path.push_back(4*v + yz);
        c = indexToCoord(v, L);
    }
    if ((c.xi[0] + c.xi[1] + c.xi[2]) % 2 == 0)
    {
        path.push_back(4*v + xy);
    }
    return path;
}

void syndromeRepair(vint &syndrome, vint &syndIndices, vvint &vertexToEdges, vpint &edgeToVertices, std::map<std::pair<int, int>, int> &defectPairDistances, int L, int cutoff, int reduction)
{
    std::map<std::pair<int, int>, int> newDefectPairDistances;
    for (std::pair<const std::pair<int, int>, int> &p : defectPairDistances)
    {
        std::pair<int, int> newPair;
        vint path1 = pathToTop(p.first.first, L);
        vint path2 = pathToTop(p.first.second, L);
        int joinRange;
        if (cutoff == 0) joinRange = (path1.size() + path2.size());
        else joinRange = cutoff;
        int reduceBy;
        if (reduction == 0) reduceBy = (path1.size() + path2.size());
        else reduceBy = reduction;

        if (p.second <= joinRange)
        {
            joinPair(p.first.first, p.first.second, syndIndices, vertexToEdges, syndrome, L);
        }
        else
        {
            for (int i : path1) syndrome[i] = (syndrome[i] + 1) % 2;
            //smaller index is first in edgeToVertices pair so will be w=0 vertex
            //w=0 vertex of final edge in path is where we end
            newPair.first = edgeToVertices[path1.back()].first;
            if (p.first.second != -1)
            {
                for (int i : path2) syndrome[i] = (syndrome[i] + 1) % 2;
                newPair.second = edgeToVertices[path2.back()].first;
            }
            else newPair.second = -1;
            newDefectPairDistances[newPair] = p.second - reduceBy;
        }
    }
    defectPairDistances = newDefectPairDistances;
}

//No references on lattice index vectors because we want copies of these
//so that the positions are reset at the start of each run
int oneRun(int L, double p, double q, vvint &faceToEdges, vvint &edgeToFaces, vvint &faceToVertices, vvint &vertexToFaces, vvint &vertexToEdges, vpint &edgeToVertices, vint lowerQubitIndices, vint middleQubitIndices, vint upperQubitIndices, vint syndIndices, vint defectIndices, vint bulkSweepVertices, vint middleBoundarySweepVertices, vint upperBoundarySweepVertices1, vint upperBoundarySweepVertices2, vint zLogical, int run, int cutoff, int reduction)
{
    std::vector<int> qubits(3*5*L*L*L, 0);
    std::vector<int> syndrome(8*5*L*L*L, 0);
    std::vector<int> defects = {};

    std::map<std::pair<int, int>, int> defectPairDistances;
    
    std::random_device rd{};
    std::mt19937 engine{rd()};
    std::uniform_real_distribution<double> dist(0,1);
    
    for (int i=0; i < (3*L)/2; i++)
    {
        calcSynd(syndrome, qubits, lowerQubitIndices, middleQubitIndices, upperQubitIndices, faceToEdges, syndIndices);
        measError(syndrome, q, engine, dist, syndIndices);
        findDefects(syndrome, vertexToEdges, edgeToVertices, defects, defectIndices);
        updatePairing(defectPairDistances, defects, L);
        syndromeRepair(syndrome, syndIndices, vertexToEdges, edgeToVertices, 
                        defectPairDistances, L, cutoff, reduction);
        findDefects(syndrome, vertexToEdges, edgeToVertices, defects, defectIndices);
        if (defects.size() != 0)
        {
            std::cout << "syndrome repair failed on run " << run << '\n';
            return 2;
        }
        middleSweepRule(qubits, middleQubitIndices, syndrome, syndIndices, bulkSweepVertices, 
                            faceToEdges, faceToVertices, vertexToFaces, L);
        middleBoundarySweepRule(qubits, middleQubitIndices, syndrome, syndIndices, 
                                    middleBoundarySweepVertices, faceToEdges, faceToVertices, 
                                    vertexToFaces, L, engine, dist);
        upperSweepRule(qubits, upperQubitIndices, syndrome, syndIndices, bulkSweepVertices, 
                            faceToEdges, faceToVertices, vertexToFaces, L);
        upperBoundarySweepRule1(qubits, upperQubitIndices, syndrome, syndIndices, 
                                    upperBoundarySweepVertices1, faceToEdges, faceToVertices, 
                                    vertexToFaces, L);
        upperBoundarySweepRule2(qubits, upperQubitIndices, syndrome, syndIndices, 
                                    upperBoundarySweepVertices2, faceToEdges, faceToVertices, 
                                    vertexToFaces, L, engine, dist);
        dataError(qubits, p, engine, dist, upperQubitIndices);
        moveIndices(lowerQubitIndices, middleQubitIndices, upperQubitIndices, syndIndices, defectIndices, bulkSweepVertices, middleBoundarySweepVertices, upperBoundarySweepVertices1, upperBoundarySweepVertices2, zLogical, vertexToFaces, faceToVertices, L);
    }

    calcSynd(syndrome, qubits, lowerQubitIndices, middleQubitIndices, upperQubitIndices, faceToEdges, syndIndices);
    findDefects(syndrome, vertexToEdges, edgeToVertices, defects, defectIndices);
    updatePairing(defectPairDistances, defects, L);
    for (auto &p : defectPairDistances)
    {
        joinPair(p.first.first, p.first.second, syndIndices, vertexToEdges, syndrome, L);
    }
    findDefects(syndrome, vertexToEdges, edgeToVertices, defects, defectIndices);
    middleSweepRule(qubits, middleQubitIndices, syndrome, syndIndices, bulkSweepVertices, 
                            faceToEdges, faceToVertices, vertexToFaces, L);
    middleBoundarySweepRule(qubits, middleQubitIndices, syndrome, syndIndices, 
                                middleBoundarySweepVertices, faceToEdges, faceToVertices, 
                                vertexToFaces, L, engine, dist);
    upperSweepRule(qubits, upperQubitIndices, syndrome, syndIndices, bulkSweepVertices, 
                        faceToEdges, faceToVertices, vertexToFaces, L);
    upperBoundarySweepRule1(qubits, upperQubitIndices, syndrome, syndIndices, 
                                upperBoundarySweepVertices1, faceToEdges, faceToVertices, 
                                vertexToFaces, L);
    upperBoundarySweepRule2(qubits, upperQubitIndices, syndrome, syndIndices, 
                                upperBoundarySweepVertices2, faceToEdges, faceToVertices, 
                                vertexToFaces, L, engine, dist);

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
