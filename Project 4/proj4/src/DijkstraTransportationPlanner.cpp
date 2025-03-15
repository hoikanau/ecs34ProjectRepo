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
#include <iterator> 
using namespace std;

// This is a helper function 
// to calculated the coordinate distances between two locations
static double CalculateCoordinateDifference(const CStreetMap::TLocation &loc1, const CStreetMap::TLocation &loc2) {
    double dx = loc1.first - loc2.first;
    double dy = loc1.second - loc2.second;
    return sqrt(dx * dx + dy * dy);
}

// PIMPLE STRUCTURE
class CTransportationPlannerImpl : public CTransportationPlanner {
public:
    CTransportationPlannerImpl(shared_ptr<SConfiguration> configuration) : InputConfig(configuration), InputStreetMap(configuration->StreetMap()), InputBusSystem(configuration->BusSystem())
    {
        // Constructing a sorted vector of nodes from input file
        size_t nodeCount = InputStreetMap->NodeCount();
        for (size_t i = 0; i < nodeCount; i++) {
            auto node = InputStreetMap->NodeByIndex(i);
            if (node)
                {InputSortedNodes.push_back(node);}
        }
        // Sorting nodes by their ID using a lambda structure.
        sort(InputSortedNodes.begin(), InputSortedNodes.end(), [](const shared_ptr<CStreetMap::SNode>& a,const shared_ptr<CStreetMap::SNode>& b) {return a->ID() < b->ID(); });
        for (size_t i = 0; i < InputSortedNodes.size(); i++) {
            InputNodeToVertex[InputSortedNodes[i]->ID()] = i;
        }

        // Using CDijkstraPathRouter to complete the Planner
        InputPathRouter = make_unique<CDijkstraPathRouter>();

        // Each node will be appended to the route as a new vertex
        for (size_t i = 0; i < InputSortedNodes.size(); i++) {
            InputPathRouter->AddVertex(InputSortedNodes[i]);
        }

        // Append the corresponding ways as edges
        size_t wayCount = InputStreetMap->WayCount();
        for (size_t i = 0; i < wayCount; i++) {
            auto way = InputStreetMap->WayByIndex(i);
            if (!way)
                {continue;}
            size_t numNodes = way->NodeCount();
            for (size_t j = 0; j < numNodes - 1; j++) {
                auto nodeID1 = way->GetNodeID(j);
                auto nodeID2 = way->GetNodeID(j + 1);
                if (InputNodeToVertex.find(nodeID1) != InputNodeToVertex.end() && InputNodeToVertex.find(nodeID2) != InputNodeToVertex.end())
                {
                    size_t v1 = InputNodeToVertex[nodeID1];
                    size_t v2 = InputNodeToVertex[nodeID2];
                    auto loc1 = InputSortedNodes[v1]->Location();
                    auto loc2 = InputSortedNodes[v2]->Location();
                    double dist = CalculateCoordinateDifference(loc1, loc2);
                    InputPathRouter->AddEdge(v1, v2, dist, true);
                }
            }
        }
        auto time = chrono::steady_clock::now() + chrono::seconds(InputConfig->PrecomputeTime());
        InputPathRouter->Precompute(time);
    }


    // Destructor
    virtual ~CTransportationPlannerImpl() = default;

    // Returns the number of nodes in the street map
    virtual size_t NodeCount() const noexcept override {
        return InputSortedNodes.size();
    }

    // Returns the street map node specified by index if index is less than the
    // NodeCount(). nullptr is returned if index is greater than or equal to
    // NodeCount(). The nodes are sorted by Node ID.
    virtual shared_ptr<CStreetMap::SNode> SortedNodeByIndex(size_t index) const noexcept override {
        if (index < InputSortedNodes.size())
           { return InputSortedNodes[index];}
        return nullptr;
    }

    // Returns the distance in miles between the src and destination nodes of the
    // shortest path if one exists. NoPathExists is returned if no path exists.
    // The nodes of the shortest path are filled in the path parameter.
    virtual double FindShortestPath(TNodeID src, TNodeID destination, vector<TNodeID> &path) override {
        // Check if source and destination exist.
        if (InputNodeToVertex.find(src) == InputNodeToVertex.end() || InputNodeToVertex.find(destination) == InputNodeToVertex.end())
        {
            return CPathRouter::NoPathExists;
        }
        size_t vsrc = InputNodeToVertex[src];
        size_t vdest = InputNodeToVertex[destination];
        // Use fully qualified type for vertex IDs.
        vector<CPathRouter::TVertexID> vPath;
        double distance = InputPathRouter->FindShortestPath(vsrc, vdest, vPath);
        path.clear();
        // Convert vertex IDs back to street map node IDs.
        for (auto v : vPath) {
            if (v < InputSortedNodes.size())
                path.push_back(InputSortedNodes[v]->ID());
        }
        return distance;
    }

    // Returns the time in hours for the fastest path between the src and destination
    // nodes of the if one exists. NoPathExists is returned if no path exists.
    // The transportation mode and nodes of the fastest path are filled in the
    // path parameter.
    virtual double FindFastestPath(TNodeID src, TNodeID destination, vector<TTripStep> &path) override {
        vector<TNodeID> nodePath;
        double distance = FindShortestPath(src, destination, nodePath);
        path.clear();
        for (auto nodeID : nodePath) {
            path.push_back({ETransportationMode::Walk, nodeID});
        }
        return (InputConfig->WalkSpeed() > 0) ? distance / InputConfig->WalkSpeed() : 0;
    }

    // Returns true if the path description is created. Takes the trip steps path
    // and converts it into a human readable set of steps.
    virtual bool GetPathDescription(const vector<TTripStep> &path, vector<string> &description) const override {
        description.clear();
        if (path.empty())
            return false;
        for (const auto &step : path) {
            ostringstream oss;
            switch (step.first) {
                case ETransportationMode::Walk:
                    oss << "Method of Transporation: Walking " << step.second;
                    break;
                case ETransportationMode::Bike:
                    oss << "Method of Transportation: Biking " << step.second;
                    break;
                case ETransportationMode::Bus:
                    oss << "Method of Transportation: Bussing " << step.second;
                    break;
            }
            description.push_back(oss.str());
        }
        return true;
    }

private:
    shared_ptr<SConfiguration> InputConfig;
    shared_ptr<CStreetMap> InputStreetMap;
    shared_ptr<CBusSystem> InputBusSystem;
    vector<shared_ptr<CStreetMap::SNode>> InputSortedNodes;
    unordered_map<TNodeID, size_t> InputNodeToVertex;
    unique_ptr<CPathRouter> InputPathRouter;
};

unique_ptr<CTransportationPlanner> CreateTransportationPlanner(shared_ptr<CTransportationPlanner::SConfiguration> configuration) {
    return make_unique<CTransportationPlannerImpl>(configuration);
}
