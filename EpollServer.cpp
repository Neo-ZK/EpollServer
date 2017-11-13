#include <iostream>
#include"include/m_Epoll.h"
#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<string.h>
#include<memory>
#include<arpa/inet.h>
using namespace std;

const int maxfdNUM = 20;
const int default_port = 8000;
const int listen_que_num = 20;
const int buffer_len = 2048;
int main()
{
    //m_Epoll* Epoll = new m_Epoll(maxfdNUM);
    shared_ptr<m_Epoll> sp_Epoll(new m_Epoll(maxfdNUM));
    struct      sockaddr_in serveraddr;
    struct      sockaddr_in clientaddr;
    int         listenfd;
    int         confd;
    socklen_t   clilen;


    if((listenfd = socket(AF_INET,SOCK_STREAM,0)) == -1)
    {
        perror("socket error");
        return -1;
    }

    memset(&serveraddr,0,sizeof(serveraddr));
    serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serveraddr.sin_port = htons(default_port);
    serveraddr.sin_family = AF_INET;


    if( bind(listenfd,(sockaddr*)&serveraddr,sizeof(sockaddr)) == -1)
    {
        cout<<"bind error"<<endl;
        return -1;
    }


    if(listen(listenfd,listen_que_num) == -1)
    {
        cout<<"listen error"<<endl;
        return -1;
    }

    sp_Epoll->add(listenfd,EPOLLIN);


    while(true)
    {
        int nfds = sp_Epoll->wait(maxfdNUM,-1);
        for(int i = 0;i < nfds;i++)
        {
            if(sp_Epoll->_events[i].data.fd == listenfd)
            {
                if((confd = accept(listenfd,(sockaddr*)&clientaddr,&clilen)) == -1)
                {
                    cout<<"accept error"<<endl;
                    return -1;
                }
                char* str = inet_ntoa(clientaddr.sin_addr);
                cout<<"accept a connection of"<<str<<endl;
                sp_Epoll->add(confd,EPOLLIN);
            }
            else if((sp_Epoll->_events[i].events & EPOLLIN) == EPOLLIN)
            {
                int tempfd = sp_Epoll->_events[i].data.fd;
                if(tempfd < 0)
                {
                    continue;
                }
                char buffer[buffer_len];
                int  n = 0;
                if((n = recv(tempfd,buffer,buffer_len,0)) > 0)
                {
                    cout<< "receive message is"<< buffer<<endl;
                    buffer[n] = '\0';
                }
                else
                {
                    if(errno == ECONNRESET) //TCP connect over time,RST
                    {
                        close(tempfd);
                        sp_Epoll->_events[i].data.fd = -1;
			continue;
                    }
                    else if(errno == EINTR)
                    {
                        cout << "connect problem"<<endl;
                        continue;
                    }
                    else
                    {
                        close(tempfd);
                        sp_Epoll->_events[i].data.fd = -1;
			cout << "the connection is terminated by client"<<endl;
			continue;
                    }
                }

                if(strcmp(buffer,"quit") == 0)
                {
                    close(tempfd);
                    continue;
                }
                sp_Epoll->mod(confd,EPOLLOUT);
            }
            else if((sp_Epoll->_events[i].events & EPOLLOUT) == EPOLLOUT)
            {
                int tempfd = sp_Epoll->_events[i].data.fd;
                send(tempfd,"success recv your message",25,0);
                //sp_Epoll->del(tempfd);
                sp_Epoll->mod(tempfd,EPOLLIN);
            }
        }
    }


    return 0;
}
