int open_listener_socket()
{
    int s = socket(PF_INET, SOCK_STREAM, 0);
    if (s == -1)
        error("Nie można otworzyć gniazda. ");
    return s;
}
