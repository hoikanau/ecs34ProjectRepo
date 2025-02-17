CDSVReader utilizes the following files:
    include/DSVReader.h    include/DataSource.h
    src/DSVReader.cpp

This class reads individual rows of a file.

bool End()
   This function of the CDSVReader checks if the file is at the end.
   If the file is at the end we want to stop reading and end the function by returning true otherwise return false.

bool ReadRow(std::vector<std::string> &row)
   This function of the CDSVReader reads the rows of the DSV file and returns true or false if it was successfully read

   For example, if your DSV file is something like this:
        Name,Age,Comment
        Bob,12,Four foot six
        Emily,20,In College

    If you call ReadRow on the DSV file, it will take the first row and input it into the vector/array &row
    Then it reads the individual characters of the row,
       In this case: row = Name,Age,Comment
       it will read The chars one by one until it hits a delimiter which would be the comma


CDSVWriter utilizes the following files:
    include/DSVWriter.h    include/DataSink.h
    src/DSVWriter.cpp

bool WriteRow(const std::vector<std::string> &row)
   DSVWriter only has one public function
   It writes the elements of row into a data sink which is essentially a output file and the function will return true or false if it was successfully written
   
   Before writing the file one must create a new CDSVWriter object by calling CDSVWriter writer(sink, delimiter, quoteall);
    sink refers to the output file
    delmiter refers to the what char that separates the values
    quoteall refers to a boolean if the user wants to quote all the values in the row when writing
    
    For example, if you already setup some CDSVWriter object called writer // CDSVWriter writer(sink, ',', false)
        You will write to a output file determined by sink with commas as spacers/delimiters and you will not quote all the values
    
    Then calling the following lines:
        writer.WriteRow({"Name", "Age", "Comment"});
        writer.WriteRow({"Bob", "21", "Attending ECS34"})
        writer.WriteRow({"Henry", "12", "Child Prodigy"})

    You will get the following output in the sink:
        Name,Age,Comment
        Bob,21,Attending ECS34
        Henry,12,Child Prodigy


CXMLReader:
    - include/XMLReader.h
    - include/XMLEntity.h
    - include/DataSource.h
    - src/XMLReader.cpp

CXMLReader reads XML data from a data source and converts it into XML entities represented by the SXMLEntity. Each entity corresponds to a start and end elements, character, or complete element in the XML document

1. bool End() const
-> Checks whether all XML entities have been read from input

-> Returns TRUE when the entire XML input has been processed, indicating that no more entities available 
-> otherwise, returns FALSE.

2. bool ReadEntity(SXMLEntity &entity, bool skipcdata = false)
-> Reads the next XML entity from the input
-> Returns True if an XML entity is able to read, or FLASE if there are no more entities or if an error occurs during parsing

CXMLWriter: 
    - include/XMLwriter.h
    - include/XMLEntity.h
    - include/DataSource.h
    - src/XMLwriter.cpp

1. bool WriteEntity(const SXMLEntity &entity)
-> Writes a single XML entity to the output
-> StartElement, the function writes an opening tag and pushes the element name onto an internal stack.
-> EndElement, the function writes the corresponding closing tag and pops the element from stack
-> CompleteElement, the function writes a self closing tag
-> CharData, the function writes the text contents after escaped special characters such as &, <, >, ", and '.

2. bool Flush():
-> Ensures that any open XML elements (from previously written start tags) are properly closed
-> Iterates the internal stack of opened elements and writes the end tags
-> Returns true if all open tags are correctly closed