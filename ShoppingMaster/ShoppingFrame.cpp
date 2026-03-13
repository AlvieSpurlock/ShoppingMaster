// Copyright © Mad Tea Party Development 2025
// ShoppingFrame.cpp — owner-drawn row panels, BudgetBar, event tables

#include "ShoppingFrame.h"
#include <wx/dcbuffer.h>
#include <sstream>
#include <iomanip>
#include <algorithm>

// ════════════════════════════════════════════════════════════════════════════
//  StoreRowPanel
// ════════════════════════════════════════════════════════════════════════════
wxBEGIN_EVENT_TABLE(StoreRowPanel, wxPanel)
EVT_PAINT(StoreRowPanel::OnPaint)
EVT_LEFT_UP(StoreRowPanel::OnMouse)
EVT_MOTION(StoreRowPanel::OnMouse)
EVT_LEAVE_WINDOW(StoreRowPanel::OnLeave)
wxEND_EVENT_TABLE()

StoreRowPanel::StoreRowPanel(wxWindow* parent, int index, const Store& s,
    std::function<void(int)> onEdit, std::function<void(int)> onDelete)
    : wxPanel(parent, wxID_ANY, wxDefaultPosition, wxSize(-1, 56),
        wxFULL_REPAINT_ON_RESIZE),
    m_idx(index), m_store(s), m_onEdit(onEdit), m_onDelete(onDelete)
{
    SetBackgroundStyle(wxBG_STYLE_PAINT);
    SetCursor(wxCURSOR_HAND);
}

StoreRowPanel::Zone StoreRowPanel::HitTest(const wxPoint& p) const
{
    if (m_rcEdit.Contains(p))   return ZONE_EDIT;
    if (m_rcDelete.Contains(p)) return ZONE_DELETE;
    return ZONE_NONE;
}

void StoreRowPanel::OnPaint(wxPaintEvent&)
{
    wxAutoBufferedPaintDC dc(this);
    wxSize sz = GetClientSize();
    const int R = 16;   // card corner radius
    const int BR = 12;  // button corner radius
    const int BW = 44;  // button width
    const int M = 6;   // gap between card and buttons

    // ── Background (app colour, so panel is "transparent") ──────────────
    dc.SetBackground(wxBrush(Pal::APP_BG));
    dc.Clear();

    // ── Card (wide rounded rect) ─────────────────────────────────────────
    wxRect card(0, 4, sz.x - (BW + M) * 2 - 8, sz.y - 8);
    Draw::RoundRect(dc, card, R, Pal::CARD_FACE);

    // Store name blue pill inside card
    int pillW = 100, pillH = 32;
    wxRect pill(card.x + 8, card.y + (card.height - pillH) / 2, pillW, pillH);
    Draw::RoundRect(dc, pill, 16, Pal::BLUE_PILL);
    wxString nameStr = m_store.name;
    if (dc.GetFont().IsOk()) dc.SetFont(Pal::FB(9));
    wxSize ts = dc.GetTextExtent(nameStr);
    while (ts.x > pillW - 12 && nameStr.length() > 2) {
        nameStr = nameStr.substr(0, nameStr.length() - 1);
        ts = dc.GetTextExtent(nameStr);
    }
    Draw::CentredText(dc, pill, nameStr, Pal::TEXT_DK, Pal::FB(9));

    // Vertical divider
    int divX = pill.GetRight() + 10;
    dc.SetPen(wxPen(wxColour(0x60, 0x60, 0x60), 1));
    dc.DrawLine(divX, card.y + 10, divX, card.GetBottom() - 10);

    // Address text
    wxString addrStr = m_store.address;
    wxRect addrR(divX + 8, card.y, card.GetRight() - divX - 12, card.height);
    {
        dc.SetFont(Pal::F(9));
        wxSize as = dc.GetTextExtent(addrStr);
        while (as.x > addrR.width - 4 && addrStr.length() > 2) {
            addrStr = addrStr.substr(0, addrStr.length() - 1);
            as = dc.GetTextExtent(addrStr);
        }
        if (addrStr != m_store.address) addrStr += "...";
    }
    Draw::LeftText(dc, addrR, addrStr, Pal::TEXT_MID, Pal::F(9), 4);

    // ── Action buttons (rounded squares to the right) ────────────────────
    int bx1 = card.GetRight() + M;
    int bx2 = bx1 + BW + M;
    int by = (sz.y - BW) / 2;
    m_rcEdit = wxRect(bx1, by, BW, BW);
    m_rcDelete = wxRect(bx2, by, BW, BW);

    wxColour editCol = (m_hover == ZONE_EDIT) ? wxColour(0xA0, 0xA0, 0xA0) : Pal::BTN_GREY;
    wxColour delCol = (m_hover == ZONE_DELETE) ? wxColour(0xA0, 0xA0, 0xA0) : Pal::BTN_GREY;

    Draw::RoundRect(dc, m_rcEdit, BR, editCol);
    Draw::RoundRect(dc, m_rcDelete, BR, delCol);

    // ~ symbol (edit)
    Draw::CentredText(dc, m_rcEdit, "~", Pal::WHITE, Pal::FB(14));
    // trash symbol (delete)
    Draw::CentredText(dc, m_rcDelete, "X", Pal::WHITE, Pal::FB(12));
}

void StoreRowPanel::OnMouse(wxMouseEvent& e)
{
    Zone z = HitTest(e.GetPosition());
    if (m_hover != z) { m_hover = z; Refresh(); }
    if (e.LeftUp()) {
        if (z == ZONE_EDIT && m_onEdit) {
            auto cb = m_onEdit; int idx = m_idx;
            CallAfter([cb, idx] { cb(idx); });
        }
        if (z == ZONE_DELETE && m_onDelete) {
            auto cb = m_onDelete; int idx = m_idx;
            CallAfter([cb, idx] { cb(idx); });
        }
    }
    e.Skip();
}
void StoreRowPanel::OnLeave(wxMouseEvent& e)
{
    m_hover = ZONE_NONE; Refresh(); e.Skip();
}

// ════════════════════════════════════════════════════════════════════════════
//  ItemRowPanel
// ════════════════════════════════════════════════════════════════════════════
wxBEGIN_EVENT_TABLE(ItemRowPanel, wxPanel)
EVT_PAINT(ItemRowPanel::OnPaint)
EVT_LEFT_UP(ItemRowPanel::OnMouse)
EVT_MOTION(ItemRowPanel::OnMouse)
EVT_LEAVE_WINDOW(ItemRowPanel::OnLeave)
wxEND_EVENT_TABLE()

ItemRowPanel::ItemRowPanel(wxWindow* parent, int index, const Item& item,
    std::function<void(int)> onEdit, std::function<void(int)> onDelete)
    : wxPanel(parent, wxID_ANY, wxDefaultPosition, wxSize(-1, 76),
        wxFULL_REPAINT_ON_RESIZE),
    m_idx(index), m_item(item), m_onEdit(onEdit), m_onDelete(onDelete)
{
    SetBackgroundStyle(wxBG_STYLE_PAINT);
    SetCursor(wxCURSOR_HAND);
}

ItemRowPanel::Zone ItemRowPanel::HitTest(const wxPoint& p) const
{
    if (m_rcEdit.Contains(p))   return ZONE_EDIT;
    if (m_rcDelete.Contains(p)) return ZONE_DELETE;
    return ZONE_NONE;
}

void ItemRowPanel::OnPaint(wxPaintEvent&)
{
    wxAutoBufferedPaintDC dc(this);
    wxSize sz = GetClientSize();
    const int R = 16;
    const int BR = 12;
    const int BW = 44;
    const int M = 6;

    dc.SetBackground(wxBrush(Pal::APP_BG));
    dc.Clear();

    // ── Main card ────────────────────────────────────────────────────────
    wxRect card(0, 4, sz.x - (BW + M) * 2 - 8, sz.y - 8);
    wxColour cardCol = (m_idx % 2 == 0) ? Pal::CARD_FACE : Pal::CARD_FACE2;
    Draw::RoundRect(dc, card, R, cardCol);

    // Temp indicator — small circle on left
    wxColour tempCol = (m_item.temp == Room) ? wxColour(0x90, 0x90, 0x90)
        : (m_item.temp == Cold) ? Pal::TEMP_COLD
        : Pal::TEMP_FREEZE;
    wxRect tempCirc(card.x + 8, card.y + 8, 22, 22);
    dc.SetBrush(wxBrush(tempCol));
    dc.SetPen(*wxTRANSPARENT_PEN);
    dc.DrawCircle(tempCirc.GetLeft() + 11, tempCirc.GetTop() + 11, 11);

    // Item name — top row of card
    wxRect nameR(card.x + 38, card.y + 4, card.width - 46, 30);
    wxString nameStr = m_item.name;
    {
        dc.SetFont(Pal::FB(10));
        wxSize ts = dc.GetTextExtent(nameStr);
        while (ts.x > nameR.width - 4 && nameStr.length() > 2) {
            nameStr = nameStr.substr(0, nameStr.length() - 1);
            ts = dc.GetTextExtent(nameStr);
        }
    }
    Draw::LeftText(dc, nameR, nameStr, Pal::WHITE, Pal::FB(10), 4);

    // Store pill — bottom row of card
    int pillW = 90, pillH = 26;
    wxRect storePill(card.x + 38, card.GetBottom() - pillH - 8, pillW, pillH);
    Draw::RoundRect(dc, storePill, 14, Pal::BLUE_PILL);
    wxString sn = m_item.store.name;
    {
        dc.SetFont(Pal::FB(8));
        wxSize ts = dc.GetTextExtent(sn);
        while (ts.x > pillW - 10 && sn.length() > 2)
            sn = sn.substr(0, sn.length() - 1);
    }
    Draw::CentredText(dc, storePill, sn, Pal::TEXT_DK, Pal::FB(8));

    // Price
    std::ostringstream pss;
    pss << "$" << std::fixed << std::setprecision(2) << m_item.price;
    wxRect priceR(storePill.GetRight() + 8, storePill.y, 100, pillH);
    Draw::LeftText(dc, priceR, pss.str(), Pal::TEXT_MID, Pal::F(9), 0);

    // ── Action buttons ────────────────────────────────────────────────────
    int bx1 = card.GetRight() + M;
    int bx2 = bx1 + BW + M;
    int by = (sz.y - BW) / 2;
    m_rcEdit = wxRect(bx1, by, BW, BW);
    m_rcDelete = wxRect(bx2, by, BW, BW);

    wxColour ec = (m_hover == ZONE_EDIT) ? wxColour(0xA0, 0xA0, 0xA0) : Pal::BTN_GREY;
    wxColour dc2 = (m_hover == ZONE_DELETE) ? wxColour(0xA0, 0xA0, 0xA0) : Pal::BTN_GREY;

    Draw::RoundRect(dc, m_rcEdit, BR, ec);
    Draw::RoundRect(dc, m_rcDelete, BR, dc2);
    Draw::CentredText(dc, m_rcEdit, "~", Pal::WHITE, Pal::FB(14));
    Draw::CentredText(dc, m_rcDelete, "X", Pal::WHITE, Pal::FB(12));
}

void ItemRowPanel::OnMouse(wxMouseEvent& e)
{
    Zone z = HitTest(e.GetPosition());
    if (m_hover != z) { m_hover = z; Refresh(); }
    if (e.LeftUp()) {
        // Use CallAfter so the mouse event fully unwinds before RefreshItems()
        // calls DestroyChildren() — otherwise "this" is destroyed mid-event.
        if (z == ZONE_EDIT && m_onEdit) {
            auto cb = m_onEdit; int idx = m_idx;
            CallAfter([cb, idx] { cb(idx); });
        }
        if (z == ZONE_DELETE && m_onDelete) {
            auto cb = m_onDelete; int idx = m_idx;
            CallAfter([cb, idx] { cb(idx); });
        }
    }
    e.Skip();
}
void ItemRowPanel::OnLeave(wxMouseEvent& e)
{
    m_hover = ZONE_NONE; Refresh(); e.Skip();
}

// ════════════════════════════════════════════════════════════════════════════
//  BudgetBar
// ════════════════════════════════════════════════════════════════════════════
wxBEGIN_EVENT_TABLE(BudgetBar, wxWindow)
EVT_PAINT(BudgetBar::OnPaint)
EVT_ERASE_BACKGROUND(BudgetBar::OnErase)
wxEND_EVENT_TABLE()

BudgetBar::BudgetBar(wxWindow* parent)
    : wxWindow(parent, wxID_ANY)
{
    SetBackgroundStyle(wxBG_STYLE_PAINT);
    SetMinSize(wxSize(-1, 18));
}

void BudgetBar::OnPaint(wxPaintEvent&)
{
    wxAutoBufferedPaintDC dc(this);
    wxSize sz = GetClientSize();
    dc.SetBackground(wxBrush(Pal::BUDGET_BG));
    dc.Clear();
    dc.SetPen(*wxTRANSPARENT_PEN);

    // Dark track
    dc.SetBrush(wxBrush(wxColour(0x30, 0x30, 0x30)));
    dc.DrawRoundedRectangle(0, 0, sz.x, sz.y, 9);

    if (m_budget <= 0) return;

    double ratio = m_spent / m_budget;

    if (ratio <= 1.0)
    {
        // Normal: fill green → yellow as ratio rises
        int fw = std::max(0, (int)(sz.x * ratio));
        if (fw < 2) return;
        wxColour col = ratio < 0.75 ? Pal::BAR_OK : Pal::BAR_WARN;
        dc.SetBrush(wxBrush(col));
        dc.SetClippingRegion(0, 0, fw, sz.y);
        dc.DrawRoundedRectangle(0, 0, sz.x, sz.y, 9);
        dc.DestroyClippingRegion();
    }
    else
    {
        // Over budget: full bar in warning colour, overflow shown as
        // a bright red "extra" stripe on the right edge so the user
        // can see HOW FAR over they are relative to the budget.
        // Budget portion = sz.x pixels (full width = 100% of budget)
        // Overflow stripe width = capped at sz.x so it doesn't vanish
        double overRatio = (m_spent - m_budget) / m_budget;
        int overW = std::min((int)(sz.x * overRatio), sz.x - 4);

        // Full bar in yellow (the budget amount)
        dc.SetBrush(wxBrush(Pal::BAR_WARN));
        dc.DrawRoundedRectangle(0, 0, sz.x, sz.y, 9);

        // Red overflow stripe on right
        if (overW > 4)
        {
            dc.SetBrush(wxBrush(Pal::BAR_OVER));
            dc.SetClippingRegion(sz.x - overW, 0, overW, sz.y);
            dc.DrawRoundedRectangle(0, 0, sz.x, sz.y, 9);
            dc.DestroyClippingRegion();
        }
    }
}