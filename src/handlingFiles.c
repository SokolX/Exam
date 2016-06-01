/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include <json-c/json_object.h>

#include "json/json.h"

int checkUserCredentials(const char* login, const char* password){

    json_object *jvalue, *user, *plik;
    
    //parsowanie zawartosci pliku User.json
    plik = json_object_from_file("../resources/Users.json");
    
    //wydobywanie tablicy uzytkownikow
    jvalue = json_object_object_get(plik, "user");
    
    int arraylen = json_object_array_length(jvalue);
    int i;
    //petla po wszystkich uzytkownikach
    for (i=0; i< arraylen; i++){
        user = json_object_array_get_idx(jvalue, i);
        
        /*For po kluczach kazdego uzytkownika*/
        json_object_object_foreach(user, key, val){ 
            const char* value = json_object_get_string(val);

            if( strcmp(key, "login") == 0 && strcmp(value, login) != 0 )
                break;

            if( strcmp(key, "haslo") == 0 ) {
                if ( !(strcmp(value, password) == 0) )
                    return -1;
            }
            
            if ( strcmp(key, "rola") == 0 ){
                return json_object_get_int(val);
            }

        }
    }
    return -1;
    
}