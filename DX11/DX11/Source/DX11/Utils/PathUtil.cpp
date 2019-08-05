#include "PathUtil.h"

#include <stdlib.h>
#include <Windows.h>


std::string PathUtil::GetFileName(const std::string& str)
{
    std::string fname = "";
    std::string ext = "";

    if (SplitPath(str, nullptr, nullptr, &fname, &ext))
    {
        return fname + ext;
    }
    return "";
}

std::string PathUtil::GetDirectoryName(const std::string& str)
{
    std::string drive = "";
    std::string dir = "";

    if (SplitPath(str, &drive, &dir, nullptr, nullptr))
    {
        return drive + dir;
    }
    return "";
}

std::string PathUtil::GetFileNameWithoutExtension(const std::string& str)
{
    std::string fname = "";

    if (SplitPath(str, nullptr, nullptr, &fname, nullptr))
    {
        return fname;
    }
    return "";
}

std::string PathUtil::GetExtension(const std::string& str)
{
    std::string ext = "";

    if (SplitPath(str, nullptr, nullptr, nullptr, &ext))
    {
        return ext;
    }
    return "";
}

bool PathUtil::SplitPath(
    const std::string& str,
    std::string* pOutDrive,
    std::string* pOutDirectory,
    std::string* pOutFilename,
    std::string* pOutExtension
)
{
    char drive[MAX_PATH + 1]
        , dir[MAX_PATH + 1]
        , fname[MAX_PATH + 1]
        , ext[MAX_PATH + 1];

    if (EINVAL != _splitpath_s(str.c_str(), drive, dir, fname, ext))
    {
        if (pOutDrive) { (*pOutDrive) = drive; }
        if (pOutDirectory) { (*pOutDirectory) = dir; }
        if (pOutFilename) { (*pOutFilename) = fname; }
        if (pOutExtension) { (*pOutExtension) = ext; }
        return true;
    }
    return false;
}

std::string PathUtil::GetFullPath(const std::string& str)
{
    char fullpath[MAX_PATH + 1];
    char* pResult = nullptr;

    pResult = _fullpath(
        fullpath,
        str.c_str(),
        sizeof(fullpath) / sizeof(fullpath[0])
    );

    if (pResult)
    {
        return fullpath;
    }
    return "";
}

std::string PathUtil::Combine(const std::string& str1, const std::string& str2)
{
    std::string path = str1;

    if (!str1.empty() && str1.back() != '\\' && str2.find('.') != 0)
    {
        path += '\\';
    }

    path += str2;

    return path;
}

std::string PathUtil::Combine(const std::string& str1, const std::string& str2, const std::string& str3)
{
    return Combine(Combine(str1, str2), str3);
}

