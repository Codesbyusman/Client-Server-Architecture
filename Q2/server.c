/*
    Muhammad Usman Shahid
      Assignment 01
        20i-1797
          CY-T
*/

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
    char Message[100] = {};

    // crafting the message to be sent to the client
    char sendToClient[MAXSIZE] = {};

    // Creating socket file descriptor
    if ((server_socket = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    {
        // not binded
        printf("\n\t\t ::::: Socket not made --- run again ::::::\n\n");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in servaddr, client_address;

    // initalizing
    memset(&servaddr, 0, sizeof(servaddr));
    memset(&client_address, 0, sizeof(client_address));

    // Filling server information
    servaddr.sin_family = AF_INET; // IPv4
    servaddr.sin_addr.s_addr = INADDR_ANY;
    servaddr.sin_port = htons(PORT);

    // Bind the socket with the server address
    if (bind(server_socket, (const struct sockaddr *)&servaddr,
             sizeof(servaddr)) < 0)
    {
        // not binded
        printf("\n\t\t ::::: Error in binding --- run again ::::::\n\n");
        exit(EXIT_FAILURE);
    }

    while (1)
    {
        Message[0] = '\0';
        sendToClient[0] = '\0';

        int len, n;

        len = sizeof(client_address);

        n = recvfrom(server_socket, (char *)Message, 100,
                     0, (struct sockaddr *)&client_address,
                     &len);
        Message[n] = '\0';

        if (!strcmp(Message, "exit"))
        {
            // exiti
            printf("\n\t\t --------------------------------------------------------------------------------- \n\n");

            printf("\n\t\t ----------------------- [+] Query : Exit ----> Exiting [+] ---------------------- \n\n");

            printf("\n\t\t --------------------------------------------------------------------------------- \n\n");
            break;
        }

        printf("\n\t\t --------------------------------------------------------------------------------- \n\n");

        printf("\n\t\t ------------------ [+] New Query ----> Resolving the Query [+] ------------------ \n\n");

        printf("\n\t\t --------------------------------------------------------------------------------- \n\n");

        // breaking back the recieved message
        char *invoiceNumber = strtok(Message, "-");
        char *attributes = strtok(NULL, "-");

        strcpy(sendToClient, "\n\nInvoice Number \t\t Description \t\t\t      Quantity \t      Price in $      Customer Id \t     Country\n\n");

        int offset = strlen(sendToClient);

        // opening the dataset file
        //  opening the dataset file in read mode
        FILE *file = fopen("dataset.txt", "r");

        if (file == NULL)
        {
            sendToClient[0] = '\0';
            strcpy(sendToClient, "exit");
            sendto(server_socket, (const char *)sendToClient, strlen(sendToClient),
                   0, (const struct sockaddr *)&client_address,
                   len);

            printf("\n\t ------------ dataset.txt doesnot exist --- kindly make sure to be in same folder ------------\n\n");
            return -1;
        }

        char currentLine[150] = {};
        char duplicate[150] = {};
        int found = 0;

        // wihle the file is not ended
        while (fgets(currentLine, sizeof(currentLine), file))
        {
            // refereshing
            duplicate[0] = '\0';

            // duplicating the current line
            strcpy(duplicate, currentLine);

            char *Read[6] = {}; // the attributes

            Read[0] = NULL;

            // getting the invoice numbeer
            Read[0] = strtok(duplicate, "\t");

            // if matches the given invoice number
            if (!strcmp(Read[0], invoiceNumber))
            {
                found = 1;
                // current line is to be sent back with specied attributes
                // for the six attributes
                for (int i = 0; i < 6; i++)
                {
                    if (attributes[i] == 'y' || attributes[i] == 'Y')
                    {
                        if (i > 0)
                            Read[i] = strtok(NULL, "\t");
                    }
                    else
                    {
                        strtok(NULL, "\t");
                        Read[i] = " - ";
                    }
                }

                offset += sprintf(sendToClient + offset, "  %-10s\t %-40s\t %-10s\t %-10s\t %-10s\t %-10s\n\n", Read[0], Read[1], Read[2], Read[3], Read[4], Read[5]);
            }
        }

        // close the file
        fclose(file);

        // if no instance found
        if (found == 0)
            strcat(sendToClient, "\n\n\t\t\t :::::::: NO data fount against this invoice Number ::::::::: \n\n");

        sendto(server_socket, (const char *)sendToClient, strlen(sendToClient),
               0, (const struct sockaddr *)&client_address,
               len);
    }

    close(server_socket);

    return 0;
}