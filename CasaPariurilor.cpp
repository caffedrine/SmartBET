//
// Created by caffedrine on 19.02.18.
//

#include "CasaPariurilor.h"

bool CasaPariurilor::fetchBets()
{

}

bool CasaPariurilor::fetchTenis()
{
    if(downloadHtml(this->urls_tenis))
        return true;
}

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
    }
    
    return counter;
}