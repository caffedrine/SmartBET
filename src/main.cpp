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

#define strContains(str, substr)    ((bool)(!!(to_lower_copy(string(str)).find( to_lower_copy( string(substr) )) != string::npos)))
#define strEquals(str1, str2)       ((bool)(str1 == str2))

using namespace std;
using std::string;
using boost::algorithm::to_lower_copy;

CasaPariurilor cp;
eFortuna ef;
Betano betano;

void fetchMatches(bool printMatches = false, bool printErrors = false)
{
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
        if(printMatches)
        {
            int i = 0;
            for( IFetchBets::meci_tenis_t &ef_curr : ef.lista_meciuri_tenis )
            {
                printf("%d. %s \tvs.\t %s (%g - %g)\n", i++, (ef_curr.player1_nume + " " + ef_curr.player1_prenume).c_str(),
                       (ef_curr.player2_nume + " " + ef_curr.player2_prenume).c_str(), ef_curr.player1_rezultat_final_cota,
                       ef_curr.player2_rezultat_final_cota);
            }
        }
    }
    if(printErrors && ef.getErrors().size() > 0)
    {
        int i = 0;
        printf("ERRORS LIST:\n");
        for(std::string &err : ef.getErrors())
        {
            printf("%d. %s\n", i++, err.c_str());
        }
    }
    
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
        
        if(printMatches)
        {
            int i = 0;
            for( IFetchBets::meci_tenis_t &cp_curr : cp.lista_meciuri_tenis )
            {
                printf("%d. %s \tvs.\t %s (%g - %g)\n", i++, (cp_curr.player1_nume + " " + cp_curr.player1_prenume).c_str(),
                       (cp_curr.player2_nume + " " + cp_curr.player2_prenume).c_str(), cp_curr.player1_rezultat_final_cota,
                       cp_curr.player2_rezultat_final_cota);
            }
        }
    }
    if(printErrors && cp.getErrors().size() > 0)
    {
        int i = 0;
        printf("ERRORS LIST:\n");
        for(std::string &err : ef.getErrors())
        {
            printf("%d. %s\n", i++, err.c_str());
        }
    }
    
    /*********************************************************/
//    cout << "Fetch matches from betano..."; fflush(stdout);
//
//    if( !ef.fetchBets())
//        printf("FAILED (%lu errors)\n", ef.getErrors().size());
//    else
//        printf("OK (%lu errors)\n", ef.getErrors().size());

}

bool isCommon(IFetchBets::meci_tenis_t m1, IFetchBets::meci_tenis_t m2)
{
    bool retResult = false;
    
    /* Keep only relevant parts of the name to compare with */
    string M1Player1RelevantName = (m1.player1_nume.length() >= m1.player1_prenume.length())?m1.player1_nume:m1.player1_prenume;
    string M1Player2RelevantName = (m1.player2_nume.length() >= m1.player2_prenume.length())?m1.player2_nume:m1.player2_prenume;
    
    string M2Player1RelevantName = (m2.player1_nume.length() >= m2.player1_prenume.length())?m2.player1_nume:m2.player1_prenume;
    string M2Player2RelevantName = (m2.player2_nume.length() >= m2.player2_prenume.length())?m2.player2_nume:m2.player2_prenume;
    
    if( m1.meci_dublu || m2.meci_dublu )  // comparatii meciuri duble
    {
        // E.g. Isner J / Shock J - Bryan B / Bryan M
        retResult =  false;
    }
    else    // comparatii pentru meciuri simple
    {
        if( strContains(M1Player1RelevantName, M2Player1RelevantName) || strContains(M2Player1RelevantName, M1Player1RelevantName) ||
            strContains(M1Player1RelevantName, M2Player2RelevantName) || strContains(M2Player2RelevantName, M1Player1RelevantName))
        {
            retResult = true;
        }

        if( strContains(M1Player2RelevantName, M2Player1RelevantName) || strContains(M2Player1RelevantName, M1Player2RelevantName) ||
            strContains(M1Player2RelevantName, M2Player2RelevantName) || strContains(M2Player2RelevantName, M1Player2RelevantName))
        {
            retResult = true;
        }
    }
    
    if(retResult == true)
    {
        /* O ora toleranta */
        if( m1.timp.tm_hour < m2.timp.tm_hour-1 || m1.timp.tm_hour > m2.timp.tm_hour+1 )
        {
            retResult = false;
        }
    }
    
    return retResult;
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
    fetchMatches(false);
    
    // Find common matches
    int commons_counter = 0;
    cout << "Running check for matches...\n";
    for(IFetchBets::meci_tenis_t &cp_curr : cp.lista_meciuri_tenis)
    {
        for(IFetchBets::meci_tenis_t &ef_curr : ef.lista_meciuri_tenis)
        {
            if( isCommon(cp_curr, ef_curr) )
            {
                printf("%d. %s \tvs.\t %s (%g - %g)\t(%g - %g)\n", commons_counter++, (cp_curr.player1_nume + " " + cp_curr.player1_prenume).c_str(),
                       (cp_curr.player2_nume + " " + cp_curr.player2_prenume).c_str(),
                       cp_curr.player1_rezultat_final_cota, cp_curr.player2_rezultat_final_cota,
                       ef_curr.player1_rezultat_final_cota, ef_curr.player2_rezultat_final_cota);
        
                //isSafeBet( bet, max( cp_curr.player1_rezultat_final_cota, ef_curr.player1_rezultat_final_cota ), min( cp_curr.player2_rezultat_final_cota, ef_curr.player2_rezultat_final_cota ));
                //isSafeBet( bet, min( cp_curr.player1_rezultat_final_cota, ef_curr.player1_rezultat_final_cota ), max( cp_curr.player2_rezultat_final_cota, ef_curr.player2_rezultat_final_cota ));
        
                fflush(stdout);
                goto MATCHED;
                //break;
            }
        }
        MATCHED:
        int cons = 0;
    }
    return 0;
}
