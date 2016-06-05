/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   md5Converter.h
 * Author: mariusz
 *
 * Created on June 1, 2016, 7:59 PM
 */
/**
 * \file md5Converter.h
 * \brief Plik nagłówkowy dla pliku "md5Converter.c"
 */
#ifndef MD5CONVERTER_H
#define MD5CONVERTER_H

#ifdef __cplusplus
extern "C" {
#endif
    
#include "md5Converter.c"

char *str2md5(char *str);


#ifdef __cplusplus
}
#endif

#endif /* MD5CONVERTER_H */

