/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include <json-c/linkhash.h>
#include "json/json.h"
#include "handlingFiles.h"

int getClientInstruction(char* buf){
    //nalezy pamietac zeby zmieniac przy dodawaniu/usuwaniu komend do tablicy
    int tab_len = 11;
    int i;
    
    //inicjalizacja listy komunikatow
    char* instructions[]={  "add_student",
                            "add_redactor",
                            "add_group",
                            "assign_to_group",
                            "check_answears",
                            "add_exam",
                            "log_in",
                            "add_answear",
                            "get_answear",
                            "assign_exam_to_group",
                            "log_out"};
    
    //parsowanie komunikatu
    struct json_object *jarray = json_tokener_parse(buf);

    //wyluskanie stringa z trescia komunikatu
    const char *instr = (char*)(json_object_get_object(jarray)->head)->k;
    
    for(i=0; i<tab_len; i++){
        if( strcmp(instr, instructions[i]) == 0 )
            return i;
    }
    
    return 0;
    
}

char* logInChecker(char* buf){
    json_object *obj, *login, *haslo;
    const char *user, *pass;
    
    //parsowanie komunikatu klienta
    struct json_object *jarray = json_tokener_parse(buf);
    
    //wydobywanie tablicy
    obj = json_object_object_get(jarray, "log_in");
    
    //wydobywanie obiektów login i haslo
    login = json_object_object_get(obj, "login");
    haslo = json_object_object_get(obj, "haslo");
    
    //zapisywanie loginu i hasła jako string
    user = json_object_get_string(login);
    pass = json_object_get_string(haslo);
    
    if ( checkUserCredentials(user, pass) == 1 )
        return "{\"message\":\"zalogowano\"}";
    else
        return "{\"message\":\"podano bledne dane\"}";
}