#pragma once

#include "Cursor.h"
#include "Misc.h"

#include <cstdio>
#include <imgui.h>
#include <string>
#include <vector>

class Editor
{
public:
    Editor()
    {
        m_text.reserve(10);
        m_text.resize(1);
    }

    void Input()
    {
        ImGuiIO& io = ImGui::GetIO();
        if (ImGui::IsWindowFocused())
        {
            for (const ImWchar c : io.InputQueueCharacters)
            {
                AddCharacter(c);
            }

            if (ImGui::IsKeyPressed(ImGuiKey_Enter))
            {
                AddNewLine();
            }

            if (ImGui::IsKeyPressed(ImGuiKey_LeftArrow))
            {
                const Line& line = m_text[m_cursor.GetPosY()];
                m_cursor.Move(-1, 0, line.size(), m_text.size());
            }

            if (ImGui::IsKeyPressed(ImGuiKey_RightArrow))
            {
                const Line& line = m_text[m_cursor.GetPosY()];
                m_cursor.Move(1, 0, line.size(), m_text.size());
            }

            if (ImGui::IsKeyPressed(ImGuiKey_UpArrow))
            {
                const int next_index = std::clamp<int>(m_cursor.GetPosY() - 1, 0, m_text.size());

                const Line& line = m_text[next_index];
                m_cursor.Move(0, -1, line.size(), m_text.size());
            }

            if (ImGui::IsKeyPressed(ImGuiKey_DownArrow))
            {
                const int next_index = std::clamp<int>(m_cursor.GetPosY() + 1, 0, m_text.size() - 1);

                const Line& line = m_text[next_index];
                m_cursor.Move(0, 1, line.size(), m_text.size() - 1);
            }
        }
    }

    void Render()
    {
        const int width = CalcNumberOfDigits(m_text.size());

        float offset_x = 0;
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

private:
    void AddCharacter(ImWchar ch)
    {
        Line& line = m_text[m_cursor.GetPosY()];
        line.insert(m_cursor.GetPosX(), 1, (char)ch);

        m_cursor.Move(1, 0, line.size(), m_text.size());
    }

    void AddNewLine()
    {
        m_text.emplace(m_text.begin() + m_cursor.GetPosY() + 1);

        const Line& line = m_text[m_cursor.GetPosY()];
        m_cursor.Move(INT32_MIN, 1, line.size(), m_text.size());
    }

    using Line = std::string;
    using Text = std::vector<Line>;

    Text m_text;
    Cursor m_cursor{};
};