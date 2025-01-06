#pragma once

#include "Highlighter.h"

#include "imgui.h"
#include <string_view>

class LuaHighlighter : public Highlighter
{
public:
    Highlighter::HighlightData Highlight(std::string_view text) override
    {
        Highlighter::HighlightData result;

        size_t start = 0;
        size_t end = 0;

        PaletteIndex index = PaletteIndex::Default;
        while ((end = text.find(' ', start)) != std::string_view::npos)
        {
            result.emplace_back(HighlightInfo{start, end, index});
            start = end + 1; // Move past the delimiter
            index = (PaletteIndex)(((int)index + 1) % (int)PaletteIndex::Max);
        }

        // Add the last segment
        result.emplace_back(HighlightInfo{start, text.size(), index});
        return result;
    }
};