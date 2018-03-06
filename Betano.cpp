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
    if( !downloadHtml( baseUrl ))
        return false;
    
    std::string tennisElements = this->get_html_node_by_key_value( this->html, "class", "y9 w js-sportlist-data", 1 );
    
    int i = 0;
    while( true )
    {
        std::string tmp = this->get_html_node_by_key_value( tennisElements, "class", "y9 w", i );
        tmp = get_attribute_value_by_key( tmp, "href", "", "href" );
        
        if( tmp.empty())
            break;
        
        //std::cout << baseUrl << tmp << std::endl;
        urls_tenis.push_back( baseUrl + tmp );
        i++;
    }
    
    // Now fetch tennis matches from every link
    i = 0;
    for(int urlIndex = 0; urlIndex < urls_tenis.size(); urlIndex++)
    {
        if( !downloadHtml( urls_tenis[urlIndex] ))
            continue;
        
        while( 1 )
        {
            std::string element = this->get_html_node_by_key_value( this->html, "class", "acn", i );
            if( element.empty())
                break;
            
            std::string adversari = this->get_html_node_by_key_value( element, "class", "js-event-click a0g", 0 );
            adversari = this->get_inner_text_by_html_tag( adversari, "span" );  // => nume1 - nume2
            if( adversari.empty())
                break;
            
            std::string timp = this->get_inner_text_by_key_value( element, "class", "z0" );
            if(timp.empty())
                break;
            
            std::string cota_adversar_1 = util::trim( this->get_inner_text_by_key_value( element, "class", "zu js-selection qr", 0 ).c_str());
            if(cota_adversar_1.empty())
                break;
            
            std::string cota_adversar_2 = util::trim( this->get_inner_text_by_key_value( element, "class", "zu js-selection qr", 1 ).c_str());
            if(cota_adversar_2.empty())
                break;
            
            MECI_TENIS meci;
            char *tmp = strdup( adversari.c_str());
            char *p = strtok( tmp, "-" );
            std::string nume1 = p;
            nume1 = util::trim( nume1.c_str());
            p = strtok( NULL, "-" );
            std::string nume2 = p;
            nume2 = util::trim( nume2.c_str());
            
            if(!parseTennisNames(nume1,nume2))
                return false;
            
            //meci.player1_nume = nume1;
            //meci.player1_prenume = "";
            meci.player1_rezultat_final_cota = std::stof(cota_adversar_1);
            
            //meci.player2_nume = nume2;
            //meci.player2_prenume = "";
            meci.player2_rezultat_final_cota = std::stof(cota_adversar_2);
            
            this->lista_meciuri_tenis.push_back( meci );
            i++;
        }
        
        //for(int k = 0; k < 100000000; k++);   // downloading too fast wil cause a fail
        i = 0;
    }
    
    //std::cout << tennisElements << std::endl;
    
    return true;
}

bool Betano::parseTennisNames(std::string name1, std::string name2)
{
    // Replace dot with space
    name1 = util::replaceChar(name1, '.', ' ');
    name2 = util::replaceChar(name2, '.', ' ');
    
    if( name1.find(" ") != std::string::npos )
    {
    
    }
    
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