#include <malloc.h>
#include <string.h>
#include <iostream>

#include "../include/json.hpp"

namespace json{
    char* append_string(char* destination, const char* new_string, int* size){
        if(strlen(destination) + strlen(new_string) > *size - 1){
            *size += strlen(new_string);
            destination = (char*) realloc(destination, *size * sizeof(char));
        }
        strcat(destination, new_string);
        return destination;
    }

    void Json::init(types ctype, types ktype, json::content key){
        this->length = 0;
        this->content_type = ctype;
        this->key_type = ktype;
        this->key = key;
        switch (this->content_type){
        case none:
        case object:
        case string:
            this->content.children = nullptr;
            break;
        case number:
            this->content.value = 0;
            break;
        default:
            break;
        }
    }

    Json::Json(){
        init(none, none, {.value = 0});
    }
    
    Json::Json(types ctype){
        init(ctype, none, {.value = 0});
    }
    
    Json::Json(types ctype, types ktype){
        init(ctype, ktype, {.value = 0});
    }

    Json::Json(Json* value){
        this->length = value->length;
        this->content_type = value->content_type;
        this->key_type = value->key_type;
        this->key = value->key;
        this->content = value->content;
    }

    Json::~Json(){
        if(this->content_type == object){
            for(int i = 0; i < this->length; i++){
                if(this->key_type == string){
                    free(this->content.children[i]->key.string);
                }
                this->content.children[i]->~Json();
                delete this->content.children[i];
                this->length = 0;
            }
            if(this->content.children != nullptr){
                free(this->content.children);
                this->content.children = nullptr;
            }
        }
        else if(this->content_type == number){
            // free(this->content.value);
        }
        else if(this->content_type == string){
            free(this->content.string);
            this->content.string = nullptr;
        }
    }

    void Json::set_value(int value){
        this->content_type = number;
        this->content.value = value;
    }

    void Json::set_value(const char* value){
        this->content_type = string;
        if(this->content.string == nullptr){
            this->content.string = (char*) malloc((strlen(value) + 1) * sizeof(char));
        }
        else{
            this->content.string = (char*) realloc(this->content.string, (strlen(value) + 1) * sizeof(char));
        }
        strcpy(this->content.string, value);
    }

    void Json::set_child(Json data){
        Json* data_buffer = nullptr;
        if(this->key_type == incremental){
            if(this->content.children == nullptr){
                this->content.children = (Json**) malloc(sizeof(Json*));
            }
            else{
                this->content.children = (Json**) realloc(this->content.children, (this->length + 1) * sizeof(Json*));
            }
            data_buffer = new Json(&data);
            this->content.children[this->length] = data_buffer;
            this->content.children[this->length]->key.value = this->length;
            this->length++;
        }
    }

    void Json::set_child(int key, Json data){
        Json* data_buffer = nullptr;
        if(this->key_type == number){
            if(this->content.children == nullptr){
                this->content.children = (Json**) malloc(sizeof(Json*));
            }
            else{
                this->content.children = (Json**) realloc(this->content.children, (this->length + 1) * sizeof(Json*));
            }
            data_buffer = new Json(&data);
            this->content.children[this->length] = data_buffer;
            this->content.children[this->length]->key.value = key;
            this->length++;
        }
    }

    void Json::set_child(const char* key, Json data){
        Json* data_buffer = nullptr;
        if(this->key_type == string){
            if(this->content.children == nullptr){
                this->content.children = (Json**) malloc(sizeof(Json*));
            }
            else{
                this->content.children = (Json**) realloc(this->content.children, (this->length + 1) * sizeof(Json*));
            }
            data_buffer = new Json(&data);
            this->content.children[this->length] = data_buffer;
            if(this->content.children[this->length]->key.string == nullptr){
                this->content.children[this->length]->key.string = (char*) malloc((strlen(key) + 1) * sizeof(char));
            }
            else{
                this->content.children[this->length]->key.string = (char*) realloc(this->content.children[this->length]->key.string, (strlen(key) + 1) * sizeof(char));
            }
            strcpy(this->content.children[this->length]->key.string, key);
            this->length++;
        }
    }

    Json* Json::get_child(int key){
        if(this->key_type == incremental){
            if(key >= 0 && key < this->length){
                return this->content.children[key];
            }
        }
        else if(this->key_type == number){
            for(int i = 0; i < this->length; i++){
                if(this->content.children[i]->key.value == key){
                    return this->content.children[i];
                }
            }
        }
        return nullptr;
    }

    Json* Json::get_child(const char* key){
        if(this->key_type == string){
            for(int i = 0; i < this->length; i++){
                if(strcmp(this->content.children[i]->key.string, key) == 0){
                    return this->content.children[i];
                }
            }
        }
        return nullptr;
    }

    char* Json::to_string(char* output, int* size, bool formatting, const char* indentation, int level){
        char buffer_string[20] = "";

        if(output == nullptr){
            output = (char*) malloc(1 * sizeof(char));
            output[0] = '\0';
            *size = 1;
        }

        if(this->content_type == number){
            sprintf(buffer_string, "%d", this->content.value);
            output = append_string(output, buffer_string, size);
        }
        else if(this->content_type == string){
            output = append_string(output, "\"", size);
            output = append_string(output, this->content.string, size);
            output = append_string(output, "\"", size);
        }
        else if(this->content_type == object){
            if(this->key_type == incremental){
                output = append_string(output, "[", size);

                for(int i = 0; i < this->length; i++){
                    if(formatting){
                        output = append_string(output, "\n", size);
                        for(int i = 0; i < level + 1; i++){
                            output = append_string(output, indentation, size);
                        }
                    }

                    output = this->content.children[i]->to_string(output, size, formatting, indentation, level+1);

                    if(i < this->length - 1){
                        output = append_string(output, ",", size);
                    }
                }

                if(formatting){
                    output = append_string(output, "\n", size);
                    for(int i = 0; i < level; i++){
                        output = append_string(output, indentation, size);
                    }
                }

                output = append_string(output, "]", size);
            }
            else if(this->key_type == number){
                output = append_string(output, "{", size);

                for(int i = 0; i < this->length; i++){
                    if(formatting){
                        output = append_string(output, "\n", size);
                        for(int i = 0; i < level + 1; i++){
                            output = append_string(output, indentation, size);
                        }
                    }
                    
                    sprintf(buffer_string, "%d", this->content.children[i]->key.value);
                    output = append_string(output, buffer_string, size);
                    output = append_string(output, ":", size);

                    output = this->content.children[i]->to_string(output, size, formatting, indentation, level+1);

                    if(i < this->length - 1){
                        output = append_string(output, ",", size);
                    }
                }

                if(formatting){
                    output = append_string(output, "\n", size);
                    for(int i = 0; i < level; i++){
                        output = append_string(output, indentation, size);
                    }
                }

                output = append_string(output, "}", size);
            }
            else if(this->key_type == string){
                output = append_string(output, "{", size);

                for(int i = 0; i < this->length; i++){
                    if(formatting){
                        output = append_string(output, "\n", size);
                        for(int i = 0; i < level + 1; i++){
                            output = append_string(output, indentation, size);
                        }
                    }

                    output = append_string(output, "\"", size);
                    output = append_string(output, this->content.children[i]->key.string, size);
                    output = append_string(output, "\":", size);
                    
                    output = this->content.children[i]->to_string(output, size, formatting, indentation, level+1);

                    if(i < this->length - 1){
                        output = append_string(output, ",", size);
                    }
                }

                if(formatting){
                    output = append_string(output, "\n", size);
                    for(int i = 0; i < level; i++){
                        output = append_string(output, indentation, size);
                    }
                }

                output = append_string(output, "}", size);
            }
        }

        return output;
    }

    char* Json::to_string(){
        int size = 0;
        return this->to_string(nullptr, &size, false, "", 0);
    }

    char* Json::to_formatted_string(const char* indentation){
        int size = 0;
        return this->to_string(nullptr, &size, true, indentation, 0);
    }
}