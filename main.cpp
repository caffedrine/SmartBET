#include <iostream>
#include <string>
#include <cstring>

#include "CasaPariurilor.h"
#include "eFortuna.h"

#include <myhtml/api.h>

using namespace std;

char *trim(char *str)
{
    char *end;
    
    // Trim leading space
    while(isspace((unsigned char)*str)) str++;
    
    if(*str == 0)  // All spaces?
        return str;
    
    // Trim trailing space
    end = str + strlen(str) - 1;
    while(end > str && isspace((unsigned char)*end)) end--;
    
    // Write new null terminator
    *(end+1) = 0;
    
    return str;
}

int main()
{
    CasaPariurilor cp;
    if( !cp.fetchBets())
    {
        cout << "Failed to fetch bets!";
        return 1;
    }

//    if(cp.lista_meciuri_tenis.size() > 0)
//    {
//        cout << "Meciuri de tenis CASA PARIURILOR:\n---------------------" << endl;
//        for(int i=0; i < cp.lista_meciuri_tenis.size(); i++)
//        {
//            cout << i << "\t";
//            cout << cp.lista_meciuri_tenis[i].ora_meci << "\t";
//            cout << cp.lista_meciuri_tenis[i].player1_nume << " " << cp.lista_meciuri_tenis[i].player1_prenume << " (" << cp.lista_meciuri_tenis[i].player1_rezultat_final_cota << ")" << "\t";
//            cout << "vs\t";
//            cout << cp.lista_meciuri_tenis[i].player2_nume << " " << cp.lista_meciuri_tenis[i].player2_prenume << " (" << cp.lista_meciuri_tenis[i].player2_rezultat_final_cota << ")";
//            cout << endl;
//        }
//    }
    
    
    
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
    
    // class needed
    char className[128] = "class";
    char attrValue[128] = "event-layout";
    collection = myhtml_get_nodes_by_attribute_value(tree, NULL, NULL, true, className, strlen(className), attrValue, strlen(attrValue), NULL);
    
    cout << "Total found: " << collection->length << "\n";
    
    if( collection->length > 0 )
    {
        for(size_t i = 0; i < collection->length; i++)
        {
            //myhtml_serialization_tree_callback(collection->list[i], serialization_callback, NULL); - display collection
            
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

//            mycore_string_raw_clean_all(&node_raw);
//            myhtml_serialization_tree_buffer(oraMeci, &node_raw);
//
//            cout << node_raw.data << endl;
            mycore_string_raw_clean_all(&node_raw);
            
            const char *txt = myhtml_node_text(oraMeci, NULL);
            
            cout << trim(txt) << endl;
            
            
            // clean raw node
            mycore_string_raw_destroy(&node_raw, false);
        }
    }
    
    // destroy first collection
    myhtml_collection_destroy(collection);
    
    // release resources
    myhtml_tree_destroy(tree);
    myhtml_destroy(myhtml);
    
    return 0;
}