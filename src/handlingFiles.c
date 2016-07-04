/**
 * \file handlingFiles.c
 * \brief Plik zawierający metodę obsłującą poprawność logowania
 */
#include <json-c/json_object.h>
#include <json-c/json_util.h>
#include <time.h>
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

/**
 * 
 * @param jobj
 * @param key
 * @param val
 */
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

/**
 * 
 * @param nazwa_gr
 * @return 
 */
int addGroup(char* nazwa_gr){
    json_object *examsArr, *studentsArr, *groupArr, *grFile, *newFileContent, *newGroup, *newObj, *node;
    char* fileName = "../resources/Groups.json";
    
    examsArr = json_object_new_object();
    studentsArr = json_object_new_object();
    newGroup = json_object_new_array();
    newFileContent = json_object_new_object();
    newObj = json_object_new_object();
    
    //pobranie struktury z pliku Groups.json
    grFile = json_object_from_file(fileName);
    
    //pobranie tablicy 'grupa'
    groupArr = json_object_object_get(grFile, "grupa");
    
    int arraylen = json_object_array_length(groupArr);
    int i;
    //petla po wszystkich grupach
    for (i=0; i< arraylen; i++){
        node = json_object_array_get_idx(groupArr, i);
        
        /*For po kluczach kazdej grupy*/
        json_object_object_foreach(node, key, val){ 
            json_object_get_string(val);
            if ( strcmp(key, nazwa_gr) == 0 )
                return -1;
        }
    }
    
    json_object_object_add(studentsArr, "students", json_object_new_array());
    json_object_object_add(examsArr, "exams", json_object_new_array());
    
    //utworzenie struktury nowej grupy
    json_object_array_add(newGroup, studentsArr);
    json_object_array_add(newGroup, examsArr);
    
    //utworzenie nowego obiektu grupy wraz z nazwa
    json_object_object_add(newObj, nazwa_gr, newGroup);
    
    //dodanie do aktualnych grup nowej z pustymi tablicami students i exams
    json_object_array_add(groupArr, newObj);
    
    //utworzenie obiektu gotowego do zrzucenia do pliku
    json_object_object_add(newFileContent, "grupa", groupArr);
    
    //zrzucenie tresci z usunietym wpisem sesji do pliku razem z formatowaniem
    json_object_to_file_ext(fileName, newFileContent,  JSON_C_TO_STRING_PRETTY);
    
    return 1;
}

/**
 * 
 * @return 
 */
char* getStudents(){
    json_object *jvalue, *user, *plik, *usersList, *srvResponse;
    
    //parsowanie zawartosci pliku User.json
    plik = json_object_from_file("../resources/Users.json");
    
    //wydobywanie tablicy uzytkownikow
    jvalue = json_object_object_get(plik, "user");
    
    usersList = json_object_new_object();
    int arraylen = json_object_array_length(jvalue);
    int i;
    //petla po wszystkich uzytkownikach
    for (i=0; i< arraylen; i++){
        user = json_object_array_get_idx(jvalue, i);
        
        /*For po kluczach kazdego uzytkownika*/
        json_object_object_foreach(user, key, val){ 
            char* name;

            if( strcmp(key, "login") == 0 ){
                name = strdup(json_object_get_string(val));
            }
            
            if ( strcmp(key, "rola") == 0){
                if( json_object_get_int(val) == 0 ){
                    addJsonObject(usersList, int2char(i), name);
                    break;
                }
            }
        }
    }
    
    srvResponse = json_object_new_object();
    json_object_object_add(srvResponse, "studentsList", usersList);
    
    return strdup(json_object_get_string(srvResponse));
}

/**
 * 
 * @return 
 */
char* getGroups(){
    json_object *jvalue, *group, *plik, *groupsList, *srvResponse;
    
    //parsowanie zawartosci pliku User.json
    plik = json_object_from_file("../resources/Groups.json");
    
    //wydobywanie tablicy grupy
    jvalue = json_object_object_get(plik, "grupa");
    
    groupsList = json_object_new_object();
    int arraylen = json_object_array_length(jvalue);
    int i;
    //petla po wszystkich grupach
    for (i=0; i< arraylen; i++){
        group = json_object_array_get_idx(jvalue, i);
        
        /*For po kluczach kazdej grupy*/
        json_object_object_foreach(group, key, val){ 
            json_object_get_string(val);
            addJsonObject(groupsList, int2char(i), key);
            break;
        }
    }
    
    srvResponse = json_object_new_object();
    json_object_object_add(srvResponse, "groupsList", groupsList);
    
    return strdup(json_object_get_string(srvResponse));
}

/**
 * 
 * @param id
 * @return 
 */
int checkSession(char* id){
    json_object *plik, *session_tab, *obj;
    char* fileName = "../resources/Session.json";
    
    //pobranie struktury z pliku Session.json
    plik = json_object_from_file(fileName);
    
    //pobranie tablicy 'sesja'
    session_tab = json_object_object_get(plik, "sesja");
    
    int arraylen = json_object_array_length(session_tab);
    int i;
    //petla po wpisach sesji
    for (i=0; i<arraylen; i++){
        obj = json_object_array_get_idx(session_tab, i);
        
        /*For po kluczach wpisu sessji*/
        json_object_object_foreach(obj, key, val){
            const char* value = json_object_get_string(val);
            
            if( strcmp(key, "id") == 0 && strcmp(value, id) == 0 ){
                return 1;
            }else{
                break;
            }

        }
        
    }

    return -1;
}

/**
 * 
 * @param student
 * @param group
 * @return 
 */
char* addStudentToGroup(char* student, char* group){
    json_object *groupsArr, *grFile, *newFileContent, *newGroup, *node;
    char* fileName = "../resources/Groups.json";
    
    newGroup = json_object_new_array();
    newFileContent = json_object_new_object();
    
    //pobranie struktury z pliku Groups.json
    grFile = json_object_from_file(fileName);
    
    //pobranie tablicy 'grupa'
    groupsArr = json_object_object_get(grFile, "grupa");
    
    int arraylen = json_object_array_length(groupsArr);
    int i;
    //petla po wszystkich grupach
    for (i=0; i< arraylen; i++){
        node = json_object_array_get_idx(groupsArr, i);
        
        /*For po nazwach kazdej grupy*/
        json_object_object_foreach(node, key, val){ 
            if ( strcmp(key, group) != 0 ){
                json_object_array_add(newGroup,node);
                break;
            }
            
            if ( addToGroup(newGroup, group, val, student, 1) == -1 )
                return "{ \"error\": \"Student juz jest dodany do grupy!\" }";
        }
    }
    
    //utworzenie obiektu gotowego do zrzucenia do pliku
    json_object_object_add(newFileContent, "grupa", newGroup);
    
    //zrzucenie tresci z usunietym wpisem sesji do pliku razem z formatowaniem
    json_object_to_file_ext(fileName, newFileContent,  JSON_C_TO_STRING_PRETTY);
    
    return "{ \"student_assigned\": \"Dodano studenta do grupy!\" }";
}

/**
 * 
 * @param newGr
 * @param key
 * @param gr
 * @param student
 * @return 
 */
int addToGroup(json_object *newGr, char* key, json_object *gr, char* student, int mode){
    json_object *groupArr, *node, *students, *studentNode, *newStudentNode, *newObj, *newGroup;
    char* array, *keyName;
    
    if (mode == 1){
        array = "students";
        keyName = "login";
    }else{
        array = "exams";
        keyName = "name";
    }
    
    newGroup = json_object_new_object();
    groupArr = json_object_new_object();
    newObj = json_object_new_array();
    students = json_object_new_array();
    int arraylen = json_object_array_length(gr);
    int i;
    //petla po wszystkich grupach
    for (i=0; i< arraylen; i++){
        node = json_object_array_get_idx(gr, i);
        
        /*For po nazwach kazdej grupy*/
        json_object_object_foreach(node, key, val){ 
            if ( strcmp(key, array) == 0 ){
                int studentsLen = json_object_array_length(val);
                int j;
                
                for (j=0; j< studentsLen; j++){
                    studentNode = json_object_array_get_idx(val, j);
                    
                    char* login = strdup(json_object_get_string(
                                    json_object_object_get(studentNode, keyName)
                    ));
                    
                    if( strcmp(login, student) == 0 ){
                        return -1;
                    }
                    
                    json_object_array_add(students, studentNode);
                    
                }
                
                //utworzenie obiektu z nowym studentem
                newStudentNode = json_object_new_object();
                addJsonObject(newStudentNode, keyName, student);
                
                if (mode == 2)
                    addJsonObject(newStudentNode, "due_date", int2char(time(NULL)+3600));
                
                json_object_array_add(students, newStudentNode);
                json_object_object_add(groupArr, array, students);
                json_object_array_add(newObj, groupArr);
                
            }else{
                json_object_array_add(newObj, node);
            }
        }
    }
    json_object_object_add(newGroup, key, newObj);
    json_object_array_add(newGr, newGroup);
    return 0;
}

/**
 * 
 * @return 
 */
char* getExams(){
    json_object *jvalue, *plik, *examsList, *srvResponse;
    
    //parsowanie zawartosci pliku Exams.json
    plik = json_object_from_file("../resources/Exams.json");
    
    //wydobywanie tablicy grupy
    jvalue = json_object_object_get(plik, "name");
    
    examsList = json_object_new_object();
    
    int i=0;
    /*For po kluczach kazdej grupy*/
    json_object_object_foreach(jvalue, key, val){ 
        json_object_get_string(val);
        addJsonObject(examsList, int2char(i++), key);
    }
    
    srvResponse = json_object_new_object();
    json_object_object_add(srvResponse, "examsList", examsList);
    
    return strdup(json_object_get_string(srvResponse));
}

/**
 * 
 * @param exam
 * @param group
 * @return 
 */
char* addExamToGroup(char* exam, char* group){
    json_object *groupsArr, *grFile, *newFileContent, *newGroup, *node;
    char* fileName = "../resources/Groups.json";
    
    newGroup = json_object_new_array();
    newFileContent = json_object_new_object();
    
    //pobranie struktury z pliku Groups.json
    grFile = json_object_from_file(fileName);
    
    //pobranie tablicy 'grupa'
    groupsArr = json_object_object_get(grFile, "grupa");
    
    int arraylen = json_object_array_length(groupsArr);
    int i;
    //petla po wszystkich grupach
    for (i=0; i< arraylen; i++){
        node = json_object_array_get_idx(groupsArr, i);
        
        /*For po nazwach kazdej grupy*/
        json_object_object_foreach(node, key, val){ 
            if ( strcmp(key, group) != 0 ){
                json_object_array_add(newGroup,node);
                break;
            }
            
            if ( addToGroup(newGroup, group, val, exam, 2) == -1 )
                return "{ \"error\": \"Egzamin juz jest dodany do grupy!\" }";
        }
    }
    
    //utworzenie obiektu gotowego do zrzucenia do pliku
    json_object_object_add(newFileContent, "grupa", newGroup);
    
    //zrzucenie tresci z usunietym wpisem sesji do pliku razem z formatowaniem
    json_object_to_file_ext(fileName, newFileContent,  JSON_C_TO_STRING_PRETTY);
    
    return "{ \"exam_assigned\": \"Przypisano egzamin grupie!\" }";
}