#include <iostream>
#include <string>
#include <cstring>
#include <iomanip>

#include "util.h"
#include "CasaPariurilor.h"
#include "eFortuna.h"

#include <myhtml/api.h>

using namespace std;

mystatus_t serialization_callback(const char* data, size_t len, void* ctx)
{
    cout << data;
    return MyCORE_STATUS_OK;
}

int main()
{
    CasaPariurilor cp;
    if( !cp.fetchBets())
    {
        cout << "Failed to fetch bets!";
        return 1;
    }

    if(cp.lista_meciuri_tenis.size() > 0)
    {
        cout << "Meciuri de tenis CASA PARIURILOR:\n---------------------" << endl;
        for(int i=0; i < cp.lista_meciuri_tenis.size(); i++)
        {
            cout << left << i << "\t"
                         << setw(18) << cp.lista_meciuri_tenis[i].player1_nume
                         << setw(6)  << cp.lista_meciuri_tenis[i].player1_rezultat_final_cota
                         << setw(6) << "vs."
                         << setw(8) << cp.lista_meciuri_tenis[i].player2_rezultat_final_cota
                         << setw(18) << cp.lista_meciuri_tenis[i].player2_nume << endl;
        }
    }
    
    
    /*
    // basic init
    myhtml_t *myhtml = myhtml_create();
    myhtml_init(myhtml, MyHTML_OPTIONS_DEFAULT, 1, 0);
    
    // init tree
    myhtml_tree_t *tree = myhtml_tree_create();
    myhtml_tree_init(tree, myhtml);
    
    // parse html
    myhtml_parse(tree, MyENCODING_UTF_8, cp.html.c_str(), strlen(cp.html.c_str()));
    
    // parse html
    
    myhtml_collection_t *collection = NULL;
    
    //Get html of class: <div class="psk-sport-group sport-group-type-soccer expanded" data-bettypeid="25">
    
    // Get main table
    // <div class="psk-sport-group sport-group-type-soccer expanded" data-bettypeid="25">
    char className[128] = "data-bettypeid";
    char attrValue[128] = "25";
    
    mycore_string_raw_t node_raw = {0};
    mycore_string_raw_clean_all(&node_raw);
    
    collection = myhtml_get_nodes_by_attribute_value(tree, NULL, NULL, true, className, strlen(className), attrValue, strlen(attrValue), NULL);
    myhtml_serialization_tree_buffer(collection->list[0], &node_raw);
    
    myhtml_tree_init(tree, myhtml);
    myhtml_parse(tree, MyENCODING_UTF_8, node_raw.data, node_raw.length);
    
    
    // ------------------------------------------------------------------------------------------------
    // Get elements div inside main table
    //  <div class="psk-event-list">
    strcpy(className, "class");
    strcpy(attrValue, "psk-event-list");
    
    mycore_string_raw_clean_all(&node_raw);
    
    myhtml_collection_destroy(collection); collection = NULL;
    collection = myhtml_get_nodes_by_attribute_value(tree, NULL, NULL, true, className, strlen(className), attrValue, strlen(attrValue), NULL);
    myhtml_serialization_tree_buffer(collection->list[0], &node_raw);
    
    myhtml_tree_init(tree, myhtml);
    myhtml_parse(tree, MyENCODING_UTF_8, node_raw.data, node_raw.length);
    
    // ------------------------------------------------------------------------------------------------
    // Get elements
    // <div class="event-layout">
    strcpy(className, "class");
    strcpy(attrValue, "event-layout");
    
    myhtml_collection_destroy(collection); collection = NULL;
    collection = myhtml_get_nodes_by_attribute_value(tree, NULL, NULL, true, className, strlen(className), attrValue, strlen(attrValue), NULL);
    
    cout << "Total found: " << collection->length << "\n";
    
    // Manipulate elements
    if( collection->length > 0 )
    {
        for(size_t i = 0; i < collection->length; i++)
        {
            // parse particular node
            mycore_string_raw_t node_raw = {0};
            mycore_string_raw_clean_all(&node_raw);
            
            if( myhtml_serialization_tree_buffer(collection->list[i], &node_raw) != MyCORE_STATUS_OK )
            {
                cout << "Serialization error...\n";
                break;
            }
            
            // parse node
            if( myhtml_tree_init(tree, myhtml) != MyCORE_STATUS_OK )
            {
                cout << "Tree init failed...\n";
                break;
            }
            
            if( myhtml_parse(tree, MyENCODING_UTF_8, node_raw.data, node_raw.length) != MyCORE_STATUS_OK )
            {
                cout << "Parser failed...\n";
                break;
            }
            
            // First player/team node
            strcpy(className, "class");
            strcpy(attrValue, "event-header-team top");
            myhtml_tree_node_t *topTeamNode = myhtml_node_child(
                    (myhtml_get_nodes_by_attribute_value(tree, NULL, NULL, true, className, strlen(className), attrValue, strlen(attrValue), NULL))->list[0]);
            
            // Second player/tean node
            strcpy(attrValue, "event-header-team bottom");
            myhtml_tree_node_t *bottomTeamNode = myhtml_node_child(
                    (myhtml_get_nodes_by_attribute_value(tree, NULL, NULL, true, className, strlen(className), attrValue, strlen(attrValue), NULL))->list[0]);
            
            // Cota player 1
            strcpy(attrValue, "bet-pick bet-pick-3 ");
            myhtml_tree_node_t *cotaTopTeam = myhtml_node_child(
                    (myhtml_get_nodes_by_attribute_value(tree, NULL, NULL, true, className, strlen(className), attrValue, strlen(attrValue), NULL))->list[0]);
            
            // Cota player2
            strcpy(attrValue, "bet-pick bet-pick-3 ");
            myhtml_tree_node_t *cotaBottomTeam = myhtml_node_child(
                    (myhtml_get_nodes_by_attribute_value(tree, NULL, NULL, true, className, strlen(className), attrValue, strlen(attrValue), NULL))->list[1]);
            
            // timp meci
            strcpy(attrValue, "event-header-date-date");
            myhtml_tree_node_t *oraMeci = myhtml_node_child(
                    myhtml_get_nodes_by_attribute_value(tree, NULL, NULL, true, className, strlen(className), attrValue, strlen(attrValue), NULL)->list[0]);
           
            mycore_string_raw_clean_all(&node_raw);
            
            char trimed[128];
            util::trim_const(trimed, sizeof(trimed), myhtml_node_text(oraMeci, NULL));
            
            static int index = 0;
            cout << index << ". " << trimed << endl;
            index++;
            
            // clean raw node
            mycore_string_raw_destroy(&node_raw, false);
        }
    }
    
    // destroy first collection
    myhtml_collection_destroy(collection);
    
    // release resources
    myhtml_tree_destroy(tree);
    myhtml_destroy(myhtml);
    */
    return 0;
}