#include "OpenStreetMap.h"
#include "StreetMap.h"
#include "StringDataSource.h"
#include "StringDataSink.h"
#include "XMLReader.h"
#include "XMLWriter.h"
#include "XMLEntity.h"
#include <iostream>
#include <memory>
#include <utility>
#include <vector>
#include <string>

using namespace std;
// PIMPL Structure for Implementation
struct COpenStreetMap::SImplementation
{
    // SNode is one of the struct located in CStreetMap that contains functions that we have to implement
    // We call SNode and derive it from CStreetMap's SNode in StreetMap.h to override the virtual functions
    struct SNode : public CStreetMap::SNode
    {
        // Let's declare a couple of variables first:
        //   nodeID - variable that holds the ID of a certain node
        //   location - variable that holds the latitude and longitude value in a pair<double,double> structure with the respective coordinates first and second
        //   attributes - this vector stores the different attributes a node may have
        TNodeID nodeID;
        TLocation location;
        vector<pair<string, string>> attributes;

        // Returns the id of the SNode
        TNodeID ID() const noexcept
        {
            return nodeID;
        }
        // Returns the lat/lon location of the SNode
        TLocation Location() const noexcept
        {
            return location;
        }
        // Returns the number of attributes attached to the SNode
        size_t AttributeCount() const noexcept
        {
            return attributes.size();
        }
        // Returns the key of the attribute at index, returns empty string if index
        // is greater than or equal to AttributeCount()
        string GetAttributeKey(size_t index) const noexcept
        {
            // User passes a value into index then we check if the index that the user passed is less than the size of the attributes
            // The user CANNOT pass a value such as 10 if our vector only hold 3 items
            if (index < attributes.size())
            {
                return attributes[index].first;
            }
            else
            {
                return "";
            }
        }
        // Returns if the attribute is attached to the SNode
        bool HasAttribute(const string &key) const noexcept
        {
            // Loop through the vector attributes and for every element check if the first value in the pair of the element is equal to the key that the user passed
            for (auto &attr : attributes)
            {
                // If there exists a key such that it is also found in the attributes vector, that means the node has the attribute
                if (attr.first == key)
                {
                    return true;
                }
            }
            return false;
        }
        // Returns the value of the attribute specified by key, returns empty string
        // if key hasn't been attached to SNode
        string GetAttribute(const string &key) const noexcept
        {
            // Loop through the vector attributes and for every element check if the first value in the pair of the element is equal to the key the user passed
            for (const auto &attr : attributes)
            {
                // If there exists a key such that it is also found in the attributes vector, return the value of the attribute which would be the second value in the pair
                if (attr.first == key)
                {
                    return attr.second;
                }
            }
            return "";
        }
        // Destructor
        ~SNode() = default;
    };

    // SWay is another struct located in CStreetMap that contains functions that we have to implement
    // We call SWay and derive it from CStreetMap's SWay in StreetMap.h to override the virtual functions
    struct SWay : public CStreetMap::SWay
    {
        // Let's set up from variables first
        //   wayID - this holds the ID of a way
        //   nodesInWay - this vector holds all the nodes that are connected by a specific way
        //   attributes - this vector holds all the possible attributes attached to the way
        TWayID wayID;
        vector<TNodeID> nodesInWay;
        vector<pair<string, string>> attributes;
        // Destructor
        ~SWay() = default;
        // Returns the id of the SWay
        TWayID ID() const noexcept
        {
            return wayID;
        }
        // Returns the number of nodes in the way
        size_t NodeCount() const noexcept
        {
            return nodesInWay.size();
        }
        // Returns the node id of the node at index, returns InvalidNodeID if index
        // is greater than or equal to NodeCount()
        TNodeID GetNodeID(size_t index) const noexcept
        {
            // Check if the user input something that is larger than the size of the total nodesInWay
            // If it is larger then there isn't any values stored in that element. We will be accessing memory that is not managed by the vector and the compiling will complain
            // Otherwise, return the value with respect to the user input from the nodesInWay vector
            if (index < nodesInWay.size())
            {
                return nodesInWay[index];
            }
            else
            {
                return InvalidNodeID;
            }
        }
        // Returns the number of attributes attached to the SWay
        size_t AttributeCount() const noexcept
        {
            return attributes.size();
        }
        // Returns the key of the attribute at index, returns empty string if index
        // is greater than or equal to AttributeCount()
        string GetAttributeKey(size_t index) const noexcept
        {
            // Check if the user input something that is larger than the size of the attributes vector
            // Meaning they are accessing memory that is not managed by the vector
            // Otherwise return the key of the attribute at the index passed by the user
            if (index < attributes.size())
            {
                return attributes[index].first;
            }
            else
            {
                return "";
            }
        }
        // Returns if the attribute is attached to the SWay
        bool HasAttribute(const string &key) const noexcept
        {
            // Check for all elements of attributes
            for (const auto &attr : attributes)
            {
                // If there exist a first value in the pair of a element of attributes that equals to the key passed by the user
                // that means we have found the attribute we are looking for so return true because the way has the attribute
                if (attr.first == key)
                {
                    return true;
                }
            }
            return false;
        }
        // Returns the value of the attribute specified by key, returns empty string
        // if key hasn't been attached to SWay
        string GetAttribute(const string &key) const noexcept
        {
            // Check for all elements of attributes
            for (const auto &attr : attributes)
            {
                // If there exists a value in the pair of a element in attributes that equals to the key passed by the user
                // that means we have found the attribute we are looking for so return the second value in the pair which is the value of the key
                if (attr.first == key)
                {
                    return attr.second;
                }
            }
            return "";
        }
    };

    // These two vectors hold ALL of the nodes and ways that is in the OSM file.
    vector<SNode> allNodes;
    vector<SWay> allWays;
};

// Constructor for the Open Street Map
COpenStreetMap::COpenStreetMap(shared_ptr<CXMLReader> src) : DImplementation(make_unique<SImplementation>())
{
    // Here I have two boolean checks to tell me whether I am currently modifying inside node or modifying inside way
    bool modifyingNode = false;
    bool modifyingWay = false;

    // Initialization of SNode and SWay as currNode and currWay respectively
    SImplementation::SNode currNode;
    SImplementation::SWay currWay;

    // SXMLEntity entity utilizes XMLEntity.h in conjunction with XMLReader.cpp that was implemented in project 2
    SXMLEntity entity;
    // We read until we reach the end of the source(src) file
    while (!src->End())
    {
        // If we were not able to read a section, break out of the loop
        if (!src->ReadEntity(entity, false))
        {
            break;
        }
        // Check if we are done processing the element
        if (entity.DType == SXMLEntity::EType::CompleteElement)
        {
            // IF we finished processing a node element
            if (entity.DNameData == "node")
            {
                // Let's create a temporary node called nodeTemporary
                SImplementation::SNode nodeTemporary;

                // Let's loop for all the attributes in the node and &attr will be referencing those attributes
                for (const auto &attr : entity.DAttributes)
                {
                    // If it is an ID turn caste it form a string to a unsigned long long and input it as the nodeID
                    if (attr.first == "id")
                    {
                        nodeTemporary.nodeID = stoull(attr.second);
                    }
                    // Else if it is referring to one of the corrdinates in this case lat, caste the string to a double and throw it into the first value of the pair data structure
                    else if (attr.first == "lat")
                    {
                        nodeTemporary.location.first = stod(attr.second);
                    }
                    // Similarly for the logitutde coordinate, caste it from a string to a double but throw it into the second value of the pair data structure
                    else if (attr.first == "lon")
                    {
                        nodeTemporary.location.second = stod(attr.second);
                    }
                    // Finally if it is neither the ID nor the coordinates, it is an attribute that simply describes the node, so append it to the vector of the node
                    else
                    {
                        nodeTemporary.attributes.push_back(attr);
                    }
                }
                DImplementation->allNodes.push_back(nodeTemporary);
            }
            else if (entity.DNameData == "way")
            {
                SImplementation::SWay tempWay = {};
                for (const auto &attr : entity.DAttributes)
                {
                    if (attr.first == "id")
                    {
                        tempWay.wayID = stoull(attr.second);
                    }
                    else
                    {
                        tempWay.attributes.push_back(attr);
                    }
                }
                DImplementation->allWays.push_back(tempWay);
            }
            continue;
        }
        // If the entity's data type hence DType is the start element tag, we need to determine if we are reading a node or a way
        if (entity.DType == SXMLEntity::EType::StartElement)
        {
            // CASE 1: WE ARE READING A NODE
            //    The if statement checks if we are reading a node
            if (entity.DNameData == "node")
            {
                // We are currently reading and will modify a node value so we set modifyingNode to true
                modifyingNode = true;
                // Essentially we are creating an empty box of which are we going to throw in attributes of the node such as it's ID and location (lon x lat)
                currNode = {};
                // Loop through all the attributes attached to the node with &attr references those attributes
                for (const auto &attr : entity.DAttributes)
                {
                    // If it is an ID turn caste it form a string to a unsigned long long and input it as the nodeID
                    if (attr.first == "id")
                    {
                        currNode.nodeID = stoull(attr.second);
                    }
                    // Else if it is referring to one of the corrdinates in this case lat, caste the string to a double and throw it into the first value of the pair data structure
                    else if (attr.first == "lat")
                    {
                        currNode.location.first = stod(attr.second);
                    }
                    // Similarly for the logitutde coordinate, caste it from a string to a double but throw it into the second value of the pair data structure
                    else if (attr.first == "lon")
                    {
                        currNode.location.second = stod(attr.second);
                    }
                    // Finally if it is neither the ID nor the coordinates, it is an attribute that simply describes the node, so append it to the vector of the node
                    else
                    {
                        currNode.attributes.push_back(attr);
                    }
                }
            }
            // CASE 2: WE ARE LOOKING AT A WAY
            else if (entity.DNameData == "way")
            {
                // Since we are looking at a way we will be modifying the elements of way
                modifyingWay = true;
                // Create an empty box to store the data of way in the osm into a data structure in this program
                currWay = {};
                // Loop through every element in the entity's attributes (in this case the way's attributes) and &attr will be referencing those elements
                for (const auto &attr : entity.DAttributes)
                {
                    // If the attribute's value is id
                    if (attr.first == "id")
                    {
                        // Let's turn the ID strin to an unsigned long long and set it to the current way's ID
                        currWay.wayID = stoull(attr.second);
                    }
                    // otherwise it just an attribute that describes the way so append it to the attributes vector within the SWay object
                    else
                    {
                        currWay.attributes.push_back(attr);
                    }
                }
            }
            // CASE 3: WE ARE LOOKING AT ESSENTIALLY DESCRIPTORS OF A SPECIFIC NODE OR WAY DENOTED BY TAG
            else if (entity.DNameData == "tag")
            {
                // In the osm file, k denotes key and v denotes value so it makes up a key value pair (exmaple: k=lanes v=2; this way has 2 lanes)
                // It just describes the way
                string key;
                string value;
                // Let's loop for all of the attributes of tag denoted by entity.attributes which contains all of the content
                for (const auto &attr : entity.DAttributes)
                {
                    // Let's take notes of the key value pairs
                    // If the first attribute is k this means it is a key
                    if (attr.first == "k")
                    {
                        key = attr.second;
                    }
                    // else if it is v then it is value
                    else if (attr.first == "v")
                    {
                        value = attr.second;
                    }
                }
                // Let's check where are we currently modifying whether it is a node or a way
                // If we are modifying a node then append the key value pair into the current node's attributes vector
                if (!key.empty() && !value.empty())
                {
                    if (modifyingNode == true)
                    {
                        currNode.attributes.push_back({key, value});
                    }
                    // else if we are modifying a way then append the key value pair into the current way's attributes vector
                    else if (modifyingWay == true)
                    {
                        currWay.attributes.push_back({key, value});
                    }
                }
            }
            // CASE 4: WE ARE LOOKING AT A REFERENCE TO A NODE FOR A SPECIFIC WAY (I.E. THIS WAY IS CONNECTING THIS NODE DENOTED BY nd ref MEANING NODE REFERENCE)
            else if (entity.DNameData == "nd" && modifyingWay == true)
            {
                // Loop through all the content/attributes inside nd
                for (const auto &attr : entity.DAttributes)
                {
                    // Ideally there should only be one thing in the line which is just a reference to the node
                    // So just to be sure, check if the first value is ref to ensure that the number after is a node ID reference
                    if (attr.first == "ref")
                    {
                        // Let's take note of the ID that references the node that the way connects to by initializing an object called TNodeID
                        CStreetMap::TNodeID referenceID = stoull(attr.second);
                        // Then append the referenceID to a vector in the current way that holds all of the nodes that is connected by the way
                        currWay.nodesInWay.push_back(referenceID);
                    }
                }
            }
        }
        // Check if we have reached the end of the element by checking if it is the end element tag
        else if (entity.DType == SXMLEntity::EType::EndElement)
        {
            // If we are done reading the node make sure we change back modifyingNode from true to false and push the current node that we have read onto a vector called allNodes
            if (entity.DNameData == "node")
            {
                modifyingNode = false;
                DImplementation->allNodes.push_back(currNode);
            }
            // Similarly with the way, change back modifyingWay from true to false and push the current way onto a vector called allWays
            else if (entity.DNameData == "way")
            {
                modifyingWay = false;
                DImplementation->allWays.push_back(currWay);
            }
            // Now we are ready to loop again for another line
        }
    }
}

// Seems like the compiler does not know where to access these functions if I implement all of them inside SImplementation so I call these functions again outside of the struct
// To let the compiler know that these are functions that will be derived and used inside the struct
COpenStreetMap::~COpenStreetMap() = default;

// Returns the number of nodes in the map
size_t COpenStreetMap::NodeCount() const noexcept
{
    return DImplementation->allNodes.size();
}

// Returns the number of ways in the map
size_t COpenStreetMap::WayCount() const noexcept
{
    return DImplementation->allWays.size();
}

// Returns the SNode associated with index, returns nullptr if index is
// larger than or equal to NodeCount()
shared_ptr<CStreetMap::SNode> COpenStreetMap::NodeByIndex(size_t index) const noexcept
{
    // Check if the user is trying to access an index that is NOT in allNodes vector which can be determined by the size of allNodes
    // If so, return nullptr
    if (index >= DImplementation->allNodes.size())
    {
        return nullptr;
    }
    // Otherwise return a new shared_ptr that poitns to the element at the given index in allNodes
    return make_shared<SImplementation::SNode>(DImplementation->allNodes[index]);
}

// Returns the SNode with the id of id, returns nullptr if doesn't exist
shared_ptr<CStreetMap::SNode> COpenStreetMap::NodeByID(TNodeID id) const noexcept
{
    // Loop through all the elements of allNodes with &node be the reference to every element in the vector
    for (const auto &node : DImplementation->allNodes)
    {
        // Check if the node ID is equal to the user inputted id.
        // If it matches, we have found the correct node to output as a new shared_ptr
        if (node.nodeID == id)
        {
            return make_shared<SImplementation::SNode>(node);
        }
    }
    // Otherwise return nullptr
    return nullptr;
}

// Returns the SWay associated with index, returns nullptr if index is
// larger than or equal to WayCount()
shared_ptr<CStreetMap::SWay> COpenStreetMap::WayByIndex(size_t index) const noexcept
{
    // Similar to NodeByIndex let's first check if the index that the user inputted exceeds the capacity of the vector
    // Meaning it doesn't exist
    if (index >= DImplementation->allWays.size())
    {
        // If so return nullptr
        return nullptr;
    }
    // Otherwise return a shared_ptr of the element at the index in allWays
    return make_shared<SImplementation::SWay>(DImplementation->allWays[index]);
}
// Returns the SWay with the id of id, returns nullptr if doesn't exist
shared_ptr<CStreetMap::SWay> COpenStreetMap::WayByID(TWayID id) const noexcept
{
    // Loop through all the elements in allWays with &way referencing those elements
    for (const auto &way : DImplementation->allWays)
    {
        // Check using the function ID in Sway to see if the way ID matches the one that the user inputted
        if (way.wayID == id)
        {
            // If it matches, return a new shared_ptr of the corresponding way
            return make_shared<SImplementation::SWay>(way);
        }
    }
    // Otherwise return nullptr
    return nullptr;
}
