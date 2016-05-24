int say(int socket, char *s)
{
    int result = send(socket, s, strlen(s), 0);
    if (result == -1)
        fprintf(stderr, "%s: %s \n", "Błąd komunikacji z serwerem", strerror(errno));
    return result;
}
