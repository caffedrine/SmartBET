#include <iostream>
#include <string>
#include <cstring>
#include <iomanip>

#define DBG true

#include "util.h"
#include "CasaPariurilor.h"
#include "eFortuna.h"
#include "Betano.h"

#include <myhtml/api.h>



using namespace std;

int main()
{
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
            cout << i << ". [" << cp.lista_meciuri_tenis[i].timp.tm_hour << ":" << cp.lista_meciuri_tenis[i].timp.tm_min << "]\t"
                         << cp.lista_meciuri_tenis[i].player1_nume << " " << cp.lista_meciuri_tenis[i].player1_prenume
                         << " (" << cp.lista_meciuri_tenis[i].player1_rezultat_final_cota << ")"
                         << "\tvs.\t"
                         << cp.lista_meciuri_tenis[i].player2_nume << " " << cp.lista_meciuri_tenis[i].player2_prenume
                         << " (" << cp.lista_meciuri_tenis[i].player2_rezultat_final_cota << ")\n";
    
    
        }
        
        if(cp.getErrors().size() > 0)
        {
            cout << "ERRORS LIST:\n";
            for(int i = 0; i< cp.getErrors().size(); i++)
                cout << cp.getErrors()[i] <<  endl;
        }
    }
    //*/
    
    /*
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
        cout << "Meciuri de tenis eFortuna:\n---------------------" << endl;
        for(int i=0; i < ef.lista_meciuri_tenis.size(); i++)
        {
            cout << left << i << ". [" << ef.lista_meciuri_tenis[i].timp.tm_hour << ":" << ef.lista_meciuri_tenis[i].timp.tm_min << "]\t"
                 << setw(18) << ef.lista_meciuri_tenis[i].player1_nume << setw(18) << ef.lista_meciuri_tenis[i].player1_prenume
                 << setw(6)  << ef.lista_meciuri_tenis[i].player1_rezultat_final_cota
                 << setw(6) << "vs."
                 << setw(8) << ef.lista_meciuri_tenis[i].player2_rezultat_final_cota
                 << setw(18) << ef.lista_meciuri_tenis[i].player2_nume << setw(18) << ef.lista_meciuri_tenis[i].player2_prenume << endl;
        }
        
        if(ef.getErrors().size() > 0)
        {
            cout << "ERRORS LIST:\n";
            for(int i = 0; i< ef.getErrors().size(); i++)
                cout << ef.getErrors()[i] <<  endl;
        }
    }
     //*/
    
    /*
    Betano betano;
    if( !betano.fetchBets())
    {
        if( betano.getErrors().size() > 0 )
        {
            cout << "ERRORS LIST:\n";
            for(int i = 0; i < betano.getErrors().size(); i++)
                cout << betano.getErrors()[i] << endl;
        }
        else
        {
            cout << "FAILED, NO ERRORS!";
        }
        return 1;
    }
    
    if( !betano.lista_meciuri_tenis.empty())
    {
        cout << "Meciuri de tenis Unibet:\n---------------------" << endl;
        for(int i = 0; i < betano.lista_meciuri_tenis.size(); i++)
        {
            cout << i << ". " << betano.lista_meciuri_tenis[i].timp.tm_hour << ":" << betano.lista_meciuri_tenis[i].timp.tm_min <<  "\t" << betano.lista_meciuri_tenis[i].player1_nume << (betano.lista_meciuri_tenis[i].meci_dublu?" / ":" ") << betano.lista_meciuri_tenis[i].player1_prenume << "\t" << betano.lista_meciuri_tenis[i].player1_rezultat_final_cota << "\tvs.\t"
                 << betano.lista_meciuri_tenis[i].player2_rezultat_final_cota << "\t" << betano.lista_meciuri_tenis[i].player2_nume << (betano.lista_meciuri_tenis[i].meci_dublu?" / ":" ") << betano.lista_meciuri_tenis[i].player2_prenume << endl;
        }
        
        if( betano.getErrors().size() > 0 )
        {
            cout << "ERRORS LIST:\n";
            for(int i = 0; i < betano.getErrors().size(); i++)
                cout << betano.getErrors()[i] << endl;
        }
    }
    //*/
    
    return 0;
}