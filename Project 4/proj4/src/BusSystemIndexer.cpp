#include "BusSystemIndexer.h"
#include "BusSystem.h"
#include <vector>
#include <algorithm>
#include <memory>

struct CBusSystemIndexer::SImplementation {
    std::vector<std::shared_ptr<CBusSystem::SStop>> Stops;
    std::vector<std::shared_ptr<CBusSystem::SRoute>> Routes;

    SImplementation(std::shared_ptr<CBusSystem> bussystem) {
        // Gather stops.
        size_t stopCount = bussystem->StopCount();
        for (size_t i = 0; i < stopCount; i++) {
            auto stop = bussystem->StopByIndex(i);
            if(stop)
                Stops.push_back(stop);
        }
        // Sort stops by their ID.
        std::sort(Stops.begin(), Stops.end(), [](const std::shared_ptr<CBusSystem::SStop>& a,
                                                   const std::shared_ptr<CBusSystem::SStop>& b) {
            return a->ID() < b->ID();
        });
        // Gather routes.
        size_t routeCount = bussystem->RouteCount();
        for (size_t i = 0; i < routeCount; i++) {
            auto route = bussystem->RouteByIndex(i);
            if(route)
                Routes.push_back(route);
        }
        // Sort routes by name.
        std::sort(Routes.begin(), Routes.end(), [](const std::shared_ptr<CBusSystem::SRoute>& a,
                                                     const std::shared_ptr<CBusSystem::SRoute>& b) {
            return a->Name() < b->Name();
        });
    }
};

CBusSystemIndexer::CBusSystemIndexer(std::shared_ptr<CBusSystem> bussystem)
    : DImplementation(std::make_unique<SImplementation>(bussystem))
{ }

CBusSystemIndexer::~CBusSystemIndexer() = default;

std::size_t CBusSystemIndexer::StopCount() const noexcept {
    return DImplementation->Stops.size();
}

std::size_t CBusSystemIndexer::RouteCount() const noexcept {
    return DImplementation->Routes.size();
}

std::shared_ptr<CBusSystem::SStop> CBusSystemIndexer::SortedStopByIndex(std::size_t index) const noexcept {
    if(index < DImplementation->Stops.size())
        return DImplementation->Stops[index];
    return nullptr;
}

std::shared_ptr<CBusSystem::SRoute> CBusSystemIndexer::SortedRouteByIndex(std::size_t index) const noexcept {
    if(index < DImplementation->Routes.size())
        return DImplementation->Routes[index];
    return nullptr;
}

std::shared_ptr<CBusSystem::SStop> CBusSystemIndexer::StopByNodeID(TNodeID id) const noexcept {
    for(const auto& stop : DImplementation->Stops) {
        if(stop->NodeID() == id)
            return stop;
    }
    return nullptr;
}

bool CBusSystemIndexer::RoutesByNodeIDs(TNodeID src, TNodeID dest,
    std::unordered_set<std::shared_ptr<SRoute>> &routes) const noexcept
{
    // For each route, we iterate through its stop IDs.
    // Since the route stores a raw stop ID, we look up the stop to obtain its node ID.
    for (const auto &route : DImplementation->Routes) {
        size_t count = route->StopCount();
        bool foundSrc = false;
        for (size_t i = 0; i < count; i++) {
            CBusSystem::TStopID stopID = route->GetStopID(i);
            // Look up the stop corresponding to stopID.
            std::shared_ptr<CBusSystem::SStop> stop = nullptr;
            for (const auto &s : DImplementation->Stops) {
                if (s->ID() == stopID) {
                    stop = s;
                    break;
                }
            }
            if (!stop)
                continue;
            if (stop->NodeID() == src)
                foundSrc = true;
            if (foundSrc && stop->NodeID() == dest) {
                routes.insert(route);
                break;
            }
        }
    }
    return !routes.empty();
}

bool CBusSystemIndexer::RouteBetweenNodeIDs(TNodeID src, TNodeID dest) const noexcept {
    std::unordered_set<std::shared_ptr<SRoute>> routes;
    return RoutesByNodeIDs(src, dest, routes);
}
