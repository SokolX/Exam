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
/**
 * \file handlingFiles.h
 * \brief Plik nagłówkowy dla pliku "handlingFiles.c"
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


#ifdef __cplusplus
}
#endif

#endif /* HANDLINGFILES_H */

#include "handlingFiles.c"