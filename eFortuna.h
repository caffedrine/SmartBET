//
// Created by ccaffedrine on 19.02.18.
//

#ifndef SMARTBET_EFORTUNA_H
#define SMARTBET_EFORTUNA_H

#include <string>

#include "Http.h"
#include "IFetchBets.h"

class eFortuna : public IFetchBets
{
public:
    /// Functie ce va parsa meciurile din sursa html
    bool fetchBets() override;
    std::string html;

private:
    bool downloadHtml(std::string url);
    bool fetchTennis();
    std::tm parseTime(std::string time) override;
    bool parseData(std::string name1, std::string name2, std::string oraMeci, float cota1, float cota2);
};

#endif //SMARTBET_EFORTUNA_H
