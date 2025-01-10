#pragma once

#include "Undo.h"

#include <iterator>
#include <deque>
#include <limits>
#include <memory>

class UndoBuffer
{
public:
    using UndoCommandPtr = std::unique_ptr<UndoCommand>;

    void SetBufferMaxSize(size_t size)
    {
        m_buffer_max_size = size;
    }

    void AddCommand(UndoCommandPtr cmd)
    {
        if (m_buffer.size() >= m_buffer_max_size)
        {
            m_buffer.erase(m_buffer.begin());
        }
        if (!m_buffer.empty())
        {
            m_buffer.erase(std::next(m_buffer.begin(), m_current), m_buffer.end());
        }
        m_current++;
        m_buffer.insert(m_buffer.end(), std::move(cmd));
    }

    void Do()
    {
        if ((m_current) < m_buffer.size())
        {
            auto& cmd = m_buffer[m_current];
            cmd->Do();
            
            m_current += 1;
        }
    }

    void Undo()
    {
        if (m_current != 0)
        {
            m_current -= 1;
            auto& cmd = m_buffer[m_current];

            cmd->Undo();
        }
    }

private:
    using BufferContainer = std::deque<UndoCommandPtr>;
    BufferContainer m_buffer{};

    static constexpr const size_t MinPosition = std::numeric_limits<size_t>::max();
    static constexpr const size_t DefaultBufferSize = 25;

    size_t m_current{0};
    size_t m_buffer_max_size{DefaultBufferSize};
};