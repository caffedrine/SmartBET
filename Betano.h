//
// Created by curiosul on 04.03.18.
//

#ifndef SMARTBET_BETANO_H
#define SMARTBET_BETANO_H

#include <string>
#include <iostream>

#include "IFetchBets.h"
#include "Http.h"

class Betano: public IFetchBets
{
public:
    /// Functie ce va parsa meciurile din sursa html
    bool fetchBets() override;
    std::string html;

private:
    
    bool downloadHtml(std::string url);
    bool fetchTennis();
    bool parseTennisNames(std::string name1, std::string name2);
};


#endif //SMARTBET_BETANO_H