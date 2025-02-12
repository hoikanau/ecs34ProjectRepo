#include <iostream>
#include <memory>
#include <vector>
#include <string>
#include "DSVReader.h"
using namespace std;


//PIMPL Format: This implementation is for the forward declaration in DSVReader.h
struct CDSVReader::SImplementation
{
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

// Returns true or false if the we are done reading the file
bool CDSVReader::End() const
{
    // If the "pointer" is at the end of the file return true, else false
    return DImplementation->SourceFile->End();
}
// There will be 3 sections i will be referring to:
// Section 1, Section 2, Section 3
// Defining ReadRow from CDSVReader
// We are going to read the individual rows of the file and returns true or false if it was successfully read
bool CDSVReader::ReadRow(vector<string>& row)
{
    // Ensure that the row is empty if we had read a row before
    // i.e. if it is the second iteration of reading the file, we need to clear the vector so we don't read the same row as the first
    // iteration
    row.clear();
    // currentField refers to the first string surrounded by quotes in the row
    // Initially it is empty, but we will slowly append the individual chars in from each string in row to currentField
    string currentField;
    // fieldHasQuotes refers to if the string has quotes inside the quotes or not
    bool fieldHasQuotes = false;
    // keepReading helps the following while loop to keep reading as long as there are more strings in the row to read
    bool keepReading = true;
    // while loop that keeps reading as long as there are strings to read in the row / in other words if we can still get more
    // strings from the vector
    while (keepReading == true)
    {
        // currentChar refers to the individual chars in the string that we will be reading one by one
        char currentChar;
        // Get a true or false value if we were able to read the char in the string
        bool gettingChar = DImplementation->SourceFile->Get(currentChar);

        // SECTION 1: BASE CASE
        // If we were not able to get the next char from the string meaning that we are done with the string
        if (gettingChar == false)
        {
            // If the given source file to read from has an incomplete string
            // example: "line1,line2,"InquotesLine3", "InCompleteLine4, line5\n"
            // Notice the fourth string is incomplete because it is missing a quote which will enter is if statement
            // then returns false because reading was unsuccessful
            if (fieldHasQuotes == true)
            {
                return false;
            }
            // Checks if the current string we are reading from is NOT empty or the entire row is NOT empty
            if (currentField.empty() == false || row.empty() == false)
            {
                // Reading was successful, push the entire string in currentField to the row
                row.push_back(currentField);
                return true;
            }
            // If it doesn't pass the previous if statement, it did not successfully read, return false
            return false;
        }
        
        // SECTION 2: IF THERE ARE INLINE QUOTES
        // If the current string has quotes enter the following if statement
        if (fieldHasQuotes == true){
            // If the read input is a quote
            if(currentChar == '"'){
                // Get the next character in the string
                char nextChar;
                bool gettingNextChar = DImplementation->SourceFile->Get(nextChar);
                // If we successfully get the next character and it is infact a quote because that is what we are expecting for
                // inline quotes
                if (gettingNextChar == true && nextChar == '"'){
                    // Push back a quote and continue
                    currentField.push_back('"');
                    continue;
                // otherwise it is either the end of string or it is incomplete
                }else{
                    // So we check if the string is completed, this is determined in section 3 when it iterates through the row
                    // and see that we have hit the delimiter so fieldHasQuotes becomes false
                    fieldHasQuotes = false;
                    // If it is still possible to get the next charater, then reading the row is still incomplete
                    if(gettingNextChar == true){
                        currentChar = nextChar;
                    }else{
                        // Reading the row is complete we can push the string in currentField to the row and return true
                        row.push_back(currentField);
                        return true;
                    }
                }
            // The input is not a quote meaning it just some char within the inline quote
            // very simple case just append it to the running string currentField.
            }else{
                // currentField.at(currentField.size()-1) = currentChar
                currentField.push_back(currentChar);
                continue;
            }
        }
        

        // SECTION 3: ITERATIVE STEP AND CHECK FOR INLINE QUOTES
        // Check if the current char is a quote or not
        if(currentChar == '"'){
            // If it is a quote check if the running string, currentField, is empty
            if(currentField.empty() == true){
                // If it is empty this means that it is the start of an inline quoted string so turn fieldHasQuotes to true which will get
                // caught by SECTION 2
                fieldHasQuotes = true;
            }else{
                // otherwise the quote is part of the string so just append it to currentField
                currentField.push_back(currentChar);
            }
        // else if the currentChar is the delimiter given by the user
        }else if(currentChar == DImplementation->userDelimiter){
            // push the currentField, which is the running string, to the vector row which will separate the individual strings by
            // their corresponding index
            // i.e. if the row was "This,is,a,row\n"
            // row[0] = "this"   row[1] = "is"   row[2] = "a"   row[3] = "row"
            row.push_back(currentField);
            // clear currentField to prep for the next string in the row
            currentField.clear();
        // else if the read input is the new line indicator, we have completed reading the row
        }else if(currentChar == '\n'){
            // push currentField to row and return true
            row.push_back(currentField);
            return true;
        // otherwise it is just a character in the string so append it to currentField and go back to the beginning of the loop
        }else{
            currentField.push_back(currentChar);
        }
    }
}

// class CDSVReader{
//     private:
//         struct SImplementation;
//         std::unique_ptr<SImplementation> DImplementation;

//     public:
//         CDSVReader(std::shared_ptr< CDataSource > src, char delimiter);
//         ~CDSVReader();

//         bool End() const;
//         bool ReadRow(std::vector<std::string> &row);