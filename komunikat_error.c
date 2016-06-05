/**
 * \file komunikat_error.c
 * 
 * \brief Uniwersalna metoda wykorzystywana w przypadku błędu w aplikacji. 
 *
 * Gdy wystąpi bład w aplikacji - zatrzyma program, a także poinformuje użytkownika
 * o występieniu błędu. Metoda w wywołaniuu przyjmuje łańcuch znaków. Wywołanie 
 * metody: error("Przykładowy komunikat. "). exit(1) - zatrzymuje działanie programu. 
 * 
 * \param *msg - wskaźnik typu char, który przechwouje treść komunikatu o błędzie
 */

void error(char *msg)
{
    fprintf(stderr, "%s: %s\n", msg, strerror(errno));
    exit(1); //kończy działanie aplikacji
}