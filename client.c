#include <stdio.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <unistd.h> 
#include <string.h> 
#define PORT 8080 
   
int main(int argc, char *argv[]) 
{ 
    int sock = 0, valread; 
    struct sockaddr_in serv_addr; 
    char buffer[1024];
    memset(buffer,0,1024); 
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
    { 
        printf("\n Socket creation error \n"); 
        return -1; 
    } 
   
    serv_addr.sin_family = AF_INET; 
    serv_addr.sin_port = htons(PORT);

    // Convert IPv4 and IPv6 addresses from text to binary form 
    if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0)  
    { 
        printf("\nInvalid address/ Address not supported \n"); 
        return -1; 
    } 
   
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) 
    { 
        printf("\nConnection Failed \n"); 
        return -1; 
    } 
    printf("Enter the domain or ip address:\n");
    send(sock , argv[1] , strlen(argv[1]) , 0 );
    send(sock , "*" , 1 , 0 );
    send(sock , argv[2] , strlen(argv[2]) , 0 );
    send(sock , "*" , 1 , 0 );
    send(sock , argv[3] , strlen(argv[3]) , 0 );  
    valread = read( sock , buffer, 1024); 
    printf("%s\n", buffer ); 
    return 0; 
} 
