#include <iostream>
#include "../../include/json.hpp"

using namespace std;
// using namespace argparse;

// argparse::parser(){

// }

// argparse::parser(char* prog, char* description, char* epilog){

// }

// argparse::ArgumentParserArgs parser_args;

int main(int argc, char** argv){
    // Declare new arguments parser
    auto obj1 = json::Json(json::types::object, json::types::incremental);
    obj1.set_child(json::Json(json::types::number));
    obj1.get_child(0)->set_value(999);
    obj1.set_child(json::Json(json::types::string));
    obj1.get_child(1)->set_value("howdy");
    obj1.set_child(json::Json(json::types::object, json::types::number));
    obj1.get_child(2)->set_child(3, json::Json(json::types::number));
    obj1.get_child(2)->get_child(3)->set_value(30);
    obj1.get_child(2)->set_child(5, json::Json(json::types::string));
    obj1.get_child(2)->get_child(5)->set_value("hello");
    obj1.get_child(2)->set_child(9, json::Json(json::types::string));
    obj1.get_child(2)->get_child(9)->set_value(std::string("test2"));
    obj1.set_child(json::Json(json::types::object, json::types::string));
    obj1.get_child(3)->set_child("3", json::Json(json::types::number));
    obj1.get_child(3)->get_child("3")->set_value(50);
    obj1.get_child(3)->set_child("5", json::Json(json::types::string));
    obj1.get_child(3)->get_child("5")->set_value("hi");
    obj1.get_child(3)->set_child(std::string("9"), json::Json(json::types::string));
    obj1.get_child(3)->get_child(std::string("9"))->set_value(std::string("test3"));
    obj1.set_child(json::Json(json::types::string));
    obj1.get_child(4)->set_value(std::string("test1"));

    char* test1 = obj1.to_formatted_char_string();
    printf("%s\n", test1);
    free(test1);

    std::string* test2 = obj1.to_formatted_string();
    printf("%s\n", test2->c_str());
    delete test2;

    std::string test3;
    test3 = obj1.to_formatted_string(test3);
    printf("%s\n", test3.c_str());

    return 0;
}