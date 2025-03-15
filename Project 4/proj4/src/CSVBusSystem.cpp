// CSVBusSystem.cpp

#include "CSVBusSystem.h"
#include "DSVReader.h"
#include "StringUtils.h"  // For RemoveQuotes helper (if needed)
#include <memory>
#include <vector>
#include <string>
#include <stdexcept>
#include <sstream>
#include <unordered_map>
#include <cctype>

// Helper function to remove surrounding quotes from a field.
static std::string RemoveQuotes(const std::string &s) {
    if (s.size() >= 2 && s.front() == '"' && s.back() == '"')
        return s.substr(1, s.size() - 2);
    return s;
}

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
// Constructor: Parses stops and routes CSV files and builds internal structures.
// Header rows are skipped if present.
//
CCSVBusSystem::CCSVBusSystem(std::shared_ptr<CDSVReader> stopsrc,
                             std::shared_ptr<CDSVReader> routesrc)
    : DImplementation(std::make_unique<SImplementation>())
{
    std::vector<std::string> row;
    // Map from stop ID to stop pointer.
    std::unordered_map<CBusSystem::TStopID, std::shared_ptr<CBusSystem::SStop>> stopMap;

    // Parse stops CSV.
    bool isFirstStopRow = true;
    while (stopsrc->ReadRow(row)) {
        if (row.empty())
            continue;
        if (isFirstStopRow) {
            isFirstStopRow = false;
            std::string header = RemoveQuotes(row[0]);
            if (header == "stop_id")
                continue; // Skip header row.
        }
        if (row.size() < 2)
            continue;
        try {
            CBusSystem::TStopID sid = std::stoull(RemoveQuotes(row[0]));
            CStreetMap::TNodeID nid = std::stoull(RemoveQuotes(row[1]));
            auto stop = std::make_shared<SImplementation::StopImpl>(sid, nid);
            DImplementation->stops.push_back(stop);
            stopMap[sid] = stop;
        } catch (const std::exception &) {
            continue;
        }
    }

    // Parse routes CSV.
    bool isFirstRouteRow = true;
    // Aggregate stop IDs by route name.
    std::unordered_map<std::string, std::vector<CBusSystem::TStopID>> aggregatedRoutes;
    while (routesrc->ReadRow(row)) {
        if (row.empty())
            continue;
        if (isFirstRouteRow) {
            isFirstRouteRow = false;
            std::string header = RemoveQuotes(row[0]);
            if (header == "route")
                continue; // Skip header row.
        }
        // Use unquoted route name.
        std::string routeName = RemoveQuotes(row[0]);
        // Process each subsequent field as a stop ID.
        for (size_t i = 1; i < row.size(); ++i) {
            std::string field = RemoveQuotes(row[i]);
            try {
                CBusSystem::TStopID sid = std::stoull(field);
                // Only add if the stop exists.
                if (stopMap.find(sid) != stopMap.end())
                    aggregatedRoutes[routeName].push_back(sid);
            } catch (const std::exception &) {
                continue;
            }
        }
    }
    // Create a route object for each aggregated route.
    for (auto &entry : aggregatedRoutes) {
        if (!entry.second.empty()) {
            auto route = std::make_shared<SImplementation::RouteImpl>(entry.first, entry.second);
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
//
std::size_t CCSVBusSystem::StopCount() const noexcept {
    return DImplementation->stops.size();
}

//
// Returns the number of routes.
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
