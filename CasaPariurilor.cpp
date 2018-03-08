//
// Created by caffedrine on 19.02.18.
//
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
        urls_tenis.push_back(baseLink + tmp);
        i++;
    }
    
    // Descarcare linkuri si culegere date
    for(int k = 0; k < urls_tenis.size(); k++)
    {
        if( !this->downloadHtml( urls_tenis[k] ))
            continue;
        
        // basic init
        myhtml_t *myhtml = myhtml_create();
        myhtml_init( myhtml, MyHTML_OPTIONS_DEFAULT, 1, 0 );
        
        // init tree
        myhtml_tree_t *tree = myhtml_tree_create();
        myhtml_tree_init( tree, myhtml );
        
        // parse html
        myhtml_parse( tree, MyENCODING_UTF_8, this->html.c_str(), strlen( this->html.c_str()));
        
        // parse html
        
        myhtml_collection_t *collection = NULL;
        
        //Get html of class: <div class="psk-sport-group sport-group-type-soccer expanded" data-bettypeid="25">
        
        // Get main table
        // <div class="psk-sport-group sport-group-type-soccer expanded" data-bettypeid="25">
        char className[128] = "data-bettypeid";
        char attrValue[128] = "25";
        
        mycore_string_raw_t node_raw = {0};
        mycore_string_raw_clean_all( &node_raw );
        
        collection = myhtml_get_nodes_by_attribute_value( tree, NULL, NULL, true, className, strlen( className ), attrValue, strlen( attrValue ), NULL );
        if( collection->length == 0 )
        {
            setLastError( "Failed to find data on " + urls_tenis[k] );
            continue;
        }
        
        myhtml_serialization_tree_buffer( collection->list[0], &node_raw );
        
        myhtml_tree_init( tree, myhtml );
        myhtml_parse( tree, MyENCODING_UTF_8, node_raw.data, node_raw.length );
        
        
        // ------------------------------------------------------------------------------------------------
        // Get elements div inside main table
        //  <div class="psk-event-list">
        strcpy( className, "class" );
        strcpy( attrValue, "psk-event-list" );
        
        mycore_string_raw_clean_all( &node_raw );
        
        myhtml_collection_destroy( collection );
        collection = NULL;
        collection = myhtml_get_nodes_by_attribute_value( tree, NULL, NULL, true, className, strlen( className ), attrValue, strlen( attrValue ), NULL );
        
        myhtml_serialization_tree_buffer( collection->list[0], &node_raw );
        myhtml_tree_init( tree, myhtml );
        myhtml_parse( tree, MyENCODING_UTF_8, node_raw.data, node_raw.length );
        
        // ------------------------------------------------------------------------------------------------
        // Get elements
        // <div class="event-layout">
        strcpy( className, "class" );
        strcpy( attrValue, "event-layout" );
        
        myhtml_collection_destroy( collection );
        collection = NULL;
        collection = myhtml_get_nodes_by_attribute_value( tree, NULL, NULL, true, className, strlen( className ), attrValue, strlen( attrValue ), NULL );
        
        // std::cout << "Total found: " << collection->length << "\n";
        
        // Manipulate elements
        if( collection->length > 0 )
        {
            for(size_t i = 0; i < collection->length; i++)
            {
                // parse particular node
                mycore_string_raw_t node_raw = {0};
                mycore_string_raw_clean_all( &node_raw );
                
                if( myhtml_serialization_tree_buffer( collection->list[i], &node_raw ) != MyCORE_STATUS_OK )
                {
                    setLastError( "Serialization error on " + urls_tenis[k] );
                    continue;
                }
                
                // parse node
                if( myhtml_tree_init( tree, myhtml ) != MyCORE_STATUS_OK )
                {
                    setLastError( "Tree init failed on " + urls_tenis[k] );
                    continue;
                }
                
                if( myhtml_parse( tree, MyENCODING_UTF_8, node_raw.data, node_raw.length ) != MyCORE_STATUS_OK )
                {
                    setLastError( "Parser failed on " + urls_tenis[k] );
                    continue;
                }
                
                // First player/team node
                strcpy( className, "class" );
                strcpy( attrValue, "event-header-team top" );
                myhtml_tree_node_t *topTeamNode = myhtml_node_child(
                        ( myhtml_get_nodes_by_attribute_value( tree, NULL, NULL, true, className, strlen( className ), attrValue, strlen( attrValue ),
                                                               NULL ))->list[0] );
                
                // Second player/tean node
                strcpy( attrValue, "event-header-team bottom" );
                myhtml_tree_node_t *bottomTeamNode = myhtml_node_child(
                        ( myhtml_get_nodes_by_attribute_value( tree, NULL, NULL, true, className, strlen( className ), attrValue, strlen( attrValue ),
                                                               NULL ))->list[0] );
                
                // Cota player 1
                strcpy( attrValue, "bet-pick bet-pick-3 " );
                myhtml_tree_node_t *cotaTopTeam = myhtml_node_child(
                        ( myhtml_get_nodes_by_attribute_value( tree, NULL, NULL, true, className, strlen( className ), attrValue, strlen( attrValue ),
                                                               NULL ))->list[0] );
                
                // Cota player2
                strcpy( attrValue, "bet-pick bet-pick-3 " );
                myhtml_tree_node_t *cotaBottomTeam = myhtml_node_child(
                        ( myhtml_get_nodes_by_attribute_value( tree, NULL, NULL, true, className, strlen( className ), attrValue, strlen( attrValue ),
                                                               NULL ))->list[1] );
                
                // timp meci
                strcpy( attrValue, "event-header-date-date" );
                myhtml_tree_node_t *oraMeci = myhtml_node_child(
                        myhtml_get_nodes_by_attribute_value( tree, NULL, NULL, true, className, strlen( className ), attrValue, strlen( attrValue ),
                                                             NULL )->list[0] );
                
                mycore_string_raw_clean_all( &node_raw );
                
                MECI_TENIS meci;
                char buffer[64];
                
                // Setare ora
                util::trim_const( buffer, sizeof( buffer ), myhtml_node_text( oraMeci, NULL ));
                meci.timp = parseTime(std::string( buffer ));
                
                // Setare miza player 1
                memset( buffer, '\0', sizeof( buffer ));
                util::trim_const( buffer, sizeof( buffer ), myhtml_node_text( cotaTopTeam, NULL ));
                meci.player1_rezultat_final_cota = std::stof( util::replaceChar(std::string(buffer), ',', '.') );
                
                // Miza player2
                memset( buffer, '\0', sizeof( buffer ));
                util::trim_const( buffer, sizeof( buffer ), myhtml_node_text( cotaBottomTeam, NULL ));
                meci.player2_rezultat_final_cota = std::stof( util::replaceChar(std::string(buffer), ',', '.') );
                
                // Nume - prenume player1
                memset( buffer, '\0', sizeof( buffer ));
                util::trim_const( buffer, sizeof( buffer ), myhtml_node_text( topTeamNode, NULL ));
                meci.player1_nume = std::string( buffer );
                meci.player1_prenume = "";
                
                // Nume - prenume plaer2
                memset( buffer, '\0', sizeof( buffer ));
                util::trim_const( buffer, sizeof( buffer ), myhtml_node_text( bottomTeamNode, NULL ));
                meci.player2_nume = std::string( buffer );
                meci.player2_prenume = "";
                
                // Adaugare meci in lista globala
                this->lista_meciuri_tenis.push_back( meci );
                
                // clean raw node
                mycore_string_raw_destroy( &node_raw, false );
                myhtml_node_delete( oraMeci );
                myhtml_node_delete( topTeamNode );
                myhtml_node_delete( bottomTeamNode );
                myhtml_node_delete( cotaBottomTeam );
                myhtml_node_delete( cotaBottomTeam );
            }
        }
        
        // destroy first collection
        myhtml_collection_destroy( collection );
        
        // release resources
        myhtml_tree_destroy( tree );
        myhtml_destroy( myhtml );
        
    }
    
    if( lista_meciuri_tenis.size() > 0 )
        return true;
    else
        return false;
}

std::tm CasaPariurilor::parseTime(std::string time)
{
    std::tm tm = {};
    //std::stringstream ss(time);
    //ss >> std::get_time(&tm, "%b.%d %H:%M");
    //auto tp = std::chrono::system_clock::from_time_t(std::mktime(&tm));
    
    return tm;
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