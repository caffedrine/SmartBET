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
        
        bool meci dublu = false;
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
