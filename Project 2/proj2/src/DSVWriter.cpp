#include <iostream>
#include <memory>
#include <vector>
#include <string>
#include <algorithm>
#include "DSVWriter.h"
using namespace std;

// PIMPL STRUCTURE DEFINED FOR THE FORWARD DECLARATION IN DSVWriter.h FILE
struct CDSVWriter::SImplementation{
    // Smart pointer pointing to the final output after writing
    shared_ptr<CDataSink> FileSink;
    char userDelimiter;
    bool needQuoteForAllString;

    // Member initializere
    SImplementation(shared_ptr<CDataSink> sink, char delimiter, bool quoteall) : FileSink(sink), userDelimiter(delimiter), needQuoteForAllString(quoteall){}

    // Returns true or false if we were able to successfully write to the file sink, FileSink
    bool WriteStr(const string& str){
        // Initialize a vector that holds individual chars called buffer with the size of str.begin() and initialize the values to str.end()
        vector<char> buffer(str.begin(), str.end());
        // This is passed to Write which is a given function then returns true if written successfully otherwise false
        return FileSink->Write(buffer);
    }
};

// Overloaded Constructor + Member initailizer  when creating a new CDSVWriter object
CDSVWriter::CDSVWriter(shared_ptr<CDataSink> sink, char delimiter, bool quoteall) : DImplementation(make_unique<SImplementation>(sink, delimiter, quoteall)) {}

// Destructor
CDSVWriter::~CDSVWriter() = default;

// Defining WriteRow
bool CDSVWriter::WriteRow(const vector<string>& row)
{
    // If there isn't a file sink to write to that the user specified then terminate the program immediately by returning false
    if (!DImplementation->FileSink){
        return false;
    }

    // Loop for the size of the row which is passed in by the user
    for (int i = 0; i < row.size(); i++){
        // declaring a const string& so that it is not modifiable and we don't copy it to improve runtime incase the size of the string in row[i] is too large
        // currentField is the string at row index i
        const string& currentString = row[i];

        // Determing if we need to quote the string
        bool doesItNeedQuotes = DImplementation->needQuoteForAllString;

        // If the user declares that the row does not require quotes for all string
        if (doesItNeedQuotes == false){
            // Then check if the string in the current field has a delimiter, quotes, a new line, which will turn doesItNeedQuotes to true
            if ((currentString.find(DImplementation->userDelimiter) != string::npos) ||(currentString.find('"') != string::npos) || (currentString.find('\n') != string::npos)){
                // This means that we will write inline quoted string
                doesItNeedQuotes = true;
            }
        }

        // Enters the if statement if we are required to write an inline quoted string
        if (doesItNeedQuotes == true){
            // This will be the running string which gets appended at the end 
            string runningString;
            // Lets reserve two spaces for the additional quotes when we are writing an inlined quoted string
            runningString.reserve(currentString.size() + 2);
            // Immediately push back one of the quotes to the beginning
            runningString.push_back('"');
            // Loop through the current string for every char
            for (auto charInString : currentString){
                // If the char in the string is a quote
                if (charInString == '"'){
                    // The quote is a part of the string so just append it to the running string
                    runningString.push_back('"');
                }
                // Otherwise the char that we are currently at is not a quote so just append it
                runningString.push_back(charInString);
            }
            // At the end of the loop we have to append another quote to complete the inline quoted string
            runningString.push_back('"');

            // Check if we successfully write the entire running string into the file sink
            if (DImplementation->WriteStr(runningString) == false){
                // if it was unsuccessful return false otherwise return true at the end
                return false;
            }
        }
        // We enter this else statement if the string does not required to be inline quoted
        else{
            // See if we can successfully write the entire string, if it was unsuccessful, return false
            if (DImplementation->WriteStr(currentString) == false)
            {
                return false;
            }
        }

        // If it's not the last field, write the delimiter
        // Checks if we are at the last set of string in the vector, if it is not the last set of string then write the delimiter then go back to the beginning of the loop
        if (i < row.size() - 1){
            // If we did not successfully write the delimiter then return false
            if (DImplementation->FileSink->Put(DImplementation->userDelimiter) == false){
                return false;
            }
        }
    }

    // This is outside the for loop meaning that we have written all the lines
    // The last time we have to do is to include the new line indicator at the end
    if (DImplementation->FileSink->Put('\n') == false){
        return false;
    }

    // If everything passes meaning that we did not encounter any annomalies than return true
    return true;
}