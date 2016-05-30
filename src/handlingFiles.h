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

int checkUserCredentials(char* login, char* password, json_object * jobj);


#ifdef __cplusplus
}
#endif

#endif /* HANDLINGFILES_H */

#include "handlingFiles.c"