#include "DijkstraPathRouter.h"
#include <vector>
#include <queue>
#include <limits>
#include <chrono>
#include <algorithm>
using namespace std;

// PIMPLE STRUCTURE
struct CDijkstraPathRouter::SImplementation
{
    // Structure to store edges to vertices
    struct Edge
    {
        // determine the destination of the edge
        CPathRouter::TVertexID destination;
        // How "far" is the destination
        double weight;
    };

    // Structure to store Vertex information
    struct Vertex
    {
        // Stores the tag of a vertex
        // And store the edges that the vertex extends to / Where can we go from this vertex
        any tag;
        vector<Edge> edges;
    };

    // Graph represented as a vector of vertices
    vector<Vertex> vertices;
};

// Constructor
CDijkstraPathRouter::CDijkstraPathRouter()
    : DImplementation(new SImplementation())
{
}

// Destructor
CDijkstraPathRouter::~CDijkstraPathRouter() = default;

// Returns the number of vertices in the path router
size_t CDijkstraPathRouter::VertexCount() const noexcept
{
    return DImplementation->vertices.size();
}

// Adds a vertex with the tag provided. The tag can be of any type.
CPathRouter::TVertexID CDijkstraPathRouter::AddVertex(any tag) noexcept
{
    auto id = DImplementation->vertices.size();
    SImplementation::Vertex vertex;
    vertex.tag = tag;
    DImplementation->vertices.push_back(move(vertex));
    return id;
}

// Gets the tag of the vertex specified by id if id is in the path router.
// A any() is returned if id is not a valid vertex ID.
any CDijkstraPathRouter::GetVertexTag(TVertexID id) const noexcept
{
    if (id < DImplementation->vertices.size())
        return DImplementation->vertices[id].tag;
    return any();
}

// Adds an edge between src and destination vertices with a weight of weight. If
// bidir is set to true an additional edge between destination and src is added. If
// src or destination nodes do not exist, or if the weight is negative the AddEdge
// will return false, otherwise it returns true.
bool CDijkstraPathRouter::AddEdge(TVertexID src, TVertexID destination, double weight, bool bidir) noexcept
{
    if (src >= DImplementation->vertices.size() || destination >= DImplementation->vertices.size())
        return false;

    DImplementation->vertices[src].edges.push_back({destination, weight});
    if (bidir)
    {
        DImplementation->vertices[destination].edges.push_back({src, weight});
    }
    return true;
}

// Allows the path router to do any desired precomputation up to the deadline
bool CDijkstraPathRouter::Precompute(chrono::steady_clock::time_point deadline) noexcept
{
    if (chrono::steady_clock::now() > deadline)
        return false;
    return true;
}

// Returns the path distance of the path from src to destination, and fills out path
// with vertices. If no path exists NoPathExists is returned.
double CDijkstraPathRouter::FindShortestPath(TVertexID src, TVertexID destination, vector<TVertexID> &path) noexcept
{
    const size_t n = DImplementation->vertices.size();
    if (src >= n || destination >= n)
    {
        return CPathRouter::NoPathExists;
    }

    vector<double> dist(n, numeric_limits<double>::infinity());
    vector<TVertexID> prev(n, CPathRouter::InvalidVertexID);
    using PQElement = pair<double, CPathRouter::TVertexID>;
    priority_queue<PQElement, vector<PQElement>, greater<PQElement>> pq;

    dist[src] = 0;
    pq.push({0, src});

    while (!pq.empty())
    {
        auto [d, u] = pq.top();
        pq.pop();
        if (d > dist[u])
        {
            continue;
        }
        if (u == destination)
        {
            break;
        }
        for (const auto &edge : DImplementation->vertices[u].edges)
        {
            auto v = edge.destination;
            double alt = d + edge.weight;
            if (alt < dist[v])
            {
                dist[v] = alt;
                prev[v] = u;
                pq.push({alt, v});
            }
        }
    }
    if (dist[destination] == numeric_limits<double>::infinity())
    {
        return CPathRouter::NoPathExists;
    }
    path.clear();
    for (CPathRouter::TVertexID at = destination; at != CPathRouter::InvalidVertexID; at = prev[at])
    {
        path.push_back(at);
        if (at == src)
        {
            break;
        }
    }
    reverse(path.begin(), path.end());

    return dist[destination];
}
