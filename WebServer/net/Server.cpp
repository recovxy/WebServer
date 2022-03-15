#include "Server.h"
#include "Util.h"

Server::Server(int port) : port_(port)
{
    listenFd_ = socket_bind_listen(port);
}



void Server::start()
{
    for(;;){
        struct sockaddr_in client_addr;
        int client_len = sizeof(client_addr);
        int connfd = 0;

        if((connfd = accept(listenFd_, (struct sockaddr*)&client_addr, &client_len)) == -1) 
            error_die("accept");
        
        int childpid = fork();
        
        if (childpid == 0){ // child
            close(listenFd_);    //child close listenfd socket，这里使用了引用计数
            
            std::cout << "http server running on port " << port_<< std::endl;
            //处理逻辑写在此处
            accept_request(connfd);

            exit(0);
        }else if(childpid > 0){ // parent 
            close(connfd); //parent close connected socket，这里使用了引用计数
        }else{          // error
            error_die("fork");
        }

    }


    started_ = true;
}


void Server::accept_request(int connfd)
{
    
}
