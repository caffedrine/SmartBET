//
// Created by caffedrine on 19.02.18.
//

#include "CasaPariurilor.h"

bool CasaPariurilor::fetchBets()
{
    if(fetchTenis())
        return true;
    else
        return false;
}

bool CasaPariurilor::fetchTenis()
{
    return true;
}

bool CasaPariurilor::downloadHtml(std::vector<std::string> urls)
{
    Http http;
    
    this->html = "";
    for(int i=0; i< urls.size(); i++)
    {
        html += http.downloadLink(urls[i]);
    }
    
    if(this->html.empty())
        return true;
    else
        return false;
}