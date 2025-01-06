#pragma once

#include <cstddef>
#include <cstdint>
#include <array>

enum class PaletteIndex : uint8_t
{
	Default,
	Keyword,
	Number,
	String,
	CharLiteral,
	Punctuation,
	Preprocessor,
	Identifier,
	KnownIdentifier,
	PreprocIdentifier,
	Comment,
	MultiLineComment,
	Background,
	Cursor,
	Selection,
	ErrorMarker,
	Breakpoint,
	LineNumber,
	CurrentLineFill,
	CurrentLineFillInactive,
	CurrentLineEdge,
	Max
};

class Palette
{
public:
    using Colors = std::array<uint32_t, (size_t)PaletteIndex::Max>;
    
    Palette()
    {
        m_colors.fill(0xFFFFFFFF);
    }
    
    Palette(Colors colors)
        : m_colors(colors)
    {}

    uint32_t GetColor(PaletteIndex index) const
    {
        return m_colors[(size_t)index];
    }

    void SetColor(PaletteIndex index, uint32_t color)
    {
        m_colors[(size_t)index] = color;
    }

    static Palette Light()
    {
        const static Palette p = {{
            0xff7f7f7f, // None
            0xffff0c06, // Keyword
            0xff008000, // Number
            0xff2020a0, // String
            0xff304070, // Char literal
            0xff000000, // Punctuation
            0xff406060, // Preprocessor
            0xff404040, // Identifier
            0xff606010, // Known identifier
            0xffc040a0, // Preproc identifier
            0xff205020, // Comment (single line)
            0xff405020, // Comment (multi line)
            0xffffffff, // Background
            0xff000000, // Cursor
            0x80600000, // Selection
            0xa00010ff, // ErrorMarker
            0x80f08000, // Breakpoint
            0xff505000, // Line number
            0x40000000, // Current line fill
            0x40808080, // Current line fill (inactive)
            0x40000000, // Current line edge
        }};
        return p;
    }

    static Palette Dark()
    {
        const static Palette p = {{
            0xff7f7f7f, // Default
            0xffd69c56, // Keyword
            0xff00ff00, // Number
            0xff7070e0, // String
            0xff70a0e0, // Char literal
            0xffffffff, // Punctuation
            0xff408080, // Preprocessor
            0xffaaaaaa, // Identifier
            0xff9bc64d, // Known identifier
            0xffc040a0, // Preproc identifier
            0xff206020, // Comment (single line)
            0xff406020, // Comment (multi line)
            0xff101010, // Background
            0xffe0e0e0, // Cursor
            0x80a06020, // Selection
            0x800020ff, // ErrorMarker
            0x40f08000, // Breakpoint
            0xff707000, // Line number
            0x40000000, // Current line fill
            0x40808080, // Current line fill (inactive)
            0x40a0a0a0, // Current line edge
        }};
        return p;
    }

private:
    Colors m_colors{};
};