/**
 * \file md5Converter.h
 * \brief Plik nagłówkowy dla pliku "md5Converter.c"
 * Plik zawierający dołączenie biblioteki do obsługi metod tworzących ciągi hash'y
 * w md5 oraz deklarację metody zamieniającej ciąg znaków na hash. 
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

