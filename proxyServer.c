#include <unistd.h> 
#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <string.h>
#include <arpa/inet.h> 
#include <unistd.h>
#define PORT 8080

int main(int argc, char *argv[])
{
    int server_fd, new_socket,valread; 
    struct sockaddr_in address;
    int opt = 1; 
    int addrlen = sizeof(address); 
    char buffer[1024] = {0};
       
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
    address.sin_port = htons( PORT ); 
       
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
    };
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address,  
                       (socklen_t*)&addrlen)) < 0) 
    { 
        perror("accept"); 
        exit(EXIT_FAILURE); 
    }

	valread = read(new_socket , buffer, 1024);
    int i=0,j=0,flag=0;     
    for(i=0;i<valread;i++)
    {
    	if(buffer[i]=='*')
			break;
    }
    for(j=i+1;i<valread;j++)
    {
    	if(buffer[j]=='*')
			break;
    }
    char server_address[i+1],port_number[j-i],search[valread-j];
    
    for(int k=0;k<i;k++)
        server_address[k]=buffer[k];
    
    for(int k=i+1;k<j;k++)
        port_number[k-i-1]=buffer[k];

    for(int k=j+1;k<valread;k++)
        search[k-j-1]=buffer[k];
    
    server_address[i]='\0';
    port_number[j-i-1]='\0';
    search[valread-j-1]='\0';

    FILE *fp;
    fp = fopen("cache.txt", "r");
    if(!fp) 
    {
    	printf("\nError: unable to open the file.\n");
    	return 1;
    }
    char ip_address[3][100];
    memset(ip_address, 0, 3*100*sizeof(char) );
    char domain_name[3][100];
    memset(domain_name, 0, 3*100*sizeof(char) );
    char c[100],d[100];
    for(int i=0;i<3;i++)
    {
    	fscanf(fp, "%[^ ]%*c", c);
    	strcpy(ip_address[i],c);
    	fscanf(fp, "%[^\n]%*c", d);
    	strcpy(domain_name[i],d);
    }
    fclose(fp);
	
	if((int)search[0]<=57&&(int)search[0]>=48)
	{
		for(i=0;i<3;i++)
		{	
			if(strcmp(search,ip_address[i])==0)
			{
				flag=1;
				send(new_socket , domain_name[i] , strlen(domain_name[i]) , 0 );
			} 
		}
	}
	else
	{
		for(i=0;i<3;i++)
		{
			if(strcmp(search,domain_name[i])==0)
			{
				flag=1;
				send(new_socket , ip_address[i] , strlen(ip_address[i]) , 0 );
			}
		}
	}


	if(!flag)
	{
	    int sock = 0, valread; 
	    struct sockaddr_in serv_addr; 
	    char buffer[1024] = {0}; 
	    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
	    { 
	        printf("\n Socket creation error \n"); 
	        return -1; 
	    } 
	    serv_addr.sin_family = AF_INET; 
	    serv_addr.sin_port = htons(atoi("8000")); 
	       
	    if(inet_pton(AF_INET, server_address, &serv_addr.sin_addr.s_addr)<=0)  
	    { 
	        printf("\nInvalid address/ Address not supported \n"); 
	        return -1; 
	    }
	    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) 
	    { 
	        printf("\nConnection Failed \n"); 
	        return -1; 
	    }

		send(sock , search , strlen(search) , 0 );
		valread = read( sock , buffer, 1024);  
    	char *np="entry not found in the database";
    	
    	send(new_socket , buffer , strlen(buffer) , 0 );

    	if(strcmp(buffer,np)!=0)
    	{
    		strcpy(ip_address[0],ip_address[1]);
			strcpy(ip_address[1],ip_address[1]);
			strcpy(domain_name[0],domain_name[1]);
			strcpy(domain_name[1],domain_name[2]);
		if((int)buffer[0]<=57&&(int)buffer[0]>=48)
		{
			strcpy(ip_address[2],search);
			buffer[valread]='\0';
			strcpy(domain_name[2],buffer);
		}
		else
		{
			strcpy(domain_name[2],search);
			buffer[valread]='\0';
			strcpy(ip_address[2],buffer);
		}
	
		remove("cache.txt");
		FILE *fp1;
		fp1=fopen("cache.txt","w");
		for(int i=0;i<2;i++)
		{
		 	fprintf(fp1,"%s ",ip_address[i]);
		 	fprintf(fp1,"%s\n",domain_name[i]);
		}
		fprintf(fp1,"%s ",domain_name[2]);
		fprintf(fp1,"%s\n",ip_address[2]);
		fclose(fp1);
	}
	}
	return 0;
} 
