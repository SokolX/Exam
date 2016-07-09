/**
 * \file handlingFiles.h
 * \brief Plik nagłówkowy dla pliku "handlingFiles.c"
 * Plik zawierający deklaracje używanych funkcji/metod potrzebnych do przetworzenia 
 * i dania odpowiedzi na żądania wysyłane przez klienta. 
 */
#ifndef HANDLINGFILES_H
#define HANDLINGFILES_H

#ifdef __cplusplus
extern "C" {
#endif

int checkUserCredentials(const char* login, const char* password);
void addSession(char* id_sesji, char* login, char* rola, int time);
static void addJsonObject(json_object *jobj, char *key, char *val);
char* removeSession(char* id);
int addGroup(char* nazwa_gr);
char* getStudents();
char* getGroups();
int checkSession(char* id);
char* addStudentToGroup(char* student, char* group);
int addToGroup(json_object *newGr, char* key, json_object *gr, char* student, int mode);
char* getExams();
char* addExamToGroup(char* exam, char* group);
char* getLogin(char* login);
char* getStudentResults(char* login);
char* getStudentsAnswears();

#ifdef __cplusplus
}
#endif

#endif /* HANDLINGFILES_H */

#include "handlingFiles.c"