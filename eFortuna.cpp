//
// Created by caffedrine on 19.02.18.
//

#include <iostream>
#include "eFortuna.h"
#include "util.h"

bool eFortuna::fetchBets()
{
    return this->fetchTennis();
}

bool eFortuna::fetchTennis()
{
    std::string baseUrl = "https://sport.efortuna.ro/en/pariuri-tenis";
    if(!downloadHtml(baseUrl))
        return false;
    
    int i = 0, j=0;
    while ( true )
    {
        std::string tmp = get_html_node_by_key_value(html, "class", "*mkt mkt_content mkt-*", i);
        if(tmp.empty())
        {
            setLastError("Can;t find elements in passed HTML source");
            break;
        }
            
        std::string time = get_inner_text_by_key_value(tmp, "class", "time");
        if( time.empty())
        {
            i++;
            continue;
        }
        
        std::string date = get_inner_text_by_key_value(tmp, "class", "date");
        std::string player1 = get_inner_text_by_key_value(tmp, "class", "seln-name", 0);
        std::string player2 = get_inner_text_by_key_value(tmp, "class", "seln-name", 1);
        std::string player1_cota = get_inner_text_by_key_value(tmp, "class", "price dec", 0);
        std::string player2_cota = get_inner_text_by_key_value(tmp, "class", "price dec", 1);
        
        std::cout << j << ". " << date << " " << time << "\t" << player1 << " (" << player1_cota << ")\tvs\t" << player2 << " (" << player2_cota << ")\n";

        i++; j++;
    }
    
    return true;
}

bool eFortuna::downloadHtml(std::string url)
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