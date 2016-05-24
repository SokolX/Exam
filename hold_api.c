void handle_shutdown(int sig)
{
    if (listener_d)
        close(listener_d);

    fprintf(stderr, "\nZakończono dziłanie aplikacji na żądanie\n");
    exit(0);
}
