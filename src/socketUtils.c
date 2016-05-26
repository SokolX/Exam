/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/**
 * 
 * @param socket
 * @param port
 */
void bind_to_port(int socket, int port)
{
    struct sockaddr_in name;
    name.sin_family = PF_INET;
    name.sin_port = (in_port_t)htons(30000);
    name.sin_addr.s_addr = htonl(INADDR_ANY);

    int reuse = 1;

    if (setsockopt(socket, SOL_SOCKET, SO_REUSEADDR, (char *)&reuse, sizeof(int)) == -1)
        error("Nie można ustawić opcji ponownego użycia gniazda. ");

    int c = bind(socket, (struct sockaddr *) &name, sizeof(name));

    if (c == -1)
        error("Nie można utworzyć powiązania z gniazdami. ");
}

/**
 * 
 * @param sig
 * @param handler
 * @return 
 */
int catch_signal(int sig, void (*handler)(int))
{
    struct sigaction action;
    action.sa_handler = handler;
    sigemptyset(&action.sa_mask);
    action.sa_flags = 0;
    return sigaction(sig, &action, NULL);
}

/**
 * 
 * @param sig
 */
void handle_shutdown(int sig)
{
    if (listener_d)
        close(listener_d);

    fprintf(stderr, "\nZakończono dziłanie aplikacji na żądanie\n");
    exit(0);
}

/**
 * 
 * @return 
 */
int open_listener_socket()
{
    int s = socket(PF_INET, SOCK_STREAM, 0);
    if (s == -1)
        error("Nie można otworzyć gniazda. ");
    return s;
}

/**
 * 
 * @param socket
 * @param buf
 * @param len
 * @return 
 */
int read_in(int socket, char *buf, int len)
{
    char *s = buf;
    int slen = len;
    int c = recv(socket, s, slen, 0);
    while ((c > 0) && (s[c-1] != '\n')) {
        s += c;
        slen -= c;
        c = recv(socket, s, slen, 0);
    }
    if (c < 0)
        return c;
    else if (c == 0)
        buf[0] = '\0';
    else
        s[c-1] = '\0';
    return len - slen;
}

/**
 * 
 * @param socket
 * @param s
 * @return 
 */
int say(int socket, char *s)
{
    int result = send(socket, s, strlen(s), 0);
    if (result == -1)
        fprintf(stderr, "%s: %s \n", "Błąd komunikacji z serwerem", strerror(errno));
    return result;
}