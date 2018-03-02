#include <iostream>
#include <string>
#include <cstring>
#include <iomanip>

#include "util.h"
#include "CasaPariurilor.h"
#include "eFortuna.h"

#include <myhtml/api.h>

using namespace std;

mystatus_t serialization_callback(const char* data, size_t len, void* ctx)
{
    cout << data;
    return MyCORE_STATUS_OK;
}

int main()
{
    /*
    CasaPariurilor cp;
    if( !cp.fetchBets())
    {
        if(cp.getErrors().size() > 0)
        {
            cout << "ERRORS LIST:\n";
            for(int i = 0; i < cp.getErrors().size(); i++)
                cout << cp.getErrors()[i] << endl;
        }
        else
        {
            cout << "FAILED, NO ERRORS!";
        }
        return 1;
    }

    if(cp.lista_meciuri_tenis.size() > 0)
    {
        cout << "Meciuri de tenis CASA PARIURILOR:\n---------------------" << endl;
        for(int i=0; i < cp.lista_meciuri_tenis.size(); i++)
        {
            cout << left << i << "\t"
                         << setw(18) << cp.lista_meciuri_tenis[i].player1_nume
                         << setw(6)  << cp.lista_meciuri_tenis[i].player1_rezultat_final_cota
                         << setw(6) << "vs."
                         << setw(8) << cp.lista_meciuri_tenis[i].player2_rezultat_final_cota
                         << setw(18) << cp.lista_meciuri_tenis[i].player2_nume << endl;
        }
        
        if(cp.getErrors().size() > 0)
        {
            cout << "ERRORS LIST:\n";
            for(int i = 0; i< cp.getErrors().size(); i++)
                cout << cp.getErrors()[i] <<  endl;
        }
    }
     */
    
    
    eFortuna ef;
    if( !ef.fetchBets())
    {
        if(ef.getErrors().size() > 0)
        {
            cout << "ERRORS LIST:\n";
            for(int i = 0; i < ef.getErrors().size(); i++)
                cout << ef.getErrors()[i] << endl;
        }
        else
        {
            cout << "FAILED, NO ERRORS!";
        }
        return 1;
    }
    
    if(ef.lista_meciuri_tenis.size() > 0)
    {
        cout << "Meciuri de tenis CASA PARIURILOR:\n---------------------" << endl;
        for(int i=0; i < ef.lista_meciuri_tenis.size(); i++)
        {
            cout << left << i << "\t"
                 << setw(18) << ef.lista_meciuri_tenis[i].player1_nume
                 << setw(6)  << ef.lista_meciuri_tenis[i].player1_rezultat_final_cota
                 << setw(6) << "vs."
                 << setw(8) << ef.lista_meciuri_tenis[i].player2_rezultat_final_cota
                 << setw(18) << ef.lista_meciuri_tenis[i].player2_nume << endl;
        }
        
        if(ef.getErrors().size() > 0)
        {
            cout << "ERRORS LIST:\n";
            for(int i = 0; i< ef.getErrors().size(); i++)
                cout << ef.getErrors()[i] <<  endl;
        }
    }
    
    return 0;
}