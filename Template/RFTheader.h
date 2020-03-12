 #include <stdio.h>
 #include <sys/types.h>
 #include <sys/socket.h>
 #include <netinet/in.h> // for sockaddr_in
 #include <stdlib.h>
 #include <unistd.h>
 #include <arpa/inet.h>
 #include <string.h>

 #define NAMESTRING 100
 #define TOTALCHAR  20
 
 typedef struct {
      char  name[NAMESTRING]; //name of the file
      int size;               //size of the file
} meta_data;

enum s_type{
	  TYPE_DATA,     //data segment
	  TYPE_ACK       //Ack segment
};
 
 struct segment{
       int size;                //size of the payload
       int sq;                  //sequence number
	   enum s_type type;        //segment type
       char payload[TOTALCHAR]; //data
	   int last;                //last segment?
       int checksum;            //checksum
};
 
 