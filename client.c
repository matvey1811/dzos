
#include <stdio.h>
#include <stdlib.h>


#include <sys/time.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <sys/stat.h>


#include <fcntl.h>
#include <unistd.h>
#include <strings.h>

#include <netinet/in.h>

#include <string>
#include <iostream>

using namespace std;




#define BUFFERT 512


int duration (struct timeval *start,struct timeval *stop, struct timeval *delta);
int create_client_socket (int port, char* ipaddr);

struct sockaddr_in sock_serv;

int main (int argc, char**argv){
	struct timeval start, stop, delta;
    int sfd,fd;
    char buf[BUFFERT];
    off_t count=0, m,sz;//long
	long int n;
    int l=sizeof(struct sockaddr_in);
	struct stat buffer;
    
	if (argc != 4){
		printf("Error usage : %s <ip_serv> <port_serv> <filename>\n",argv[0]);
		return EXIT_FAILURE;
	}
	argv[1] = "192.168.0.2"
	argv[2] = "223"
	argv[3] = "result.txt"

	//--------------
	float a, b, res;
	char choice, ch;
	
	printf("1.Addition\n");
	printf("2.Subtraction\n");
	printf("3.Multiplication\n");
	printf("4.Division\n");
	printf("Enter Your Choice : ");
	scanf("%c", &choice);
	switch(choice)
	{
		case '1': 
			printf("Enter two number : ");
			scanf("%f %f", &a, &b);
			res=a+b;
			printf("Result = %f",res);
			break;
		case '2': 
			printf("Enter two number : ");
			scanf("%f %f", &a, &b);
			res=a-b;
			printf("Result = %f",res);
			break;
		case '3': 
			printf("Enter two number : ");
			scanf("%f %f", &a, &b);
			res=a*b;
			printf("Result = %f",res);
			break;
		case '4': 
			printf("Enter two number : ");
			scanf("%f %f", &a, &b);
			res=a/b;
			printf("Result = %f",res);
			break;
		default: 
			printf("Wrong Choice..!!");
			break;
	}
	//-------------------------


	char str1[20];
	char str2[20];
	char str3[20];

	std::cout << "String 1 :" << endl;
	std::cin >> str1;
	std::cout << "String 2:" << endl;
	std::cin >> str2;

	strcat(strcpy(str3, str1), str2)

	std::ofstream myfile;
	myfile.open("result.txt");
	myfile << str3;
	myfile.close();

	FILE * pFile;
	pFile = fopen (argc[3],"w");
	fprintf (pFile, "%f \n", res);
	fclose (pFile);
	
    
    sfd=create_client_socket(atoi(argv[2]), argv[1]);
    
	if ((fd = open(argv[3],O_RDONLY))==-1){
		perror("open fail");
		return EXIT_FAILURE;
	}
    

	if (stat(argv[3],&buffer)==-1){
		perror("stat fail");
		return EXIT_FAILURE;
	}
	else
		sz=buffer.st_size;
    

	bzero(&buf,BUFFERT);
    
    if(connect(sfd,(struct sockaddr*)&sock_serv,l)==-1){
        perror("conexion error\n");
        exit (3);
    }
	gettimeofday(&start,NULL);
    n=read(fd,buf,BUFFERT);
	while(n){
		if(n==-1){
			perror("read fails");
			return EXIT_FAILURE;
		}
		m=sendto(sfd,buf,n,0,(struct sockaddr*)&sock_serv,l);
		if(m==-1){
			perror("send error");
			return EXIT_FAILURE;
		}
		count+=m;
		fprintf(stdout,"----\n%s\n----\n",buf);
		bzero(buf,BUFFERT);
        n=read(fd,buf,BUFFERT);
	}

	m=sendto(sfd,buf,0,0,(struct sockaddr*)&sock_serv,l);
	gettimeofday(&stop,NULL);
	duration(&start,&stop,&delta);
    

    close(sfd);
	return EXIT_SUCCESS;
}

int duration (struct timeval *start,struct timeval *stop,struct timeval *delta)
{
    suseconds_t microstart, microstop, microdelta;
    
    microstart = (suseconds_t) (100000*(start->tv_sec))+ start->tv_usec;
    microstop = (suseconds_t) (100000*(stop->tv_sec))+ stop->tv_usec;
    microdelta = microstop - microstart;
    
    delta->tv_usec = microdelta%100000;
    delta->tv_sec = (time_t)(microdelta/100000);
    
    if((*delta).tv_sec < 0 || (*delta).tv_usec < 0)
        return -1;
    else
        return 0;
}


int create_client_socket (int port, char* ipaddr){
    int l;
	int sfd;
    
	sfd = socket(PF_INET,SOCK_STREAM,0);
	if (sfd == -1){
        perror("socket fail");
        return EXIT_FAILURE;
	}

	l=sizeof(struct sockaddr_in);
	bzero(&sock_serv,l);
	
	sock_serv.sin_family=AF_INET;
	sock_serv.sin_port=htons(port);
    if (inet_pton(AF_INET,ipaddr,&sock_serv.sin_addr)==0){
		printf("Invalid IP adress\n");
		return EXIT_FAILURE;
	}
    
    return sfd;
}