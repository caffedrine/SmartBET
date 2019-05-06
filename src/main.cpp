#include <iostream>
#include <string>
#include <cstring>
#include <iomanip>
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/replace.hpp>

#include "util.h"
#include "IFetchBets.h"
#include "CasaPariurilor.h"
#include "eFortuna.h"
#include "Betano.h"

using namespace std;

CasaPariurilor cp;
eFortuna ef;
Betano betano;

void fetchMatches(bool print = false)
{
    /*********************************************************/
    cout << "Fetch matches from Casa Pariurilor...";
    fflush( stdout );

    if( !cp.fetchBets())
    {
        printf("FAILED (%lu errors)\n", cp.getErrors().size());
    }
    else
    {
        printf("OK (%lu errors)\n", cp.getErrors().size());
    }

    
    /*********************************************************/
    cout << "Fetch matches from eFortuna...";
    fflush( stdout );
    
    if( !ef.fetchBets())
    {
        printf("FAILED (%lu errors)\n", ef.getErrors().size());
    }
    else
    {
        printf("OK (%lu errors)\n", ef.getErrors().size());
    }
    
    /*********************************************************/
//    cout << "Fetch matches from betano..."; fflush(stdout);
//
//    if( !ef.fetchBets())
//        printf("FAILED (%lu errors)\n", ef.getErrors().size());
//    else
//        printf("OK (%lu errors)\n", ef.getErrors().size());

}

bool isCommon(IFetchBets::MECI_TENIS m1, IFetchBets::MECI_TENIS m2)
{
    if( m1.timp.tm_hour != m2.timp.tm_hour || m1.timp.tm_min != m2.timp.tm_min )
        return false;
    std::string m1player1full = boost::algorithm::to_lower_copy( m1.player1_nume + m1.player1_prenume );
    std::string m1player2full = boost::algorithm::to_lower_copy( m1.player2_nume + m1.player2_prenume );
    
    std::string m2player1full = boost::algorithm::to_lower_copy( m2.player1_nume + m2.player1_prenume );
    std::string m2player2full = boost::algorithm::to_lower_copy( m2.player2_nume + m2.player2_prenume );
    
    if( m1.meci_dublu && m2.meci_dublu )  // comparatii meciuri duble
    {
        // E.g. Isner J / Shock J - Bryan B / Bryan M
        
    }
    else    // comparatii pentru meciuri simple
    {
        if( !( m2player1full.find( boost::algorithm::to_lower_copy( m1.player1_nume )) != string::npos || m2player1full.find(
                boost::algorithm::to_lower_copy( m2.player1_prenume )) != string::npos ))
            if( !( m1player1full.find( boost::algorithm::to_lower_copy( m2.player1_nume )) != string::npos || m1player1full.find(
                    boost::algorithm::to_lower_copy( m2.player1_prenume )) != string::npos ))
                return false;
        
        if( !( m2player2full.find( boost::algorithm::to_lower_copy( m1.player2_nume )) != string::npos || m2player2full.find(
                boost::algorithm::to_lower_copy( m1.player2_prenume )) != string::npos ))
            if( !( m1player2full.find( boost::algorithm::to_lower_copy( m2.player2_nume )) != string::npos || m1player2full.find(
                    boost::algorithm::to_lower_copy( m2.player2_prenume )) != string::npos ))
                return false;
    }
    
    return true;
}

bool isSafeBet(float bet, float max1, float min2)
{
    // Calcul suma proportionala pentru fiecare cota
    float s1 = ( max1 * 1000 ) / ( max1 + min2 );
    float s2 = ( min2 * 1000 ) / ( max1 + min2 );
    
    // Calcul castiguri potentiale
    float cp1 = s1 * min2;
    float cp2 = s2 * max1;
    
    // Verificam daca castigul pentru fiecare dintre cote este > decat suma pariata
    if( cp1 > bet && cp2 > bet )
    {
        cout << "Suma totala pariata: " << bet << endl;
        cout << "Pariaza " << s1 << "RON pe cota " << min2 << " pentru castig de " << ( cp1 ) << endl;
        cout << "Pariaza " << s2 << "RON pe cota " << max1 << " pentru castig de " << ( cp2 ) << endl;
        return true;
    }
    return false;
}

int main()
{
    float bet = 1000;   // bet de 1000 RON by default
    
    // Download bets from internet
    fetchMatches(true);

    // Find common matches
    int i = 0;
    cout << "Running check for matches...\n";
    for(IFetchBets::MECI_TENIS &cp_curr : cp.lista_meciuri_tenis)
    {
        for(IFetchBets::MECI_TENIS &ef_curr : ef.lista_meciuri_tenis)
        {
            if( isCommon( cp_curr, ef_curr ))
            {
                printf( "%d. %s \tvs.\t %s (%g - %g)\t(%g - %g)\n", i++, ( cp_curr.player1_nume + " " + cp_curr.player1_prenume ).c_str(),
                        ( cp_curr.player2_nume + " " + cp_curr.player2_prenume ).c_str(), cp_curr.player1_rezultat_final_cota, cp_curr.player2_rezultat_final_cota,
                        ef_curr.player1_rezultat_final_cota, ef_curr.player2_rezultat_final_cota );

                //isSafeBet( bet, max( cp_curr.player1_rezultat_final_cota, ef_curr.player1_rezultat_final_cota ), min( cp_curr.player2_rezultat_final_cota, ef_curr.player2_rezultat_final_cota ));
                //isSafeBet( bet, min( cp_curr.player1_rezultat_final_cota, ef_curr.player1_rezultat_final_cota ), max( cp_curr.player2_rezultat_final_cota, ef_curr.player2_rezultat_final_cota ));

                fflush( stdout );

                //break;
            }
        }
    }
    
    return 0;
}
