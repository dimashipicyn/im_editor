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
            if (ImGui::IsWindowHovered())
                ImGui::SetMouseCursor(ImGuiMouseCursor_TextInput);

            for (const ImWchar c : io.InputQueueCharacters)
            {
                AddCharacter(c);
            }

            if (ImGui::IsKeyPressed(ImGuiKey_Enter))
            {
                m_text.emplace_back();

                m_cursor.Move(0, 1);
                m_cursor.SetPos(0, m_cursor.GetPosY());
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
        m_text[m_cursor.GetPosY()].insert(m_cursor.GetPosX(), 1, (char)ch);
        m_cursor.Move(1, 0);
    }

    using Line = std::string;
    using Text = std::vector<Line>;

    Text m_text;
    Cursor m_cursor{};
};