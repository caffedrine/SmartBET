//
// Created by caffedrine on 19.02.18.
//

#ifndef SMARTBET_HTTP_H
#define SMARTBET_HTTP_H

#include <string>

#ifdef _WIN32
#include <HttpWebRequests.hpp>
#else
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#endif

class Http
{
public:
    void Http();
    
    void ~Http();

#ifdef _WIN32
    std::string downloadLink(std::string url)
    {
        HttpWebRequest req;
        req.setUrl(url);

        // Proxy support enabled
        req.proxy.enable = false;
        req.proxy.proxy_type = PROXY_SOCKS4;
        req.proxy.ipOrHostname = "xx.xx.xx.xx";
        req.proxy.port = 1080;
        
        req.headers.httpMethod =  GET;
        req.headers.keepCookies = true;
        req.headers.cookieContainer = "";
        req.headers.userAgent = "Safari Browser/macBook Pro A3260";
        req.headers.connection = "close";
    
        // Store response here
        std::string document, header;
    
        if( req.downloadDocument(header, document) )
        {
            return document;
        }
        else
        {
            setLastError( req.getLastError() );
            return NULL;
        }
    }
#else
    std::string downloadLink(std::string url)
    {
        socklen_t sockfd;
        struct sockaddr_in serv_addr;
        struct hostent *server;
        
        uint16_t portno;
        ssize_t n;
        
        char buffer[512];
        
        portno = 80;
        sockfd = socket(AF_INET, SOCK_STREAM, 0);
        if (sockfd < 0)
        {
            setLastError("Can't open linux socket");
            return NULL;
        }
        
        const char *hstname = url.c_str();
        server = gethostbyname( hstname );
        if (server == NULL)
        {
            setLastError("Can't resolve hostname to ip");
            return NULL;
        }
        
        memset(&serv_addr, 0, sizeof(serv_addr));
        serv_addr.sin_family = AF_INET;
        
        memcpy(&serv_addr.sin_addr.s_addr, server->h_addr, sizeof(server->h_addr));
        serv_addr.sin_port = htons(portno);
        
        if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0)
        {
            std::string str = std::string(hstname, strnlen(hstname, 32));
            setLastError("Can't connect to host " + str );
            return NULL;
        }
        
        printf("Please enter the message: ");
        memset(buffer, 0, sizeof(buffer));
        //fgets(buffer,255,stdin);
        
        
        n = write(sockfd,buffer,strlen(buffer));
        if (n < 0)
        {
            error("ERROR writing to socket");
        }
        
        memset(buffer, 0, sizeof(buffer));
        n = read(sockfd,buffer,255);
        if (n < 0)
        {
            error("ERROR reading from socket");
        }
        printf("%s\n",buffer);
        close(sockfd);
        
        return 0;
    }
#endif
    
    std::string getLastError()
    {
        return this->lastError;
    }
    
private:
    std::string lastError;
    
    void setLastError(std::string err)
    {
        this->lastError = err;
    }
    
    std::string getHostFromUrl(const std::string url)
    {
        std::string urlcopy = url;
        
        urlcopy = this->removeSubstrs(urlcopy, "http://");
        urlcopy = removeSubstrs(urlcopy, "https://");
        urlcopy = urlcopy.substr(0, urlcopy.find("/"));
        
        return urlcopy;
    }
    
    std::string removeSubstrs(std::string &input, const std::string pattern)
    {
        std::string source = input;
        std::string::size_type n = pattern.length();
        
        for (std::string::size_type i = source.find(pattern); i != std::string::npos; i = source.find(pattern))
        {
            source.erase(i, n);
        }
        return source;
    }
};

#endif //SMARTBET_HTTP_H
