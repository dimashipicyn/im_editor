#pragma once

#include "Document.h"
#include <imgui.h>

class UndoCommand
{
public:
    virtual ~UndoCommand() = default;
    virtual void Do() = 0;
    virtual void Undo() = 0;
};

class AddCharacterCommand final : public UndoCommand
{
public:
    AddCharacterCommand(Document& doc, ImWchar ch)
        : m_doc(doc)
        , m_ch(ch)
    {
        Do();
    }

    void Do() override
    {
        m_doc.AddCharacter(m_ch);
        m_cursor_pos = m_doc.GetCursorPosition();
    }

    void Undo() override
    {
        std::apply(&Document::SetCursorPosition, std::tuple_cat(std::make_tuple(&m_doc), m_cursor_pos));
        m_doc.RemoveCharacter();
    }

private:
    Document& m_doc;
    ImWchar m_ch{};
    std::tuple<int, int> m_cursor_pos{};
};

class RemoveCharacterCommand final : public UndoCommand
{
public:
    RemoveCharacterCommand(Document& doc)
        : m_doc(doc)
    {
        Do();
    }

    void Do() override
    {
        
    }

    void Undo() override
    {
        
    }

private:
    Document& m_doc;
    std::tuple<int, int> m_cursor_pos{};
};