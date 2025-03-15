#include <iostream>
#include <memory>
#include <vector>
#include <string>
#include "DSVReader.h"
using namespace std;

// Helper function to post-process a CSV row.
// For each field that is enclosed in quotes, it removes the outer quotes
// and unescapes any doubled quotes (i.e. "" becomes ").
static void PostProcessRow(vector<string>& row) {
    for (auto &field : row) {
        if (field.size() >= 2 && field.front() == '"' && field.back() == '"') {
            string inner = field.substr(1, field.size() - 2);
            string unescaped;
            for (size_t i = 0; i < inner.size(); i++) {
                if (inner[i] == '"' && i + 1 < inner.size() && inner[i + 1] == '"') {
                    unescaped.push_back('"');
                    i++; // Skip the next quote
                } else {
                    unescaped.push_back(inner[i]);
                }
            }
            field = unescaped;
        }
    }
}

// PIMPL Format: This implementation is for the forward declaration in DSVReader.h
struct CDSVReader::SImplementation {
    // SourceFile refers to the input file that the program will read from
    shared_ptr<CDataSource> SourceFile;
    // userDelimiter refers to the specific delimiter that the program is 
    char userDelimiter;

    // Member initializer with Source and delimiter for SourceFile and userDelimiter respectively
    SImplementation(shared_ptr<CDataSource> Source, char delimiter) : SourceFile(Source), userDelimiter(delimiter) {}
};

// Overloaded Constructor + Member initializer
CDSVReader::CDSVReader(shared_ptr<CDataSource> source, char delimiter) : DImplementation(make_unique<SImplementation>(source, delimiter)) {}

// Destructor
CDSVReader::~CDSVReader() = default;

// Returns true if at end of source.
bool CDSVReader::End() const
{
    return DImplementation->SourceFile->End();
}

// ReadRow: Reads one row from the data source.
bool CDSVReader::ReadRow(vector<string>& row)
{
    row.clear();
    string currentField;
    bool fieldHasQuotes = false;
    bool keepReading = true;
    while (keepReading == true)
    {
        char currentChar;
        bool gettingChar = DImplementation->SourceFile->Get(currentChar);

        // SECTION 1: BASE CASE
        if (gettingChar == false)
        {
            if (fieldHasQuotes == true) {
                return false;
            }
            if (!currentField.empty() || !row.empty()) {
                row.push_back(currentField);
                PostProcessRow(row); // UPDATED: Unescape quoted fields.
                return true;
            }
            return false;
        }
        
        // SECTION 2: When inside a quoted field.
        if (fieldHasQuotes == true) {
            if (currentChar == '"') {
                char nextChar;
                bool gettingNextChar = DImplementation->SourceFile->Get(nextChar);
                if (gettingNextChar == true && nextChar == '"') {
                    currentField.push_back('"');
                    continue; // Consume both quotes as one.
                } else {
                    // End of quoted field.
                    fieldHasQuotes = false;
                    if (gettingNextChar == true) {
                        currentChar = nextChar; // Continue processing this character.
                    } else {
                        row.push_back(currentField);
                        PostProcessRow(row); // UPDATED: Unescape quoted fields.
                        return true;
                    }
                }
            } else {
                currentField.push_back(currentChar);
                continue;
            }
        }

        // SECTION 3: Not inside a quoted field.
        if (currentChar == '"') {
            if (currentField.empty() == true) {
                // Start of a quoted field.
                fieldHasQuotes = true;
            } else {
                // Quote in the middle of an unquoted field; treat as normal character.
                currentField.push_back(currentChar);
            }
        } else if (currentChar == DImplementation->userDelimiter) {
            row.push_back(currentField);
            currentField.clear();
        } else if (currentChar == '\n') {
            row.push_back(currentField);
            PostProcessRow(row); // UPDATED: Unescape quoted fields.
            return true;
        } else {
            currentField.push_back(currentChar);
        }
    }
    return false;
}