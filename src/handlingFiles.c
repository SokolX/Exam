/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
#include "jsonParser.h"
#include "json/json.h"

int checkUserCredentials(char* login, char* password, json_object * jobj){
    enum json_type type;
    
    json_object_object_foreach(jobj, key, val) { /*For po elementach tablicy users*/
        type = json_object_get_type(val);
        json_object * jvalue;
        
        if(type == json_type_array){
            int i;
            json_object *jarray = json_object_object_get(jobj, key);
            int arraylen = json_object_array_length(jarray);
            
            for (i=0; i< arraylen; i++){
                jvalue = json_object_array_get_idx(jarray, i);
                
                json_object_object_foreach(jvalue, key2, val2){ /*For po kluczach kazdego uzytkownika*/
                    const char* value = json_object_get_string(val2);
                    
                    if( strcmp(key2, "login") == 0 && strcmp(value, login) != 0 )
                        break;
                    
                    if( strcmp(key2, "haslo") == 0 && strcmp(value, password) == 0 )
                        return 1;
                }
            }
        }

    }
    
    return 0;
}