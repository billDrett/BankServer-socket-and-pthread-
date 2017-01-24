#include <iostream>
using namespace std;

/*#include "bankServer.h"
#include "bankAccount.h"
#include "transaction.h"*/
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include "string.h"
#include "stdlib.h"
#include <sstream>
#include<fstream>

#include <sys/socket.h>	     /* sockets */
#include <netinet/in.h>	     /* internet sockets */
#include <netdb.h>	         /* gethostbyaddr */

#include "readWriteFunct.h"

int checkCommand(string& line);

int main(int argc, char *argv[])
{
    int sock, i, port = -1;
    char buf[256];
    char buffer[1024];
    string buff;
    struct sockaddr_in server;
    struct sockaddr *serverptr = (struct sockaddr*)&server;
    struct hostent *rem;

    char* serverHost = NULL;
    ifstream inFile;

    if (argc != 7)
    {
    	cout<<"Please give all attributes -h <server host> -p <server port> -i <command file>"<<endl;
       	exit(1);
    }

    for(int i=1; i < argc; i++) //diavasma parametrwn grammis entolis me morfi -h <server host> -p <server port> -i <command file>
    {
        if(i+1 != argc)
        {
            if(strcmp(argv[i], "-h") == 0)
            {
                serverHost = argv[i+1];
            }
            else if(strcmp(argv[i], "-p") == 0)
            {
                port = atoi(argv[i+1]);
            }
            else if(strcmp(argv[i], "-i") == 0)
            {
                inFile.open(argv[i+1]);
            }
        }
    }

    if(port == -1 || serverHost == NULL || inFile.is_open() == false)
    {
        cout<<"Please give all attributes -h <server host> -p <server port> -i <command file>"<<endl;
       	exit(1);
    }
    //kwdikas socket me vash tis diafaneies tou mathimatos
	/* Create socket */
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror("socket");
        exit(2);
    }

	/* Find server address */
    if ((rem = gethostbyname(serverHost)) == NULL) {
	   herror("gethostbyname"); exit(1);
    }

    server.sin_family = AF_INET;       /* Internet domain */
    memcpy(&server.sin_addr, rem->h_addr, rem->h_length);
    server.sin_port = htons(port);         /* Server port */
    /* Initiate connection */
    if (connect(sock, serverptr, sizeof(server)) < 0)
    {
        perror("connect");
        exit(1);
    }
    printf("Connecting to %s port %d\n", argv[1], port);
    //exoume sindethei tora mem ton server

    int result;
    while(getline(inFile, buff)) //diavasma apo arxeio
    {
        cout<<buff<<endl;
        result = checkCommand(buff);
        if(result == 1)
        {
            continue;
        }
        else if(result == 0)
        {
            write_data(sock, buff.c_str());
            i = read_data(sock, buffer);
            printf("%.*s\n", i, buffer);
        }
        else if (result == -2)
        {
            close(sock);
            cout<<"terminate client"<<endl;
            exit(0);
        }
        else //result -1
        {
            cout<<"command doesnt exists"<<endl;
        }
    }

    while(!getline(cin, buff).eof()) //diavasma apo stdin
    {
        result = checkCommand(buff);
        if(result == 1)
        {
            continue;
        }
        else if(result == 0)
        {
            write_data(sock, buff.c_str());
            i = read_data(sock, buffer);
            printf("%.*s\n", i, buffer);
        }
        else if (result == -2)
        {
            close(sock);
            cout<<"terminate client"<<endl;
            exit(0);
        }
        else //result -1
        {
            cout<<"command doesnt exists"<<endl;
        }
    }
}

int checkCommand(string& line)
{
    string command, name;
    double amount;

    istringstream iss(line);

    iss>> command; //to prwto stoixeio einai h entolh
    if(iss.fail())
    {
        return -1;
    }
    if(command == "add_account")
    {
        iss>> amount>>name;
        if(!iss)
        {
            return -1;
        }
    }
    else if(command == "add_transfer")
    {
        string fromAc, ToAc;
        iss >>amount >> fromAc >> ToAc;
        if(!iss)
        {
            return -1;
        }
    }
    else if(command == "add_multi_transfer")
    {
        string dstName1, dstName2;

        iss >> amount >> name >> dstName1 >> dstName2; //thelw na exei toulaxiston dyo gia na einai swsth

        if(!iss)
        {
            return -1;
        }
    }
    else if(command == "print_balance")
    {
        iss>> name;
        if(!iss)
        {
            return -1;
        }
    }
    else if(command == "print_multi_balance")
    {
        string name1, name2;

        iss >> name1 >>name2; //thelw na exei toulaxiston dyo gia na einai swsth
        if(!iss)
        {
            return -1;
        }

    }
    else if(command == "sleep")
    {
        long time;

        iss >> time;
        usleep(time);
        return 1;
    }
    else if(command == "exit") //error
    {
        return -2;
    }
    else //error
    {
        return -1;
    }

    return 0;
}
