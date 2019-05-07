//
// Created by caffedrine on 19.02.18.
//

#include <iostream>
#include <algorithm>
#include <boost/algorithm/string.hpp>
#include "eFortuna.h"
#include "util.h"

bool eFortuna::fetchBets()
{
    return this->fetchTennis();
}

bool eFortuna::fetchTennis()
{
    std::vector<std::string> baseUrls =
            {
                "https://efortuna.ro/pariuri-online/tenis-masculin",
                "https://efortuna.ro/pariuri-online/tenis-masculin/madrid-dublu",
                "https://efortuna.ro/pariuri-online/tenis-feminin/madrid-simplu",
                "https://efortuna.ro/pariuri-online/tenis-feminin/madrid-dublu",
                "https://efortuna.ro/pariuri-online/tenis-challenger-masculin/busan-simplu",
                "https://efortuna.ro/pariuri-online/tenis-challenger-masculin/braga-simplu",
                "https://efortuna.ro/pariuri-online/tenis-challenger-masculin/shymkent-simplu_0",
                "https://efortuna.ro/pariuri-online/tenis-challenger-masculin/aix-en-provence-simplu_0",
                "https://efortuna.ro/pariuri-online/tenis-challenger-masculin/roma-simplu_0",
            };
    for(int urlIndex = 0; urlIndex < baseUrls.size(); urlIndex++)
    {
        if( !downloadHtml(baseUrls[urlIndex]) )
        {
            this->setLastError("Failed to fetch: ' " + baseUrls[urlIndex] + "'");
            continue;
        }
    
        int match_index = 0, fetched_matches = 0;
        int tr_index = 0;
        while( true )
        {
            std::string tmp;
            tmp = get_html_node_by_key_value(html, "class", "*events-table", 0);
            /* Remove all whice characters */
            tmp = boost::replace_all_copy(tmp, "\n", "");
            tmp = boost::replace_all_copy(tmp, "\t", "");
            
            tmp = get_html_node_by_tag(tmp, "tbody", 0);
            /* Workaround for not detecting tr elements */
            tmp = boost::replace_all_copy(tmp, "<tr>", "<div custom_key=\"122\">"); // replace all 'x' to 'y'
            tmp = boost::replace_all_copy(tmp, "</td>\n</tr>", "</td></div>");
            
            tmp = get_html_node_by_key_value(tmp, "custom_key", "122", match_index);
            if( tmp.empty() )
            {
                if( match_index == 0 )
                    setLastError("Can't find ANY elements in passed HTML source '" + tmp + "'");
                break;
            }
            else
            {
                tr_index += 2;
            }
        
            std::string time = get_inner_text_by_key_value(tmp, "class", "event-datetime");
            boost::trim(time);
            if( time.empty() )
            {
                match_index++;
                continue;
            }
        
            std::string date = get_inner_text_by_key_value(tmp, "class", "event-datetime");
            boost::trim(date);
            std::string player1 = get_inner_text_by_key_value(tmp, "class", "market-name", 0);
            std::string player2 = get_inner_text_by_key_value(tmp, "class", "market-name", 0);
            std::string player1_cota = get_inner_text_by_key_value(tmp, "class", "odds-value", 0);
            std::string player2_cota = get_inner_text_by_key_value(tmp, "class", "odds-value", 1);
        
            if( player1_cota.empty() || player2_cota.empty() || player1.empty() || player2.empty() || date.empty() )
            {
                match_index++;
                setLastError("Can't find ALL valid elements (bets, time, players etc) in passed HTML source '" + tmp + "'");
                continue;
            }
        
            std::string strTime = time;
            for( int i = 0; i < date.length(); i++ )
            {
                if( strTime[i] == ' ' || (strTime[i] < 0x00 || strTime[i] > 0x7f) )
                    strTime[i] = '_';
            }
        
            date = util::strSplit(strTime, "__", 0);
            time = util::strSplit(strTime, "__", 1);
            player1 = util::strSplit(player1, "-", 0);
            boost::trim(player1);
            player2 = util::strSplit(player2, "-", 1);
            boost::trim(player2);
            player1_cota = util::replaceChar(player1_cota, '\n', ' ');
            player2_cota = util::replaceChar(player2_cota, '\n', ' ');
        
            //std::cout << fetched_matches << ". " << date << " " << time << "\t" << player1 << " (" << player1_cota << ")\tvs\t" << player2 << " (" << player2_cota << ")\n"; fflush(stdout);
            parseData(player1, player2, (date + " " + time), std::stof(player1_cota), std::stof(player2_cota));
        
            match_index++;
            fetched_matches++;
        }
    }
    
    return true;
}

bool eFortuna::parseData(std::string name1, std::string name2, std::string oraMeci, float cota1, float cota2)
{
    meci_tenis_t meci;
    
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
//                char *tmp = strdup( name.c_str());
//                char *p = strtok( tmp, " " );
//                nume = util::trim( p );
//                p = strtok( NULL, " " );
//                prenume = util::trim( p );
                int namesNumber = static_cast<int>(util::split(name, " ").size());
                if(namesNumber >= 2)
                {
                    nume = "";
                    for( int i = 0; i < namesNumber; i++)
                    {
                        if( util::split(name, " ")[i].length() >= nume.length() )
                        {
                            nume = util::split(name, " ")[i];
                            if( i == 0)
                            {
                                prenume = util::strSplit(name, " ", static_cast<uint32_t>(1));
                            }
                            else
                            {
                                prenume = util::strSplit(name, " ", static_cast<uint32_t>(i-1));
                            }
                        }
                    }
                }
                else
                {
                    nume = name;
                    prenume = "";
                }
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
    
    if(!meci.meci_dublu)
    {
        boost::replace_all(meci.player1_nume, " ", "");
        boost::replace_all(meci.player1_prenume, " ", "");
        boost::replace_all(meci.player2_nume, " ", "");
        boost::replace_all(meci.player2_prenume, " ", "");
    
        boost::replace_all(meci.player1_nume, ".", " ");
        boost::replace_all(meci.player1_prenume, ".", " ");
        boost::replace_all(meci.player2_nume, ".", " ");
        boost::replace_all(meci.player2_prenume, ".", " ");
    }
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