#include "StringUtil.h"
#include <locale>


namespace StringUtil
{
    //! ロケールを規定値に設定します。
    void InitLocale()
    {
        setlocale(LC_CTYPE, "");
    }


    // マルチバイト文字列からワイド文字列を返す
    std::wstring Widen(const std::string &src)
    {
        size_t bufSize = src.length() + 1;
        wchar_t *wcs = new wchar_t[bufSize];
        size_t size;
        mbstowcs_s(&size, wcs, bufSize, src.c_str(), bufSize);
        std::wstring dest = wcs;
        delete[] wcs;
        return dest;
    }


    // ワイド文字列からマルチバイト文字列を返す
    std::string Narrow(const std::wstring &src)
    {
        size_t bufSize = src.length() * MB_CUR_MAX + 1;
        char *mbs = new char[bufSize];
        size_t size;
        wcstombs_s(&size, mbs, bufSize, src.c_str(), bufSize);
        std::string dest = mbs;
        delete[] mbs;
        return dest;
    }
}

