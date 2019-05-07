//
// Created by caffedrine on 19.02.18.
//
#include <iostream>
#include <boost/algorithm/string.hpp>
#include "CasaPariurilor.h"
#include "util.h"

bool CasaPariurilor::fetchBets()
{
    return this->fetchTennis();
}

bool CasaPariurilor::fetchTennis()
{
    /// Definire linkuri tenis de unde se vor descarca meciurile
    std::vector<std::string> urls_tenis;
    
    // Descarcare linkuri liste meciuri
    std::string baseLink = "https://www.casapariurilor.ro";
    if( !this->downloadHtml( baseLink ))
    {
        setLastError( "Failed to retrieve tennis matches links" );
        return false;
    }
    
    std::string nod;
    nod = get_html_node_by_key_value( this->html, "class", "sports", 0 );
    nod = get_html_node_by_key_value( nod, "class", "inner-list", 4 );                // tenisul este nr 4 in lista de sporturi
    
    int i = 0;
    while( true )
    {
        std::string tmp = get_attribute_value_by_key( nod, "data-toggle", "tooltip-sidebar", "href", i );
        if( tmp.empty())
            break;
        
        //std::cout << baseLink << tmp << std::endl;
        urls_tenis.push_back( baseLink + tmp );
        i++;
    }
    
    if( urls_tenis.empty())
    {
        setLastError( "No tenis links fetched" );
        return false;
    }
    
    // Descarcare linkuri si culegere date
    for(std::string &curr_url: urls_tenis)
    {
        if( !this->downloadHtml( curr_url ))
        {
            setLastError( "Failed to download " + curr_url );
            continue;
        }
        
        //std::cout << curr_url << "\n";
        
        // Get main table which contain the bets 1/2
        std::string tmpHtml = get_html_node_by_key_value( this->html, "data-bettypeid", "25" );
        tmpHtml = get_html_node_by_key_value( tmpHtml, "class", "psk-event-list" );
        
        // loop to get every match from table
        uint32_t event_layout_index = 0;
        while( true )
        {
            std::string currMatch = get_html_node_by_key_value( tmpHtml, "class", "event-layout", event_layout_index);
            if( currMatch.empty())
            {
                break;
            }
            
            std::string player1, player2, cota1, cota2, data;
            
            // Retrieve details about each player
            player1 = get_inner_text_by_key_value( currMatch, "class", "event-header-team top" );
            player1 = util::trim(const_cast<char *>(player1.c_str()));
            player2 = get_inner_text_by_key_value( currMatch, "class", "event-header-team bottom" );
            player2 = util::trim(const_cast<char *>(player2.c_str()));
            cota1 = get_inner_text_by_key_value( currMatch, "class", "*bet-pick-3*", 0 );
            cota1 = util::trim(const_cast<char *>(cota1.c_str()));
            cota2 = get_inner_text_by_key_value( currMatch, "class", "*bet-pick-3*", 1 );
            cota2 = util::trim(const_cast<char *>(cota2.c_str()));
            data = get_inner_text_by_key_value( currMatch, "class", "event-header-date-date" );
            data = util::trim(const_cast<char *>(data.c_str()));
            
            if( cota1.empty() || cota2.empty())
            {
                event_layout_index++;
                continue;
            }
            
            // Cotele au "," in loc de "."
            cota1 = util::replaceChar(cota1, ',', '.');
            cota2 = util::replaceChar(cota2, ',', '.');
            
            //std::cout << data << "\t" << player1 << " (" << cota1 << ") vs " << player2 << " (" << cota2 << ")" << std::endl;
            
            // Parsare date
            parseData(player1, player2, data, std::stof(cota1), std::stof(cota2));
    
            event_layout_index++;
        }
    }
    return true;
}

bool CasaPariurilor::parseData(std::string name1, std::string name2, std::string oraMeci, float cota1, float cota2)
{
    meci_tenis_t meci;
    
    // Replace dot with space
    name1 = util::replaceChar( name1, '.', ' ' );
    name2 = util::replaceChar( name2, '.', ' ' );
    name1 = util::trim(const_cast<char *>(name1.c_str()));
    name2 = util::trim(const_cast<char *>(name2.c_str()));
    
    auto nameCheck = [ &meci ](std::string name, std::string *_nume, std::string *_prenume)
    {
        std::string nume, prenume;
        
        if( name.find( ' ' ) != std::string::npos )
        {
            if( name.find( '/' ) != std::string::npos )  // if it's team match
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
    
    boost::replace_all(meci.player1_nume, ".", "");
    boost::replace_all(meci.player1_prenume, ".", "");
    boost::replace_all(meci.player2_nume, ".", "");
    boost::replace_all(meci.player2_prenume, ".", "");
    boost::replace_all(meci.player1_nume, " ", "");
    boost::replace_all(meci.player1_prenume, " ", "");
    boost::replace_all(meci.player2_nume, " ", "");
    boost::replace_all(meci.player2_prenume, " ", "");
    
    meci.timp = parseTime( oraMeci );
    meci.player1_rezultat_final_cota = cota1;
    meci.player2_rezultat_final_cota = cota2;
    this->lista_meciuri_tenis.push_back( meci );
    return true;
}

std::tm CasaPariurilor::parseTime(std::string strTime)
{
// current date/time based on current system
    time_t now = time( 0 );
    tm gmtm = *gmtime( &now );
    
    //D 20:00 -> ignoram ziua
    if( sscanf( strTime.c_str(), "%*s %d:%d", &gmtm.tm_hour, &gmtm.tm_min ) != 2 )
        setLastError( "Can't parse time from string " + strTime );
    
    // To print: asctime(gmtm);
    return gmtm;
}

/*
 * @return Numarul de elemente descarcate!
 */
int CasaPariurilor::downloadHtml(std::vector<std::string> urls)
{
    int counter = 0;
    Http http;
    
    this->html = "";
    
    for(int i = 0; i < urls.size(); i++)
    {
        std::string tmp = http.downloadLink( urls[i] );
        if( !tmp.empty())
        {
            html += tmp;
            counter++;
        }
        else
        {
            setLastError( "Failed to download " + urls[i] );
        }
    }
    
    return counter;
}

bool CasaPariurilor::downloadHtml(std::string url)
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