#pragma once

#include "Cursor.h"
#include "Misc.h"

#include <cstdint>
#include <imgui.h>
#include <string>
#include <vector>

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
                m_cursor.Render();
            }

            ImGui::Text("%s", current_line.c_str());
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