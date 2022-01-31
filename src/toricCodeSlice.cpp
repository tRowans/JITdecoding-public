#include "toricCodeSlice.h"

int coordToIndex(coord c, int L)
{
    if (0 <= c.xi[0] && c.xi[0] < L && 
        0 <= c.xi[1] && c.xi[1] < L && 
        0 <= c.xi[2] && c.xi[2] < 5*L)
    {
        return (c.xi[0] + c.xi[1] * L + c.xi[2] * L * L);
    }
    else
    {
        throw std::invalid_argument("Coord out of range");
    }
}

coord indexToCoord(int i, int L)
{
    if (0 <= i && i < 5*L*L*L)
    {
        coord c;
        c.xi[0] = i % L;
        c.xi[1] = (int)floor(i / L) % L;
        c.xi[2] = (int)floor(i / (L * L)) % (5*L);
        return c;
    }
    else
    {
        throw std::invalid_argument("Index out of range");
    }
}

int neigh(int v, int dir, int sign, int L)
{
    coord c = indexToCoord(v, L);
    if (sign > 0)
    {
        if (dir == 0) c.xi[0] = (c.xi[0] + 1) % L;
        else if (dir == 1) c.xi[1] = (c.xi[1] + 1) % L;
        else if (dir == 2) c.xi[2] = (c.xi[2] + 1) % (5*L);
    }
    else if (sign < 0)
    {
        if (dir == 0) c.xi[0] = (c.xi[0] - 1 + L) % L;
        else if (dir == 1) c.xi[1] = (c.xi[1] - 1 + L) % L;
        else if (dir == 2) c.xi[2] = (c.xi[2] - 1 + 5*L) % (5*L);
    }
    return coordToIndex(c, L);
}

int edgeIndex(int v, int dir, int sign, int L)
{
    if (sign < 0)
    {
        v = neigh(v, dir, sign, L);
    }
    if (dir == x)
        return 3 * v;
    else if (dir == y)
        return 3 * v + 1;
    else if (dir == z)
        return 3 * v + 2;
    else
        throw std::invalid_argument("Invalid direction");
}

vvint buildFaceToEdges(int L)
{
    vvint faceToEdges;
    for (int f = 0; f < 3 * 5 * L * L * L; ++f)
    {
        int v = f / 3;
        int dir = f % 3;
        if (dir == 0)
        {
            faceToEdges.push_back({edgeIndex(v, x, 1, L), edgeIndex(v, y, 1, L), edgeIndex(neigh(v, x, 1, L), y, 1, L), edgeIndex(neigh(v, y, 1, L), x, 1, L)});
        }
        else if (dir == 1)
        {
            faceToEdges.push_back({edgeIndex(v, x, 1, L), edgeIndex(v, z, 1, L), edgeIndex(neigh(v, x, 1, L), z, 1, L), edgeIndex(neigh(v, z, 1, L), x, 1, L)});
        }
        else if (dir == 2)
        {
            faceToEdges.push_back({edgeIndex(v, y, 1, L), edgeIndex(v, z, 1, L), edgeIndex(neigh(v, y, 1, L), z, 1, L), edgeIndex(neigh(v, z, 1, L), y, 1, L)});
        }
    }
    return faceToEdges;
}

/*
CURRENTLY UNUSED
vvint listToBin(vvint& in, int L)
{
    vvint bin;
    for (auto const& vec : in)
    {   
        vint row(3 * L * L * L, 0);
        for (auto const& elem : vec)
        {
            row[elem] = 1;
        }
        bin.push_back(row);
    }
    return bin;
}
*/

vvint buildEdgeToFaces(int L)
{
    vvint edgeToFaces;
    for (int e = 0; e < 3 * 5 * L * L * L; ++e)
    {
        vint faces;
        int v = e / 3;
        int dir = e % 3;

        if (dir == x)
        {
            faces.push_back(3 * v); // xy
            faces.push_back(3 * v + 1); // xz
            faces.push_back(3 * neigh(v, y, -1, L)); // x,-y
            faces.push_back(3 * neigh(v, z, -1, L) + 1); // x,-z
        }
        else if (dir == y)
        {
            faces.push_back(3 * v); // xy
            faces.push_back(3 * v + 2); // yz
            faces.push_back(3 * neigh(v, x, -1, L)); // -x,y
            faces.push_back(3 * neigh(v, z, -1, L) + 2); // y,-z
        }
        else if (dir == z)
        {
            faces.push_back(3 * v + 2); // yz
            faces.push_back(3 * v + 1); // xz
            faces.push_back(3 * neigh(v, y, -1, L) + 2); // -y,z
            faces.push_back(3 * neigh(v, x, -1, L) + 1); // -x,z
        }

        edgeToFaces.push_back(faces);
    }
    return edgeToFaces;
}

vvint buildVertexToEdges(int L)
{
    vvint vertexToEdges;
    for (int v = 0; v < 5 * L * L * L; ++v)
    {
        vint edges;
        edges.push_back(edgeIndex(v, x, 1, L));
        edges.push_back(edgeIndex(v, y, 1, L));
        edges.push_back(edgeIndex(v, z, 1, L));
        edges.push_back(edgeIndex(v, x, -1, L));
        edges.push_back(edgeIndex(v, y, -1, L));
        edges.push_back(edgeIndex(v, z, -1, L));
        vertexToEdges.push_back(edges);
    }
    return vertexToEdges;
}

vpint buildEdgeToVertices(int L)
{
    vpint edgeToVertices;
    for (int e = 0; e < 3 * 5 * L * L * L; ++e)
    {
        int v1 = e / 3;
        int dir = e % 3;
        int v2 = neigh(v1, dir, 1, L);
        edgeToVertices.push_back({v1, v2});
    }
    return edgeToVertices;
}

//Don't need this one anymore but leave it here just in case
/*
vvint buildAdjacentFaces(vvint &faceToEdges, vpint &edgeToVertices, vvint &vertexToEdges, vvint &edgeToFaces, int L)
//Adjacency includes touched at vertices
{
    vvint adjacentFaces;
    for (int i = 0; i < 3*L*L*L; ++i)
    {
        int v = i / 3;
        std::set<int> faces;
        std::set<int> edges;
        std::set<int> vertices;
        for (int j : faceToEdges[i])
        {
            edges.insert(j);
        }
        for (int j : edges)
        {
            vertices.insert(edgeToVertices[j].first);
            vertices.insert(edgeToVertices[j].second);
        }
        for (int j : vertices)
        {
            for (int k : vertexToEdges[j])
            {
                edges.insert(k);
            }
        }
        for (int j : edges)
        {
            for (int k : edgeToFaces[j])
            {
                faces.insert(k);
            }
        }
        faces.erase(i);
        vint facesVec;
        for (int j : faces)
        {
            facesVec.push_back(j);
        }
        adjacentFaces.push_back(facesVec);
    }
    return adjacentFaces;
}
*/

//NOT CURRENTLY IN USE
/*
void printindex(vint &bin)
{
    std::cout << "[";
    bool trivial = true;
    for (int i = 0; i < bin.size(); ++i)
    {
        if (bin[i] == 1)
        {
             std::cout << i << ", ";
            trivial = false;
        }
    }
    if (trivial)
    {
        std::cout << "]" << std::endl;
    }
    else 
    {
        std::cout << "\b\b]" << std::endl;
    }
}
*/

//DOING THIS IN MOVE INDICES NOW
/*
vvint buildZLogicals(int L)
{
    vint zLower;
    vint zUpper;
    int v = L*L - 2*L - 2;
    zLower.push_back(3*v + 2);
    for (int i = 0; i < (L - 4); ++i)
    {
        v = neigh(v, z, 1, L);
        v = neigh(v, x, -1, L);
        zLower.push_back(3*v);
        zLower.push_back(3*v + 2);
    }

    for (int i : zLower)
    {
        zUpper.push_back(i + 6*L*L);
    }
    
    vvint logicals = {zLower, zUpper};
    return logicals;
}
*/

void buildIndexVectors (vint &lowerQubitIndices, vint &middleQubitIndices, vint &upperQubitIndices, vint &syndIndices, vint &defectIndices, vint &zLogical, vint &sweepVertices, vvint &vertexToEdges, vvint &faceToEdges, vpint &edgeToVertices, int L)
{
    vint generatorVertices = {L*L-2*L-2};
    coord c;
    vint edges;
    while (!generatorVertices.empty())
    {
        //Lower qubits
        int v = generatorVertices[0];
        lowerQubitIndices.push_back(3*v + 2);
        
        c = indexToCoord(v,L);
        if (c.xi[0] < (L - 2))
        {
            lowerQubitIndices.push_back(3*v);
            if (c.xi[1] > 1)
            {
                lowerQubitIndices.push_back(3*v + 1);
            }
        }
        if (c.xi[0] > 2)
        {
            c.xi[0] -= 1;
            c.xi[2] = (c.xi[2] + 1);
            if (std::find(generatorVertices.begin(), generatorVertices.end(), coordToIndex(c,L)) == generatorVertices.end()) generatorVertices.push_back(coordToIndex(c, L));
            c = indexToCoord(v, L);
        }
        if (c.xi[1] > 1)
        {
            c.xi[1] -= 1;
            c.xi[2] = (c.xi[2] + 1);
            if (std::find(generatorVertices.begin(), generatorVertices.end(), coordToIndex(c,L)) == generatorVertices.end()) generatorVertices.push_back(coordToIndex(c, L));
            c = indexToCoord(v, L);
        }

        //Z logical
        if (c.xi[1] == L-3)
        {
            zLogical.push_back(3*v + 2);
            if (c.xi[0] < L-2) zLogical.push_back(3*v);
        }
       
        //lower half syndrome + defects 
        v = v + L*L; 
        edges = vertexToEdges[v];
        syndIndices.push_back(edges[1]);
        if (c.xi[1] > 1) 
        {
            syndIndices.push_back(edges[2]);
            if (c.xi[0] < (L - 2)) 
            {
                syndIndices.push_back(edges[0]);
            }

            defectIndices.push_back(v);
        }

        v = v - L*L;
        
        generatorVertices.erase(generatorVertices.begin());
    }

    //middle and upper qubits 
    for (int i : lowerQubitIndices)
    {
        middleQubitIndices.push_back(i + 3*L*L);
        upperQubitIndices.push_back(i + 6*L*L);
    }
    
    //upper half syndrome
    //Two step thing needed to prevent weird compiler errors
    //I think it doesn't like inserting to the thing its pulling from
    //sometimes just gets random numbers (memory addresses?) instead
    vint upperSyndIndices = {};
    for (int i : syndIndices) upperSyndIndices.push_back(i + 3*L*L);
    for (int i : upperSyndIndices) syndIndices.push_back(i);
    
    //upper half defects 
    //same here as above
    vint upperDefectIndices = {};
    for (int i : defectIndices) upperDefectIndices.push_back(i + L*L);
    for (int i : upperDefectIndices) defectIndices.push_back(i);

    //move logical to top face
    for (int &i : zLogical) i = i + 6*L*L;
    
    //sweep vertices
    std::set<int> sweepVerticesSet;
    for (vint qubits : {middleQubitIndices, upperQubitIndices})
    {
        for (int face : qubits)
        {
            vint edges = faceToEdges[face];
            for (int edge : edges)
            {
                std::pair<int, int> vertices = edgeToVertices[edge];
                sweepVerticesSet.insert(vertices.first);
                sweepVerticesSet.insert(vertices.second);
            }
        }
    }
    for (int i : sweepVerticesSet) sweepVertices.push_back(i);
}

//UNUSED
/*
void collapseExpand(vint &qubits, std::mt19937& engine, std::uniform_real_distribution<double>& dist, vint &qubitIndices)
{
    //is this correct?
    for (int i : qubitIndices)
    {
        if (dist(engine) < 0.5)
        {
            qubits[i] = 0;
        }
        else
        {
            qubits[i] = 1;
        } 
    }
}
*/

void moveIndices(vint &lowerQubitIndices, vint &middleQubitIndices, vint &upperQubitIndices, vint &syndIndices, vint &defectIndices, vint &zLogical, vint &sweepVertices, int L)
{
    for (int &i : lowerQubitIndices) i += 6*L*L;
    for (int &i : middleQubitIndices) i += 6*L*L;
    for (int &i : upperQubitIndices) i += 6*L*L;
    for (int &i : syndIndices) i += 6*L*L;
    for (int &i : defectIndices) i += 2*L*L;
    for (int &i : zLogical) i += 6*L*L;
    for (int &i : sweepVertices) i += 2*L*L;
}

//UNUSED
/*
void removeStabilisers(vint &qubits, vvint &upperXStabIndices, vvint &faceToEdges)
{
    for (vint xStabiliser : upperXStabIndices)
    {
        int check = 1;
        for (int i : xStabiliser)
        {
            check = check*qubits[i];
        }
        if (check)
        {
            for (int i : xStabiliser)
            {
                qubits[i] = 0;
            }
        }
    }
}
*/

