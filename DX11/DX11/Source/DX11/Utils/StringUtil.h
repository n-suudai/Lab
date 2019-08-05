#pragma once


#include <string>


namespace StringUtil
{
    // ロケールを規定値に設定します。
    void InitLocale();

    // マルチバイト文字列からワイド文字列を返す
    std::wstring Widen(const std::string &src);

    // ワイド文字列からマルチバイト文字列を返す
    std::string Narrow(const std::wstring &src);
}

