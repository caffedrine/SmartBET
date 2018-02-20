//
// Created by caffedrine on 19.02.18.
//
#include <iostream>
#include "CasaPariurilor.h"

bool CasaPariurilor::fetchBets()
{
    return this->fetchTenis();
}

bool CasaPariurilor::fetchTenis()
{
    if(downloadHtml(this->urls_tenis) == 0)
        return false;
    
    // Now if source was downloaded, let's parse it!
}

/*
 * @return Numarul de elemente descarcate!
 */
int CasaPariurilor::downloadHtml(std::vector<std::string> urls)
{
    int counter = 0;
    Http http;
    
    this->html = "";
    
    for(int i=0; i < urls.size(); i++)
    {
        std::string tmp = http.downloadLink(urls[i]);
        if(!tmp.empty())
        {
            html += tmp;
            counter++;
        }
        else
        {
            setLastError("Failed to download " + urls[i]);
        }
    }
    
    return counter;
}