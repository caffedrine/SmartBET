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
    if( !downloadHtml( baseUrl ))
        return false;
    
    int i = 0, j = 0;
    while( true )
    {
        std::string tmp = get_html_node_by_key_value( html, "class", "*mkt mkt_content mkt-*", i );
        if( tmp.empty())
        {
            if(i == 0)
                setLastError( "Can't find elements in passed HTML source '" + tmp + "'" );
            break;
        }
        
        std::string time = get_inner_text_by_key_value( tmp, "class", "time" );
        if( time.empty())
        {
            i++;
            continue;
        }
        
        std::string date = get_inner_text_by_key_value( tmp, "class", "date" );
        std::string player1 = get_inner_text_by_key_value( tmp, "class", "seln-name", 0 );
        std::string player2 = get_inner_text_by_key_value( tmp, "class", "seln-name", 1 );
        std::string player1_cota = get_inner_text_by_key_value( tmp, "class", "price dec", 0 );
        std::string player2_cota = get_inner_text_by_key_value( tmp, "class", "price dec", 1 );
        
        if(player1_cota.empty() || player2_cota.empty() || player1.empty() || player2.empty() || date.empty())
        {
            i++;
            setLastError( "Can't find valid elements in passed HTML source '" + tmp + "'" );
            continue;
        }
    
        date = util::replaceChar(date, '\n', ' ');
        time = util::replaceChar(time, '\n', ' ');
        player1 = util::replaceChar(player1, '\n', ' ');
        player2 = util::replaceChar(player2, '\n', ' ');
        player1_cota = util::replaceChar(player1_cota, '\n', ' ');
        player2_cota = util::replaceChar(player2_cota, '\n', ' ');
        
        //std::cout << j << ". " << date << " " << time << "\t" << player1 << " (" << player1_cota << ")\tvs\t" << player2 << " (" << player2_cota << ")\n"; fflush(stdout);
        parseData( player1, player2, (date + " " + time), std::stof( player1_cota ), std::stof( player2_cota ));
        
        i++;
        j++;
    }
    
    return true;
}

bool eFortuna::parseData(std::string name1, std::string name2, std::string oraMeci, float cota1, float cota2)
{
    MECI_TENIS meci;
    
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
    
    meci.timp = parseTime( oraMeci );
    meci.player1_rezultat_final_cota = cota1;
    meci.player2_rezultat_final_cota = cota2;
    this->lista_meciuri_tenis.push_back( meci );
    return true;
}

std::tm eFortuna::parseTime(std::string strTime)
{
// current date/time based on current system
    time_t now = time( 0 );
    tm gmtm = *gmtime( &now );
    
    //parse format: 10 Mar 22:15
    if( sscanf( strTime.c_str(), "%d %*s %d:%d", &gmtm.tm_mday, &gmtm.tm_hour, &gmtm.tm_min ) != 3 )
        setLastError( "Can't parse time from string '" + strTime + "'" );
    
    // To print: asctime(gmtm);
    return gmtm;
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