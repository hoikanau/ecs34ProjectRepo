#include "../include/main.h"

void XMLCALL startElement(void *userData, const char *name, const char **atts) {
    std::cout << "Start Element: " << name << std::endl;
    for (int i = 0; atts[i]; i += 2) {
        std::cout << " - Attribute: " << atts[i] << " = " << atts[i + 1] << std::endl;
    }
}

void XMLCALL endElement(void *userData, const char *name) {
    std::cout << "End Element: " << name << std::endl;
}

bool parseXML(const std::string &filename) {
    char buffer[BUFSIZ];
    XML_Parser parser = XML_ParserCreate(nullptr);
    
    if (!parser) {
        std::cerr << "Failed to create parser!" << std::endl;
        return false;
    }

    XML_SetElementHandler(parser, startElement, endElement);

    std::ifstream file(filename, std::ios::binary);
    if (!file) {
        std::cerr << "Failed to open file: " << filename << std::endl;
        XML_ParserFree(parser);
        return false;
    }

    while (file.read(buffer, sizeof(buffer)) || file.gcount() > 0) {
        if (XML_Parse(parser, buffer, file.gcount(), file.eof()) == XML_STATUS_ERROR) {
            std::cerr << "Parse error: " << XML_ErrorString(XML_GetErrorCode(parser))
                      << " at line " << XML_GetCurrentLineNumber(parser) << std::endl;
            XML_ParserFree(parser);
            return false;
        }
    }

    XML_ParserFree(parser);
    std::cout << "XML parsing completed successfully." << std::endl;
    return true;
}

// int main() {
//     std::string filename = "sample.xml";
//     parseXML(filename);
//     return 0;
// }
