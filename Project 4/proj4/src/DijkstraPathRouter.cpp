// DijkstraPathRouter.cpp

#include "DijkstraPathRouter.h"
#include <vector>
#include <queue>
#include <limits>
#include <chrono>
#include <algorithm>

// Private implementation details for CDijkstraPathRouter
struct CDijkstraPathRouter::SImplementation {
    // Internal edge structure
    struct Edge {
        CPathRouter::TVertexID dest;
        double weight;
    };

    // Each vertex stores its tag and a list of edges
    struct Vertex {
        std::any tag;
        std::vector<Edge> edges;
    };

    // Graph represented as a vector of vertices
    std::vector<Vertex> vertices;
};

//
// Public interface implementation
//

// Constructor: initialize the private implementation.
CDijkstraPathRouter::CDijkstraPathRouter()
    : DImplementation(new SImplementation())
{
}

// Destructor: unique_ptr automatically cleans up.
CDijkstraPathRouter::~CDijkstraPathRouter() = default;

// Return the number of vertices in the graph.
std::size_t CDijkstraPathRouter::VertexCount() const noexcept {
    return DImplementation->vertices.size();
}

// Adds a new vertex with the provided tag.
// Returns the new vertex's ID (which is its index in the vector).
CPathRouter::TVertexID CDijkstraPathRouter::AddVertex(std::any tag) noexcept {
    auto id = DImplementation->vertices.size();
    SImplementation::Vertex vertex;
    vertex.tag = tag;
    DImplementation->vertices.push_back(std::move(vertex));
    return id;
}

// Returns the tag associated with the vertex identified by id.
// If id is invalid, returns an empty std::any.
std::any CDijkstraPathRouter::GetVertexTag(TVertexID id) const noexcept {
    if (id < DImplementation->vertices.size())
        return DImplementation->vertices[id].tag;
    return std::any();
}

// Adds a directed edge from src to dest with the given weight.
// If bidir is true, also adds an edge from dest to src.
bool CDijkstraPathRouter::AddEdge(TVertexID src, TVertexID dest, double weight, bool bidir) noexcept {
    if (src >= DImplementation->vertices.size() || dest >= DImplementation->vertices.size())
        return false;

    DImplementation->vertices[src].edges.push_back({dest, weight});
    if (bidir) {
        DImplementation->vertices[dest].edges.push_back({src, weight});
    }
    return true;
}

// Precompute: This implementation does not perform any precomputation.
// We optionally check if the deadline has already passed.
bool CDijkstraPathRouter::Precompute(std::chrono::steady_clock::time_point deadline) noexcept {
    if (std::chrono::steady_clock::now() > deadline)
        return false;
    return true;
}

// Uses Dijkstra's algorithm to compute the shortest path from src to dest.
// On success, returns the total weight and populates 'path' with the sequence of vertex IDs.
// If no path exists or src/dest is invalid, returns CPathRouter::NoPathExists.
double CDijkstraPathRouter::FindShortestPath(TVertexID src, TVertexID dest, std::vector<TVertexID> &path) noexcept {
    const std::size_t n = DImplementation->vertices.size();
    if (src >= n || dest >= n)
        return CPathRouter::NoPathExists;

    // Initialize distance vector and predecessor vector.
    std::vector<double> dist(n, std::numeric_limits<double>::infinity());
    std::vector<TVertexID> prev(n, CPathRouter::InvalidVertexID);
    
    // Priority queue (min-heap) stores pairs: (current distance, vertex id).
    using PQElement = std::pair<double, CPathRouter::TVertexID>;
    std::priority_queue<PQElement, std::vector<PQElement>, std::greater<PQElement>> pq;
    
    dist[src] = 0;
    pq.push({0, src});
    
    while (!pq.empty()) {
        auto [d, u] = pq.top();
        pq.pop();

        // If this entry is outdated, skip it.
        if (d > dist[u])
            continue;

        // Early exit if we reached the destination.
        if (u == dest)
            break;

        // Examine adjacent edges.
        for (const auto &edge : DImplementation->vertices[u].edges) {
            auto v = edge.dest;
            double alt = d + edge.weight;
            if (alt < dist[v]) {
                dist[v] = alt;
                prev[v] = u;
                pq.push({alt, v});
            }
        }
    }
    
    // No path exists if the distance to dest is still infinity.
    if (dist[dest] == std::numeric_limits<double>::infinity())
        return CPathRouter::NoPathExists;
    
    // Reconstruct the path from dest back to src.
    path.clear();
    for (CPathRouter::TVertexID at = dest; at != CPathRouter::InvalidVertexID; at = prev[at]) {
        path.push_back(at);
        if (at == src)
            break;
    }
    std::reverse(path.begin(), path.end());
    
    return dist[dest];
}
