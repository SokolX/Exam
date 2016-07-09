/**
 * \file handlingClient.c
 * \brief Główny plik zawierający metody obsługujące żądania ze 
 * strony klienta. 
 */

#include <json-c/linkhash.h>
#include "json/json.h"
#include "handlingFiles.h"
#include "md5Converter.h"
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/** param tab_len
 * Określa ilość instrukcji obsługiwanych przez serwer. W zależności od roli 
 * użytkownika w systemie określana jest ich dostępność.  
 * W razie dodawania nowych instrukcji należy pamiętać, aby inkrementować/
 * dekrementować tą zmienną. 
 * 
 * param instruction[] 
 * Zmienna tablicowa zawierająca w sobie nazwy obsługiwanych instrukcji 
 * obsługiwanych przez serwer. 
 *  
 */
//nalezy pamietac zeby zmieniac przy dodawaniu/usuwaniu komend do tablicy
int tab_len = 13;
//!< Brief description after the member
char* instructions[]={  "3view_results", //0
                        "3get_exams", //1
                        "2add_group", //2
                        "2assign_to_group", //3
                        "3check_answears", //4
                        "3add_exam", //5
                        "3log_in", //6
                        "0add_answear", //7
                        "0get_answears", //8
                        "3log_out", //9
                        "3assign_exam_to_group", //10
                        "2get_students", //11
                        "3get_groups", //12
                        "0get_"
}; //**!< Detailed description after the member 

/**
 * Metoda, której głównym zadaniem jest obsługa żądań klienta. Każde z żądań 
 * jest traktowana jako oddzielna instrukacja. 
 * 
 * @param buf
 * @return 
 */
int getClientInstruction(char* buf){
    int i;
    
    //parsowanie komunikatu
    struct json_object *jarray = json_tokener_parse(buf); 

    //wyluskanie stringa z trescia komunikatu
    const char *instr = (char*)(json_object_get_object(jarray)->head)->k;
    
    for(i=0; i<tab_len; i++){
        if( strcmp(instr, &instructions[i][1]) == 0 )
            return i;
    }
    
    return -1; 
}

/*!< To jest krótki opis zmiennej */
/**
 * Metoda obsługująca logowanie - cały proces uwierzytelniający użytkownika 
 * w systemie. Metoda przyjmuje informacje od klienta o loginie i haśle (MD5) 
 * w postaci ciągu znaków typu json i sprawdza zgodność podanych danych z . 
 * plikie Users.json, w którym znajdują się dane o użytkownikach systemu. 
 * @param buf - zawiera dane w formacie json do logawania podane przez użytkownika
 * @return 
 */
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
    int tim = time(NULL);
    
    if ( rola >= 0 ){
        char* md5 = str2md5( int2char(tim) );
        //rola jest dolaczana do md5 po jej utworzeniu
        char* id_sesji = strcat( md5 , int2char(rola) );
        char* srv_response;
        
        //strdup: pozbycie sie const
        addSession(id_sesji, strdup(user), int2char(rola), tim);
        
        //generowanie odpowiedzi do klienta
        json_object *message = json_object_new_object();
        
        //dodanie do obiektu JSON klucza "logged" i wartosci będącej id_sesji
        addJsonObject(message, "logged", id_sesji);

        srv_response = strdup( json_object_get_string(message) );
        
        return srv_response;
    }
    else{
        char* message = "{ \"error\": \"Podano bledne dane.\" }";
        writeToLog(tim, buf, message);
        return message;
    }
}

/**
 * Metoda zamieniająca liczbę typu int na ciąg znaków typu char.
 * Wykorzystywana przy tworzeniu hash'y. 
 * @param i - liczba do zamiany 
 * @return 
 */
char* int2char(int i){
    char* str = malloc(16);
    snprintf(str, 16, "%d", i);
        /* @param str - cięg znaków typu char */
    return str; 
}

/**
 * Metoda służąca do logowania informacji o działaniach klienta na serwerze. 
 * Każde z działanie klienta będzie logowane w pliku log.txt w katalogu resources. 
 * W pliku logowane będą: \n 
 * - czas żądania \n 
 * - informację o rodzaju żądania do serwera \n 
 * - odpowiedź do klienta jakiej udzielił serwer. 
 * @param timestamp - zwraca czas żądania w sekundach
 * @param *client_message - informacja o żądaniu od klienta
 * @param *serv_response - jaką informację wysłał klientowi serwer
 * @return zamyka plik, w którym logowane są akcje
 */
int writeToLog(int timestamp, char* client_message, char* srv_response) {
    FILE *log; ///< FILE 
    char* file_name = "../resources/log.txt";    
    //otwieranie pliku do dodawania
    log = fopen(file_name, "a");
    fprintf(log, "%d; %s; %s\n", timestamp, client_message, srv_response);
    
    return fclose(log);
}

/**
 * Funkcja odpowiadająca za wylogowanie użytkownika z aplikacji. Polega na 
 * usunięciu wpisu id sesji w pliku sesji. 
 * @param buf - zawiera id sesji do usunięcia
 * @return odpowiedź do klienta
 */
char* loggingOut(char* buf){
    json_object *session;
    char* session_id, *srv_response;
    
    //parsowanie komunikatu klienta
    struct json_object *obj = json_tokener_parse(buf);
    
    //wydobywanie tablicy
    session = json_object_object_get(obj, "log_out");
    
    //wydobywanie id sesji
    session_id = strdup(json_object_get_string(session));
    
    srv_response = removeSession(session_id);
    
    return srv_response;
}

/**
 * Funkcja odpowiadająca za dodanie grupy. Przed dodaniem grupy, weryfikujemy czy 
 * użytkownik próbujący dodać grupę ma uprawnienie do wykonania operacji. Wykorzystujemy 
 * do tego role. Dodawanie grupy jest sprawdzanie na wypadek próby dodania grupy o tej samej nazwie. 
 * @param buf
 * @return 
 */
char* addingGroup(char* buf){
    json_object *obj, *nazwaGrupy, *sessionID;
    const char *nazwa, *id;
    
    //parsowanie komunikatu klienta
    struct json_object *jarray = json_tokener_parse(buf);
    
    //wydobywanie tablicy
    obj = json_object_object_get(jarray, "add_group");
    
    //wydobywanie obiektów login i haslo
    nazwaGrupy = json_object_object_get(obj, "name");
    sessionID = json_object_object_get(obj, "session_id");
    
    //zapisywanie loginu i hasła jako string
    nazwa = json_object_get_string(nazwaGrupy);
    id = json_object_get_string(sessionID);
    
    //sprawdzenie czy uzytkownik ma dostep do uslugi
    if ( checkPermissions(strdup(id), "add_group") != 1){
        char* message = "{ \"error\": \"Brak uprawnien do uslugi!\" }";
        writeToLog(time(NULL), buf, message);
        return message;
    }
    
    //sprawdzanie czy grupa zostala dodana prawidlowo
    if ( addGroup( strdup(nazwa) ) == 1){
        return "{ \"message\": \"Dodano grupe!\" }";
    }
    else{
        char* message = "{ \"error\": \"Grupa o podanej nazwie juz istnieje!\" }";
        writeToLog(time(NULL), buf, message);
        return message;
    }
    
}

/**
 * Metoda do sprawdzania czy klient o przeslanym id sesji ma dostep do żądanej
 * uslugi.
 * 
 * @param session_id
 * @param req
 * @return 
 */
int checkPermissions(char* session_id, char* req){
    int i, role;
    
    //sprawdzenie czy id sesji ma poprawna dlugosc
    if( strlen(session_id) != 33 )
        return -1;
    
    for(i=0; i<tab_len; i++){
        if( strcmp(req, &instructions[i][1] ) == 0 ){
            role = instructions[i][0];
            break;
        }
    }
    
    if( checkSession(session_id) == 1 && (session_id[32] == role || role == '3') )
        return 1;
    else
        return -1;
    
}

/**
 * Metoda zwraca klientowi listę studentów po wcześniejszej weryfikacji czy klient
 * ma dostęp do usługi.
 * 
 * @param buf   Żądanie klienta
 * @return      Lista studentów formacie JSON.
 */
char* getStudentList(char* buf){
    json_object *session;
    char* session_id;
    
    //parsowanie komunikatu klienta
    struct json_object *obj = json_tokener_parse(buf);
    
    //wydobywanie tablicy
    session = json_object_object_get(obj, "get_students");
    
    //wydobywanie id sesji
    session_id = strdup(json_object_get_string(session));
    
    //sprawdzenie czy uzytkownik ma dostep do uslugi
    if ( checkPermissions(strdup(session_id), "get_students") != 1){
        char* message = "{ \"error\": \"Brak uprawnien do uslugi!\" }";
        writeToLog(time(NULL), buf, message);
        return message;
    }
    
    return getStudents();
}

/**
 * Funkcja pobierająca dla egzaminatora listę dostępnych egzaminów oraz 
 * listę grup, którym może przypisać egzamin. 
 * Weryfikowane jest w tym przypadku czy użytkownik ma uprawnienie do tej funkcji. 
 * @param buf - przyjmuje id sesji
 * @return 
 */
char* getGroupList(char* buf){
    json_object *session;
    char* session_id;
    
    //parsowanie komunikatu klienta
    struct json_object *obj = json_tokener_parse(buf);
    
    //wydobywanie tablicy
    session = json_object_object_get(obj, "get_groups");
    
    //wydobywanie id sesji
    session_id = strdup(json_object_get_string(session));
    
    //sprawdzenie czy uzytkownik ma dostep do uslugi
    if ( checkPermissions(strdup(session_id), "get_groups") != 1){
        char* message = "{ \"error\": \"Brak uprawnien do uslugi!\" }";
        writeToLog(time(NULL), buf, message);
        return message;
    }
    
    return getGroups();
}

/**
 * Metoda parsuje żądanie klienta ws. dodania studenta do grupy. Sprawdza czy klient
 * posiada uprawnienia do usługi i przekazuje dane w formacie JSON do metody 
 * addStudentToGroup(), która dokonuje zmian w bazie danych.
 * 
 * @param buf   Komunikat klienta.
 * @return      Status operacji.
 */
char* assignStudentToGroup(char* buf){
    json_object *session, *student, *group, *assignment;
    char* session_id, *student_name, *group_name;
    
    //parsowanie komunikatu klienta
    struct json_object *obj = json_tokener_parse(buf);
    
    //wydobywanie tablicy
    session = json_object_object_get(obj, "assign_to_group");
    
    //wydobywanie id sesji
    session_id = strdup(json_object_get_string(session));
    
    //sprawdzenie czy uzytkownik ma dostep do uslugi
    if ( checkPermissions(strdup(session_id), "assign_to_group") != 1){
        char* message = "{ \"error\": \"Brak uprawnien do uslugi!\" }";
        writeToLog(time(NULL), buf, message);
        return message;
    }
    
    //wydobywanie obiektu z danymi
    assignment = json_object_object_get(obj, "assignment");
    
    student = json_object_object_get(assignment, "user");
    group = json_object_object_get(assignment, "group");
    
    student_name = strdup(json_object_get_string(student));
    group_name = strdup(json_object_get_string(group));
    
    return addStudentToGroup(student_name, group_name);
}

/**
 *  Metoda zwraca klientowi listę egzaminów po wcześniejszej weryfikacji czy klient
 * ma dostęp do usługi.
 * 
 * @param buf   Żądanie klienta.
 * @return      Listę egzaminów w formacie JSON.
 */
char* getExamsList(char* buf){
    json_object *session;
    char* session_id;
    
    //parsowanie komunikatu klienta
    struct json_object *obj = json_tokener_parse(buf);
    
    //wydobywanie tablicy
    session = json_object_object_get(obj, "get_exams");
    
    //wydobywanie id sesji
    session_id = strdup(json_object_get_string(session));
    
    //sprawdzenie czy uzytkownik ma dostep do uslugi
    if ( checkPermissions(strdup(session_id), "get_exams") != 1){
        char* message = "{ \"error\": \"Brak uprawnien do uslugi!\" }";
        writeToLog(time(NULL), buf, message);
        return message;
    }
    
    return getExams();
}

/**
 * Metoda do parsowania żadania dodania egzaminu otrzymana od klienta. Sprawdza czy klient
 * posiada uprawnienia do usługi i przekazuje dane w formacie JSON do metody 
 * addExamToGroup() dokonujacej zmian w bazie danych.
 * 
 * @param buf   Treść żądania klienta.
 * @return      Status operacji.
 */
char* assignExamToGroup(char* buf){
    json_object *session, *exam, *group, *assignment;
    char* session_id, *exam_name, *group_name;
    
    //parsowanie komunikatu klienta
    struct json_object *obj = json_tokener_parse(buf);
    
    //wydobywanie tablicy
    session = json_object_object_get(obj, "assign_exam_to_group");
    
    //wydobywanie id sesji
    session_id = strdup(json_object_get_string(session));
    
    //sprawdzenie czy uzytkownik ma dostep do uslugi
    if ( checkPermissions(strdup(session_id), "assign_exam_to_group") != 1){
        char* message = "{ \"error\": \"Brak uprawnien do uslugi!\" }";
        writeToLog(time(NULL), buf, message);
        return message;
    }
    
    //wydobywanie obiektu z danymi
    assignment = json_object_object_get(obj, "assignment");
    
    exam = json_object_object_get(assignment, "exam");
    group = json_object_object_get(assignment, "group");
    
    exam_name = strdup(json_object_get_string(exam));
    group_name = strdup(json_object_get_string(group));
    
    return addExamToGroup(exam_name, group_name);
}

/**
 * Funkcja pozwalająca studentowi na zobaczenie listy wyników z egzaminów, z których
 * ma już wynik. Na podstawie id sesji jest określana dostępność tej funkcji dla 
 * użytkownika w systemie. 
 * @param buf - id sesji
 * @return getStudentResults(login) - funkcję na rcze login by studnet widział tylko swoje wyniki
 */
char* getStudentAnswears(char* buf){
    json_object *session;
    char* session_id, *login;
    
    //parsowanie komunikatu klienta
    struct json_object *obj = json_tokener_parse(buf);
    
    //wydobywanie tablicy
    session = json_object_object_get(obj, "get_answears");
    
    //wydobywanie id sesji
    session_id = strdup(json_object_get_string(session));
    
    //sprawdzenie czy uzytkownik ma dostep do uslugi
    if ( checkPermissions(strdup(session_id), "get_answears") != 1){
        char* message = "{ \"error\": \"Brak uprawnien do uslugi!\" }";
        writeToLog(time(NULL), buf, message);
        return message;
    }
    
    //wydobycie loginu studenta na podstawie przeslanego ID sesji
    login = getLogin(session_id);
    
    return getStudentResults(login);
}

/**
 * Funkcja pozwalająca egzaminatorowi na zobaczenie listy użytkowników, którzy
 * mają już wyniki z egzaminów. Na podstawie id sesji jest określana dostępność 
 * tej funkcji dla użytkownika w systemie. 
 * @param buf - id sesji do weryfikacji 
 * @return getStudentsAnswears - czyli metodę, która pozwoli wydobyć listę
 */
char* getStudentsResults(char* buf){
    json_object *session;
    char* session_id;
    
    //parsowanie komunikatu klienta
    struct json_object *obj = json_tokener_parse(buf);
    
    //wydobywanie tablicy
    session = json_object_object_get(obj, "view_results");
    
    //wydobywanie id sesji
    session_id = strdup(json_object_get_string(session));
    
    //sprawdzenie czy uzytkownik ma dostep do uslugi
    if ( checkPermissions(strdup(session_id), "view_results") != 1){
        char* message = "{ \"error\": \"Brak uprawnien do uslugi!\" }";
        writeToLog(time(NULL), buf, message);
        return message;
    }
    
    return getStudentsAnswears();
}

/**
 * Metoda do parsowania żądania dodania egzaminu przez klienta. Sprawdza czy klient
 * posiada uprawnienia do usługi i przekazuje dane w formacie JSON do metody 
 * addExam() dokonujacej zmian w bazie.
 * 
 * @param buf   Treść żądania klienta.
 * @return      Status operacji.
 */
char* addingExam(char* buf){
    
    //parsowanie komunikatu klienta
    struct json_object *obj = json_tokener_parse(buf);
    
    json_object *node = json_object_new_object();
    
    node = json_object_object_get(obj, "add_exam");
    
    return addExam(node);
}