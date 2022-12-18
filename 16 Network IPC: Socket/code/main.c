#include <netdb.h>
#include <stdio.h>

int main(void)
{
    struct hostent * h = gethostent();

    if (h)
    {
        printf("%s %d %d\n", h->h_name, h->h_addrtype, h->h_length);
    }

    return 0;
}