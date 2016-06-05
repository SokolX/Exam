/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   jsonParser.h
 * Author: mariusz
 *
 * Created on May 30, 2016, 7:51 PM
 */
/**
 * \file jsonParser.h
 * \brief Plik nagłówkowy dla pliku "jsonParser.c"
 */
#ifndef JSONPARSER_H
#define JSONPARSER_H

#ifdef __cplusplus
extern "C" {
#endif

#include "jsonParser.c"
    
void print_json_value(char* key, json_object *jobj);
void json_parse_array( json_object *jobj, char *key);
void json_parse(json_object * jobj);


#ifdef __cplusplus
}
#endif

#endif /* JSONPARSER_H */

