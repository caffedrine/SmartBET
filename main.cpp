#include <iostream>
#include <string>

using namespace std;

#include "IFetchBets.h"
#include "Http.h"

int main()
{
    Http http;
    std::string src = http.downloadLink("https://www.casapariurilor.ro/Sport/Tenis/53?date=sve");
    
    
    
    return 0;
}