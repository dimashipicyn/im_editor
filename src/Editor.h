#pragma once

#include "Document.h"

#include <imgui.h>

class Editor
{
public:
    Editor()
    {
        
    }

    void Input()
    {
        ImGuiIO& io = ImGui::GetIO();
        if (ImGui::IsWindowFocused())
        {
            for (const ImWchar c : io.InputQueueCharacters)
            {
                m_doc.AddCharacter(c);
            }

            if (ImGui::IsKeyPressed(ImGuiKey_Enter))
            {
                m_doc.AddNewLine();
            }

            if (ImGui::IsKeyPressed(ImGuiKey_LeftArrow))
            {
                m_doc.MoveCursor(-1, 0);
            }

            if (ImGui::IsKeyPressed(ImGuiKey_RightArrow))
            {
                m_doc.MoveCursor(1, 0);
            }

            if (ImGui::IsKeyPressed(ImGuiKey_UpArrow))
            {
                m_doc.MoveCursor(0, -1);
            }

            if (ImGui::IsKeyPressed(ImGuiKey_DownArrow))
            {
                m_doc.MoveCursor(0, 1);
            }
        }
    }

    void Render()
    {
        m_doc.Render();
    }

private:
    Document m_doc{};
};