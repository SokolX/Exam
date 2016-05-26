/* 
 * File:   main.c
 * Author: sokolx
 *
 * Created on 23 maj 2016, 18:24
 */

//potrzebne biblioteki
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
int listener_d; //musi byc zadeklarowane przed funkcją handle_shotdown
#include "src/socketUtils.h"
#include "yajl/yajl_tree.h"


int main(int argc, char *argv[])
{

    if(catch_signal(SIGINT, handle_shutdown) == -1)
        error("\nNie można ustawić procedury obsługi przerwania");
    
    listener_d = open_listener_socket();
    bind_to_port(listener_d, 30000); //port 30000
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

            //if (say(connect_d, "Czas zacząć egzamin testowy\n - Egzamin jednokrotnego wyboru\n2 + 2 = ?\na) 1\nb) 2\nc) 3\nd) 4\r\n>") != -1)

            read_in(connect_d, buf, sizeof(buf));         

            if (strlen(buf) == 1) //sizeof musi być == podanej odp z klienta
                say(connect_d, "Odpowiedzi powinny byc jednoliterowe");

            else if (say(connect_d, "2 + 2 = ?\na) 1\nb) 2\nc) 3\nd) 4\r\n>>") != -1)
                    read_in(connect_d, buf, sizeof(buf));

                    if (strlen(buf) == 1)
                        say(connect_d, "Odpowiedzi powinne być jednoliterowe\r\n");

            else {
                if (say(connect_d, "2 + 2 = ?\na) 1\nb) 2\nc) 3\nd) 4\r\n>>") != -1)
                    read_in(connect_d, buf, sizeof(buf));

                    if (strlen(buf) == 1)
                        say(connect_d, "Odpowiedzi powinne być jednoliterowe\r\n");

               else if (say(connect_d, "2 + 2 = ?\na) 1\nb) 2\nc) 3\nd) 4\r\n>>") != -1) {
                    read_in(connect_d, buf, sizeof(buf));

                    if (strlen(buf) == 1)
                        say(connect_d, "Odpowiedzi powinne być jednoliterowe\r\n");
                }

                /*  while (int i = 0; i < 10; i++) {
                    read_in(connect_d, buf, sizeof(buf));
                    if (strlen(buf) == 1) //sizeof musi być == podanej odp z klienta
                        say(connect_d, "Odpowiedzi powinny byc jednoliterowe");
                    else if (say(connect_d, "2 + 2 = ?\na) 1\nb) 2\nc) 3\nd) 4\r\n>>") != -1)
                            read_in(connect_d, buf, sizeof(buf));
                            if (strlen(buf) == 1)
                                say(connect_d, "Odpowiedzi powinne być jednoliterowe\r\n");
                }*/
            }

            close(connect_d);
            exit(0);
        }
        
        close(connect_d);
    }
    
    return 0;
}