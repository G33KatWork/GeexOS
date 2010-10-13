#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#define BUFFER_SIZE 1024

int handling(int sock)
{
	while(1)
	{
	    getchar();
	    
	    printf("sending -\n");
        char c = '2';
        int bytes = send(sock, &c, 1, 0);
        if (bytes == -1)
            return -1;
            
	    getchar();
	    
	    printf("sending a\n");
        c = '1';
        bytes = send(sock, &c, 1, 0);
        if (bytes == -1)
            return -1;
        
        getchar();
        
        printf("sending b\n");
        c = '2';
        bytes = send(sock, &c, 1, 0);
        if (bytes == -1)
            return -1;
            
        getchar();
        
        printf("sending b\n");
        c = '3';
        bytes = send(sock, &c, 1, 0);
        if (bytes == -1)
            return -1;
        
        getchar();
        
        printf("sending b\n");
        c = 'b';
        bytes = send(sock, &c, 1, 0);
        if (bytes == -1)
            return -1;
        
        getchar();
        
        printf("sending b\n");
        c = 'b';
        bytes = send(sock, &c, 1, 0);
        if (bytes == -1)
            return -1;
            
        getchar();
        
        printf("sending b\n");
        c = 'b';
        bytes = send(sock, &c, 1, 0);
        if (bytes == -1)
            return -1;
            
        getchar();
        
        printf("sending b\n");
        c = 'b';
        bytes = send(sock, &c, 1, 0);
        if (bytes == -1)
            return -1;
        
        getchar();
        
        printf("sending e\n");
        c = 'e';
        bytes = send(sock, &c, 1, 0);
        if (bytes == -1)
            return -1;
        
        getchar();
        
        printf("sending c\n");
        c = 'c';
        bytes = send(sock, &c, 1, 0);
        if (bytes == -1)
            return -1;
            
        getchar();
        
        printf("sending \\n\n");
        c = '\n';
        bytes = send(sock, &c, 1, 0);
        if (bytes == -1)
            return -1;
	}
	
	return 0;
}

int main(int argc, char *argv[])
{
	int s;
	struct sockaddr_in srv;
	
	if (argc != 3)
	{
		fprintf(stderr, "usage: %s host port\n", argv[0]);
		return 1;
	}
	
	s = socket(AF_INET, SOCK_STREAM, 0);
	if (s == -1)
	{
		perror("socket failed()");
		return 2;
	}
	
	srv.sin_addr.s_addr = inet_addr(argv[1]);
	srv.sin_port = htons( (unsigned short int) atol(argv[2]));
	srv.sin_family = AF_INET;
	
	if (connect(s, &srv, sizeof(srv)) == -1)
	{
		perror("connect failed()");
		return 3;
	}
	
	if (handling(s) == -1)
	{
		fprintf(stderr, "%s: error in handling()\n", argv[0]);
		return 4;
	}
	
	close(s);
	
	return 0;
}
