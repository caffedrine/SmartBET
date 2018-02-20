//
// Created by caffedrine on 19.02.18.
//

#ifndef SMARTBET_IFETCHBETS_H
#define SMARTBET_IFETCHBETS_H

#include <string>
#include <vector>

class IFetchBets
{
public:
    
    /// Structure of any tennis match
    typedef struct MECI_TENIS
    {
        std::string ora_meci;
        
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
    
private:
};


#endif //SMARTBET_IFETCHBETS_H
