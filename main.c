/**
 * \mainpage
 * \authors
 *  Mariusz Czabaj \n
 *  Łukasz Sokołek
 * \date 4.06.2016
 * \version 1.0
 * \par Kontakt
 * \a czabaj@wit.edu.pl \n
 * \a sokolek@wit.edu.pl
 * 
 * \file main.c
 * \brief Główny plik wykonywalny
 */
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <signal.h>
#include <errno.h>
#include "komunikat_error.h"
///zmienna przechowująca główne gniazdo odbiorcze servera do odbierania połączeń (deskryptor gniazda)
int listener_d; //musi byc zadeklarowane przed funkcją handle_shotdown
#include "src/socketUtils.h"
#include "src/handlingClient.h"


int main(int argc, char *argv[])
{   
    //deklaracja zmiennej przechowujacej numer portu
    int port;
    
    //jezeli w wierszu wywolania zostanie podany port to przypisz do zmienenj port
    //w przeciwnym razie port = 30000
    if(argc == 2){
        port = atoi(argv[1]);
    }
    else{
        port = 30000;
    }
    
    if(catch_signal(SIGINT, handle_shutdown) == -1)
        error("\nNie można ustawić procedury obsługi przerwania");
    
    listener_d = open_listener_socket();
    bind_to_port(listener_d, port);
    if (listen(listener_d, 10) == -1) //utw kolejki 10 el
        error("Nie można odbierać połączeń");

    struct sockaddr_storage client_addr;
    unsigned int address_size = sizeof(client_addr);
    puts("Czekam na połączenie");

    char buf[255];

    while (1) {
        int connect_d = accept(listener_d, (struct sockaddr *)&client_addr, &address_size);
        if (connect_d == -1)
            error("Nie można otworzyć pomocnicznego gniazda");

        if (!fork()) {
            close(listener_d);

            read_in(connect_d, buf, sizeof(buf));
            
            //sprawdzanie czego klienta zada
            int selector = getClientInstruction(buf);
            
            //wybór odpowiedniej akcji na podstawie zadania
            switch(selector){
                case 6:
                    say(connect_d, logInChecker(buf));
                    break;
                case 0:
                case 1:
                case 2:
                case 3:
                case 4:
                case 5:
                case 7:
                case 8:
                case 9:
                case 10:
                default:
                    say(connect_d, "Nie rozpoznano komunikatu");
                    break;
            }

            close(connect_d);
            exit(0);
        }
        
        close(connect_d);
    }
    
    return 0;
}