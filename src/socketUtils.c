/**
 * \file socketUtils.c
 * \brief Plik zawierający metody umożliwiające komunikację z serwerem. 
 * 
 */

/**
 * Metoda odpowiedzialana dowiązanie nazwy do gniazda. Korzystamy w niej z 
 * funkcji bind(), która dowiązuje nazwę do portu oraz funkcji socket(), która 
 * tworzy gniazdo (deskryptor). W tym przypadku korzystamy z protokółów z 
 * rodziny TCP/IP (PF_INET). Metoda także zapewnia obsługę ponownego użycia 
 * gniazda po zamknięciu dzięki funkcji setsockopt (zdarzają 
 * się problemy w przypadku wyłączenia i natychmiastowej próby uruchomienia 
 * servera, zwykle około 30 sekund jest niedostępne po zamknięciu). Dzięki 
 * temu problem nie wystąpi.  
 * @param socket - gniazdo, na którym są/będą udostępniane usługi servera
 * @param port - port, który udostępniamy usługi na świat (klientom)
 * @param reuse - jeżeli 1, to użyj portu ponownie 
 */
void bind_to_port(int socket, int port)
{
    //tworzenie portu internetowego
    struct sockaddr_in name;
    name.sin_family = PF_INET;
    name.sin_port = (in_port_t)htons(port);
    name.sin_addr.s_addr = htonl(INADDR_ANY);
    //reuse = 1, umożliwa wykorzystanie gniazda ponownie
    int reuse = 1;

    if (setsockopt(socket, SOL_SOCKET, SO_REUSEADDR, (char *)&reuse, sizeof(int)) == -1)
        error("Nie można ustawić opcji ponownego użycia gniazda. ");

    int c = bind(socket, (struct sockaddr *) &name, sizeof(name));

    if (c == -1)
        error("Nie można utworzyć powiązania z gniazdami. ");
}

/**
 * Funkcja określająca metodę obsłużenia sygnału. Zwraca wskaźnik do metody, 
 * która ma obsłużyć dany sygnał. 
 * @param sig - nr sygnału, który zostanie przechwycony
 * @param handler - wskaźnik do metody, która ma być wykonana w przypadku wystąpienia
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
 * Funkcja obsługująca zamknięcie gniazda przed zakończeniem pracy programu, 
 * gdy ktoś naciśnie CTRL+C podczas działania servera. 
 * @param sig - sygnał otrzymany do obsługi
 */
void handle_shutdown(int sig)
{
    
    if (listener_d)
        close(listener_d);  //zamyka połącznie

    fprintf(stderr, "\nZakończono działanie aplikacji na żądanie\n");
    exit(0);
}

/**
 * Metoda, która tworzy gniazdo do przesyłania strumieniowego. Do tworznia 
 * gniazda wykorzystywna jest funkcja socket(), która w przypadku powodzenia 
 * zwaraca deskryptor gniazda, w przeciwnym razie -1 (informacja o błędzie). 
 * Zwaraca deskryptor gniazda lub -1 w przypadku błędu. 
 * @param s - dyskryptor gniazda lub -1
 * @return - zwraca s do main() 
 */
int open_listener_socket()
{
    int s = socket(PF_INET, SOCK_STREAM, 0);
    if (s == -1)
        error("Nie można otworzyć gniazda. ");
    return s;
}

/**
 * Metoda, która odczytuje strumień danych od klienta. Znaki są odczytywane 
 * aż do napotkania znaku "\n". Do odczytywania danych wykorzystywana jest 
 * funkcja systemowa recv(), która umożliwa odczytać dane od klienta - zwaraca 
 * liczbę odczytanych danych lub wartość -1. 
 * 
 * @param socket - gniazdo komunikacji z klientem
 * @param buf - ciąg znaków otrzymany od klienta
 * @param len - długość ciągu znaków otrzymanego od klienta
 * @return 
 */
int read_in(int socket, char *buf, int len)
{
    char *s = buf;
    int slen = len;
    int c = recv(socket, s, slen, 0);
    while ((c > 0) && (s[c-1] != '\n')) {   //odczytujemy aż do napotkania "\n"
        s += c;
        slen -= c;
        c = recv(socket, s, slen, 0);
    }
    if (c < 0)  //w przypadku błędu
        return c;
    else if (c == 0)    //pusty ciąg znaków
        buf[0] = '\0';
    else
        s[c-1] = '\0';  //zamieniamy znak "\r" na "\0" 
    return len - slen;
}

/**
 * Wysyła do klienta ciąg znaków w przypadku powodzenia. W przypadku wystąpienia 
 * błędu nie korzysta z funkcji error(), gdyż nie ma powodu zatrzymywać 
 * działania servera z powodu problemu komunikacji z jednym klientem. 
 * @param socket - gniazdo komunikacji z klientem
 * @param s - informacja do klienta
 * @param result - 0 w przypadku powodzenia lub -1 w przypadku błędu
 * @return 
 */
int say(int socket, char *s)
{
    //strlen(s) - długość bufora, który chcemy wysłać do klienta
    int result = send(socket, s, strlen(s), 0);
    if (result == -1)
        fprintf(stderr, "%s: %s \n", "Błąd komunikacji z serwerem", strerror(errno));
    return result;
}