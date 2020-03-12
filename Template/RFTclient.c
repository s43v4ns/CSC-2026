#include "RFTheader.h"
#include <stdbool.h>
#include <sys/time.h>
#include <string.h>

int sockfd; //This is to be used in each function
int total_segments_sent = 0;
static const char *ERR_FMT = "%s:%d - %s, errno %d: %s\n";/* error message format for fatalerror - Source Nick Cook*/


/**************************************************************************
**************************************************************************
 All functions' signature
**************************************************************************
**************************************************************************/
int checksum(char *content, int len);
FILE * check_file(char *fileName);
int file_size(FILE *file);
void fatalerror(int line, char *msg);
bool isCorrupted(float prob);

struct sockaddr_in create_UDP_socket( char* host, int port);
void send_meta_data(FILE *file,char *output, struct sockaddr_in server);
void send_file_normalPAR(FILE *fp, struct sockaddr_in server);
void send_file_with_timeoutPAR(FILE *fp, struct sockaddr_in server, float prob_loss);
void send_file_normalGBN(FILE *fp,int sockfd, struct sockaddr_in server);

/**************************************************************************
**************************************************************************
 Functions that you need to implement
**************************************************************************
**************************************************************************/
/* arguments: host name, port number 
   purpose: create a socket and fill in server address structure
   return: The internet socket address of the server 
   note the sockfd is defined as a global variable*/
struct sockaddr_in create_UDP_socket(char* host, int port)
 {  
    struct sockaddr_in server;
	/* Replace the following with function implementation */ 
    fatalerror(__LINE__, "create_UDP_socket is not implemented"); 
    return server;
 } 
  
/* arguments: input file, the name of the output file, 
              port number, structure containing the server internet address 
  note the sockfd is defined as a global variable*/ 
void send_meta_data(FILE *file,char *output, struct sockaddr_in server)
{     
    /* Replace the following with function implementation */ 
    fatalerror(__LINE__, "send_meta_data is not implemented");  
} 
  
  
/* arguments: input file, the internet socket address of the server 
    note the sockfd is defined as a global variable */
void send_file_normalPAR(FILE *fp, struct sockaddr_in server){
	
   /* Replace the following with function implementation */ 
    fatalerror(__LINE__, "send_file_normalPAR is not implemented");  

}


/* arguments: input file, the internet socket address of the server, the probability of loosing a segment  
   note the sockfd is defined as a global variable*/ 
void send_file_with_timeoutPAR(FILE *fp, struct sockaddr_in server, float prob_loss)
{   
    /* Replace the following with function implementation */ 
    fatalerror(__LINE__, "send_file_with_timeoutPAR is not implemented");  

}


/* arguments: input file, the window size, the internet socket address of the server 
   note the sockfd is defined as a global variable */
void send_file_normalGBN(FILE *fp,int window_size, struct sockaddr_in server)
{  
   /* Replace the following with function implementation */ 
    fatalerror(__LINE__, "send_meta_data is not implemented");  
		
} 


/**************************************************************************
**************************************************************************
                  The main function  
**************************************************************************
**************************************************************************/
int main(int argc,char *argv[])
{
	FILE *file;
    struct sockaddr_in server;
	
	/* 	accept input from console in the form of 
	./client inputFile outputFile localhost portNumber 	*/
	if (argc != 5) {
		fprintf(stderr, "Usage: <inputFile> <outputFile> <localhost> <portNumber> \n");
		exit(1);
	}
		
	/* check the file actually exist	*/
	file=check_file(argv[1]);
	printf ("----------------------------------------------------\n");
	printf ("SENDER: File (%s) exists with a size of (%d) bytes\n",argv[1], file_size(file));
	printf ("----------------------------------------------------\n");
	printf ("----------------------------------------------------\n");

	/* create a UDP socket and assign all server information */
	server = create_UDP_socket(argv[3], atoi(argv[4]));
 	
	printf ("SENDER: Server data is prepared \n");
	 
 	
	/* Send meta data to the server */
	send_meta_data(file,argv[2],server);
	
	/* transmission options */
	int choice=0;
    float loss=0;
	int window=0;
	printf ("----------------------------------------------------\n");
	printf ("----------------------------------------------------\n");
	printf("Choose one of the following options (1 or 2):\n ");
	printf("1. Normal transmission using the PAR protocol (no data segment is lost) \n ");
	printf("2: Transmission with time-out capabilities using the PAR protocol\n ");
	printf("3: Normal transmission using the GoBackN protocol(no data segment is lost) \n ");
    scanf("%d", &choice);
    switch(choice)
    {
        case 1:
            send_file_normalPAR(file, server);
            break;
        
        case 2:
			printf("Enter the probability of a corrupted checksum (between 0 and 1):\n ");
			scanf("%f", &loss);
			send_file_with_timeoutPAR(file, server, loss);
            break;

         case 3:
		    printf("Enter the window size:\n ");
			scanf("%d", &window);
            send_file_normalGBN(file,window, server);
            break;
       
	   default:
            printf("Error! enter 1, 2 or 3 \n");
    } 

	printf("SENDER: File is sent\n");
	
	/* Close the file */
	fclose(file);
	
	/* Close the socket */
	close (sockfd );
	
 	return 0;
} 

/*************************************************************************
**************************************************************************
 These functions are implemented for you .. Do NOT Change them
**************************************************************************
**************************************************************************/

/* calculate the segment checksum by adding the payload */
int checksum(char *content, int len)
{
	int i;
	int sum = 0;
	for (i = 0; i < len; i++)
		sum += (int)(*content++);
	return sum;
}

/* check if the input file does exist */
FILE * check_file(char *fileName)
{
	FILE *file = fopen(fileName, "rb");
	if(!file) {
		perror("SENDER: File does not exists");
 		fclose(file);
		exit(1);
	}
	return file;
}

/* return file size */
int file_size(FILE *file)
{
	fseek(file, 0L, SEEK_END);
	int size = ftell(file);
	fseek(file, 0L, SEEK_SET);
	return size;
}

/* decide whether the segment is corrupted or not given a sertain probability */
bool isCorrupted(float prob)
{
    if(((float)(rand())/(float)(RAND_MAX)) < prob)
	{
	return true;
	}
    else return false;
}

void fatalerror(int line, char *msg) {
    printf("Line %d : %s\n", line, msg);
    exit(0);
}
/*************************************************************************** 
 *************************************************************************** 
 *************************************************************************** 
 ***************************************************************************/

