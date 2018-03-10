//
// Created by caffedrine on 19.02.18.
//

#ifndef SMARTBET_IFETCHBETS_H
#define SMARTBET_IFETCHBETS_H

#include <string>
#include <vector>
#include <ctime>
#include <iomanip>  // for get_time
#include <chrono>   // also for time
#include <cstdio>

#include <myhtml/api.h>

class IFetchBets
{
public:
    /// Structure of any tennis match
    typedef struct MECI_TENIS
    {
        std::tm timp;
        
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
    
    /// Return vector errors
    std::vector<std::string> getErrors()
    {
        return this->errors;
    }
    
    /// Return only last error
    std::string getLastError()
    {
        if( this->errors.empty())
            return "";
        
        return this->errors[this->errors.size()];
    }

protected:
    // Function used to parse time
    virtual std::tm parseTime(std::string time) = 0;
    
    void setLastError(const std::string err)
    {
        std::string timeStr = "[" + this->currentDateTime() + "] ";
        
        this->errors.push_back( timeStr + err );
    }
    
    /**
     * Example: get_attribute_value_by_key(html_source, "class", "cls_name", "id")
     * Nodes found are like: <div class="cls_name" id="value_to_be_found"></div> and value provided is "value_to_be_found"
     *
     * @param html Source code
     * @param key The key to look for
     * @param value Value of provided key
     * @param attribute Attribute name (e.g. href, id, class)
     * @param index Element index
     * @return Attribute value as string. NULL is returned when no element with such attributes are found
     */
    std::string get_attribute_value_by_key(std::string html, std::string key, std::string value, std::string attribute, uint32_t index = 0)
    {
        if( html.empty())
            return "";
        
        // basic init
        myhtml_t *myhtml = myhtml_create();
        myhtml_init( myhtml, MyHTML_OPTIONS_DEFAULT, 1, 0 );
        
        // init tree
        myhtml_tree_t *tree = myhtml_tree_create();
        myhtml_tree_init( tree, myhtml );
        
        // parse html
        if( myhtml_parse( tree, MyENCODING_UTF_8, html.c_str(), strlen( html.c_str())) != MyCORE_STATUS_OK )
        {
            setLastError( "Failed to parse tree for: ---" + html + "---" );
            return "";
        }
        
        mycore_string_raw_t node_raw = {0};
        mycore_string_raw_clean_all( &node_raw );
        
        // get elements
        myhtml_collection_t *collection = NULL;
        if( value == "*" || value.empty())
            collection = myhtml_get_nodes_by_attribute_key( tree, NULL, NULL, key.c_str(), strlen( key.c_str()), NULL ); /// BUG?
        else
            collection = myhtml_get_nodes_by_attribute_value( tree, NULL, NULL, true, key.c_str(), strlen( key.c_str()), value.c_str(), strlen( value.c_str()),
                                                              NULL );
        if( collection == NULL || collection->length <= 0 )
            return "";
        
        if( index >= collection->length )
            return "";
        
        myhtml_tree_node_t *node = collection->list[index];
        myhtml_tree_attr_t *gets_attr = myhtml_attribute_by_key( node, attribute.c_str(), strlen( attribute.c_str()));
        
        if( gets_attr == NULL )
            return "";
        
        std::string result;
        if( myhtml_attribute_value( gets_attr, NULL ) != NULL )
            result = std::string( myhtml_attribute_value( gets_attr, NULL ));
        else
            result = "";
        
        // release resources
        myhtml_attribute_free( tree, gets_attr );
        myhtml_node_delete( node );
        myhtml_collection_destroy( collection );
        myhtml_tree_destroy( tree );
        myhtml_destroy( myhtml );
        
        return result;
    }
    
    /**
     * Function provides the html node with  provided index which match key="index" in the HTML source code provided
     *
     * @param html HTML source
     * @param key The HTML attribute name
     * @param value The value of key
     * @param index Index of desired node
     * @return HTML node as string if found. NULL is returned when no such nodes are found or it's the outside index
     */
    std::string get_html_node_by_key_value(std::string html, std::string key, std::string value, uint32_t index = 0)
    {
        if( html.empty())
            return "";
        
        // basic init
        myhtml_t *myhtml = myhtml_create();
        myhtml_init( myhtml, MyHTML_OPTIONS_DEFAULT, 1, 0 );
        
        // init tree
        myhtml_tree_t *tree = myhtml_tree_create();
        myhtml_tree_init( tree, myhtml );
        
        // parse html
        myhtml_parse( tree, MyENCODING_UTF_8, html.c_str(), strlen( html.c_str()));
        
        mycore_string_raw_t node_raw = {0};
        mycore_string_raw_clean_all( &node_raw );
        
        // get elements
        myhtml_collection_t *collection = NULL;
        if( value == "*" || value.empty())
        {
            collection = myhtml_get_nodes_by_attribute_key( tree, NULL, NULL, key.c_str(), strlen( key.c_str()), NULL );
        }
        else if(( value.c_str()[0] == '*' && value.c_str()[strlen( value.c_str()) - 1] == '*' ))
        {
            // Trim the * from begind and end before process
            value = value.substr(0, value.length() - 1);    // trim last *
            value = value.substr(1 , value.length());       // trim first  *
            collection = myhtml_get_nodes_by_attribute_value_contain( tree, NULL, NULL, true, key.c_str(), strlen( key.c_str()), value.c_str(),
                                                                      strlen( value.c_str()), NULL );
        }
        else if( value.c_str()[0] == '*' )
        {
            value = value.substr(1 , value.length());       // trim first  *
            collection = myhtml_get_nodes_by_attribute_value_begin( tree, NULL, NULL, true, key.c_str(), strlen( key.c_str()), value.c_str(), strlen( value.c_str()),
                                                                    NULL );
        }
        else if( value.c_str()[strlen( value.c_str()) - 1] == '*' )
        {
            value = value.substr(0, value.length() - 1);    // trim last *
            collection = myhtml_get_nodes_by_attribute_value_end( tree, NULL, NULL, true, key.c_str(), strlen( key.c_str()), value.c_str(), strlen( value.c_str()),
                                                                  NULL );
        }
        else
        {
            collection = myhtml_get_nodes_by_attribute_value( tree, NULL, NULL, true, key.c_str(), strlen( key.c_str()), value.c_str(), strlen( value.c_str()),
                                                              NULL );
        }
        
        if( collection == NULL || collection->length <= 0 )
            return "";
        
        if( index >= collection->length )
            return "";
        
        myhtml_serialization_tree_buffer( collection->list[index], &node_raw );
        
        std::string result = std::string( node_raw.data );
        
        // release resources
        mycore_string_raw_destroy( &node_raw, false );
        myhtml_collection_destroy( collection );
        myhtml_tree_destroy( tree );
        myhtml_destroy( myhtml );
        
        return result;
    }
    
    /**
     * Example:
     * For key="class", value="test_class" will find these nodes: <div class="test_class">Foo Bar!</div> and will return "Foo Bar!"
     *
     *
     * @param htmlNode HTML node
     * @param key The attribute contained in that node
     * @param value The value of given attribute
     * @param index Function will grab all elements with key="val". Provide the index. number of element found
     * @return INNER TEXT if found. NULL is returned when no such nodes are found or it's the outside index
     */
    std::string get_inner_text_by_key_value(std::string htmlNode, std::string key, std::string value, uint32_t index = 0)
    {
        if( htmlNode.empty())
            return "";
        
        // basic init
        myhtml_t *myhtml = myhtml_create();
        myhtml_init( myhtml, MyHTML_OPTIONS_DEFAULT, 1, 0 );
        
        // init tree
        myhtml_tree_t *tree = myhtml_tree_create();
        myhtml_tree_init( tree, myhtml );
        
        // parse html
        myhtml_parse( tree, MyENCODING_UTF_8, htmlNode.c_str(), strlen( htmlNode.c_str()));
        
        // get elements
        myhtml_collection_t *collection = NULL;
        if( value == "*" || value.empty())
        {
            collection = myhtml_get_nodes_by_attribute_key( tree, NULL, NULL, key.c_str(), strlen( key.c_str()), NULL ); /// BUG?
        }
        else if(( value.c_str()[0] == '*' && value.c_str()[strlen( value.c_str()) - 1] == '*' ))
        {
            // Trim the * from begind and end before process
            value = value.substr(0, value.length() - 1);    // trim last *
            value = value.substr(1 , value.length());       // trim first  *
            collection = myhtml_get_nodes_by_attribute_value_contain( tree, NULL, NULL, true, key.c_str(), strlen( key.c_str()), value.c_str(),
                                                                      strlen( value.c_str()), NULL );
        }
        else if( value.c_str()[0] == '*' )
        {
            value = value.substr(1 , value.length());       // trim first  *
            collection = myhtml_get_nodes_by_attribute_value_begin( tree, NULL, NULL, true, key.c_str(), strlen( key.c_str()), value.c_str(), strlen( value.c_str()),
                                                                    NULL );
        }
        else if( value.c_str()[strlen( value.c_str()) - 1] == '*' )
        {
            value = value.substr(0, value.length() - 1);    // trim last *
            collection = myhtml_get_nodes_by_attribute_value_end( tree, NULL, NULL, true, key.c_str(), strlen( key.c_str()), value.c_str(), strlen( value.c_str()),
                                                                  NULL );
        }
        else
        {
            collection = myhtml_get_nodes_by_attribute_value( tree, NULL, NULL, true, key.c_str(), strlen( key.c_str()), value.c_str(), strlen( value.c_str()),
                                                              NULL );
        }
        
        
        if( collection == NULL || collection->length <= 0 )
            return "";
        
        if( index >= collection->length ) // !!
            return "";
        
        // Get inner text
        myhtml_tree_node_t *text_node = myhtml_node_child( collection->list[index] );
        std::string result = std::string( myhtml_node_text( text_node, NULL ));
        
        // release resources
        myhtml_node_delete( text_node );
        myhtml_collection_destroy( collection );
        myhtml_tree_destroy( tree );
        myhtml_destroy( myhtml );
        
        return result;
    }
    
    /**
     * Example:
     * Function will return the node with the given index which have the given key. Node found if key = "class"</b>: <p class="anything">Foo Bar</p>
     *
     * @param html Input HTML source code
     * @param key HTML key name
     * @param index Function will grab all elements with given key! Select which one you want
     * @return HTML node if found. NULL is returned when no such nodes are found or it's the outside index
     */
    std::string get_html_node_by_key(std::string html, std::string key, uint32_t  index = 0)
    {
        if( html.empty())
            return "";
    
        // basic init
        myhtml_t *myhtml = myhtml_create();
        myhtml_init( myhtml, MyHTML_OPTIONS_DEFAULT, 1, 0 );
    
        // init tree
        myhtml_tree_t *tree = myhtml_tree_create();
        myhtml_tree_init( tree, myhtml );
    
        // parse html
        myhtml_parse( tree, MyENCODING_UTF_8, html.c_str(), strlen( html.c_str()));
    
        mycore_string_raw_t node_raw = {0};
        mycore_string_raw_clean_all( &node_raw );
    
        // get elements
        myhtml_collection_t *collection = NULL;
        if( key == "*" || key.empty())
        {
            return "";
        }
    
        collection = myhtml_get_nodes_by_attribute_key( tree, NULL, NULL, key.c_str(), strlen( key.c_str()), NULL );
    
        if( collection == NULL || collection->length <= 0 )
            return "";
    
        if( index >= collection->length )
            return "";
    
        myhtml_serialization_tree_buffer( collection->list[index], &node_raw );
    
        std::string result = std::string( node_raw.data );
    
        // release resources
        mycore_string_raw_destroy( &node_raw, false );
        myhtml_collection_destroy( collection );
        myhtml_tree_destroy( tree );
        myhtml_destroy( myhtml );
    
        return result;
    }

    /**
     * Example:
     * For html = "<span>test</span>" and tag = "span" will return "test"
     *
     * @param html Input HTML source code
     * @param tag HTML Tag name (e.g. html, span, dic etc
     * @return Inner html (text) of that node. NULL is returned when no such tags are found or it's the outside index
     */
    std::string get_inner_text_by_html_tag(std::string html, std::string tag, uint32_t index = 0)
    {
        if( html.empty() || tag.empty())
            return "";
    
        // basic init
        myhtml_t *myhtml = myhtml_create();
        myhtml_init( myhtml, MyHTML_OPTIONS_DEFAULT, 1, 0 );
    
        // init tree
        myhtml_tree_t *tree = myhtml_tree_create();
        myhtml_tree_init( tree, myhtml );
    
        // parse html
        myhtml_parse( tree, MyENCODING_UTF_8, html.c_str(), strlen( html.c_str()));
        
        // get elements
        myhtml_collection_t *collection = myhtml_get_nodes_by_name(tree, NULL, tag.c_str(), strlen(tag.c_str()), NULL);
    
        if( collection == NULL || collection->length <= 0 )
            return "";
    
        if( index >= collection->length ) // !!
            return "";
    
        // Get inner text
        myhtml_tree_node_t *text_node = myhtml_node_child( collection->list[index] );
        std::string result = std::string( myhtml_node_text( text_node, NULL ));
    
        // release resources
        myhtml_node_delete( text_node );
        myhtml_collection_destroy( collection );
        myhtml_tree_destroy( tree );
        myhtml_destroy( myhtml );
    
        return result;
    }

private:
    std::vector<std::string> errors;
    
    /// Get current date/time, format is YYYY-MM-DD.HH:mm:ss
    const std::string currentDateTime()
    {
        time_t now = time( 0 );
        struct tm tstruct;
        char buf[80];
        tstruct = *localtime( &now );
        // Visit http://en.cppreference.com/w/cpp/chrono/c/strftime
        // for more information about date/time format
        //strftime(buf, sizeof(buf), "%Y-%m-%d.%X", &tstruct);
        strftime( buf, sizeof( buf ), "%X", &tstruct );
        
        return buf;
    }
};


#endif //SMARTBET_IFETCHBETS_H
