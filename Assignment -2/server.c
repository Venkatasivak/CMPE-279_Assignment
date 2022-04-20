// Server side C/C++ program to demonstrate Socket programming
#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <pwd.h>
#include <errno.h>
#define PORT 8080

int main(int argc, char const *argv[])
{


    int server_fd, new_socket, valread;
    struct sockaddr_in address;
    struct passwd* pwd;
    pid_t pid;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[102] = {0};
    char *hello = "Hello from server";
    char *user = "nobody";

    printf("execve=0x%p\n", execve);

if( argc == 1)
{

    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // Forcefully attaching socket to the port 8080
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,
                                                  &opt, sizeof(opt)))
    {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons( PORT );

    // Forcefully attaching socket to the port 8080
    if (bind(server_fd, (struct sockaddr *)&address,
                                 sizeof(address))<0)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    if (listen(server_fd, 3) < 0)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }
  

    pid = fork();
    if (pid == 0) {
    /*child process */

    	    if ((pwd = getpwnam(user)) == NULL) {
            perror("No  UID for nobody");
        }

         //printf( " fd before exec %d\n", new_socket);
	
       if (setuid(pwd->pw_uid) < 0)
            {
                printf("Error in drop privilege: %s\n", strerror(errno));
                return 0;
            }
            
        char fd_arg[10];
            char port_arg[10];
            snprintf(fd_arg, 10, "%d", server_fd);
            snprintf(port_arg, 10, "%d", PORT);
            char *args[] = {fd_arg, port_arg, NULL};
//printf("getuid %d \n", getuid());

            execv("./server", args);
        
        //exit(0);
    }
    else if (pid > 0)
    {

        
        wait(NULL);
       

       
            //printf("child  connection from client\n");
    }else
        {
            perror("\n forking process failed \n");
            _exit(2);
        }
        printf("\n End of privilege separation\n");
        return 0; 
        


}
else // 
{	
	
      
         server_fd = atoi(argv[0]);
         
        
         
         if ((new_socket = accept(server_fd, (struct sockaddr *)&address,
                                 (socklen_t *)&addrlen)) < 0)
        {
            perror("Error ");
            exit(EXIT_FAILURE);
        }
        //printf("in child \n");

      
        
        valread = read(new_socket , buffer, 1024);
        printf("%s\n", buffer);
        send(new_socket , hello , strlen(hello) , 0);
        printf("Hello message sent\n");
	
}

    return 0;
}