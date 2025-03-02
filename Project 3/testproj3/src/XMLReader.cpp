#include "XMLReader.h"
#include "DataSource.h"
#include "XMLEntity.h"
#include <expat.h>
#include <memory>
#include <string>
#include <vector>

// PIMPL structure for CXMLReader
struct CXMLReader::SImplementation {
    std::shared_ptr<CDataSource> source;
    std::string xmlData;                // holds complete XML text
    std::vector<SXMLEntity> entities;  // parsed XML entities
    mutable size_t currIndex;                 // next entity index to return
    XML_Parser pars;                 // Expat parser instance

    SImplementation(std::shared_ptr<CDataSource> src): source(src), currIndex(0){
        // Read entire XML input from the data source
        char ch;

        while (source->Get(ch)) {
            xmlData.push_back(ch);

        }

        // Create an Expat parser
        pars = XML_ParserCreate(nullptr);

        XML_SetUserData(pars, this);
        XML_SetElementHandler(pars, StartElementHandler, EndElementHandler);
        XML_SetCharacterDataHandler(pars, CharacterDataHandler);

        // Parse the XML data
        if (XML_Parse(pars, xmlData.c_str(), static_cast<int>(xmlData.size()), XML_TRUE) == XML_STATUS_ERROR) {
            // If a parsing error occurs, clear the entities
            entities.clear();
        }

        XML_ParserFree(pars);
    }

    // Callback for start elements
    static void XMLCALL StartElementHandler(void *userData,const XML_Char *name,const XML_Char **atts)
    {
        SImplementation* impl = static_cast<SImplementation*>(userData);
        SXMLEntity entity;

        entity.DType = SXMLEntity::EType::StartElement;
        entity.DNameData = name;

        // Process attributes: atts is a null-terminated array of name/value pairs
        if (atts) {
            for (int i = 0; atts[i]; i += 2) {
                entity.DAttributes.push_back(std::make_pair(atts[i], atts[i+1]));

            }
        }

        impl-> entities.push_back(entity);
    }

    // Callback for end element
    static void XMLCALL EndElementHandler(void *userData,const XML_Char *name)
    {
        SImplementation* impl = static_cast<SImplementation*>(userData);
        SXMLEntity entity;

        entity.DType = SXMLEntity::EType::EndElement;
        entity.DNameData = name;
        impl->entities.push_back(entity);
    }

    // Callback for character data
    static void XMLCALL CharacterDataHandler(void *userData, const XML_Char *s,int len)
    {
        SImplementation* impl = static_cast<SImplementation*>(userData);
        std::string data(s, len);

        // Create an entity for the character datas
        SXMLEntity entity;
        entity.DType = SXMLEntity::EType::CharData;
        entity.DNameData = data;
        impl->entities.push_back(entity);
    }
};

//
// CXMLReader Member Functions
//

CXMLReader::CXMLReader(std::shared_ptr<CDataSource> src): DImplementation(std::make_unique<SImplementation>(src))
{
    
}

CXMLReader::~CXMLReader() = default;

// End() returns true if all entities have been returned
bool CXMLReader::End() const {
    return DImplementation->currIndex >= DImplementation->entities.size();
}

// ReadEntity returns the next entity 
//(skipping character data if skipcdata==true)

bool CXMLReader::ReadEntity(SXMLEntity &entity, bool skipcdata) {

    while (DImplementation->currIndex < DImplementation->entities.size()) {
        entity = DImplementation->entities[DImplementation->currIndex];
        DImplementation->currIndex++;

        if (skipcdata && entity.DType == SXMLEntity::EType::CharData)
            continue;
        return true;

    }

    return false;

}

void CXMLReader::Reset() const{
    DImplementation->currIndex = 0;
}
