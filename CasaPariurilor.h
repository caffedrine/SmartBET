//
// Created by caffedrine on 19.02.18.
//

#ifndef SMARTBET_CASAPARIURILOR_H
#define SMARTBET_CASAPARIURILOR_H

#include <list>

#include "Http.h"
#include "IFetchBets.h"

class CasaPariurilor : public IFetchBets
{
public:
    /// Functie ce va parsa meciurile din sursa html
    bool fetchBets() override;
    
private:

};

#endif //SMARTBET_CASAPARIURILOR_H
