#pragma once

#include "Palette.h"

#include <cstddef>
#include <string_view>
#include <vector>

struct HighlightInfo
{
    size_t start_range = 0;
    size_t end_range = 0;

    PaletteIndex color = PaletteIndex::Default;
};

class Highlighter
{
public:
    virtual ~Highlighter() = default;

    using HighlightData = std::vector<HighlightInfo>;
    virtual HighlightData Highlight(const std::string_view text) = 0;
};