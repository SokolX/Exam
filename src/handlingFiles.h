/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   handlingFiles.h
 * Author: mariusz
 *
 * Created on May 26, 2016, 3:10 PM
 */

#ifndef HANDLINGFILES_H
#define HANDLINGFILES_H

#ifdef __cplusplus
extern "C" {
#endif

int checkUserCredentials(const char* login, const char* password);
void addSession(char* id_sesji, char* login, char* rola);
static void addJsonObject(json_object *jobj, char *key, char *val);


#ifdef __cplusplus
}
#endif

#endif /* HANDLINGFILES_H */

#include "handlingFiles.c"