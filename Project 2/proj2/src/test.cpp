#include <iostream>
#include "../src/StringDataSink.cpp"
#include "../src/DSVWriter.cpp"

int main(){
    // Create a StringDataSink to accumulate the CSV/DSV as a string
    auto sink = std::make_shared<CStringDataSink>();

    // Create a writer that uses ',' as the delimiter. Quoting only when necessary.
    CDSVWriter writer(sink, ',');

    // Write a couple of rows
    writer.WriteRow({"Name", "Address", "Age"});
    writer.WriteRow({"Alice", "123 Main St", "30"});
    writer.WriteRow({"Bob", "He said \"Hello\" today", "40"});
    writer.WriteRow({"\"House\""}); // note the quotes

    // Now retrieve what was written
    std::cout << "Resulting CSV string:\n" << sink->String() << std::endl;

    return 0;
}





// #include <iostream>
// #include <string>
// #include <cstring>
// using namespace std;

// int main(){
//     string myString = "House";
//     std::string quoted;
//     quoted.reserve(myString.size() + 2); // Reserve space for quotes plus possible extra escaping
//     quoted.push_back('"');
//     for(char c : myString){
//         if(c == '"'){
//             // Double the quotes
//             quoted.push_back('"');
//         }
//         quoted.push_back(c);
//     }
//     quoted.push_back('"');
//     cout << quoted << "\n";
// }