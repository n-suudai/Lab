#include "Path.hpp"
#include <shlobj.h>


std::string Path::GetFileName(const std::string& str)
{
    std::string fname = "";
    std::string ext = "";

    if (SplitPath(str, nullptr, nullptr, &fname, &ext))
    {
        return fname + ext;
    }
    return "";
}

std::string Path::GetDirectoryName(const std::string& str)
{
    std::string drive = "";
    std::string dir = "";

    if (SplitPath(str, &drive, &dir, nullptr, nullptr))
    {
        return drive + dir;
    }
    return "";
}

std::string Path::GetFileNameWithoutExtension(const std::string& str)
{
    std::string fname = "";

    if (SplitPath(str, nullptr, nullptr, &fname, nullptr))
    {
        return fname;
    }
    return "";
}

std::string Path::GetExtension(const std::string& str)
{
    std::string ext = "";

    if (SplitPath(str, nullptr, nullptr, nullptr, &ext))
    {
        return ext;
    }
    return "";
}

bool Path::SplitPath(
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

std::string Path::Combine(const std::string& str1, const std::string& str2)
{
    std::string path = str1;

    if (!str1.empty() && str1.back() != '\\' && str2.find('.') != 0)
    {
        path += '\\';
    }

    path += str2;

    return path;
}

std::string Path::Combine(const std::string& str1, const std::string& str2, const std::string& str3)
{
    return Combine(Combine(str1, str2), str3);
}

