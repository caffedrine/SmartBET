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
        cout << "Meciuri de tenis CASA PARIURILOR:" << endl;
        cout <<
    }
    
    
    
    
    return 0;
}