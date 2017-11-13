EpollServer:EpollServer.cpp src/m_Epoll.cpp include/m_Epoll.h
		g++ -std=c++11 EpollServer.cpp src/m_Epoll.cpp include/m_Epoll.h -o EpollServer -g 

.PHONY:clean
clean:
	-rm -f EpollServer
