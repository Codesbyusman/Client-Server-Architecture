#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#define PORT 8080
#define MAXSIZE 2024

// Driver code
int main()
{
    int server_socket;
    char Message[MAXSIZE] = {};

    // Creating socket
    if ((server_socket = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    {
        printf("\n\t\t ::::: Socket not made --- run again ::::::\n\n");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in server_address;

    // setting --- initalizing memory
    memset(&server_address, 0, sizeof(server_address));

    // Filling server information
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(PORT);
    server_address.sin_addr.s_addr = INADDR_ANY;

    while (1)
    {

        // exiting
        printf("\n\t\t --------------------------------------------------------------------------------- \n\n");

        printf("\n\t\t ------------ [+] Write the Query [---] Else exit to exit this [+] --------------- \n\n");

        printf("\n\t\t --------------------------------------------------------------------------------- \n\n");

        Message[0] = '\0';
        char invoiceNumber[15];

        // taking the info from the user about invoive number and attributes
        printf("\n\t\t Enter the Invoice Number : ");
        scanf("%s", invoiceNumber);

        if (!strcmp(invoiceNumber, "exit"))
        {
            // exiting
            strcat(Message, "exit");

            sendto(server_socket, (const char *)Message, strlen(Message),
                   0, (const struct sockaddr *)&server_address,
                   sizeof(server_address));

            // exiting
            printf("\n\t\t --------------------------------------------------------------------------------- \n\n");

            printf("\n\t\t --------------------- [+] Quering : Exit ----> Exiting [+] ---------------------- \n\n");

            printf("\n\t\t --------------------------------------------------------------------------------- \n\n");

            break;
        }

        // saving in the message
        strcat(Message, invoiceNumber);
        strcat(Message, "-"); // the sperator

        // taking the attributes
        char input[2];

        printf("\n Select the attributes you want :- \n\n");

        do
        {
            printf("\t\t Invoice Number (y/n) : ");
            scanf(" %s", input);
        } while (!(input[0] == 'y' || input[0] == 'Y' || input[0] == 'N' || input[0] == 'n'));

        // adding up the response
        strcat(Message, input);

        do
        {
            printf("\t\t Description    (y/n) : ");
            scanf(" %s", input);
        } while (!(input[0] == 'y' || input[0] == 'Y' || input[0] == 'N' || input[0] == 'n'));

        // adding up the response
        strcat(Message, input);

        do
        {
            printf("\t\t Quantity       (y/n) : ");
            scanf(" %s", input);
        } while (!(input[0] == 'y' || input[0] == 'Y' || input[0] == 'N' || input[0] == 'n'));

        // adding up the response
        strcat(Message, input);

        do
        {
            printf("\t\t Price in $     (y/n) : ");
            scanf(" %s", input);
        } while (!(input[0] == 'y' || input[0] == 'Y' || input[0] == 'N' || input[0] == 'n'));

        // adding up the response
        strcat(Message, input);

        do
        {
            printf("\t\t Customer Id    (y/n) : ");
            scanf(" %s", input);
        } while (!(input[0] == 'y' || input[0] == 'Y' || input[0] == 'N' || input[0] == 'n'));

        // adding up the response
        strcat(Message, input);

        do
        {
            printf("\t\t Country        (y/n) : ");
            scanf(" %s", input);
        } while (!(input[0] == 'y' || input[0] == 'Y' || input[0] == 'N' || input[0] == 'n'));

        // adding up the response
        strcat(Message, input);

        // thus message crafted with the inputs and ready to send to the srver

        int n, len;

        sendto(server_socket, (const char *)Message, strlen(Message),
               0, (const struct sockaddr *)&server_address,
               sizeof(server_address));

        // exiting
        printf("\n\t\t --------------------------------------------------------------------------------- \n\n");

        printf("\n\t\t ------------------------- [+] Sending The Query [+] ----------------------------- \n\n");

        printf("\n\t\t --------------------------------------------------------------------------------- \n\n");

        n = recvfrom(server_socket, (char *)Message, MAXSIZE,
                     0, (struct sockaddr *)&server_address,
                     &len);
        Message[n] = '\0';

        if (!strcmp(Message, "exit"))
        {
            printf("\n\n\t\t\t :::::::: Server Issues --- having problems connect again ::::::::: \n\n");
            return -1;
        }

        printf("\n%s\n", Message);
        sleep(2);
    }
    close(server_socket);
    return 0;
}
