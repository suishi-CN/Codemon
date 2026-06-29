#pragma once
#include <string>
#include <filesystem>

// UTF-8路径转系统编码，给SFML的loadFromFile用
inline std::string Utf8ToSystemPath(const std::string& utf8) {
    std::wstring wide;
    wide.reserve(utf8.size());
    for (size_t i = 0; i < utf8.size(); ) {
        unsigned char c = (unsigned char)utf8[i];
        wchar_t wc;
        if (c <= 0x7F)                        { wc = (wchar_t)c; i += 1; }
        else if (c >= 0xC2 && c <= 0xDF && i + 1 < utf8.size())
            { wc = (wchar_t)(((c & 0x1F) << 6) | ((unsigned char)utf8[i+1] & 0x3F)); i += 2; }
        else if (c >= 0xE0 && c <= 0xEF && i + 2 < utf8.size())
            { wc = (wchar_t)(((c & 0x0F) << 12) | (((unsigned char)utf8[i+1] & 0x3F) << 6) | ((unsigned char)utf8[i+2] & 0x3F)); i += 3; }
        else { wc = L'?'; i += 1; }
        wide += wc;
    }
    return std::filesystem::path(wide).string();
}

// 直接用UTF-8路径加载纹理
inline bool loadTextureFromUtf8(sf::Texture& tex, const std::string& utf8Path) {
    return tex.loadFromFile(Utf8ToSystemPath(utf8Path));
}
