/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "../yajl/yajl_tree.h"

char* logInChecker(char* buf){
    //magic happens here
    return " {\"id_sesji\":\"0/1/2\"} jeżeli ok, lub {\"message\":\"Nie można zalogować\"} jeżeli błąd";
}