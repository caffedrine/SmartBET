//
// Created by caffedrine on 19.02.18.
//

#ifndef SMARTBET_IFETCHBETS_H
#define SMARTBET_IFETCHBETS_H

#include <string>
#include <vector>
#include <ctime>

#include <myhtml/api.h>

class IFetchBets
{
public:
    /// Structure of any tennis match
    typedef struct MECI_TENIS
    {
        std::string ora_meci;
        
        bool meci_dublu = false;
        bool nume_prescurtat = false;
        bool prenume_prescurtat = false;
        
        std::string player1_nume;
        std::string player1_prenume;
        float player1_rezultat_final_cota;
        
        std::string player2_nume;
        std::string player2_prenume;
        float player2_rezultat_final_cota;
    } MECI_TENIS;
    
    /// Lista ce va stoca meciurile de tenis
    std::vector<MECI_TENIS> lista_meciuri_tenis;
    
    /// Function to populate  matches to be implemented by every implementation
    virtual bool fetchBets() = 0;
    
    /// Return last error
    std::vector<std::string> getErrors()
    {
        return this->errors;
    }

protected:
    void setLastError(const std::string err)
    {
        std::string timeStr = "[" + this->currentDateTime() + "] ";
        
        this->errors.push_back(timeStr + err);
    }
    
    // Get inner text of an element by key-value
    std::string get_html_node_by_key_value(std::string html, std::string key, std::string value, uint32_t index = 0)
    {
        // basic init
        myhtml_t *myhtml = myhtml_create();
        myhtml_init(myhtml, MyHTML_OPTIONS_DEFAULT, 1, 0);
        
        // init tree
        myhtml_tree_t *tree = myhtml_tree_create();
        myhtml_tree_init(tree, myhtml);
        
        // parse html
        myhtml_parse(tree, MyENCODING_UTF_8, html.c_str(), strlen(html.c_str()));
        
        mycore_string_raw_t node_raw = {0};
        mycore_string_raw_clean_all(&node_raw);
        
        // get elements
        myhtml_collection_t *collection = collection = myhtml_get_nodes_by_attribute_value(tree, NULL, NULL, true, key.c_str(), strlen(key.c_str()), value.c_str(), strlen(value.c_str()), NULL);
        
        // store desired element
        if(index > collection->length)
            return "";
        
        myhtml_serialization_tree_buffer(collection->list[index], &node_raw);
        
        std::string result = std::string(node_raw.data);
        
        // release resources
        mycore_string_raw_destroy(&node_raw, false);
        myhtml_collection_destroy(collection);
        myhtml_tree_destroy(tree);
        myhtml_destroy(myhtml);
        
        return result;
    }
    
    std::string get_inner_text_by_key_value(std::string htmlNode, std::string key, std::string value, index = 0)
    {
        // basic init
        myhtml_t *myhtml = myhtml_create();
        myhtml_init(myhtml, MyHTML_OPTIONS_DEFAULT, 1, 0);
    
        // init tree
        myhtml_tree_t *tree = myhtml_tree_create();
        myhtml_tree_init(tree, myhtml);
    
        // parse html
        myhtml_parse(tree, MyENCODING_UTF_8, htmlNode.c_str(), strlen(htmlNode.c_str()));
    
        mycore_string_raw_t node_raw = {0};
        mycore_string_raw_clean_all(&node_raw);
    
        // get elements
        myhtml_collection_t *collection = collection = myhtml_get_nodes_by_attribute_value(tree, NULL, NULL, true, key.c_str(), strlen(key.c_str()), value.c_str(), strlen(value.c_str()), NULL);
    
        // store desired element
        if(index > collection->length)
            return "";
        
    
        myhtml_serialization_tree_buffer(collection->list[index], &node_raw);
    
        std::string result = std::string(node_raw.data);
    
        // release resources
        mycore_string_raw_destroy(&node_raw, false);
        myhtml_collection_destroy(collection);
        myhtml_tree_destroy(tree);
        myhtml_destroy(myhtml);
    
        return result;
    }
    

private:
    std::vector<std::string> errors;
    
    // Get current date/time, format is YYYY-MM-DD.HH:mm:ss
    const std::string currentDateTime()
    {
        time_t now = time(0);
        struct tm tstruct;
        char buf[80];
        tstruct = *localtime(&now);
        // Visit http://en.cppreference.com/w/cpp/chrono/c/strftime
        // for more information about date/time format
        //strftime(buf, sizeof(buf), "%Y-%m-%d.%X", &tstruct);
        strftime(buf, sizeof(buf), "%X", &tstruct);
        
        return buf;
    }
};


#endif //SMARTBET_IFETCHBETS_H
