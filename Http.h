//
// Created by caffedrine on 19.02.18.
//

#ifndef SMARTBET_HTTP_H
#define SMARTBET_HTTP_H

#include <string>
#include <sstream>
#include <curl/curl.h>

class Http
{
public:
    Http()
    {
        curl = curl_easy_init();
    }
    
    ~Http()
    {
        curl_easy_cleanup(curl);
    }
    
    std::string downloadLink(std::string url)
    {
        
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_TIMEOUT_MS, 10000);
        curl_easy_setopt(curl, CURLOPT_USERAGENT, "Mozilla/5.0 (X11; Ubuntu; Linux x86_64; rv:58.0) Gecko/20100101 Firefox/58.0");
        
        std::stringstream out;
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, Http::write_data);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &out);
        CURLcode res = curl_easy_perform(curl);
        
        if(res != CURLE_OK)
        {
            setLastError( curl_easy_strerror( CURLE_HTTP_RETURNED_ERROR ) );
            return "";
        }
        return out.str();
    }
    
    std::string getLastError()
    {
        return this->lastError;
    }

private:
    void *curl;
    std::string lastError;
    
    void setLastError(std::string err)
    {
        this->lastError = err;
    }
    
    std::string removeSubstrs(std::string &input, const std::string pattern)
    {
        std::string source = input;
        std::string::size_type n = pattern.length();
        
        for(std::string::size_type i = source.find(pattern); i != std::string::npos; i = source.find(pattern))
        {
            source.erase(i, n);
        }
        return source;
    }
    
    static size_t write_data(void *ptr, size_t size, size_t nmemb, void *stream)
    {
        string data((const char *) ptr, (size_t) size * nmemb);
        *((stringstream *) stream) << data << endl;
        return size * nmemb;
    }
};

#endif //SMARTBET_HTTP_H
