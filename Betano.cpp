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
            if( !downloadHtml( urls_tenis[urlIndex] ))  // one more try
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
            if( timp.empty())
                break;
            
            std::string cota_adversar_1 = util::trim( this->get_inner_text_by_key_value( element, "class", "zu js-selection qr", 0 ).c_str());
            if( cota_adversar_1.empty())
                break;
            
            std::string cota_adversar_2 = util::trim( this->get_inner_text_by_key_value( element, "class", "zu js-selection qr", 1 ).c_str());
            if( cota_adversar_2.empty())
                break;
            
            char *tmp = strdup( adversari.c_str());
            char *p = strtok( tmp, "-" );
            std::string nume1 = p;
            nume1 = util::trim( nume1.c_str());
            p = strtok( NULL, "-" );
            std::string nume2 = p;
            nume2 = util::trim( nume2.c_str());
            
            parseData( nume1, nume2, timp, std::stof( cota_adversar_1 ), std::stof( cota_adversar_2 ));
    
//            MECI_TENIS meci;
//            meci.player1_nume = std::to_string(urlIndex) + " " + nume1;
//            meci.player1_prenume = "";
//            meci.player1_rezultat_final_cota = std::stof(cota_adversar_1);
//
//            meci.player2_nume = nume2;
//            meci.player2_prenume = "";
//            meci.player2_rezultat_final_cota = std::stof(cota_adversar_2);
//
//            this->lista_meciuri_tenis.push_back( meci );
            
            i++;
        }
        
        for(int k = 0; k < 500000000; k++);   // downloading too fast wil cause a fail
        i = 0;
    }
    
    //std::cout << tennisElements << std::endl;
    
    return true;
}

bool Betano::parseData(std::string name1, std::string name2, std::string oraMeci, float cota1, float cota2)
{
    MECI_TENIS meci;
    
    // Replace dot with space
    name1 = util::replaceChar( name1, '.', ' ' );
    name2 = util::replaceChar( name2, '.', ' ' );
    
    auto nameCheck = [ &meci ](std::string name, std::string *_nume, std::string *_prenume)
    {
        std::string nume, prenume;
        
        if( name.find( ' ' ) != std::string::npos )
        {
            if( name.find( '/' ) != std::string::npos )  // it it's team match
            {
                if( name.find( '.' ) != std::string::npos )
                {
                    meci.prenume_prescurtat = true;
                    meci.nume_prescurtat = true;
                }
                meci.meci_dublu = true;
                
                char *tmp = strdup( name.c_str());
                char *p = strtok( tmp, "/" );
                nume = util::trim( p );
                p = strtok( NULL, "/" );
                prenume = util::trim( p );
            }
            else
            {
                meci.meci_dublu = false;
                char *tmp = strdup( name.c_str());
                char *p = strtok( tmp, " " );
                nume = util::trim( p );
                p = strtok( NULL, " " );
                prenume = util::trim( p );
            }
        }
        else if( name.find( '.' ) != std::string::npos )
        {
            meci.meci_dublu = false;
            char *tmp = strdup( name.c_str());
            char *p = strtok( tmp, "." );
            meci.player1_nume = util::trim( p );
            p = strtok( NULL, "." );
            meci.player1_prenume = util::trim( p );
        }
        else if( !name.empty())
        {
            nume = name;
            prenume = "";
        }
        else
        {
            return false;
        }
        
        *_nume = nume;
        *_prenume = prenume;
        
        return true;
    };
    
    if( !nameCheck( name1, &meci.player1_nume, &meci.player1_prenume ))
        return false;
    
    if( !nameCheck( name2, &meci.player2_nume, &meci.player2_prenume ))
        return false;
    
    meci.timp = parseTime(oraMeci);
    meci.player1_rezultat_final_cota = cota1;
    meci.player2_rezultat_final_cota = cota2;
    this->lista_meciuri_tenis.push_back( meci );
    return true;
}

std::tm Betano::parseTime(std::string strTime)
{
// current date/time based on current system
    time_t now = time(0);
    tm gmtm = *gmtime(&now);
    
    //parse format: 10.03 21:00
    if(sscanf(strTime.c_str(), "%d.%d %d:%d", &gmtm.tm_mday, &gmtm.tm_mon - 1, &gmtm.tm_hour, &gmtm.tm_min) != 4)
        setLastError("Can't parse time from string " + strTime);
    
    // To print: asctime(gmtm);
    return gmtm;
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