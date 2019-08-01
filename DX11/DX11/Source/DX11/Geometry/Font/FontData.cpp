#include "FontData.hpp"
#include "DX11/Utils/PathUtil.hpp"

#include <cereal/archives/json.hpp>
#include <cereal/archives/binary.hpp>
#include <cereal/archives/portable_binary.hpp>
#include <cereal/archives/xml.hpp>


#include <fstream>
#include <iterator>


namespace {

bool Load_Fnt(const std::string& file, Font::FontData& out);
bool Load_Cereal_JSON(const std::string& file, Font::FontData& out);
bool Load_Cereal_Binary(const std::string& file, Font::FontData& out);
bool Load_Cereal_PortableBinary(const std::string& file, Font::FontData& out);
bool Load_Cereal_XML(const std::string& file, Font::FontData& out);

bool Save_Cereal_JSON(const std::string& file, const Font::FontData& data);
bool Save_Cereal_Binary(const std::string& file, const Font::FontData& data);
bool Save_Cereal_PortableBinary(const std::string& file, const Font::FontData& data);
bool Save_Cereal_XML(const std::string& file, const Font::FontData& data);

} // unnamed namespace


// フォント読み込み
bool Font::LoadFont(const std::string& file, FontData& out)
{
    std::string ext = PathUtil::GetExtension(file);

    if (ext == ".fnt")  { return Load_Fnt(file, out); }
    if (ext == ".json") { return Load_Cereal_JSON(file, out); }
    if (ext == ".bin")  { return Load_Cereal_Binary(file, out); }
    if (ext == ".pbin") { return Load_Cereal_PortableBinary(file, out); }
    if (ext == ".xml")  { return Load_Cereal_XML(file, out); }

    return false;
}

// フォント保存
bool Font::SaveFont(const std::string& file, const FontData& data)
{
    std::string ext = PathUtil::GetExtension(file);

    if (ext == ".json") { return Save_Cereal_JSON(file, data); }
    if (ext == ".bin")  { return Save_Cereal_Binary(file, data); }
    if (ext == ".pbin") { return Save_Cereal_PortableBinary(file, data); }
    if (ext == ".xml")  { return Save_Cereal_XML(file, data); }

    return false;
}


namespace {

std::string GetValueStr(const std::string& line, const std::string& name)
{
    size_t pos = line.find(name);
    size_t size = name.size();

    // 見つからなかった
    if (pos == std::string::npos) { return ""; }

    size_t valueFirst = line.find('=', pos + size);
    valueFirst += 1;
    size_t valueEnd = line.find(' ', valueFirst);

    std::string result = "";

    if (valueFirst < valueEnd)
    {
        result = line.substr(valueFirst, valueEnd - valueFirst);
    }

    // ダブルクォーテーションを取り除く
    for (size_t c = result.find_first_of('\"'); c != std::string::npos; c = result.find_first_of('\"'))
    {
        result.erase(c, 1);
    }

    return result;
}

template<class T>
T ToValue(const std::string& valueStr, T notFound = T())
{
    if (valueStr == "") { return notFound; }

    std::istringstream iss;

    iss.str(valueStr);

    T value;

    iss >> value;

    return value;
}

template<class T>
T GetValue(const std::string& line, const std::string& name, T notFound = T())
{
    return ToValue<T>(GetValueStr(line, name), notFound);
}

Font::FontPadding GetPadding(const std::string& line, const std::string& name)
{
    std::string valueStr = GetValueStr(line, name);

    Font::FontPadding padding;

    size_t first = 0;
    size_t last = 0;

    // up
    last = valueStr.find_first_of(',', first);
    padding.up = ToValue<u16>(valueStr.substr(first, last - first), 0);
    first = last + 1;

    // down
    last = valueStr.find_first_of(',', first);
    padding.down = ToValue<u16>(valueStr.substr(first, last - first), 0);
    first = last + 1;

    // right
    last = valueStr.find_first_of(',', first);
    padding.right = ToValue<u16>(valueStr.substr(first, last - first), 0);
    first = last + 1;

    // left
    padding.left = ToValue<u16>(valueStr.substr(first, valueStr.size() - first), 0);

    return padding;
}


Font::FontSpacing GetSpacing(const std::string& line, const std::string& name)
{
    std::string valueStr = GetValueStr(line, name);

    Font::FontSpacing spacing;

    size_t first = 0;
    size_t last = 0;

    // horizontal
    last = valueStr.find_first_of(',', first);
    spacing.horizontal = ToValue<u16>(valueStr.substr(first, last - first), 0);
    first = last + 1;

    // vertical
    spacing.vertical = ToValue<u16>(valueStr.substr(first, valueStr.size() - first), 0);

    return spacing;
}


bool Load_Fnt(const std::string& file, Font::FontData& out)
{
    std::ifstream ifs(file);

    if (ifs.fail())
    {
        return false;
    }

    ifs.unsetf(std::ios::skipws);   // スペースを読み飛ばさない
    ifs.seekg(std::ios::beg);       // 先頭にセット

    std::string lineStr = "";

    // info
    {
        std::getline(ifs, lineStr);
        out.info.face = GetValue<std::string>(lineStr, "face", "");
        out.info.size = GetValue<u16>(lineStr, "size", 0);
        out.info.bold = GetValue<u8>(lineStr, "bold", 0);
        out.info.italic = GetValue<u8>(lineStr, "italic", 0);
        out.info.charSet = GetValue<std::string>(lineStr, "charSet", "");
        out.info.unicode = GetValue<u8>(lineStr, "unicode", 0);
        out.info.stretchH = GetValue<u16>(lineStr, "streachH", 0);
        out.info.smooth = GetValue<u8>(lineStr, "smooth", 0);
        out.info.aa = GetValue<u8>(lineStr, "aa", 0);
        out.info.padding = GetPadding(lineStr, "padding");
        out.info.spacing = GetSpacing(lineStr, "spacing");
        out.info.outline = GetValue<u16>(lineStr, "outline", 0);
    }

    // common
    {
        lineStr = "";
        std::getline(ifs, lineStr);
        out.common.lineHeight = GetValue<u16>(lineStr, "lineHeight", 0);
        out.common.base = GetValue<u16>(lineStr, "base", 0);
        out.common.scaleW = GetValue<u16>(lineStr, "scaleW", 0);
        out.common.scaleH = GetValue<u16>(lineStr, "scaleH", 0);
        out.common.pages = GetValue<u16>(lineStr, "pages", 0);
        out.common.packed = GetValue<u8>(lineStr, "packed", 0);
        out.common.alphaChnl = GetValue<u8>(lineStr, "alphaChnl", 0);
        out.common.redChnl = GetValue<u8>(lineStr, "redChnl", 0);
        out.common.greenChnl = GetValue<u8>(lineStr, "greenChnl", 0);
        out.common.blueChnl = GetValue<u8>(lineStr, "blueChnl", 0);
    }

    // pages
    {
        Font::FontPage page;
        for (u16 i = 0; i < out.common.pages; ++i)
        {
            lineStr = "";
            std::getline(ifs, lineStr);

            page.id = GetValue<u16>(lineStr, "id", 0);
            page.file = GetValue<std::string>(lineStr, "file", "");
            out.pages.push_back(page);
        }
    }

    // chars
    {
        lineStr = "";
        std::getline(ifs, lineStr);

        u32 charsCount = GetValue<u32>(lineStr, "count");
        u16 id = 0;

        for (u32 i = 0; i < charsCount; ++i)
        {
            lineStr = "";
            std::getline(ifs, lineStr);

            id = GetValue<u16>(lineStr, "id", 0);
            out.chars[id].x = GetValue<u16>(lineStr, "x", 0);
            out.chars[id].y = GetValue<u16>(lineStr, "y", 0);
            out.chars[id].width = GetValue<u16>(lineStr, "width", 0);
            out.chars[id].height = GetValue<u16>(lineStr, "height", 0);
            out.chars[id].xoffset = GetValue<u16>(lineStr, "xoffset", 0);
            out.chars[id].yoffset = GetValue<u16>(lineStr, "yoffset", 0);
            out.chars[id].xadvance = GetValue<u16>(lineStr, "xadvance", 0);
            out.chars[id].page = GetValue<u16>(lineStr, "page", 0);
            out.chars[id].chnl = GetValue<u8>(lineStr, "chnl", 0);
        }
    }

    // kerning
    {
        lineStr = "";
        std::getline(ifs, lineStr);

        u32 kerningsCount = GetValue<u32>(lineStr, "count", 0);

        Font::FontKerning kerning;

        for (u32 i = 0; i < kerningsCount; ++i)
        {
            lineStr = "";
            std::getline(ifs, lineStr);

            kerning.first = GetValue<u16>(lineStr, "first", 0);
            kerning.second = GetValue<u16>(lineStr, "second", 0);
            kerning.amount = GetValue<s16>(lineStr, "amount", 0);

            out.kernings.push_back(kerning);
        }
    }

    return true;
}


#define ROOT_ELEMENT_NAME "FontData"


bool Load_Cereal_JSON(const std::string& file, Font::FontData& out)
{
    std::ifstream ifs(file);

    if (ifs.fail())
    {
        return false;
    }

    cereal::JSONInputArchive i_archive(ifs);

    i_archive(
        cereal::make_nvp(
            ROOT_ELEMENT_NAME, out
        )
    );

    return true;
}

bool Load_Cereal_Binary(const std::string& file, Font::FontData& out)
{
    std::ifstream ifs(file, std::ios::binary);

    if (ifs.fail())
    {
        return false;
    }

    cereal::BinaryInputArchive i_archive(ifs);

    i_archive(
        cereal::make_nvp(
            ROOT_ELEMENT_NAME, out
        )
    );

    return true;
}

bool Load_Cereal_PortableBinary(const std::string& file, Font::FontData& out)
{
    std::ifstream ifs(file, std::ios::binary);

    if (ifs.fail())
    {
        return false;
    }

    cereal::PortableBinaryInputArchive i_archive(ifs);

    i_archive(
        cereal::make_nvp(
            ROOT_ELEMENT_NAME, out
        )
    );

    return true;
}

bool Load_Cereal_XML(const std::string& file, Font::FontData& out)
{
    std::ifstream ifs(file);

    if (ifs.fail())
    {
        return false;
    }

    cereal::XMLInputArchive i_archive(ifs);

    i_archive(
        cereal::make_nvp(
            ROOT_ELEMENT_NAME, out
        )
    );

    return true;
}

bool Save_Cereal_JSON(const std::string& file, const Font::FontData& data)
{
    std::ofstream ofs(file);

    if (ofs.fail())
    {
        return false;
    }

    cereal::JSONOutputArchive o_archive(ofs);

    o_archive(
        cereal::make_nvp(
            ROOT_ELEMENT_NAME, data
        )
    );

    return true;
}

bool Save_Cereal_Binary(const std::string& file, const Font::FontData& data)
{
    std::ofstream ofs(file, std::ios::binary);

    if (ofs.fail())
    {
        return false;
    }

    cereal::BinaryOutputArchive o_archive(ofs);

    o_archive(
        cereal::make_nvp(
            ROOT_ELEMENT_NAME, data
        )
    );

    return true;
}

bool Save_Cereal_PortableBinary(const std::string& file, const Font::FontData& data)
{
    std::ofstream ofs(file, std::ios::binary);

    if (ofs.fail())
    {
        return false;
    }

    cereal::PortableBinaryOutputArchive o_archive(ofs);

    o_archive(
        cereal::make_nvp(
            ROOT_ELEMENT_NAME, data
        )
    );

    return true;
}

bool Save_Cereal_XML(const std::string& file, const Font::FontData& data)
{
    std::ofstream ofs(file);

    if (ofs.fail())
    {
        return false;
    }

    cereal::XMLOutputArchive o_archive(ofs);

    o_archive(
        cereal::make_nvp(
            ROOT_ELEMENT_NAME, data
        )
    );

    return true;
}

} // unnamed namespace

