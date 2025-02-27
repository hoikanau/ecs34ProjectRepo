#include "XMLWriter.h"
#include "DataSink.h"
#include "XMLEntity.h"
#include <memory>
#include <string>
#include <vector>

// PIMPL structure for CXMLWriter
struct CXMLWriter::SImplementation {
    std::shared_ptr<CDataSink> sink;
    std::vector<std::string> openElements; // Stack of currently opened element names

    SImplementation(std::shared_ptr<CDataSink> s)
        : sink(s)
    { }

    // Helper Function to write a string to the sink
    bool WriteStr(const std::string &str) {
        std::vector<char> buf(str.begin(), str.end());
        return sink->Write(buf);
    }

    // Helper function to escape XML special characters
    std::string Escape(const std::string &str) {
        std::string out;
        for (char c : str) {
            switch (c) {
                case '&':  out.append("&amp;"); break; //special characters
                case '<':  out.append("&lt;"); break;
                case '>':  out.append("&gt;"); break;
                case '\"': out.append("&quot;"); break;
                case '\'': out.append("&apos;"); break;
                default:   out.push_back(c); break;
            }

        }
        return out;
    }

};

//
// CXMLWriter Member Functions
//

CXMLWriter::CXMLWriter(std::shared_ptr<CDataSink> sink): DImplementation(std::make_unique<SImplementation>(sink))
{ }

CXMLWriter::~CXMLWriter() = default;
// WriteEntity writes out the  XML entity
bool CXMLWriter::WriteEntity(const SXMLEntity &entity) {
    if (!DImplementation->sink)
        return false;

    std::string output;

    switch(entity.DType) {
        case SXMLEntity::EType::StartElement:

            // Write a start tag with attributes.
            output = "<" + entity.DNameData;

            for (const auto &attr : entity.DAttributes) {
                output += " " + attr.first + "=\"" + DImplementation -> Escape(attr.second) + "\"";
            }

            output += ">";

            if (!DImplementation -> WriteStr(output))
                return false;

            DImplementation -> openElements.push_back(entity.DNameData);

                break;
        
        case SXMLEntity::EType::EndElement:
            // If matching an open element, pop it
            if (!DImplementation -> openElements.empty() && DImplementation -> openElements.back() == entity.DNameData){
                DImplementation -> openElements.pop_back();
            }

            output = "</" + entity.DNameData + ">";
            if (!DImplementation ->WriteStr(output)){
                return false;
            }

            break;

        case SXMLEntity::EType::CompleteElement:
            
        // Write a self-closing tag.
            output = "<" + entity.DNameData;
            for (const auto &attr : entity.DAttributes) {
                output += " " + attr.first + "=\"" + DImplementation-> Escape(attr.second) + "\"";
            }
            output += "/>";
            if (!DImplementation -> WriteStr(output))
                return false;
            break;
        case SXMLEntity::EType::CharData:
            // Write escaped character data.
            output = DImplementation -> Escape(entity.DNameData);
            if (!DImplementation -> WriteStr(output)){
                return false;
            }

            break;

        default:
            return false;

    }
    return true;

}

// Flush outputs end tags for any remaining open elements.
bool CXMLWriter::Flush() {

    bool success = true;

    while (!DImplementation->openElements.empty()) {
        std::string tag = DImplementation -> openElements.back();
        DImplementation -> openElements.pop_back();
        
        std::string output = "</" + tag + ">";
        success = success && DImplementation->WriteStr(output);
    }

    return success;

}
