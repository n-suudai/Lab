
#pragma once


#include <string>


class Path
{
public:
    static std::string GetFileName(const std::string& str);

    static std::string GetDirectoryName(const std::string& str);

    static std::string GetFileNameWithoutExtension(const std::string& str);

    static std::string GetExtension(const std::string& str);

    static bool SplitPath(
        const std::string& str,
        std::string* pOutDrive,
        std::string* pOutDirectory,
        std::string* pOutFilename,
        std::string* pOutExtension);

    static std::string Combine(const std::string& str1, const std::string& str2);
    static std::string Combine(const std::string& str1, const std::string& str2, const std::string& str3);

};

