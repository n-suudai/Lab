#pragma once


#include <string>


namespace WinSockUtil
{
    bool CleanUp();

    std::string GetLastErrorText();

    std::string GetErrorText(int errorCode);
}

