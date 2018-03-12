#include <iostream>
#include <string>
#include <cstring>
#include <iomanip>
#include <boost/algorithm/string.hpp>

#include "util.h"
#include "IFetchBets.h"
#include "CasaPariurilor.h"
#include "eFortuna.h"
#include "Betano.h"

using namespace std;

CasaPariurilor cp;
eFortuna ef;
Betano betano;

void fetchMatches()
{
    /*********************************************************/
    cout << "Fetch matches from Casa Pariurilor..."; fflush(stdout);
    
    if( !cp.fetchBets())
        printf("FAILED (%lu errors)\n", cp.getErrors().size());
    else
        printf("OK (%lu errors)\n", cp.getErrors().size());
    
    
    /*********************************************************/
    cout << "Fetch matches from eFortuna..."; fflush(stdout);
    
    if( !ef.fetchBets())
        printf("FAILED (%lu errors)\n", ef.getErrors().size());
    else
        printf("OK (%lu errors)\n", ef.getErrors().size());
    
    /*********************************************************/
//    cout << "Fetch matches from betano..."; fflush(stdout);
//
//    if( !ef.fetchBets())
//        printf("FAILED (%lu errors)\n", ef.getErrors().size());
//    else
//        printf("OK (%lu errors)\n", ef.getErrors().size());
//
}

bool isCommon(IFetchBets::MECI_TENIS m1, IFetchBets::MECI_TENIS m2)
{
    if(m1.timp.tm_hour != m2.timp.tm_hour || m1.timp.tm_min != m2.timp.tm_min)
        return false;
    
    std::string m2player1full = boost::algorithm::to_lower_copy( m2.player1_nume + m2.player1_prenume);
    std::string m2player2full = boost::algorithm::to_lower_copy( m2.player2_nume + m2.player2_prenume);
    
    if()
}

int main()
{
    // Download bets from internet
    fetchMatches();
    
    // Find common matches
    //printf("Nr.\tPlayer1\tPlayer2");
    int i = 0;
    cout << "Running check for matches...\n";
    for( IFetchBets::MECI_TENIS &cp_curr : cp.lista_meciuri_tenis )
    {
        for(IFetchBets::MECI_TENIS &ef_curr : ef.lista_meciuri_tenis)
        {
            if(isCommon(cp_curr, ef_curr))
            {
                printf("%d. %s \tvs.\t %s (%f - %f)\t(%f - %f)",
                       i++,
                       (cp_curr.player1_nume + " " + cp_curr.player1_prenume),
                       (cp_curr.player2_nume + " " + cp_curr.player2_prenume),
                       cp_curr.player1_rezultat_final_cota,
                       cp_curr.player2_rezultat_final_cota,
                       ef_curr.player1_rezultat_final_cota,
                       ef_curr.player2_rezultat_final_cota);
                fflush(stdout);
            }
        }
    }
    return 0;
}
