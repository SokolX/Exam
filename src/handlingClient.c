/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "json/json.h"
#include "handlingFiles.h"

char* logInChecker(char* buf){
    json_object *obj, *login, *haslo, *plik;
    char *user, *pass;
    
    plik = json_object_from_file("../resources/Users.json");
    
    //parsowanie komunikatu klienta
    struct json_object *jarray = json_tokener_parse(buf);
    
    //wydobywanie tablicy
    obj = json_object_object_get(jarray, "log_in");
    
    login = json_object_object_get(obj, "login");
    haslo = json_object_object_get(obj, "haslo");
    
    user = json_object_get_string(login);
    pass = json_object_get_string(haslo);
    
    if ( checkUserCredentials(user, pass, plik) == 1 )
        return "{\"message\":\"zalogowano\"}";
    else
        return "{\"message\":\"podano bledne dane\"}";
}