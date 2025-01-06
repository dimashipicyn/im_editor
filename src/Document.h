#pragma once

#include "Cursor.h"
#include "Misc.h"

#include <cctype>
#include <cstdint>
#include <cstring>
#include <imgui.h>
#include <string>
#include <string_view>
#include <vector>
#include <random>

class Document
{
public:
    Document()
    {
        m_text.reserve(10);
        m_text.resize(1);
    }

    void Render()
    {
        const int width = CalcNumberOfDigits(m_text.size());
        for (size_t line_index = 0; line_index < m_text.size(); line_index++)
        {
            const Line& current_line = m_text[line_index];

            ImGui::Text("%*zu", width, line_index);

            ImGui::SameLine();
            if (m_cursor.InSameLine(line_index))
            {
                const auto s = std::string_view(current_line).substr(0, m_cursor.GetPosX());
                const auto size = ImGui::CalcTextSize(s.data(), s.data() + s.size());
                const auto offset = ImGui::GetCursorScreenPos();
                m_cursor.Render({offset.x + size.x, offset.y});
            }

            float spacing = -1;
            std::string_view prev;
            for (const auto [str, color] : Highlight(current_line.c_str()))
            {
                ImGui::SameLine(0, std::exchange(spacing, 0));
                if (auto old = std::exchange(prev, str); !old.empty())
                {
                    ImGui::TextUnformatted(old.data() + old.size(), str.data());
                    ImGui::SameLine(0, spacing);
                }

                ImGui::PushStyleColor(ImGuiCol_Text, ImGui::ColorConvertU32ToFloat4(color));
                ImGui::TextUnformatted(str.data(), str.data() + str.size());
                ImGui::PopStyleColor();
            }
        }
    }

    void AddCharacter(ImWchar ch)
    {
        Line& line = m_text[m_cursor.GetPosY()];
        line.insert(m_cursor.GetPosX(), 1, (char)ch);

        MoveCursor(1, 0);
    }

    void AddNewLine()
    {
        m_text.emplace(m_text.begin() + m_cursor.GetPosY() + 1);
        MoveCursor(INT32_MIN, 1);
    }

    void SetCursorAbsPos(int x, int y)
    {
        if (IsValidCursorPosition(x, y))
        {
            m_cursor.SetPos(x, y);
        }
    }

    void MoveCursor(int x, int y)
    {
        auto [xc, yc] = GetClampedPosition(m_cursor.GetPosX() + x, m_cursor.GetPosY() + y);
        SetCursorAbsPos(xc, yc);
    }

    std::vector<std::tuple<std::string_view, ImU32>> Highlight(std::string_view str)
    {
        std::vector<std::tuple<std::string_view, ImU32>> result;

        size_t start = 0;
        size_t end = 0;

        float val = 0;
        while ((end = str.find(' ', start)) != std::string_view::npos)
        {
            result.emplace_back(str.substr(start, end - start), ImGui::ColorConvertFloat4ToU32({val, 1, 1, 1}));
            start = end + 1; // Move past the delimiter
            val += 0.2;
        }

        // Add the last segment
        result.emplace_back(str.substr(start), ImGui::ColorConvertFloat4ToU32({val, 1, 1, 1}));
        return result;
    }

private:
    bool IsValidCursorPosition(int x, int y) const
    {
        if (y >= 0 && y <= m_text.size())
        {
            return x >= 0 && x <= m_text[y].size();
        }
        return false;
    }

    std::tuple<int, int> GetClampedPosition(int x, int y)
    {
        int ret_y = std::clamp<int>(y, 0, m_text.size());
        int ret_x = std::clamp<int>(x, 0, m_text[ret_y].size());
        return {ret_x, ret_y};
    }

    using Line = std::string;
    using Text = std::vector<Line>;

    Text m_text;
    Cursor m_cursor{};
};