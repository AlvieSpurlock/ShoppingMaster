#pragma once
// Copyright © Mad Tea Party Development 2025

#include <wx/wx.h>
#include <wx/scrolwin.h>
#include <wx/spinctrl.h>
#include <wx/dcbuffer.h>
#include "ShoppingData.h"
#include <vector>

// ─── Palette ─────────────────────────────────────────────────────────────────
namespace Pal
{
    const wxColour APP_BG(0x6B, 0x6B, 0x6B);  // overall grey background
    const wxColour CARD_FACE(0x7E, 0x7E, 0x7E);  // store/item card face
    const wxColour CARD_FACE2(0x78, 0x78, 0x78);  // alternating shade
    const wxColour BTN_GREY(0x8A, 0x8A, 0x8A);  // action button bg
    const wxColour BTN_GREY_D(0x76, 0x76, 0x76);  // button darker outline
    const wxColour HDR_BLACK(0x18, 0x18, 0x18);  // "Stores"/"Items" pill
    const wxColour BLUE_PILL(0x42, 0xB2, 0xF5);  // store name pill
    const wxColour BLUE_PILL2(0x6A, 0xC8, 0xF8);  // lighter blue variant
    const wxColour TEMP_COLD(0x7E, 0xCE, 0xF5);
    const wxColour TEMP_FREEZE(0x4C, 0xB8, 0xF8);
    const wxColour BTN_RED(0xE0, 0x3C, 0x3C);
    const wxColour BTN_GREEN(0x4C, 0xC4, 0x5A);
    const wxColour BUDGET_BG(0x18, 0x18, 0x18);
    const wxColour BUDGET_PILL(0x42, 0xB2, 0xF5);
    const wxColour BAR_OK(0x4C, 0xC4, 0x5A);
    const wxColour BAR_WARN(0xF0, 0xA0, 0x20);
    const wxColour BAR_OVER(0xD4, 0x38, 0x38);
    const wxColour WHITE(0xFF, 0xFF, 0xFF);
    const wxColour BLACK(0x00, 0x00, 0x00);
    const wxColour TEXT_DK(0x18, 0x18, 0x18);
    const wxColour TEXT_MID(0xDD, 0xDD, 0xDD);
    const wxColour TEXT_DIM(0xAA, 0xAA, 0xAA);

    // Segoe UI via SWISS family — no deprecated constructor
    inline wxFont F(int pt)
    {
        return wxFont(wxFontInfo(pt).Family(wxFONTFAMILY_SWISS));
    }
    inline wxFont FB(int pt)
    {
        return wxFont(wxFontInfo(pt).Family(wxFONTFAMILY_SWISS).Bold());
    }
    inline wxFont FI(int pt)
    {
        return wxFont(wxFontInfo(pt).Family(wxFONTFAMILY_SWISS).Italic());
    }
}

// ─── Drawing helpers ─────────────────────────────────────────────────────────
namespace Draw
{
    // Filled rounded rectangle
    inline void RoundRect(wxDC& dc, const wxRect& r, int radius,
        const wxColour& fill, const wxColour& border = wxNullColour)
    {
        dc.SetBrush(wxBrush(fill));
        dc.SetPen(border.IsOk() ? wxPen(border, 1) : *wxTRANSPARENT_PEN);
        dc.DrawRoundedRectangle(r, radius);
    }

    // Centred text inside a rect
    inline void CentredText(wxDC& dc, const wxRect& r, const wxString& s,
        const wxColour& fg, const wxFont& font)
    {
        dc.SetFont(font);
        dc.SetTextForeground(fg);
        wxSize ts = dc.GetTextExtent(s);
        dc.DrawText(s, r.x + (r.width - ts.x) / 2,
            r.y + (r.height - ts.y) / 2);
    }

    // Left-aligned text vertically centred in rect
    inline void LeftText(wxDC& dc, const wxRect& r, const wxString& s,
        const wxColour& fg, const wxFont& font, int padLeft = 8)
    {
        dc.SetFont(font);
        dc.SetTextForeground(fg);
        wxSize ts = dc.GetTextExtent(s);
        dc.DrawText(s, r.x + padLeft, r.y + (r.height - ts.y) / 2);
    }
}

// ─── IDs ─────────────────────────────────────────────────────────────────────
enum {
    ID_ADD_STORE = wxID_HIGHEST + 100,
    ID_ADD_ITEM,
    ID_SORT_ITEMS,
    ID_CLEAR_ALL,
    ID_SAVE_LIST,
    ID_BUDGET_FIELD,
    // Per-row dynamic IDs handled via hit-testing in OnClick
};

// ─── StoreRow canvas ─────────────────────────────────────────────────────────
// Draws one store row exactly like the Canva reference:
//   [  StoreName | Address                    ] [~] [X]
class StoreRowPanel : public wxPanel
{
public:
    StoreRowPanel(wxWindow* parent, int index, const Store& s,
        std::function<void(int)> onEdit,
        std::function<void(int)> onDelete);
private:
    int   m_idx;
    Store m_store;
    std::function<void(int)> m_onEdit, m_onDelete;

    enum Zone { ZONE_NONE, ZONE_EDIT, ZONE_DELETE };
    Zone m_hover = ZONE_NONE;

    wxRect m_rcEdit, m_rcDelete;

    void OnPaint(wxPaintEvent&);
    void OnMouse(wxMouseEvent&);
    void OnLeave(wxMouseEvent&);
    Zone HitTest(const wxPoint& p) const;
    wxDECLARE_EVENT_TABLE();
};

// ─── ItemRow canvas ──────────────────────────────────────────────────────────
class ItemRowPanel : public wxPanel
{
public:
    ItemRowPanel(wxWindow* parent, int index, const Item& item,
        std::function<void(int)> onEdit,
        std::function<void(int)> onDelete);
private:
    int  m_idx;
    Item m_item;
    std::function<void(int)> m_onEdit, m_onDelete;

    enum Zone { ZONE_NONE, ZONE_EDIT, ZONE_DELETE };
    Zone m_hover = ZONE_NONE;

    wxRect m_rcEdit, m_rcDelete;

    void OnPaint(wxPaintEvent&);
    void OnMouse(wxMouseEvent&);
    void OnLeave(wxMouseEvent&);
    Zone HitTest(const wxPoint& p) const;
    wxDECLARE_EVENT_TABLE();
};

// ─── BudgetBar ───────────────────────────────────────────────────────────────
class BudgetBar : public wxWindow
{
public:
    BudgetBar(wxWindow* parent);
    void Set(double spent, double budget) { m_spent = spent; m_budget = budget; Refresh(); }
private:
    double m_spent = 0, m_budget = 0;
    void OnPaint(wxPaintEvent&);
    void OnErase(wxEraseEvent&) {}
    wxDECLARE_EVENT_TABLE();
};

// ─── Main Frame ──────────────────────────────────────────────────────────────
class ShoppingFrame : public wxFrame
{
public:
    ShoppingFrame();
private:
    std::vector<Store> m_stores;
    std::vector<Item>  m_items;
    double m_budget = 0.0;

    wxScrolledWindow* m_storeScroll = nullptr;
    wxScrolledWindow* m_itemScroll = nullptr;
    wxTextCtrl* m_budgetField = nullptr;
    BudgetBar* m_budgetBar = nullptr;
    wxStaticText* m_spentLabel = nullptr;
    bool          m_budgetSet = false;

    void Build();
    wxWindow* MakeTopBar(wxWindow* p);
    wxWindow* MakeStoreSection(wxWindow* p);
    wxWindow* MakeItemSection(wxWindow* p);
    wxWindow* MakeBottomBar(wxWindow* p);

    void RefreshStores();
    void RefreshItems();
    void UpdateBudget();
    double TotalSpent() const;

    // Round button factory (owner-drawn via wxBitmapButton not needed —
    // plain wxButton with no border, sized square)
    wxButton* Btn(wxWindow* p, wxWindowID id, const wxString& lbl,
        const wxColour& bg, const wxColour& fg,
        const wxSize& sz = wxSize(44, 44));

    void OnAddStore(wxCommandEvent&);
    void OnAddItem(wxCommandEvent&);
    void OnSort(wxCommandEvent&);
    void OnClearAll(wxCommandEvent&);
    void OnSave(wxCommandEvent&);
    void OnBudget(wxCommandEvent&);
    void OnClose(wxCloseEvent&);

    void DoEditStore(int i);
    void DoDeleteStore(int i);
    void DoEditItem(int i);
    void DoDeleteItem(int i);

    void SortItems();
    void ApplyBudgetColour();
    wxDECLARE_EVENT_TABLE();
};

// ─── Dialogs ─────────────────────────────────────────────────────────────────
class StoreDialog : public wxDialog {
public:
    StoreDialog(wxWindow* p, const Store& s, const wxString& title);
    Store Get() const;
private:
    wxTextCtrl* m_name = nullptr, * m_addr = nullptr;
};

class ItemDialog : public wxDialog {
public:
    ItemDialog(wxWindow* p, const std::vector<Store>& stores,
        const Item& item, const wxString& title);
    Item Get() const;
private:
    const std::vector<Store>& m_stores;
    wxChoice* m_store = nullptr;
    wxTextCtrl* m_name = nullptr;
    wxSpinCtrl* m_count = nullptr;
    wxTextCtrl* m_price = nullptr;
    wxChoice* m_temp = nullptr;
};