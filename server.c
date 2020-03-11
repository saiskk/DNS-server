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
       
    // Creating socket file descriptor 
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) 
    { 
        perror("socket failed"); 
        exit(EXIT_FAILURE); 
    } 
       
    // Forcefully attaching socket to the port 8080 
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) 
    { 
        perror("setsockopt"); 
        exit(EXIT_FAILURE); 
    } 
    address.sin_family = AF_INET; 
    address.sin_addr.s_addr = INADDR_ANY; 
    address.sin_port = htons( atoi(argv[1]) ); 
       
    // Forcefully attaching socket to the port 8080 
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
                       (socklen_t*)&addrlen))<0) 
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
    int n=0;
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
    int i=0,flag=0;     
    for(i=0;i<valread;i++)
    {
    	if(buffer[i]=='*')
		      break;
    }
    char s[i];
    for(int k=0;k<i;k++)
        s[k]=buffer[k];
    s[i]='\0';

    char *np="entry not found in the database";
	if((int)s[0]<=57&&(int)s[0]>=48)
	{
		for(i=0;i<n;i++)
		{	
			if(strcmp(s,ip_address[i])==0)
			{
				flag=1;
				send(new_socket , domain_name[i] , strlen(domain_name[i]) , 0 );
			} 
		}
        if(!flag)
            send(new_socket , np , strlen(np) , 0 );
	}
	else
	{
		for(i=0;i<n;i++)
		{
			if(strcmp(s,domain_name[i])==0)
			{
				flag=1;
				send(new_socket , ip_address[i] , strlen(ip_address[i]) , 0 );
			}
		}
        if(!flag)
            send(new_socket , np , strlen(np) , 0 );
	}
    printf("%s\n",s);
    printf("%s\n",argv[1]); 
    return 0; 
} 
