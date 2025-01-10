#pragma once

#include "Document.h"
#include "LuaHighlighter.h"
#include "UndoBuffer.h"

#include <imgui.h>
#include <memory>

class Editor
{
public:
    Editor()
    {
        m_doc.SetHighlighter(std::make_unique<LuaHighlighter>());
        m_doc.SetPalette(Palette::Light());
    }

    void Render()
    {
        Input();
        m_doc.Render();
    }

private:
    void Input()
    {
        ImGuiIO& io = ImGui::GetIO();
        if (ImGui::IsWindowFocused())
        {
            for (const ImWchar c : io.InputQueueCharacters)
            {
                //m_doc.AddCharacter(c);
                m_undo_buffer.AddCommand(std::make_unique<AddCharacterCommand>(m_doc, c));
            }

            if (ImGui::IsKeyChordPressed(ImGuiMod_Ctrl | ImGuiKey_Z))
            {
                m_undo_buffer.Undo();
                return;
            }

            if (ImGui::IsKeyChordPressed(ImGuiMod_Ctrl | ImGuiMod_Shift | ImGuiKey_Z))
            {
                m_undo_buffer.Do();
                return;
            }

            if (ImGui::IsKeyPressed(ImGuiKey_Backspace))
            {
                m_doc.RemoveCharacter();
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

    Document m_doc{};
    UndoBuffer m_undo_buffer{};
};