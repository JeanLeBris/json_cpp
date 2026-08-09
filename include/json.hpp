#ifndef JSON_HPP
#define JSON_HPP

namespace json{
    // enum key_type{
    //     none,           // if there are no children (the object contains a number, a string or 'none')
    //     incremental,    // if the children are : an incremental table (starts with 0)
    //     number,         //                       a dictionary with numeric keys
    //     string          //                       a dictionary with string keys
    // };

    // enum content_type{
    //     none,
    //     number,
    //     string,
    //     object
    // };

    char* append_string(char* destination, const char* new_string, int* size);

    enum types{
        none,
        number,
        string,

        incremental,

        object
    };

    union content;

    class Json;

    union content{
        Json** children;
        char* string;
        int value;
    };

    class Json{
        public:
        // private:

        // The type of keys in the children objects
        // The type of the key is always none if the type of content is not 'object'
        types key_type;

        // The type of content in this object
        types content_type;

        // public:

        int length;
        content key;
        content content;

        void init(types ctype, types ktype, json::content key);

        Json();
        Json(types ctype);
        Json(types ctype, types ktype);
        Json(types ctype, types ktype, json::content key);
        Json(Json* value);
        ~Json();

        void set_value();
        void set_value(int value);
        void set_value(const char* value);

        void set_child(Json data);
        void set_child(int key, Json data);
        void set_child(const char* key, Json data);

        Json* get_child(int key);
        Json* get_child(const char* key);

        char* to_string(char* output, int* size, bool formatting, const char* indentation, int level);
        char* to_string();
        char* to_formatted_string(const char* indentation = "\t");
    };
}

#endif