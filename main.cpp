#include <iostream>
#include <string>

#include "CasaPariurilor.h"
#include "eFortuna.h"

using namespace std;

int main()
{
    CasaPariurilor cp;
    if(!cp.fetchBets())
        cout << "Failed to fetch bets!";
        
    
    return 0;
}