#include <iostream>
#include <string>

#include "CasaPariurilor.h"
#include "eFortuna.h"

using namespace std;

int main()
{
    CasaPariurilor cp;
    if(!cp.fetchBets())
    {
        cout << "Failed to fetch bets!";
        return 1;
    }
    
    if(cp.lista_meciuri_tenis.size() > 0)
    {
        cout << "Meciuri de tenis CASA PARIURILOR:\n---------------------" << endl;
        for(int i=0; i < cp.lista_meciuri_tenis.size(); i++)
        {
            cout << i << "\t";
            cout << cp.lista_meciuri_tenis[i].ora_meci << "\t";
            cout << cp.lista_meciuri_tenis[i].player1_nume << " " << cp.lista_meciuri_tenis[i].player1_prenume << " (" << cp.lista_meciuri_tenis[i].player1_rezultat_final_cota << ")" << "\t";
            cout << "vs\t";
            cout << cp.lista_meciuri_tenis[i].player2_nume << " " << cp.lista_meciuri_tenis[i].player2_prenume << " (" << cp.lista_meciuri_tenis[i].player2_rezultat_final_cota << ")";
            cout << endl;
        }
    }
    
    
    
    
    return 0;
}