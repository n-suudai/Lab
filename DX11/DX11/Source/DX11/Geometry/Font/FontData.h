
#pragma once

#include "DX11/External/cereal/cereal_Prerequisites.h"


namespace Font {


    enum ChannelFlag
    {
        Blue = (1 << 0),
        Green = (1 << 1),
        Red = (1 << 2),
        Alpha = (1 << 3),
        All = Blue | Green | Red | Alpha,
    };


    enum ChannelUsage
    {
        Glyph = 0,
        Outline = 1,
        GlyphAndOutline = 2,
        Zero = 3,
        One = 4,
    };


    struct FontPadding
    {
        u16 up;
        u16 down;
        u16 right;
        u16 left;

        inline FontPadding()
        {
            up = 0;
            down = 0;
            right = 0;
            left = 0;
        }

        template<class Archive>
        inline void serialize(Archive& archive)
        {
            archive(
                CEREAL_NVP(up),
                CEREAL_NVP(down),
                CEREAL_NVP(right),
                CEREAL_NVP(left)
            );
        }
    };



    struct FontSpacing
    {
        u16 horizontal;
        u16 vertical;

        inline FontSpacing()
        {
            horizontal = 0;
            vertical = 0;
        }

        template<class Archive>
        inline void serialize(Archive& archive)
        {
            archive(
                CEREAL_NVP(horizontal),
                CEREAL_NVP(vertical)
            );
        }
    };


    // This tag holds information on how the font was generated.
    struct FontInfo
    {
        std::string face;       // This is the name of the true type font.
        u16         size;       // The size of the true type font.
        u8          bold;       // The font is bold.
        u8          italic;     // The font is italic.
        std::string charSet;    // The name of the OEM charset used (when not unicode).
        u8          unicode;    // Set to 1 if it is the unicode charset.
        u16         stretchH;   // The font height stretch in percentage. 100% means no stretch.
        u8          smooth;     // Set to 1 if smoothing was turned on.
        u8          aa;         // The supersampling level used. 1 means no supersampling was used.
        FontPadding padding;    // The padding for each character (up, right, down, left).
        FontSpacing spacing;    // The spacing for each character (horizontal, vertical).
        u16         outline;    // The outline thickness for the characters.

        inline FontInfo()
        {
            face = "";
            size = 0;
            bold = 0;
            italic = 0;
            charSet = "";
            unicode = 0;
            stretchH = 0;
            smooth = 0;
            aa = 0;
            outline = 0;
        }

        template<class Archive>
        inline void serialize(Archive& archive)
        {
            archive(
                CEREAL_NVP(face),
                CEREAL_NVP(size),
                CEREAL_NVP(bold),
                CEREAL_NVP(italic),
                CEREAL_NVP(charSet),
                CEREAL_NVP(unicode),
                CEREAL_NVP(stretchH),
                CEREAL_NVP(smooth),
                CEREAL_NVP(aa),
                CEREAL_NVP(padding),
                CEREAL_NVP(spacing),
                CEREAL_NVP(outline)
            );
        }
    };


    // This tag holds information common to all characters.
    struct FontCommon
    {
        u16 lineHeight;    // This is the distance in pixels between each line of text.
        u16 base;          // The number of pixels from the absolute top of the line to the base of the characters.
        u16 scaleW;        // The width of the texture, normally used to scale the x pos of the character image.
        u16 scaleH;        // The height of the texture, normally used to scale the y pos of the character image.
        u16 pages;         // The number of texture pages included in the font.
        u8  packed;        // Set to 1 if the monochrome characters have been packed into each of the texture channels. In this case alphaChnl describes what is stored in each channel.
        u8  alphaChnl;     // Set to 0 if the channel holds the glyph data, 1 if it holds the outline, 2 if it holds the glyph and the outline, 3 if its set to zero, and 4 if its set to one.
        u8  redChnl;       // Set to 0 if the channel holds the glyph data, 1 if it holds the outline, 2 if it holds the glyph and the outline, 3 if its set to zero, and 4 if its set to one.
        u8  greenChnl;     // Set to 0 if the channel holds the glyph data, 1 if it holds the outline, 2 if it holds the glyph and the outline, 3 if its set to zero, and 4 if its set to one.
        u8  blueChnl;      // Set to 0 if the channel holds the glyph data, 1 if it holds the outline, 2 if it holds the glyph and the outline, 3 if its set to zero, and 4 if its set to one.

        inline FontCommon()
        {
            lineHeight = 0;
            base = 0;
            scaleW = 0;
            scaleH = 0;
            pages = 0;
            packed = 0;
            alphaChnl = 0;
            redChnl = 0;
            greenChnl = 0;
            blueChnl = 0;
        }

        template<class Archive>
        inline void serialize(Archive& archive)
        {
            archive(
                CEREAL_NVP(lineHeight),
                CEREAL_NVP(base),
                CEREAL_NVP(scaleW),
                CEREAL_NVP(scaleH),
                CEREAL_NVP(pages),
                CEREAL_NVP(packed),
                CEREAL_NVP(alphaChnl),
                CEREAL_NVP(redChnl),
                CEREAL_NVP(greenChnl),
                CEREAL_NVP(blueChnl)
            );
        }
    };


    // This tag gives the name of a texture file. There is one for each page in the font.
    struct FontPage
    {
        u16         id;     // The page id.
        std::string file;   // The texture file name.

        inline FontPage()
        {
            id = 0;
            file = "";
        }

        template<class Archive>
        inline void serialize(Archive& archive)
        {
            archive(
                CEREAL_NVP(id),
                CEREAL_NVP(file)
            );
        }
    };


    // This tag describes on character in the font. There is one for each included character in the font.
    struct FontCharacter
    {
        u16 x;         // The left position of the character image in the texture.
        u16 y;         // The top position of the character image in the texture.
        u16 width;     // The width of the character image in the texture.
        u16 height;    // The height of the character image in the texture.
        s16 xoffset;   // How much the current position should be offset when copying the image from the texture to the screen.
        s16 yoffset;   // How much the current position should be offset when copying the image from the texture to the screen.
        u16 xadvance;  // How much the current position should be advanced after drawing the character.
        u16 page;      // The texture page where the character image is found.
        u8  chnl;      // The texture channel where the character image is found (1 = blue, 2 = green, 4 = red, 8 = alpha, 15 = all channels).

        inline FontCharacter()
        {
            x = 0;
            y = 0;
            width = 0;
            height = 0;
            xoffset = 0;
            yoffset = 0;
            xadvance = 0;
            page = 0;
            chnl = 0;
        }

        template<class Archive>
        inline void serialize(Archive& archive)
        {
            archive(
                CEREAL_NVP(x),
                CEREAL_NVP(y),
                CEREAL_NVP(width),
                CEREAL_NVP(height),
                CEREAL_NVP(xoffset),
                CEREAL_NVP(yoffset),
                CEREAL_NVP(xadvance),
                CEREAL_NVP(page),
                CEREAL_NVP(chnl)
            );
        }
    };


    // The kerning information is used to adjust the distance between certain characters,
    // e.g. some characters should be placed closer to each other than others.
    struct FontKerning
    {
        u16 first;     // The first character id.
        u16 second;    // The second character id.
        s16 amount;    // How much the x position should be adjusted when drawing the second character immediately following the first.

        inline FontKerning()
        {
            first = 0;
            second = 0;
            amount = 0;
        }

        template<class Archive>
        inline void serialize(Archive& archive)
        {
            archive(
                CEREAL_NVP(first),
                CEREAL_NVP(second),
                CEREAL_NVP(amount)
            );
        }
    };


    struct FontData
    {
        FontInfo                        info;
        FontCommon                      common;
        std::vector<FontPage>           pages;
        std::map<u16, FontCharacter>    chars;
        std::vector<FontKerning>        kernings;

        inline FontData()
        {
            chars.clear();
            pages.clear();
            kernings.clear();
        }

        template<class Archive>
        inline void serialize(Archive& archive)
        {
            archive(
                CEREAL_NVP(info),
                CEREAL_NVP(common),
                CEREAL_NVP(pages),
                CEREAL_NVP(chars),
                CEREAL_NVP(kernings)
            );
        }
    };


    // フォント読み込み
    bool LoadFont(const std::string& file, FontData& out);

    // フォント保存
    bool SaveFont(const std::string& file, const FontData& data);

}