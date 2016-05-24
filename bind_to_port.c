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
