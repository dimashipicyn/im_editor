#pragma once

#include "Breakpoint.h"
#include "Cursor.h"
#include "Highlighter.h"
#include "Misc.h"


#include <cstdint>
#include <cstring>
#include <imgui.h>
#include <map>
#include <memory>
#include <string>
#include <string_view>
#include <vector>


class Document
{
public:
    Document()
    {
        m_text.reserve(10);
        m_text.resize(10);
    }

    void Render()
    {
        const int width = CalcNumberOfDigits(m_text.size());
        for (size_t line_index = 0; line_index < m_text.size(); line_index++)
        {
            RenderBreakpoint(line_index);

            const Line& current_line = m_text[line_index];

            const auto number_color = ImGui::ColorConvertU32ToFloat4(m_palette.GetColor(PaletteIndex::LineNumber));
            ImGui::SameLine(0, 0);
            ImGui::TextColored(number_color, "%*zu", width, line_index + 1);

            ImGui::SameLine();
            if (m_cursor.InSameLine(line_index))
            {
                const auto s = std::string_view(current_line).substr(0, m_cursor.GetPosX());
                const auto size = ImGui::CalcTextSize(s.data(), s.data() + s.size());
                const auto offset = ImGui::GetCursorScreenPos();
                m_cursor.Render({offset.x + size.x, offset.y});
            }

            RenderLine(current_line);
        }
    }

    void AddCharacter(ImWchar ch)
    {
        Line& line = m_text[m_cursor.GetPosY()];
        line.insert(m_cursor.GetPosX(), 1, (char)ch);

        MoveCursor(1, 0);
    }

    void RemoveCharacter()
    {
        Line& line = m_text[m_cursor.GetPosY()];
        const int x = m_cursor.GetPosX() - 1;
        if (x < 0)
        {
            const int prev_y = m_cursor.GetPosY() - 1;
            if (IsValidCursorPosition(0, prev_y))
            {
                const int y = m_cursor.GetPosY();
                MoveCursor(INT32_MAX, -1);
                m_text[prev_y].append(std::move(m_text[y]));
                m_text.erase(m_text.begin() + y);
                return;
            }
        }

        if (IsValidCursorPosition(x, m_cursor.GetPosY()))
        {
            line.erase(x, 1);
            MoveCursor(-1, 0);
        }
    }

    void AddNewLine()
    {
        auto s = m_text[m_cursor.GetPosY()].substr(m_cursor.GetPosX());
        m_text[m_cursor.GetPosY()].erase(m_cursor.GetPosX());

        m_text.emplace(m_text.begin() + m_cursor.GetPosY() + 1, std::move(s));
        MoveCursor(INT32_MIN, 1);
    }

    std::tuple<int, int> GetCursorPosition() const
    {
        return {m_cursor.GetPosX(), m_cursor.GetPosY()};
    }

    void SetCursorPosition(int x, int y)
    {
        if (IsValidCursorPosition(x, y))
        {
            m_cursor.SetPos(x, y);
        }
    }

    void MoveCursor(int x, int y)
    {
        auto [xc, yc] = GetClampedPosition(m_cursor.GetPosX() + x, m_cursor.GetPosY() + y);
        SetCursorPosition(xc, yc);
    }

    void SetHighlighter(std::unique_ptr<Highlighter> h)
    {
        m_highlighter = std::move(h);
    }

    const Palette& GetPalette() const
    {
        return m_palette;
    }

    void SetPalette(const Palette& palette)
    {
        m_palette = palette;
    }

    void SetBreakpoint(const Breakpoint breakpoint, bool remove = false)
    {
        if (remove)
        {
            m_breakpoints.erase(breakpoint.line_index);
            return;
        }

        if (breakpoint.line_index < m_text.size())
        {
            m_breakpoints[breakpoint.line_index] = breakpoint;
        }
    }

    bool HasBreakpoint(size_t line_index) const
    {
        return m_breakpoints.count(line_index);
    }

private:
    bool IsValidCursorPosition(int x, int y) const
    {
        if (y >= 0 && y < m_text.size())
        {
            return x >= 0 && x <= m_text[y].size();
        }
        return false;
    }

    std::tuple<int, int> GetClampedPosition(int x, int y)
    {
        int ret_y = std::clamp<int>(y, 0, m_text.size() - 1);
        int ret_x = std::clamp<int>(x, 0, m_text[ret_y].size());
        return {ret_x, ret_y};
    }

    void RenderBreakpoint(size_t line_index)
    {
        const auto offset = ImGui::GetCursorScreenPos();
        const float half_font_size = ImGui::GetFontSize() / 2;

        ImGui::PushID(line_index);
        if (ImGui::InvisibleButton("", ImVec2(half_font_size * 2, half_font_size * 2), ImGuiButtonFlags_MouseButtonLeft))
        {
            SetBreakpoint({line_index, true}, HasBreakpoint(line_index));
        }
        ImGui::PopID();

        if (HasBreakpoint(line_index))
        {
            ImDrawList* draw_list = ImGui::GetWindowDrawList();

            const auto color = m_palette.GetColor(PaletteIndex::Breakpoint);
            draw_list->AddCircleFilled({offset.x + half_font_size, offset.y + half_font_size}, half_font_size, color);
        }
    }

    void RenderLine(const std::string& line)
    {
        auto highlighted_data = m_highlighter ? m_highlighter->Highlight(line) : Highlighter::HighlightData{};
        if (highlighted_data.empty())
        {
            ImGui::Text("%s", line.c_str());
        }
        else
        {
            float spacing = -1;
            size_t prev_end = 0;
            for (auto [start, end, color] : highlighted_data)
            {
                ImGui::SameLine(0, std::exchange(spacing, 0));
                ImGui::TextUnformatted(line.data() + prev_end, line.data() + start);

                ImGui::SameLine(0, spacing);
                ImGui::PushStyleColor(ImGuiCol_Text, ImGui::ColorConvertU32ToFloat4(m_palette.GetColor(color)));
                ImGui::TextUnformatted(line.data() + start, line.data() + end);
                ImGui::PopStyleColor();

                prev_end = end;
            }
        }
    }

    using Line = std::string;
    using Text = std::vector<Line>;

    Text m_text;
    Cursor m_cursor{};
    Palette m_palette{};

    std::unique_ptr<Highlighter> m_highlighter{};

    std::map<int, Breakpoint> m_breakpoints{};
};