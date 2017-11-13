#ifndef M_EPOLL_H
#define M_EPOLL_H

#include<sys/epoll.h>
#include<unistd.h>
#include<assert.h>
#include<stdio.h>
#include<iostream>
class m_Epoll
{
    public:
        m_Epoll(int maxfdNum = 0,bool et = true);
        ~m_Epoll();
        void add(int fd,uint32_t event);
        void del(int fd);
        void mod(int fd,uint32_t event);
        int wait(int maxevents,int timeout);
    protected:

    public:
        struct  epoll_event* _events;
    private:
        int     _maxfdNum;
        int     _epfd;
        bool    _et;
};

#endif // M_EPOLL_H
