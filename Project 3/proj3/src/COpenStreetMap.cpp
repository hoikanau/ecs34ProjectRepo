#include "OpenStreetMap.h"
#include "StreetMap.h"
#include "StringDataSource.h"
#include "StringDataSink.h"
#include "XMLReader.h"
#include "XMLWriter.h"
#include "XMLEntity.h"
#include "XMLReader.cpp"
#include <iostream>
#include <memory>
#include <utility>
#include <vector>
#include <string>

using namespace std;
struct COpenStreetMap::SImplementation
{
    struct SNode : public CStreetMap::SNode
    {
        TNodeID nodeID;
        TLocation DLocation;
        vector<pair<string, string>> DAttributes;
        // Returns the id of the SNode
        TNodeID ID() const noexcept
        {
            return nodeID;
        }
        // Returns the lat/lon location of the SNode
        TLocation Location() const noexcept
        {
            return DLocation;
        }
        // Returns the number of attributes attached to the SNode
        size_t AttributeCount() const noexcept
        {
            return DAttributes.size();
        }
        // Returns the key of the attribute at index, returns empty string if index
        // is greater than or equal to AttributeCount()
        string GetAttributeKey(std::size_t index) const noexcept
        {
            if (index < DAttributes.size())
            {
                return DAttributes[index].first;
            }
            else
            {
                return "";
            }
        }
        // Returns if the attribute is attached to the SNode
        bool HasAttribute(const std::string &key) const noexcept
        {
            for (const auto &attr : DAttributes)
            {
                if (attr.first == key)
                {
                    return true;
                }
            }
            return false;
        }
        // Returns the value of the attribute specified by key, returns empty string
        // if key hasn't been attached to SNode
        string GetAttribute(const std::string &key) const noexcept
        {
            for (const auto &attr : DAttributes)
            {
                if (attr.first == key)
                {
                    return attr.second;
                }
            }
            return "";
        }
        ~SNode() = default;
    };
    struct SWay : public CStreetMap::SWay
    {
        TWayID DWayID;
        vector<TNodeID> DNodeIDs;
        vector<pair<string, string>> DAttributes;
        ~SWay() {};
        // Returns the id of the SWay
        TWayID ID() const noexcept
        {
            return DWayID;
        }
        // Returns the number of nodes in the way
        std::size_t NodeCount() const noexcept
        {
            return DNodeIDs.size();
        }
        // Returns the node id of the node at index, returns InvalidNodeID if index
        // is greater than or equal to NodeCount()
        TNodeID GetNodeID(std::size_t index) const noexcept
        {
            if (index < DNodeIDs.size())
            {
                return DNodeIDs[index];
            }
            else
            {
                return InvalidNodeID;
            }
        }
        // Returns the number of attributes attached to the SWay
        std::size_t AttributeCount() const noexcept
        {
            return DAttributes.size();
        }
        // Returns the key of the attribute at index, returns empty string if index
        // is greater than or equal to AttributeCount()
        std::string GetAttributeKey(std::size_t index) const noexcept
        {
            if (index < DAttributes.size())
            {
                return DAttributes[index].first;
            }
            else
            {
                return "";
            }
        }
        // Returns if the attribute is attached to the SWay
        bool HasAttribute(const std::string &key) const noexcept
        {
            for (const auto &attr : DAttributes)
            {
                if (attr.first == key)
                {
                    return true;
                }
            }
            return false;
        }
        // Returns the value of the attribute specified by key, returns empty string
        // if key hasn't been attached to SWay
        std::string GetAttribute(const std::string &key) const noexcept
        {
            for (const auto &attr : DAttributes)
            {
                if (attr.first == key)
                {
                    return attr.second;
                }
            }
            return "";
        }
    };
    vector<SNode> DNodes;
    vector<SWay> DWays;

    // Returns the number of nodes in the map
    size_t NodeCount() const noexcept
    {
        return DNodes.size();
    }

    // Returns the number of ways in the map
    size_t WayCount() const noexcept
    {
        return DWays.size();
    }
    std::shared_ptr<CStreetMap::SNode> NodeByIndex(size_t index) const noexcept
    {
        if (index >= DNodes.size())
        {
            return nullptr;
        }
        return make_shared<SNode>(DNodes[index]);
    }

    // Returns the SNode with the id of id, returns nullptr if doesn't exist
    shared_ptr<CStreetMap::SNode> NodeByID(TNodeID id) const noexcept
    {
        for (const auto &node : DNodes)
        {
            if (node.ID() == id)
            {
                return make_shared<SNode>(node);
            }
        }
        return nullptr;
    }

    // Returns the SWay associated with index, returns nullptr if index is
    // larger than or equal to WayCount()
    shared_ptr<CStreetMap::SWay> WayByIndex(std::size_t index) const noexcept
    {
        if (index >= DWays.size())
        {
            return nullptr;
        }
        return make_shared<SWay>(DWays[index]);
    }

    // Returns the SWay with the id of id, returns nullptr if doesn't exist
    shared_ptr<CStreetMap::SWay> WayByID(TWayID id) const noexcept
    {
        for (const auto &way : DWays)
        {
            if (way.ID() == id)
            {
                return make_shared<SWay>(way);
            }
        }
        return nullptr;
    }
};

// Constructor for the Open Street Map
COpenStreetMap::COpenStreetMap(shared_ptr<CXMLReader> src) : DImplementation(make_unique<SImplementation>())
{
    bool insideNode = false;
    bool insideWay = false;
    SImplementation::SNode currentNode;
    SImplementation::SWay currentWay;

    SXMLEntity entity;
    while (!src->End())
    {
        if (!src->ReadEntity(entity, false))
        {
            break;
        }
        if (entity.DType == SXMLEntity::EType::StartElement)
        {
            if (entity.DNameData == "node")
            {
                insideNode = true;
                currentNode = SImplementation::SNode();
                for (auto &att : entity.DAttributes)
                {
                    if (att.first == "id")
                    {
                        currentNode.nodeID = stoull(att.second);
                    }
                    else if (att.first == "lat")
                    {
                        currentNode.DLocation.first = stod(att.second);
                    }
                    else if (att.first == "lon")
                    {
                        currentNode.DLocation.second = stod(att.second);
                    }
                    else
                    {
                        currentNode.DAttributes.push_back(att);
                    }
                }
            }
            else if (entity.DNameData == "way")
            {
                insideWay = true;
                currentWay = SImplementation::SWay();

                for (const auto &att : entity.DAttributes)
                {
                    if (att.first == "id")
                    {
                        currentWay.DWayID = stoull(att.second);
                    }
                    else
                    {
                        currentWay.DAttributes.push_back(att);
                    }
                }
            }
            else if (entity.DNameData == "tag")
            {
                string key, value;
                for (const auto &att : entity.DAttributes)
                {
                    if (att.first == "k")
                    {
                        key = att.second;
                    }
                    else if (att.first == "v")
                    {
                        value = att.second;
                    }
                }
                if (insideNode)
                {
                    currentNode.DAttributes.push_back({key, value});
                }
                else if (insideWay)
                {
                    currentWay.DAttributes.push_back({key, value});
                }
            }
            else if (entity.DNameData == "nd" && insideWay)
            {
                for (const auto &att : entity.DAttributes)
                {
                    if (att.first == "ref")
                    {
                        CStreetMap::TNodeID refId = stoull(att.second);
                        currentWay.DNodeIDs.push_back(refId);
                    }
                }
            }
        }
        else if (entity.DType == SXMLEntity::EType::EndElement){
            if (entity.DNameData == "node"){
                insideNode = false;
                DImplementation->DNodes.push_back(currentNode);
            }
            else if (entity.DNameData == "way"){
                insideWay = false;
                DImplementation->DWays.push_back(currentWay);
            }
        }
    }
}

COpenStreetMap::~COpenStreetMap() = default;

std::size_t COpenStreetMap::NodeCount() const noexcept {
    return DImplementation->NodeCount();
}

std::size_t COpenStreetMap::WayCount() const noexcept {
    return DImplementation->WayCount();
}

std::shared_ptr<CStreetMap::SNode> COpenStreetMap::NodeByIndex(std::size_t index) const noexcept {
    return DImplementation->NodeByIndex(index);
}

std::shared_ptr<CStreetMap::SNode> COpenStreetMap::NodeByID(TNodeID id) const noexcept {
    return DImplementation->NodeByID(id);
}

std::shared_ptr<CStreetMap::SWay> COpenStreetMap::WayByIndex(std::size_t index) const noexcept {
    return DImplementation->WayByIndex(index);
}

std::shared_ptr<CStreetMap::SWay> COpenStreetMap::WayByID(TWayID id) const noexcept {
    return DImplementation->WayByID(id);
}
