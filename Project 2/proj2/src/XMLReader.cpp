#include "XMLReader.h"
#include <iostream>
#include <string>
#include <vector>

using namespace std;

CXMLReader::CXMLReader(shared_ptr<CDataSource> src){

}

CXMLReader::~CXMLReader(){

}

bool CXMLReader::End() const{
    return false;
}

bool CXMLReader::ReadEntity(SXMLEntity &entity, bool skipcdata){
    return false;
}


        // CXMLReader(std::shared_ptr< CDataSource > src);
        // ~CXMLReader();
        
        // bool End() const;
        // bool ReadEntity(SXMLEntity &entity, bool skipcdata = false);