//
// Created by curiosul on 04.03.18.
// JAVASCRIPT needs to be enabled to do this job ^_^

#include <iostream>
#include "Unibet.h"

bool Unibet::fetchBets()
{
    return this->fetchTennis();
}

bool Unibet::fetchTennis()
{
    return false;
    
    std::string baseUrl = "https://www.unibet.ro/betting#filter/tennis";
    if( !downloadHtml(baseUrl) )
        return false;
    
    std::cout << this->html << std::endl;
    
    return true;
}

bool Unibet::downloadHtml(std::string url)
{
    Http http;
    
    std::string tmp = http.downloadLink( url );
    if( !tmp.empty())
    {
        html = tmp;
        return true;
    }
    else
    {
        setLastError( "Failed to download " + url );
        return false;
    }
}