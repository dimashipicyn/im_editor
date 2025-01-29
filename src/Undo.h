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

class AddStrCommand final : public UndoCommand
{
public:
    AddStrCommand(Document& doc, std::string str)
        : m_doc(doc)
        , m_str(str)
    {
        Do();
    }

    void Do() override
    {
        m_doc.Insert(m_str);
        m_cursor_pos = m_doc.GetCursorPosition();
    }

    void Undo() override
    {
        m_doc.SetCursorPosition(m_cursor_pos.x, m_cursor_pos.y);
        m_doc.Remove(m_str.size());
    }

private:
    Document& m_doc;
    std::string m_str{};
    Coord m_cursor_pos{};
};

class RemoveStrCommand final : public UndoCommand
{
public:
    RemoveStrCommand(Document& doc)
        : m_doc(doc)
    {
        Do();
    }

    void Do() override
    {
        m_str = m_doc.Remove(1);
        m_cursor_pos = m_doc.GetCursorPosition();
    }

    void Undo() override
    {
        m_doc.SetCursorPosition(m_cursor_pos.x, m_cursor_pos.y);
        m_doc.Insert(m_str);
    }

private:
    Document& m_doc;
    std::string m_str{};
    Coord m_cursor_pos{};
};