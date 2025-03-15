// CTransportationPlanner.cpp

#include "TransportationPlanner.h"
#include "StreetMap.h"
#include "BusSystem.h"
#include "PathRouter.h"
#include "DijkstraPathRouter.h"
#include <vector>
#include <memory>
#include <algorithm>
#include <unordered_map>
#include <chrono>
#include <cmath>
#include <sstream>
#include <iterator>  // For std::begin and std::end

// Helper function to compute Euclidean distance between two locations.
static double EuclideanDistance(const CStreetMap::TLocation &loc1, const CStreetMap::TLocation &loc2) {
    double dx = loc1.first - loc2.first;
    double dy = loc1.second - loc2.second;
    return std::sqrt(dx * dx + dy * dy);
}

// Concrete implementation of CTransportationPlanner.
class CTransportationPlannerImpl : public CTransportationPlanner {
public:
    CTransportationPlannerImpl(std::shared_ptr<SConfiguration> config)
        : mConfig(config),
          mStreetMap(config->StreetMap()),
          mBusSystem(config->BusSystem())
    {
        // Build a sorted vector of nodes from the street map.
        std::size_t nodeCount = mStreetMap->NodeCount();
        for (std::size_t i = 0; i < nodeCount; i++) {
            auto node = mStreetMap->NodeByIndex(i);
            if (node)
                mSortedNodes.push_back(node);
        }
        // Sort nodes by their ID.
        std::sort(mSortedNodes.begin(), mSortedNodes.end(),
            [](const std::shared_ptr<CStreetMap::SNode>& a,
               const std::shared_ptr<CStreetMap::SNode>& b) {
                return a->ID() < b->ID();
            });
        // Build a mapping from node ID to its vertex index.
        for (std::size_t i = 0; i < mSortedNodes.size(); i++) {
            mNodeToVertex[mSortedNodes[i]->ID()] = i;
        }

        // Create the path router (using a Dijkstra-based implementation).
        mPathRouter = std::make_unique<CDijkstraPathRouter>();

        // Add each node as a vertex in the router.
        for (std::size_t i = 0; i < mSortedNodes.size(); i++) {
            mPathRouter->AddVertex(mSortedNodes[i]);
        }

        // Add edges based on the ways in the street map.
        std::size_t wayCount = mStreetMap->WayCount();
        for (std::size_t i = 0; i < wayCount; i++) {
            auto way = mStreetMap->WayByIndex(i);
            if (!way)
                continue;
            std::size_t numNodes = way->NodeCount();
            // For each consecutive pair of nodes in the way, add an edge.
            for (std::size_t j = 0; j < numNodes - 1; j++) {
                auto nodeID1 = way->GetNodeID(j);
                auto nodeID2 = way->GetNodeID(j + 1);
                if (mNodeToVertex.find(nodeID1) != mNodeToVertex.end() &&
                    mNodeToVertex.find(nodeID2) != mNodeToVertex.end())
                {
                    std::size_t v1 = mNodeToVertex[nodeID1];
                    std::size_t v2 = mNodeToVertex[nodeID2];
                    auto loc1 = mSortedNodes[v1]->Location();
                    auto loc2 = mSortedNodes[v2]->Location();
                    double dist = EuclideanDistance(loc1, loc2);
                    // Assume ways are bidirectional.
                    mPathRouter->AddEdge(v1, v2, dist, true);
                }
            }
        }
        // Precompute the router using the configuration's precompute time.
        auto deadline = std::chrono::steady_clock::now() + std::chrono::seconds(mConfig->PrecomputeTime());
        mPathRouter->Precompute(deadline);
    }

    virtual ~CTransportationPlannerImpl() = default;

    // Returns the number of nodes.
    virtual std::size_t NodeCount() const noexcept override {
        return mSortedNodes.size();
    }

    // Returns the node at the given sorted index.
    virtual std::shared_ptr<CStreetMap::SNode> SortedNodeByIndex(std::size_t index) const noexcept override {
        if (index < mSortedNodes.size())
            return mSortedNodes[index];
        return nullptr;
    }

    // Find the shortest path (by distance) from src to dest on the street network.
    virtual double FindShortestPath(TNodeID src, TNodeID dest, std::vector<TNodeID> &path) override {
        // Check if source and destination exist.
        if (mNodeToVertex.find(src) == mNodeToVertex.end() ||
            mNodeToVertex.find(dest) == mNodeToVertex.end())
        {
            return CPathRouter::NoPathExists;
        }
        std::size_t vsrc = mNodeToVertex[src];
        std::size_t vdest = mNodeToVertex[dest];
        // Use fully qualified type for vertex IDs.
        std::vector<CPathRouter::TVertexID> vPath;
        double distance = mPathRouter->FindShortestPath(vsrc, vdest, vPath);
        path.clear();
        // Convert vertex IDs back to street map node IDs.
        for (auto v : vPath) {
            if (v < mSortedNodes.size())
                path.push_back(mSortedNodes[v]->ID());
        }
        return distance;
    }

    // Find the fastest path from src to dest.
    virtual double FindFastestPath(TNodeID src, TNodeID dest, std::vector<TTripStep> &path) override {
        std::vector<TNodeID> nodePath;
        double distance = FindShortestPath(src, dest, nodePath);
        path.clear();
        for (auto nodeID : nodePath) {
            path.push_back({ETransportationMode::Walk, nodeID});
        }
        return (mConfig->WalkSpeed() > 0) ? distance / mConfig->WalkSpeed() : 0;
    }

    // Generate textual description for each trip step.
    virtual bool GetPathDescription(const std::vector<TTripStep> &path, std::vector<std::string> &desc) const override {
        desc.clear();
        if (path.empty())
            return false;
        for (const auto &step : path) {
            std::ostringstream oss;
            switch (step.first) {
                case ETransportationMode::Walk:
                    oss << "Walk to node " << step.second;
                    break;
                case ETransportationMode::Bike:
                    oss << "Bike to node " << step.second;
                    break;
                case ETransportationMode::Bus:
                    oss << "Take bus to node " << step.second;
                    break;
            }
            desc.push_back(oss.str());
        }
        return true;
    }

private:
    std::shared_ptr<SConfiguration> mConfig;
    std::shared_ptr<CStreetMap> mStreetMap;
    std::shared_ptr<CBusSystem> mBusSystem;
    std::vector<std::shared_ptr<CStreetMap::SNode>> mSortedNodes;
    std::unordered_map<TNodeID, std::size_t> mNodeToVertex;
    std::unique_ptr<CPathRouter> mPathRouter;
};

std::unique_ptr<CTransportationPlanner> CreateTransportationPlanner(std::shared_ptr<CTransportationPlanner::SConfiguration> config) {
    return std::make_unique<CTransportationPlannerImpl>(config);
}
