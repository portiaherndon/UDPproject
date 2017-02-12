#include <sys/socket.h>       /*  socket definitions        */
#include <sys/types.h>        /*  socket types              */
#include <arpa/inet.h>        /*  inet (3) funtions         */
#include <unistd.h>           /*  misc. UNIX functions      */


#include <stdlib.h>
#include <stdio.h>
#include <errno.h>


/*  Global constants  */

#define ECHO_PORT          (2002)
#define MAX_LINE           (1000)



/* for helper functions */
#ifndef PG_SOCK_HELP
#define PG_SOCK_HELP


#include <unistd.h>             /*  for ssize_t data type  */

#define LISTENQ        (1024)   /*  Backlog for listen()   */


/*  Function declarations  */

ssize_t Readline(int fd, void *vptr, size_t maxlen);
ssize_t Writeline(int fc, const void *vptr, size_t maxlen);


#endif  /*  PG_SOCK_HELP  */




int main(int argc, char *argv[]) {
    int       list_s;                /*  listening socket          */
    int       conn_s;                /*  connection socket         */
    short int port;                  /*  port number               */
    struct    sockaddr_in servaddr;  /*  socket address structure  */
    char      buffer[MAX_LINE];      /*  character buffer          */
    char      buffer2[MAX_LINE];     /*  buffer for cap/file */
    char      buffer3[MAX_LINE];     /*  buffer for cap string */ 
    char     *endptr;                /*  for strtol()              */
    int       y = 0;		     /*  counter 		*/
    int       x = 0;		     /*  counter 		*/
    int	      z = 0;		     /*  counter		*/
    FILE     *ifp;		     /*  file pointer		   */
 

    /*  Get port number from the command line, and
        set to default port if no arguments were supplied  */

    if ( argc == 2 ) {
	port = strtol(argv[1], &endptr, 0);
	if ( *endptr ) {
	    fprintf(stderr, "ECHOSERV: Invalid port number.\n");
	    exit(EXIT_FAILURE);
	}
    }
    else if ( argc < 2 ) {
	port = ECHO_PORT;
    }
    else {
	fprintf(stderr, "ECHOSERV: Invalid arguments.\n");
	exit(EXIT_FAILURE);
    }

	
    /*  Create the listening socket  */

    if ( (list_s = socket(AF_INET, SOCK_STREAM, 0)) < 0 ) {
	fprintf(stderr, "ECHOSERV: Error creating listening socket.\n");
	exit(EXIT_FAILURE);
    }


    /*  Set all bytes in socket address structure to
        zero, and fill in the relevant data members   */

    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family      = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port        = htons(port);


    /*  Bind our socket addresss to the 
	listening socket, and call listen()  */

    if ( bind(list_s, (struct sockaddr *) &servaddr, sizeof(servaddr)) < 0 ) {
	fprintf(stderr, "ECHOSERV: Error calling bind()\n");
	exit(EXIT_FAILURE);
    }

    if ( listen(list_s, LISTENQ) < 0 ) {
	fprintf(stderr, "ECHOSERV: Error calling listen()\n");
	exit(EXIT_FAILURE);
    }

    
    /*  Enter an infinite loop to respond
        to client requests and echo input  */

    while ( 1 ) {

	/*  Wait for a connection, then accept() it  */

	if ( (conn_s = accept(list_s, NULL, NULL) ) < 0 ) {
	    fprintf(stderr, "ECHOSERV: Error calling accept()\n");
	    exit(EXIT_FAILURE);
	}


	/*  Retrieve an input line from the connected socket
	    then simply write it back to the same socket.     */

	Readline(conn_s, buffer, MAX_LINE-1);
	
	
	printf("%s\n", buffer);
	memset(buffer2,'\0',sizeof(buffer2)); 
	y = 0;
	while (buffer[y] != '\n')
	{
	    buffer2[y] = buffer[y];
	    y++;
	}
	y = 0;
	printf("%s\n", buffer2);
	memset(buffer3,'\0',sizeof(buffer3)); /*clear out buffer */
	while(buffer2[x] != '\0')
	{
	    if(buffer2[x] == 'C')
	    {
		x++;
		if(buffer2[x] == 'A')
		{
		    x++;
		    if(buffer2[x] == 'P')
		    {
			printf("%s\n",buffer2);
			while(buffer[y+4] != '\n')
	    		{
                		buffer3[y] = toupper(buffer[y+4]);
				y++;
	    		} 
		    }
		    else
			break;
		}
		else
		    break;
	    }
            else if (buffer2[x] == 'F')
	    {
		x++;
		if (buffer2[x] == 'I')
		{
		    x++;
		    if(buffer2[x] == 'L')
		    {
			x++;
			if(buffer2[x] == 'E')
			{
			    z = 0;
			    while(buffer[z+5] != '\n')
			    {
			   	buffer3[z] = buffer[z+5];
			   	z++;
			    }
			    z=0; 
	    		    if((access(buffer3,F_OK) == 0) && (access(buffer3,R_OK)==0))   
			    {
				printf("exists\n");
				ifp = fopen("client.c","r"); /*open file*/
				if (ifp == NULL)
				   //printf("open");
				    perror("Error");
		            }
			    else
				printf("Does not exist\n");
			}
			else
				break;
		    }
		    else
			break;
		}
		else
		    break;
	    }
	    else
		break;		/*if buffer2 is not CAP or FILE */ 
		
	} 
	x=0;
	Writeline(conn_s, buffer3, strlen(buffer3));

	/*  Close the connected socket  */

	if ( close(conn_s) < 0 ) {
	    fprintf(stderr, "ECHOSERV: Error calling close()\n");
	    exit(EXIT_FAILURE);
	}
    }
}


/*helper functions */


/*  Read a line from a socket  */

ssize_t Readline(int sockd, void *vptr, size_t maxlen) {
    ssize_t n, rc;
    char    c, *buffer;
	int counter = 0;
    buffer = vptr;

    for ( n = 1; n < maxlen; n++ ) {
	
	if ( (rc = read(sockd, &c, 1)) == 1 ) {
	    *buffer++ = c;
	    if ( c == '\n' )
	    {
	        counter++;
	        if (counter > 1)
	            break;
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


