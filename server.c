#include <unistd.h> 
#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <string.h>  

int main(int argc, char *argv[]) 
{ 
    int server_fd,new_socket,valread; 
    struct sockaddr_in address; 
    int opt = 1; 
    int addrlen = sizeof(address); 
    char buffer[1024] = {0};
       
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) 
    { 
        perror("socket failed"); 
        exit(EXIT_FAILURE); 
    } 
       
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) 
    { 
        perror("setsockopt"); 
        exit(EXIT_FAILURE); 
    } 
    address.sin_family = AF_INET; 
    address.sin_addr.s_addr = INADDR_ANY; 
    address.sin_port = htons( atoi(argv[1]) ); 
       
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address))<0) 
    { 
        perror("bind failed"); 
        exit(EXIT_FAILURE); 
    } 
    if (listen(server_fd, 3) < 0) 
    { 
        perror("listen"); 
        exit(EXIT_FAILURE); 
    }
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address,  
                       (socklen_t*)&addrlen)) < 0) 
    { 
        perror("accept"); 
        exit(EXIT_FAILURE); 
    } 
        
    FILE *fp;
    fp = fopen("database.txt", "r");
    if(!fp) 
    {
        printf("\nError: unable to open the file.\n");
        return 1;
    }
    int n=1;
    char p;
    for (p = getc(fp); p!= EOF; p = getc(fp)) 
    {
         if (p=='\n') 
            n = n + 1;
    }
    fseek(fp, 0, SEEK_SET);

    char ip_address[n][100];
    memset(ip_address, 0, n*100*sizeof(char) );
    char domain_name[n][100];
    memset(domain_name, 0, n*100*sizeof(char) );
    char c[100],d[100];
    for(int i=0;i<n;i++)
    {
        fscanf(fp, "%[^ ]%*c", c);
        strcpy(ip_address[i],c);
        fscanf(fp, "%[^\n]%*c", d);
        strcpy(domain_name[i],d);
    }
    fclose(fp); 

    valread = read(new_socket , buffer, 1024);
    int i=0,flag=0,k;     
    for(i=0;i<valread;i++)
    {
    	if(buffer[i]=='*')
		      break;
    }
    char s1[i],s2[i+1];
    for(k=0;k<i;k++)
    {
        s1[k]=buffer[k];
        s2[k]=buffer[k];
    }
    s2[k] = '\r' ;
    s1[k] = '\0' ;
    s2[k+1] = '\0' ;

    char *np="entry not found in the database";

	if((int)s1[0]<=57&&(int)s1[0]>=48)
	{
		for(i=0;i<n;i++)
		{	
			if(strcmp(s1,ip_address[i])==0)
			{
				flag=1;
				send(new_socket , domain_name[i] , strlen(domain_name[i]) , 0 );
                printf("testdadey\n");
			} 
		}
        if(!flag)
            send(new_socket , np , strlen(np) , 0 );
	}
	else
	{
		for(i=0;i<n;i++)
		{
			if(strcmp(s2,domain_name[i])==0)
			{
				flag=1;
				send(new_socket , ip_address[i] , strlen(ip_address[i]) , 0 );
			}
		}
        if(!flag)
            send(new_socket , np , strlen(np) , 0 );
	}
    return 0; 
} 
