#include "../include/m_Epoll.h"

m_Epoll::m_Epoll(int maxfdNum,bool et):_maxfdNum(maxfdNum),_et(et)
{
    _epfd = epoll_create(_maxfdNum);
    assert(_epfd >= 0);
    _events = new struct epoll_event[_maxfdNum];
}

m_Epoll::~m_Epoll()
{
   delete[] _events;
   int ret = close(_epfd);
   assert(ret >= 0);
}

void m_Epoll::add(int fd,uint32_t event)
{
    struct epoll_event temp_event;
    temp_event.data.fd = fd;
    if(_et)
    {
        temp_event.events = event|EPOLLET;
    }
    else
    {
        temp_event.events = event;
    }
    int ret = epoll_ctl(_epfd,EPOLL_CTL_ADD,fd,&temp_event);
    if(ret == -1)
    {
        perror("epoll_ctl add error");
    }
}

void m_Epoll::del(int fd)
{
    int ret = epoll_ctl(_epfd,EPOLL_CTL_DEL,fd,NULL);
    if(ret == -1)
    {
        perror("epoll_ctl del error");
    }
}

void m_Epoll::mod(int fd,uint32_t event)
{
    struct epoll_event temp_event;
    temp_event.data.fd = fd;
    if(_et)
    {
        temp_event.events = event|EPOLLET;
    }
    else
    {
        temp_event.events = event;
    }
    int ret = epoll_ctl(_epfd,EPOLL_CTL_MOD,fd,&temp_event);
    if(ret == -1)
    {
        perror("epoll_ctl mod error");
    }
}

int m_Epoll::wait(int max_events,int time_out)
{
    return epoll_wait(_epfd,_events,max_events,time_out);
}
