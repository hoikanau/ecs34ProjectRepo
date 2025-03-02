#include "CSVBusSystem.h"
#include "DSVReader.h"
#include <memory>
#include <vector>
#include <string>
#include <stdexcept>
#include <sstream>
#include <unordered_map>
#include <cctype>

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
            if (index < stop_ids.size())
                return stop_ids[index];
            return CBusSystem::InvalidStopID;
        }
    };
};

//
// Constructor: Parse the stops and routes CSV files and build internal structures.
// This version skips header rows if present.
CCSVBusSystem::CCSVBusSystem(std::shared_ptr<CDSVReader> stopsrc, std::shared_ptr<CDSVReader> routesrc)
    : DImplementation(std::make_unique<SImplementation>())
{
    std::vector<std::string> row;
    std::unordered_map<CBusSystem::TStopID, std::shared_ptr<CBusSystem::SStop>> stopMap;

    // Parse stops CSV. Assume a header if the first row's first field is nonnumeric.
    bool isFirstStopRow = true;
    while (stopsrc->ReadRow(row)) {
        if (row.size() < 2)
            continue;
        if (isFirstStopRow) {
            isFirstStopRow = false;
            // If the first field is nonnumeric, treat it as a header and skip.
            if (!row[0].empty() && !std::isdigit(row[0][0])) {
                continue;
            }
        }
        try {
            CBusSystem::TStopID stopId = std::stoull(row[0]);
            CStreetMap::TNodeID nodeId = std::stoull(row[1]);
            if (stopMap.find(stopId) == stopMap.end()) {
                auto stop = std::make_shared<SImplementation::StopImpl>(stopId, nodeId);
                stopMap[stopId] = stop;
                DImplementation->stops.push_back(stop);
            }
        } catch (const std::exception &) {
            continue;
        }
    }

    // Parse routes CSV. Since routes.csv has a header, skip the first row.
    bool isFirstRouteRow = true;
    while (routesrc->ReadRow(row)) {
        if (row.empty())
            continue;
        if (isFirstRouteRow) {
            isFirstRouteRow = false;
            // If the first field equals "route_name", skip this header row.
            if (!row.empty() && row[0] == "route_name")
                continue;
        }
        std::string routeName = row[0];
        std::vector<CBusSystem::TStopID> validRouteStops;
        for (size_t i = 1; i < row.size(); ++i) {
            try {
                CBusSystem::TStopID sid = std::stoull(row[i]);
                // Only include stops that were successfully parsed.
                if (stopMap.find(sid) != stopMap.end())
                    validRouteStops.push_back(sid);
            } catch (const std::exception &) {
                continue;
            }
        }
        // Only add the route if at least one valid stop is present.
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
// Returns the number of stops.
std::size_t CCSVBusSystem::StopCount() const noexcept {
    return DImplementation->stops.size();
}

//
// Returns the number of routes.
std::size_t CCSVBusSystem::RouteCount() const noexcept {
    return DImplementation->routes.size();
}

//
// Returns the stop at the given index, or nullptr if out-of-bounds.
std::shared_ptr<CBusSystem::SStop> CCSVBusSystem::StopByIndex(std::size_t index) const noexcept {
    if (index < DImplementation->stops.size())
        return DImplementation->stops[index];
    return nullptr;
}

//
// Returns the stop with the specified stop ID, or nullptr if not found.
std::shared_ptr<CBusSystem::SStop> CCSVBusSystem::StopByID(TStopID id) const noexcept {
    for (const auto &stop : DImplementation->stops) {
        if (stop->ID() == id)
            return stop;
    }
    return nullptr;
}

//
// Returns the route at the given index, or nullptr if out-of-bounds.
std::shared_ptr<CBusSystem::SRoute> CCSVBusSystem::RouteByIndex(std::size_t index) const noexcept {
    if (index < DImplementation->routes.size())
        return DImplementation->routes[index];
    return nullptr;
}

//
// Returns the route with the given name, or nullptr if not found.
std::shared_ptr<CBusSystem::SRoute> CCSVBusSystem::RouteByName(const std::string &name) const noexcept {
    for (const auto &route : DImplementation->routes) {
        if (route->Name() == name)
            return route;
    }
    return nullptr;
}