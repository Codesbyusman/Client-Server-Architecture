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
#include <time.h>

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>

// return the number of lines in the file
long int countLines(char *fileName)
{

    long int toReturn = 0;

    // open in read mode
    FILE *file;
    file = fopen(fileName, "r");

    // if file opened successfully
    if (file)
    {
        char character = (char)fgetc(file);

        // till end of file
        while (character != EOF)
        {
            // if next line
            if (character == '\n')
                toReturn++;

            character = (char)fgetc(file);
        }

        // close the file
        fclose(file);
    }

    return toReturn;
}

int main()
{

    printf("\n\t\t\t --------------- [+] GYM Registration Server [+] ---------------\n\n");

    printf("");

    // for the tcp connection
    // socket creation
    int server_soc = socket(AF_INET, SOCK_STREAM, 0);

    // address of the server
    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(8080);
    server_address.sin_addr.s_addr = INADDR_ANY;

    // binding
    if (bind(server_soc, (struct sockaddr *)&server_address, sizeof(server_address)) != 0)
    {
        // not binded
        printf("\n\t\t ::::: Error in binding --- run again ::::::\n\n");
        return 0;
    }

    // listen as tcp connection
    if (listen(server_soc, 5) != 0)
    {
        // not binded
        printf("\n\t\t ::::: Error in listening --- run again ::::::\n\n");
        return 0;
    }

    char server_message[200] = {};
    char client_message_Registration[200] = {};
    char client_message_Details[200] = {};

    for (int i = 0; i < 5; i++)
    {
        server_message[0] = client_message_Details[0] = client_message_Registration[0] = '\0';

        // accept
        int client_soc = accept(server_soc, NULL, NULL);

        // connection established
        // start reciving and sending messages
        recv(client_soc, &client_message_Registration, sizeof(client_message_Registration), 0);
        printf("\n\t\t ----------------- [+] New Client ----> Registration Request [+] ----------------- \n\n");
        printf("\nRegistration By : %s \n", client_message_Registration);

        char *name = strtok(client_message_Registration, "-");
        char *age = strtok(NULL, "-"); // take the age

        // sending message to the clinet
        strcat(server_message, "\n\n\t\t\t\t\t Hi ");
        strcat(server_message, name);
        strcat(server_message, ", GYM Server there \n\n\t\t\t\t ----------- [+] Registering you [+] ------------ \n\n");

        send(client_soc, server_message, sizeof(server_message), 0);

        // recive the details
        recv(client_soc, &client_message_Details, sizeof(client_message_Details), 0);
        printf("\nDetails : %s \n", client_message_Details);

        char *weight = strtok(client_message_Details, "-");
        char *height = strtok(NULL, "-");
        char *day = strtok(NULL, "-");
        char *package = strtok(NULL, "-");

        // the time making
        time_t current_time = time(NULL);
        struct tm *tm = localtime(&current_time);

        // the registration date
        char registrationDate_Time[30] = {};
        sprintf(registrationDate_Time, "%02d/%02d/%02d %02d:%02d:%02d", tm->tm_mday, tm->tm_mon + 1, tm->tm_year + 1900, tm->tm_hour, tm->tm_min, tm->tm_sec);

        // adding the days specified
        tm->tm_mday += atoi(day) + 1; // exculding the today as in example

        // making the new time
        mktime(tm);

        // calculating the joining time
        char joiningDate[20] = {};

        // saving this joining time
        sprintf(joiningDate, "%02d/%02d/%02d", tm->tm_mday, tm->tm_mon + 1, tm->tm_year + 1900);

        // writing the info to the desitred file
        FILE *file;

        char fileName[10] = {};

        // calculating the file name
        if (!strcmp(package, "01"))
            strcpy(fileName, "PO1.txt");
        else if (!strcmp(package, "02"))
            strcpy(fileName, "PO2.txt");
        else if (!strcmp(package, "03"))
            strcpy(fileName, "PO3.txt");
        else if (!strcmp(package, "04"))
            strcpy(fileName, "PO4.txt");

        // opening the file
        file = fopen(fileName, "a");

        if (file)
        {
            fseek(file, 0L, SEEK_END); // taking to the end
            long int size = ftell(file);

            // taking the pointer o start
            fseek(file, 0, SEEK_SET);

            if (size == 0)
                fprintf(file, "Serial  Package \t   Date\\Time \t\tName \t\tAge-Weight-Height \tJoining Date \n");

            fclose(file);

            long int serial = countLines(fileName);

            file = fopen(fileName, "a");

            if (file)
            {
                fprintf(file, "  %02ld      P%s   \t%s   %-22s %s-%s-%s\t\t %s \n", serial, package, registrationDate_Time, name, age, weight, height, joiningDate);
                fclose(file);
            }
        }

        // creating sever message
        sprintf(server_message, "\n\t\t\t ------------- [+] Registered Successfully [+] ------------------ \n\n\t\t\t\t Registration Date/Time : %s \n\n\t\t\t\t Joining Date : %s\n\n", registrationDate_Time, joiningDate);

        printf("\n\t\t --------------------------------------------------------------------------------- \n\n");

        // sending to the client
        send(client_soc, server_message, sizeof(server_message), 0);
    }

    // close the socket
    close(server_soc);

    return 0;
}