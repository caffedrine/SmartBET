//
// Created by caffedrine on 19.02.18.
//

#ifndef SMARTBET_CASAPARIURILOR_H
#define SMARTBET_CASAPARIURILOR_H

#include <string>

#include "util.h"
#include "Http.h"
#include "IFetchBets.h"

class CasaPariurilor : public IFetchBets
{
public:
    /// Functie ce va parsa meciurile din sursa html
    bool fetchBets() override;
    std::string html;
    
private:
    
    std::tm parseTime(std::string time) override;
    int downloadHtml(std::vector<std::string>);
    bool downloadHtml(std::string url);
    bool fetchTennis();
    bool parseData(std::string name1, std::string name2, std::string oraMeci, float cota1, float cota2);
    
};

#endif //SMARTBET_CASAPARIURILOR_H
