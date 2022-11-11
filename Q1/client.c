/*
    Muhammad Usman Shahid 
      Assignment 01
        20i-1797
          CY-T
*/

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>

// will return if whole string is number else 0
int isNumber(char *string)
{
    // iterate whole string
    for (int i = 0; i < strlen(string); i++)
        if (!isdigit(string[i])) // if is not number return 0
            return 0;

    return 1; // the strig was number
}

// will return if whole string is all aplhabets else 0
int isAlphabets(char *string)
{
    // iterate whole string
    for (int i = 0; i < strlen(string); i++)
        if (((string[i] >= 'a' && string[i] <= 'z') || (string[i] >= 'A' && string[i] <= 'Z')) || string[i] == ' ')
            continue;
        else
            return 0; // if is not in characters

    return 1; // the strig was alphabet
}

int main()
{
#define size1 50
#define size2 5

    // taking the information from the client then will make the tcp connection with
    // the server
    char name[size1] = {};
    char input[size2] = {}; // the other inputs such as age weight etc

    // sending the information
    char registration[size1 + size2] = {};
    char details[size2 * 4] = {};

    int a = 0;

    printf("\n\t ------------------------- [+] Enter the information for registeration [+] ------------------------- \n\n");

    do
    {
        if (a > 0)
            printf("\n\t\t :::::::: Must be Alphabetic only --- Enter again :::::::::\n\n");

        printf("\t\t Name : ");
        scanf("%[^\n]%*c", name);

        a++;

    } while (!isAlphabets(name));

    strcpy(registration, name); // name

    a = 0;
    // if not a number ask again
    do
    {
        if (a > 0)
            printf("\n\t\t :::::::: must be between 15 and 90 & numeric :::::::::\n\n");

        printf("\t\t Age : ");
        scanf("%s", input);

        a++;

    } while (!(atoi(input) >= 15 && atoi(input) <= 90 && isNumber(input)));

    strcat(registration, "-");
    strcat(registration, input); // age

    printf("\t\t Weight : ");
    scanf("%s", input);

    strcpy(details, input); // weight

    printf("\t\t Height : ");
    scanf("%s", input);

    strcat(details, "-");
    strcat(details, input); // height

    a = 0;
    // if not a number ask again
    do
    {
        if (a > 0)
            printf("\n\t\t :::::::: Must be valid numeric --- Enter again :::::::::\n\n");

        printf("\t\t JoinDayStart : ");
        scanf("%s", input);

        a++;

    } while (!(isNumber(input)&& atoi(input) >= 0));

    strcat(details, "-");
    strcat(details, input); // day

    a = 0;

    // ask again on wrong pakage
    do
    {
        if (a > 0)
            printf("\n\t\t :::::::: Invalid Package --- Enter again :::::::::\n\n");

        // asking for the pakage
        printf("\n\t\t Select Pakage :-  \n\n\t\t\t1 - for P01 \n\t\t\t2 - for P02 \n\t\t\t3 - for P03 \n\t\t\t4 - for P04 \n\n\t\t your pakage : ");
        scanf("%s", input);

    } while (!(input[0] == '1' || input[0] == '2' || input[0] == '3' || input[0] == '4'));

    strcat(details, "-0");
    strcat(details, input); // pakage

    printf("\n\t ----------------------------- [+] Registering with the details [+] -------------------------------- \n\n");

    // the socket
    int client_soc = socket(AF_INET, SOCK_STREAM, 0);

    // setting the address
    struct sockaddr_in client_address;
    client_address.sin_family = AF_INET;
    client_address.sin_addr.s_addr = INADDR_ANY;
    client_address.sin_port = htons(8080);

    // the connect from client
    connect(client_soc, (struct sockaddr *)&client_address, sizeof(client_address));

    // start sending and reciving receving messages
    send(client_soc, registration, sizeof(registration), 0);

    char buf[200];
    recv(client_soc, &buf, sizeof(buf), 0);
    printf("\n %s \n", buf);

    send(client_soc, details, sizeof(details), 0);

    recv(client_soc, &buf, sizeof(buf), 0);
    printf("\n %s \n", buf);

    close(client_soc);

    return 0;
}