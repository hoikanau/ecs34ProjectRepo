#include "CSVBusSystem.h"
#include "DSVReader.h"
#include <memory>
#include <vector>
#include <string>
#include <stdexcept>
#include <sstream>
#include <unordered_map>

struct CCSVBusSystem::SImplementation {
    // Vectors to hold stops and routes.
    std::vector<std::shared_ptr<CBusSystem::SStop>> stops;
    std::vector<std::shared_ptr<CBusSystem::SRoute>> routes;

    // Concrete implementation for a bus stop.
    struct StopImpl : public CBusSystem::SStop {
        CBusSystem::TStopID stop_id;
        CStreetMap::TNodeID node_id;
        StopImpl(CBusSystem::TStopID sid, CStreetMap::TNodeID nid)
            : stop_id(sid), node_id(nid) { }
        CBusSystem::TStopID ID() const noexcept override {
            return stop_id;
        }
        CStreetMap::TNodeID NodeID() const noexcept override {
            return node_id;
        }
    };

    // Concrete implementation for a bus route.
    struct RouteImpl : public CBusSystem::SRoute {
        std::string name;
        std::vector<CBusSystem::TStopID> stop_ids;
        RouteImpl(const std::string &routeName, const std::vector<CBusSystem::TStopID> &stops)
            : name(routeName), stop_ids(stops) { }
        std::string Name() const noexcept override {
            return name;
        }
        std::size_t StopCount() const noexcept override {
            return stop_ids.size();
        }
        CBusSystem::TStopID GetStopID(std::size_t index) const noexcept override {
            if (index < stop_ids.size()) {
                return stop_ids[index];
            }
            return CBusSystem::InvalidStopID;
        }
    };
};

//
// Constructor: Parses stops and routes CSV files and builds internal structures.
// If a stop appears more than once, only the first instance is kept.
// For routes, only stops that are found in the stops CSV are included; if none are found, the route is discarded.
//
CCSVBusSystem::CCSVBusSystem(std::shared_ptr<CDSVReader> stopsrc, std::shared_ptr<CDSVReader> routesrc)
    : DImplementation(std::make_unique<SImplementation>())
{
    std::vector<std::string> row;
    // Use a map to ensure uniqueness of stops.
    std::unordered_map<CBusSystem::TStopID, std::shared_ptr<CBusSystem::SStop>> stopMap;

    // Parse stops CSV. Expected format per row: stop_id,node_id
    while (stopsrc->ReadRow(row)) {
        if (row.size() < 2)
            continue; // Skip invalid rows
        try {
            CBusSystem::TStopID stopId = std::stoull(row[0]);
            CStreetMap::TNodeID nodeId = std::stoull(row[1]);
            // Only add if this stopID hasn't been seen before.
            if (stopMap.find(stopId) == stopMap.end()) {
                auto stop = std::make_shared<SImplementation::StopImpl>(stopId, nodeId);
                stopMap[stopId] = stop;
                DImplementation->stops.push_back(stop);
            }
        } catch (const std::exception &) {
            continue;
        }
    }

    // Parse routes CSV. Expected format per row: route_name,stop_id1,stop_id2,...
    while (routesrc->ReadRow(row)) {
        if (row.empty())
            continue; // Skip empty rows
        std::string routeName = row[0];
        std::vector<CBusSystem::TStopID> validRouteStops;
        // Only include stop IDs that exist in the stops CSV.
        for (size_t i = 1; i < row.size(); ++i) {
            try {
                CBusSystem::TStopID sid = std::stoull(row[i]);
                if (stopMap.find(sid) != stopMap.end()) {
                    validRouteStops.push_back(sid);
                }
            } catch (const std::exception &) {
                continue;
            }
        }
        // Only add the route if at least one valid stop is found.
        if (!validRouteStops.empty()) {
            auto route = std::make_shared<SImplementation::RouteImpl>(routeName, validRouteStops);
            DImplementation->routes.push_back(route);
        }
    }
}

//
// Destructor
//
CCSVBusSystem::~CCSVBusSystem() = default;

//
// Returns the number of stops
//
std::size_t CCSVBusSystem::StopCount() const noexcept {
    return DImplementation->stops.size();
}

//
// Returns the number of routes
//
std::size_t CCSVBusSystem::RouteCount() const noexcept {
    return DImplementation->routes.size();
}

//
// Returns the stop at the given index, or nullptr if out-of-bounds.
//
std::shared_ptr<CBusSystem::SStop> CCSVBusSystem::StopByIndex(std::size_t index) const noexcept {
    if (index < DImplementation->stops.size())
        return DImplementation->stops[index];
    return nullptr;
}

//
// Returns the stop with the specified stop ID, or nullptr if not found.
//
std::shared_ptr<CBusSystem::SStop> CCSVBusSystem::StopByID(TStopID id) const noexcept {
    for (const auto &stop : DImplementation->stops) {
        if (stop->ID() == id)
            return stop;
    }
    return nullptr;
}

//
// Returns the route at the given index, or nullptr if out-of-bounds.
//
std::shared_ptr<CBusSystem::SRoute> CCSVBusSystem::RouteByIndex(std::size_t index) const noexcept {
    if (index < DImplementation->routes.size())
        return DImplementation->routes[index];
    return nullptr;
}

//
// Returns the route with the given name, or nullptr if not found.
//
std::shared_ptr<CBusSystem::SRoute> CCSVBusSystem::RouteByName(const std::string &name) const noexcept {
    for (const auto &route : DImplementation->routes) {
        if (route->Name() == name)
            return route;
    }
    return nullptr;
}