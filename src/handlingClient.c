/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include <json-c/linkhash.h>
#include "json/json.h"
#include "handlingFiles.h"
#include "md5Converter.h"
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

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
    
    int rola = checkUserCredentials(user, pass);
    int tim = (int)time(NULL);
    
    if ( rola >= 0 ){
        char* md5 = str2md5( int2char(tim) );
        //rola jest dolaczana do md5 po jej utworzeniu
        char* id_sesji = strcat( md5 , int2char(rola) );
        char* srv_response;
        
        //strdup: pozbycie sie const
        addSession(id_sesji, strdup(user), int2char(rola));
        
        //generowanie odpowiedzi do klienta
        json_object *message = json_object_new_object();
        
        //dodanie do obiektu JSON klucza "message" i wartosci będącej id_sesji
        addJsonObject(message, "message", id_sesji);
        
        srv_response = strdup(json_object_get_string(message));
        
        //pisanie do loga
        writeToLog(tim, buf, srv_response);
        
        return srv_response;
    }
    else{
        char* message = "{ \"error\": \"Podano bledne dane.\" }";
        writeToLog(tim, buf, message);
        return message;
    }
}

char* int2char(int i){
    char* str = malloc(16);
    snprintf(str, 16, "%d", i);
    
    return str;
}

int writeToLog(int timestamp, char* client_message, char* srv_response){
    FILE *log;
    char* file_name = "../resources/log.txt";
    
    //otwieranie pliku do dodawania
    log = fopen(file_name, "a");
    
    fprintf(log, "%d; %s; %s\n", timestamp, client_message, srv_response);
    
    return fclose(log);
}