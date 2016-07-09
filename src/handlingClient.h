/**
 * \file handlingClient.h
 * \brief Plik nagłówkowy dla pliku "handlingClient.c"
 * Plik zawierający deklaracje używanych funkcji/metod potrzebnych do obsługi 
 * żądań wysyłanych przez klienta. 
 */
#ifndef HANDLINGCLIENT_H
#define HANDLINGCLIENT_H

#ifdef __cplusplus
extern "C" {
#endif
    
char* logInChecker(char* buf);
int getClientInstruction(char* buf);
char* int2char(int i);
int writeToLog(int timestamp, char* client_message, char* srv_response);
char* loggingOut(char* buf);
char* addingGroup(char* buf);
int checkPermissions(char* session_id, char* req);
char* getStudentList(char* buf);
char* getGroupList(char* buf);
char* assignStudentToGroup(char* buf);
char* getExamsList(char* buf);
char* assignExamToGroup(char* buf);
char* getStudentAnswears(char* buf);
char* getStudentsResults(char* buf);

#ifdef __cplusplus
}
#endif

#endif /* HANDLINGCLIENT_H */

#include "handlingClient.c"