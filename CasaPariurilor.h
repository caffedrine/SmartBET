//
// Created by caffedrine on 19.02.18.
//

#ifndef SMARTBET_CASAPARIURILOR_H
#define SMARTBET_CASAPARIURILOR_H

#include <string>

#include "Http.h"
#include "IFetchBets.h"

class CasaPariurilor : public IFetchBets
{
public:
    /// Functie ce va parsa meciurile din sursa html
    bool fetchBets() override;
    std::string html;
private:
    /// Definire linkuri tenis de unde se vor descarca meciurile
    std::vector<std::string> urls_tenis =
            {
                    "https://www.casapariurilor.ro/Sport/Tenis/53?date=sve",
                    "254.ro"
            };
    
    
    int downloadHtml(std::vector<std::string>);
    
    bool fetchTenis();

};

#endif //SMARTBET_CASAPARIURILOR_H
