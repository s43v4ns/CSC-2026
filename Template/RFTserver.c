#include "RFTheader.h"
#include <stdbool.h> 
int sockfd;


/**************************************************************************
**************************************************************************
 All functions' signature
**************************************************************************
**************************************************************************/
int checksum(char *content, int len);
void receive_file_normal(char * name, int size, struct sockaddr_in server);
bool isLost(float prob);

 
/**************************************************************************
**************************************************************************
                  The main function  
**************************************************************************
**************************************************************************/
int main(int argc,char *argv[])
{                
    struct sockaddr_in server, client;
	
	/* user needs to enter the port number */
	if(argc!=2)
	{
		fprintf(stderr, "Usage: <portNumber> \n");
		exit(1);
	}
	
	/* create a socket */
	sockfd =socket(AF_INET,SOCK_DGRAM,0);
	if(sockfd <0)
	{
		perror("SERVER: Failed to open socket"); 
		close (sockfd );
		exit(1);
	}
	printf("------------------------------------------------------------------\n");
	printf ("SERVER: Socket is created\n");
 
    memset(&server, 0, sizeof(server));
    memset(&client, 0, sizeof(client));
	
	//Fill in the address structure (Filling server information)
    server.sin_family=AF_INET;
	server.sin_addr.s_addr=INADDR_ANY;
	server.sin_port=htons(atoi(argv[1]));
	
	/* bind - associate a socket "sockfd" with the "server" address */
	if(bind(sockfd, (struct sockaddr *)&server,sizeof(server))){
		perror("SERVER: Bind Failed\n");
		close(sockfd);
		exit(1);
	}
	
	printf("SERVER: binding ... Ready to receive meta info from the client \n"); 
	printf("------------------------------------------------------------------\n");
	printf("------------------------------------------------------------------\n");
	  
	meta_data  file_info;
	int len = sizeof(client);
	int val;

	//receive from the client  	
	if ((val=recvfrom(sockfd,&file_info,sizeof(file_info),0, (struct sockaddr *)&client, &len))<0){
		perror("SERVER: Reading Stream Message Error\n");
		exit(1);
	} else if(val==0){
		printf("SERVER: Ending connection\n");
	} else {
		printf("SERVER: Meta info are received successfully: file name(%s), size(%d)\n",file_info.name, file_info.size);
	}
	
 
	printf("------------------------------------------------------------------\n");
	printf("------------------------------------------------------------------\n");
	printf("SERVER: Waiting for the actual file ..\n"); 
	printf("------------------------------------------------------------------\n");
	 
    
    receive_file_normal(file_info.name, file_info.size, client);
             
	
	close(sockfd);
	return 0;
}

/*************************************************************************
**************************************************************************
 These functions are implemented for you .. Do NOT Change them
**************************************************************************
**************************************************************************/
int checksum(char *content, int len)
{
	int i;
	int sum = 0;
	for (i = 0; i < len; i++)
		sum += (int)(*content++);
	return sum;
}



void receive_file_normal(char * name, int size, struct sockaddr_in client )
{ 
    int len = sizeof(client);
    struct segment data_msg, ack_msg;
	    
  	
	FILE *file;
	/*Open the file*/
	file=fopen(name, "wb");
	int count=0;
    int val=0;
	bool receiving=true;
	
 	printf("------------------------------------------------\n");
 	printf("------------------------------------------------\n"); 
	
	//while the last segment is not received
	while(receiving) {

		//receive the segment
		if ((val=recvfrom(sockfd,&data_msg, sizeof(data_msg), 0,(struct sockaddr *)&client, &len))<0)
			perror("SERVER: Reading Stream Message Error\n");
		else if (val==0)
			printf("SERVER: Ending connection\n");
		else {
			if (count==0) //if it is the first segment to be received
			{
			printf("SERVER: File copying start\n"); 
 	        printf("------------------------------------------------\n");
			}

			printf("SERVER: A Segment with sq (%d) is received:\n",data_msg.sq); 
			printf("\tINFO: size %d, checksum %d, content (",data_msg.size,data_msg.checksum );
	     	
			//print the segment content nicely
			for (int c = 0;  c < data_msg.size; c++)
	            printf("%c", data_msg.payload[c]); 
			printf(")\n");
			
			//is it the last segment
			if (data_msg.last==1) {receiving=false;} 
			
		 
			int x = checksum(data_msg.payload, data_msg.size);
			
			/* If the calculated checksum is same as that of recieved 
			checksum then send corrosponding ack */
			if(x == data_msg.checksum){
				printf("SERVER: Calculated checksum is (%d) --  VALID\n",x);
				
				//prepare the Ack segment
				ack_msg.sq = data_msg.sq;
 				ack_msg.type= TYPE_ACK;
			    printf("SERVER: Sending an ACK with sq (%d)\n", ack_msg.sq);
				
				//send the Ack segment
				if ((val=sendto(sockfd, &ack_msg, sizeof(ack_msg), 0,(struct sockaddr *) &client, len))<0)
					    perror("SERVER: Reading Stream Message Error\n");
				else if(val==0)
				    	printf("SERVER: Ending connection\n");
				else
					{  
						//write the payload of the data segment to a file
					     fwrite(&data_msg.payload[0],data_msg.size,1,file);
					}
				 
			    printf("\t\t>>>>>>> NETWORK: ACK is sent successfully <<<<<<<<<\n");
			    count += data_msg.size;
				printf("------------------------------------------------\n");
				printf("------------------------------------------------\n");
			}	
			else 
			{
				printf("SERVER: Segment checksum is (%d) -- INVALID\n",x );
				printf("SERVER: Do NOT SEND ANY ACK\n");
				receiving = true;
				printf("************************************\n");
			}	
		}
	}
	printf("SERVER: File copying complete\n");
	fclose(file);
}
 
 