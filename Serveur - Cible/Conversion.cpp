//
// Fonction provenant de librairie conversion windows
//https://cplusplus.com/reference/string/string/c_str/

#include "Conversion.h"

LPTSTR Conversion::convStringToLPTSTR(std::string s)
{
    //LOOK FOR WAY
    LPSTR cString = _strdup(s.c_str());
    return cString;
}

std::wstring Conversion::convStringToWidestring(const std::string& s)
{
    int len;
    int slength = (int)s.length() + 1;
    len = MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, 0, 0);
    wchar_t* buf = new wchar_t[len];
    MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, buf, len);
    std::wstring r(buf);
    delete[] buf;
    return r;
}