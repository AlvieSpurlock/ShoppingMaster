// ShoppingFrame_Lists.cpp
// Copyright © Mad Tea Party Development 2025

#include "ShoppingFrame.h"
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <cctype>

void ShoppingFrame::RefreshStores()
{
    m_storeScroll->DestroyChildren();
    wxBoxSizer* col = new wxBoxSizer(wxVERTICAL);

    if (m_stores.empty())
    {
        wxStaticText* t = new wxStaticText(m_storeScroll, wxID_ANY,
            "No stores yet.  Press + to add one.");
        t->SetFont(Pal::FI(9));
        t->SetForegroundColour(Pal::TEXT_DIM);
        col->Add(t, 0, wxALL, 12);
    }
    else
    {
        for (int i = 0; i < (int)m_stores.size(); ++i)
        {
            auto* row = new StoreRowPanel(m_storeScroll, i, m_stores[i],
                [this](int idx) { DoEditStore(idx); },
                [this](int idx) { DoDeleteStore(idx); });
            col->Add(row, 0, wxEXPAND | wxBOTTOM, 5);
        }
    }
    col->AddSpacer(4);
    m_storeScroll->SetSizer(col);
    m_storeScroll->FitInside();
    m_storeScroll->Layout();
    Layout();
}

void ShoppingFrame::RefreshItems()
{
    m_itemScroll->DestroyChildren();
    wxBoxSizer* col = new wxBoxSizer(wxVERTICAL);

    if (m_items.empty())
    {
        wxStaticText* t = new wxStaticText(m_itemScroll, wxID_ANY,
            "No items yet.  Press + to add one.");
        t->SetFont(Pal::FI(9));
        t->SetForegroundColour(Pal::TEXT_DIM);
        col->Add(t, 0, wxALL, 12);
    }
    else
    {
        for (int i = 0; i < (int)m_items.size(); ++i)
        {
            auto* row = new ItemRowPanel(m_itemScroll, i, m_items[i],
                [this](int idx) { DoEditItem(idx); },
                [this](int idx) { DoDeleteItem(idx); });
            col->Add(row, 0, wxEXPAND | wxBOTTOM, 5);
        }
    }
    col->AddSpacer(6);
    m_itemScroll->SetSizer(col);
    m_itemScroll->FitInside();
    m_itemScroll->Layout();
    UpdateBudget();
    Layout();
}

double ShoppingFrame::TotalSpent() const
{
    double t = 0;
    for (auto& i : m_items) t += i.price;
    return t;
}

void ShoppingFrame::UpdateBudget()
{
    double spent = TotalSpent();

    // ── Spent / Budget label (always same width, no shifting) ────────────
    std::ostringstream oss;
    oss << "Spent: $" << std::fixed << std::setprecision(2) << spent
        << "   /   Budget: $" << std::fixed << std::setprecision(2) << m_budget;
    if (m_spentLabel) m_spentLabel->SetLabel(oss.str());

    // ── Progress bar (handles overflow internally) ────────────────────────
    if (m_budgetBar) m_budgetBar->Set(spent, m_budget);

    // ── Budget field colour: grey=not set, green=ok, yellow=near, red=over
    ApplyBudgetColour();
}

void ShoppingFrame::ApplyBudgetColour()
{
    if (!m_budgetField) return;
    if (!m_budgetSet) {
        // Not yet confirmed — neutral grey, white text
        m_budgetField->SetBackgroundColour(wxColour(0x55, 0x55, 0x55));
        m_budgetField->SetForegroundColour(Pal::WHITE);
        m_budgetField->Refresh();
        return;
    }
    double spent = TotalSpent();
    double ratio = m_budget > 0 ? spent / m_budget : 0.0;
    wxColour bg, fg;
    if (ratio >= 1.0) {
        bg = Pal::BAR_OVER;   // red
        fg = Pal::WHITE;
    }
    else if (ratio >= 0.85) {
        bg = Pal::BAR_WARN;   // yellow/orange
        fg = Pal::TEXT_DK;
    }
    else {
        bg = Pal::BAR_OK;     // green
        fg = Pal::TEXT_DK;
    }
    m_budgetField->SetBackgroundColour(bg);
    m_budgetField->SetForegroundColour(fg);
    m_budgetField->Refresh();
}