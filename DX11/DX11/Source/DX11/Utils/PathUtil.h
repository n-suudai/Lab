
#pragma once


#include <string>


namespace PathUtil
{
    std::string GetFileName(const std::string& str);

    std::string GetDirectoryName(const std::string& str);

    std::string GetFileNameWithoutExtension(const std::string& str);

    std::string GetExtension(const std::string& str);

    bool SplitPath(
        const std::string& str,
        std::string* pOutDrive,
        std::string* pOutDirectory,
        std::string* pOutFilename,
        std::string* pOutExtension);

    std::string GetFullPath(const std::string& str);

    std::string Combine(const std::string& str1, const std::string& str2);
    std::string Combine(const std::string& str1, const std::string& str2, const std::string& str3);

};

