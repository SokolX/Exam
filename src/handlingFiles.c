/**
 * \file handlingFiles.c
 * \brief Plik zawierający metodę obsłującą poprawność logowania
 */
#include <json-c/json_object.h>
#include <json-c/json_util.h>

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

void addSession(char* id_sesji, char* login, char* rola, int time){
    
    json_object *plik, *wpis, *sesja, *new_file_content;
    char* fileName = "../resources/Session.json";
    
    //pobranie struktury z pliku Session.json
    plik = json_object_from_file(fileName);
    
    //stworzenie obieku JSONa
    wpis = json_object_new_object();
    new_file_content = json_object_new_object();
    
    //dodanie wpisow do obiektu
    addJsonObject(wpis, "id", id_sesji);
    addJsonObject(wpis, "login", login);
    addJsonObject(wpis, "rola", rola);
    addJsonObject(wpis, "logged", int2char(time));
    
    //pobranie tablicy 'sesja'
    sesja = json_object_object_get(plik, "sesja");
    
    //dodanie do tablicy wpisu z nowym id_sesji
    json_object_array_add(sesja, wpis);
    
    //zapisanie w obiekcie JSON nowej tresci pliku
    json_object_object_add(new_file_content, "sesja", sesja);
    
    //zrzucenie tresci do pliku razem z formatowaniem
    json_object_to_file_ext(fileName, new_file_content,  JSON_C_TO_STRING_PRETTY);
    
}

static void addJsonObject(json_object *jobj, char *key, char *val) {
      //utworzenie obiektu
      json_object *jstring = json_object_new_string(val);

      //forma jsonowa
      json_object_object_add(jobj,key, jstring);
}

/**
 * 
 * @param id
 * @return 
 */
char* removeSession(char* id){
    json_object *plik, *session_tab, *new_file_content, *buf, *obj, *wpis;
    char* fileName = "../resources/Session.json";
    char* status = "wylogowano";
    
    //pobranie struktury z pliku Session.json
    plik = json_object_from_file(fileName);
    
    //pobranie tablicy 'sesja'
    session_tab = json_object_object_get(plik, "sesja");
    
    new_file_content = json_object_new_object();
    wpis = json_object_new_array();
    
    int arraylen = json_object_array_length(session_tab);
    int i, flag;
    //petla po wpisach sesji
    for (i=0; i<arraylen; i++){
        obj = json_object_array_get_idx(session_tab, i);
        buf = json_object_new_object();
        flag = 1;
        
        /*For po kluczach wpisu sessji*/
        json_object_object_foreach(obj, key, val){
            const char* value = json_object_get_string(val);
            
            if( strcmp(key, "id") == 0 && strcmp(value, id) == 0 ){
                flag = 0;
                break;
            }
            
            addJsonObject(buf, key, strdup(value));

        }
        
        if( flag )
            json_object_array_add(wpis, buf);
        
    }
    
    json_object_object_add(new_file_content, "sesja", wpis);
    
    //zrzucenie tresci z usunietym wpisem sesji do pliku razem z formatowaniem
    json_object_to_file_ext(fileName, new_file_content,  JSON_C_TO_STRING_PRETTY);
    
    return status;
}