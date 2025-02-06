#ifndef MAIN_H
#define MAIN_H

#include <expat.h>
#include <iostream>
#include <fstream>
#include <string>

// Function declarations
void XMLCALL startElement(void *userData, const char *name, const char **atts);
void XMLCALL endElement(void *userData, const char *name);
bool parseXML(const std::string &filename);

#endif // MAIN_H
