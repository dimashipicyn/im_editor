#pragma once

#include <cstdint>
#include <imgui.h>
#include <algorithm>

class Cursor
{
public:
    void Render()
    {
        const auto curr = ImGui::GetTime();
        const auto elapsed = curr - m_last_update_sec;

        if (elapsed >= m_blink_period_sec)
        {
            m_last_update_sec = curr;
            m_visible = !m_visible;
        }

        if (m_visible)
        {
            ImDrawList* draw_list = ImGui::GetWindowDrawList();

            const float font_size = ImGui::GetFontSize();
            const auto pos = GetAbsPos();
            draw_list->AddRectFilled(pos, {pos.x + 1, pos.y + font_size}, m_color);
        }
    }

    int GetPosX() const
    {
        return (int)m_pos.x;
    }

    int GetPosY() const
    {
        return (int)m_pos.y;
    }

    void Move(int x, int y, int max_x, int max_y)
    {
        m_pos.x = std::clamp<int>(m_pos.x + x, 0, max_x);
        m_pos.y = std::clamp<int>(m_pos.y + y, 0, max_y);
    }

    void SetPos(int x, int y)
    {
        m_pos.x = x;
        m_pos.y = y;
    }

    bool InSameLine(int line_index) const
    {
        return GetPosY() == line_index;
    }

    void SetColor(ImU32 color)
    {
        m_color = color;
    }

    void SetBlinkPeriod(float blink_period_sec)
    {
        m_blink_period_sec = blink_period_sec;
    }

private:
    ImVec2 GetAbsPos() const
    {
        const auto offset = ImGui::GetCursorScreenPos();
        return {offset.x + m_pos.x * ImGui::CalcTextSize("#").x, offset.y};
    }

    ImVec2 m_pos{};
    double m_last_update_sec{};
    float m_blink_period_sec{0.5};
    ImU32 m_color{0xff7f7f7f};
    bool m_visible{};
};