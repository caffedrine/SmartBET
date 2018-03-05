//
// Created by curiosul on 04.03.18.
//

#include "Betano.h"
#include "util.h"


bool Betano::fetchBets()
{
    return this->fetchTennis();
}

bool Betano::fetchTennis()
{
    /// Definire linkuri tenis de unde se vor descarca meciurile
    std::vector<std::string> urls_tenis;
    
    std::string baseUrl = "https://ro.betano.com";
    if( !downloadHtml(baseUrl) )
        return false;
    
    std::string tennisElements = this->get_html_node_by_key_value(this->html, "class", "y9 w js-sportlist-data", 1);

    int i = 0;
    while( true )
    {
        std::string tmp = this->get_html_node_by_key_value(tennisElements, "class", "y9 w", i);
        tmp = get_attribute_value_by_key(tmp, "href", "", "href");
        
        if( tmp.empty())
            break;
        
        //std::cout << baseUrl << tmp << std::endl;
        urls_tenis.push_back(baseUrl + tmp);
        i++;
    }
    
    // Now fetch tennis matches from every link
    for(int i = 0, j=0; i < urls_tenis.size(); i++)
    {
        if( !downloadHtml(urls_tenis[i]) )
            continue;
        
        std::string element = this->get_html_node_by_key_value(this->html, "class", "acn", j);
        
        std::string adversari = this->get_html_node_by_key_value(element, "class", "js-event-click a0g");
        
        // to be continued -> get inner text by ta name
        
        std::string timp = util::trim ( this->get_inner_text_by_key_value(element, "class", "z0").c_str() );
        std::string cota_adversar_1 = util::trim( this->get_inner_text_by_key_value(element, "class", "zu js-selection qr", 0).c_str());
        std::string cota_adversar_2 = util::trim( this->get_inner_text_by_key_value(element, "class", "zu js-selection qr", 1).c_str());
        
        std::cout << timp << "\t" << adversari << "\t" << cota_adversar_1 << "\t" << cota_adversar_2;
        
        j++;
        break;
    }
    
    //std::cout << tennisElements << std::endl;
    
    return true;
}

bool Betano::downloadHtml(std::string url)
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