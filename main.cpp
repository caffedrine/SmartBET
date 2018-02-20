#include <iostream>
#include <string>

using namespace std;

#include "Http.h"
#include "CasaPariurilor.h"

int main()
{
    Http http;
    
    std::string src = http.downloadLink("https://www.casapariurilor.ro/Sport/Tenis/53?date=sve");
    
    if(src.empty())
    {
        cout << "ERROR: " << http.getLastError() << endl;
    }
    else
    {
        cout << src;
    }
    
    CasaPariurilor cp;
    
    return 0;
}