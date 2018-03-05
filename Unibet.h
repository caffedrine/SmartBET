//
// Created by curiosul on 04.03.18.
//

#ifndef SMARTBET_UNIBET_H
#define SMARTBET_UNIBET_H

#include <string>

#include "Http.h"
#include "util.h"
#include "IFetchBets.h"

class Unibet : public IFetchBets
{
public:
    /// Functie ce va parsa meciurile din sursa html
    bool fetchBets() override;
    std::string html;

private:
    
    bool downloadHtml(std::string url);
    bool fetchTennis();
};

#endif //SMARTBET_UNIBET_H
