#include <sys/socket.h>       /*  socket definitions        */
#include <sys/types.h>        /*  socket types              */
#include <arpa/inet.h>        /*  inet (3) funtions         */
#include <unistd.h>           /*  misc. UNIX functions      */


#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>

/* for helper functions */

#ifndef PG_SOCK_HELP
#define PG_SOCK_HELP


#include <unistd.h>             /*  for ssize_t data type  */

#define LISTENQ        (1024)   /*  Backlog for listen()   */


/*  Function declarations  */

ssize_t Readline(int fd, void *vptr, size_t maxlen);
ssize_t Writeline(int fc, const void *vptr, size_t maxlen);


#endif  /*  PG_SOCK_HELP  */





/*  Global constants  */

#define MAX_LINE           (1000)


/*  Function declarations  */

int ParseCmdLine(int argc, char *argv[], char **szAddress, char **szPort);


/*  main()  */

int main(int argc, char *argv[]) {

    int       conn_s;                /*  connection socket         */
    short int port;                  /*  port number               */
    struct    sockaddr_in servaddr;  /*  socket address structure  */
    struct    sockaddr_in cliaddr;   /*  socket address for client */
    char      buffer[MAX_LINE];      /*  character buffer          */
    char      buffer2[MAX_LINE];	     /*  character buffer	   */	
    char     *szAddress;             /*  Holds remote IP address   */
    char     *szPort;                /*  Holds remote port         */
    char     *endptr;                /*  for strtol()              */
    char      temp[MAX_LINE];	     	     /*  user input     	   */
    socklen_t clilen;
    int msglen;



    /*  Get command line arguments  */

    ParseCmdLine(argc, argv, &szAddress, &szPort);


    /*  Set the remote port  */

    port = strtol(szPort, &endptr, 0);
    if ( *endptr ) {
	printf("ECHOCLNT: Invalid port supplied.\n");
	exit(EXIT_FAILURE);
    }
	

    /*  Create the listening socket  */

    /*if ( (conn_s = socket(AF_INET, SOCK_STREAM, 0)) < 0 ) {
	fprintf(stderr, "ECHOCLNT: Error creating listening socket.\n");
	exit(EXIT_FAILURE);
    }*/

    conn_s = socket(PF_INET,SOCK_DGRAM,0);
    



     /*  Set all bytes in socket address structure to
        zero, and fill in the relevant data members   */

    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family      = AF_INET;
    servaddr.sin_port        = htons(port);


    /*  Set the remote IP address  */

    if ( inet_aton(szAddress, &servaddr.sin_addr) <= 0 ) {
	printf("ECHOCLNT: Invalid remote IP address.\n");
	exit(EXIT_FAILURE);
    }


    
    
    
    
    
    
    while(1)
    {
        memset(temp,'\0',sizeof(temp));    /*clear temp buffer */
	printf("Please enter s for string, t for file, or q to quit: ");
    	fgets(temp,MAX_LINE,stdin);
    	if(temp[1] != '\n')
    	{
	    printf("Input is incorrect!");  //test for right amount of char
	    memset(temp,'\0',sizeof(temp)); //reinitialize buffer
	    exit(0); 				//exit upon error
	    //break;	
    	}
	if((temp[0] == 's') || (temp[0] == 'S'))   
	    strcpy(buffer,"CAP\n");   		//send cap if user enter s
	if((temp[0] == 't') || (temp[0] == 'T'))
	{
	    strcpy(buffer,"FILE\n");		//send file is user enter t
	    
	}
	memset(temp,'\0',sizeof(temp));		//reinitalize buffer
	printf("Enter the string: ");		//request string from user
	fgets(buffer2,MAX_LINE,stdin);		//receive string from user
	strncat(buffer,buffer2,sizeof(buffer2));//concat string with action
 		
	/*send string to server and receive response*/
	sendto(conn_s,buffer,MAX_LINE,0,(struct sockaddr *)&servaddr,sizeof(servaddr));
	msglen = recvfrom(conn_s,buffer,MAX_LINE,0,NULL,NULL);
	/*output echoed results */
	printf("Print response: %s\n",buffer);
	memset(buffer,'\0',sizeof(buffer));
	memset(buffer2,'\0',sizeof(buffer2));
	if( close(conn_s)< 0)
	{
	   fprintf(stderr, "SERVER: Error calling close(): %s\n", strerror(errno)) ;
	}	
    }    
    

    /*  connect() to the remote echo server  */

    /*if ( connect(conn_s, (struct sockaddr *) &servaddr, sizeof(servaddr) ) < 0 ) {
	printf("ECHOCLNT: Error calling connect()\n");
	exit(EXIT_FAILURE);
    }*/


    /*  Get string to echo from user  */

       // memset(temp,'\0',sizeof(temp));   /* clear temp buffer */
       // printf("Please enter s for string, t for file, or q to quit: ");
       // fgets(temp,MAX_LINE,stdin);
       //if (temp[1] != '\n')
	//{ 
	//    printf("Input is incorrect\n");
	//    memset(temp,'\0',sizeof(temp));
	//    exit(0);	
	//   // break;
        //}
	//if ((temp[0] == 's') || (temp[0] == 'S'))
        //    strcpy(buffer, "CAP\n");
        //if ((temp[0] == 't') || (temp[0] == 'T'))
	//    strcpy(buffer, "FILE\n");
        //memset(temp,'\0',sizeof(temp));
        //printf("Enter the string: ");
        //fgets(buffer2, MAX_LINE, stdin);
	//strncat(buffer,buffer2,strlen(buffer2));


        /*  Send string to echo server, and retrieve response  */

        //Writeline(conn_s, buffer, strlen(buffer));
        //Readline(conn_s, buffer, MAX_LINE-1);
	/*  Output echoed string  */

        //printf("Echo response: %s\n", buffer);
        //memset(buffer,'\0',sizeof(buffer));
        //memset(buffer2,'\0',sizeof(buffer2));
        

   
    return EXIT_SUCCESS;
}


int ParseCmdLine(int argc, char *argv[], char **szAddress, char **szPort) {

    int n = 1;

    while ( n < argc ) {
	if ( !strncmp(argv[n], "-a", 2) || !strncmp(argv[n], "-A", 2) ) {
	    *szAddress = argv[++n];
	}
	else if ( !strncmp(argv[n], "-p", 2) || !strncmp(argv[n], "-P", 2) ) {
	    *szPort = argv[++n];
	}
	else if ( !strncmp(argv[n], "-h", 2) || !strncmp(argv[n], "-H", 2) ) {
	    printf("Usage:\n\n");
	    printf("    timeclnt -a (remote IP) -p (remote port)\n\n");
	    exit(EXIT_SUCCESS);
	}
	++n;
    }

    return 0;
}






/* helper functions */



/*  Read a line from a socket  */

ssize_t Readline(int sockd, void *vptr, size_t maxlen) {
    ssize_t n, rc;
    char    c, *buffer;
    int    counter   = 0;
    buffer = vptr;

    for ( n = 1; n < maxlen; n++ ) {
	
	if ( (rc = read(sockd, &c, 1)) == 1 ) {
	    *buffer++ = c;
	    if ( c == '\n')
	    {	
		counter++;
		if(counter > 1) 
		{
		    break;
		}
	    }
	}
	else if ( rc == 0 ) {
	    if ( n == 1 )
		return 0;
	    else
		break;
	}
	else {
	    if ( errno == EINTR )
		continue;
	    return -1;
	}
    }

    *buffer = 0;
    return n;
}


/*  Write a line to a socket  */

ssize_t Writeline(int sockd, const void *vptr, size_t n) {
    size_t      nleft;
    ssize_t     nwritten;
    const char *buffer;

    buffer = vptr;
    nleft  = n;

    while ( nleft > 0 ) {
	if ( (nwritten = write(sockd, buffer, nleft)) <= 0 ) {
	    if ( errno == EINTR )
		nwritten = 0;
	    else
		return -1;
	}
	nleft  -= nwritten;
	buffer += nwritten;
    }
  
    return n;
}


