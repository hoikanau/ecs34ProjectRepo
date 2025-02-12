#include <iostream>
#include <memory>
#include <vector>
#include <string>
#include <algorithm>
#include "DSVWriter.h"
using namespace std;

//
// Implementation details are hidden in a private struct SImplementation
//
struct CDSVWriter::SImplementation
{
    shared_ptr<CDataSink> DSink;
    char DDelimiter;
    bool DQuoteAll;

    SImplementation(shared_ptr<CDataSink> sink, char delimiter, bool quoteall)
        : DSink(sink), DDelimiter(delimiter), DQuoteAll(quoteall) {}

    // Helper to write an entire  string to the sink
    bool WriteString(const string &str)
    {
        // Convert  string to  vector<char>
        vector<char> buffer(str.begin(), str.end());
        return DSink->Write(buffer);
    }
};

CDSVWriter::CDSVWriter(shared_ptr<CDataSink> sink, char delimiter, bool quoteall)
    : DImplementation(make_unique<SImplementation>(sink, delimiter, quoteall)) {}

CDSVWriter::~CDSVWriter() = default;

bool CDSVWriter::WriteRow(const vector<string> &row)
{
    // If sink is not available, fail immediately
    if (!DImplementation->DSink)
    {
        return false;
    }

    // Go through each field in the row
    for (size_t i = 0; i < row.size(); i++)
    {
        const string &field = row[i];

        // Determine if we need to quote
        bool doesItNeedQuotes = DImplementation->DQuoteAll;

        if (doesItNeedQuotes == false)
        {
            // If not quoting everything, check for delimiter, quotes, or newlines
            if ((field.find(DImplementation->DDelimiter) != string::npos) ||
                (field.find('"') != string::npos) ||
                (field.find('\n') != string::npos) ||
                (field.find('\r') != string::npos))
            {
                doesItNeedQuotes = true;
            }
        }

        if (doesItNeedQuotes)
        {
            // Build the quoted string
            string quoted;
            quoted.reserve(field.size() + 2);
            quoted.push_back('"');
            for (char c : field)
            {
                if (c == '"')
                {
                    // Double existing quotes
                    quoted.push_back('"');
                }
                quoted.push_back(c);
            }
            quoted.push_back('"');

            // Write the fully-quoted field
            if (!DImplementation->WriteString(quoted))
            {
                return false;
            }
        }
        else
        {
            // Write the unquoted field
            if (!DImplementation->WriteString(field))
            {
                return false;
            }
        }

        // If it's not the last field, write the delimiter
        if (i < row.size() - 1)
        {
            if (!DImplementation->DSink->Put(DImplementation->DDelimiter))
            {
                return false;
            }
        }
    }

    // After the row, write a newline
    if (!DImplementation->DSink->Put('\n'))
    {
        return false;
    }

    return true;
}