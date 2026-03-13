// ShoppingDialogs.cpp
// Copyright © Mad Tea Party Development 2025

#include "ShoppingFrame.h"
#include <wx/spinctrl.h>
#include <wx/statline.h>

static wxTextCtrl* DlgField(wxWindow* p, wxSizer* s,
    const wxString& lbl, const wxString& val)
{
    wxStaticText* l = new wxStaticText(p, wxID_ANY, lbl);
    l->SetFont(Pal::F(9)); l->SetForegroundColour(Pal::TEXT_DIM);
    wxTextCtrl* c = new wxTextCtrl(p, wxID_ANY, val);
    c->SetBackgroundColour(wxColour(0x3A, 0x3A, 0x3A));
    c->SetForegroundColour(Pal::WHITE);
    c->SetFont(Pal::F(10));
    s->Add(l, 0, wxLEFT | wxTOP, 6);
    s->Add(c, 0, wxEXPAND | wxLEFT | wxRIGHT | wxBOTTOM, 6);
    return c;
}

// ─── StoreDialog ─────────────────────────────────────────────────────────────
StoreDialog::StoreDialog(wxWindow* p, const Store& s, const wxString& title)
    : wxDialog(p, wxID_ANY, title, wxDefaultPosition, wxSize(320, 200),
        wxDEFAULT_DIALOG_STYLE)
{
    SetBackgroundColour(wxColour(0x44, 0x44, 0x44));
    wxBoxSizer* col = new wxBoxSizer(wxVERTICAL);

    wxStaticText* hdr = new wxStaticText(this, wxID_ANY, title);
    hdr->SetFont(Pal::FB(11)); hdr->SetForegroundColour(Pal::BLUE_PILL);
    col->Add(hdr, 0, wxALL, 12);
    col->Add(new wxStaticLine(this), 0, wxEXPAND | wxLEFT | wxRIGHT, 8);

    m_name = DlgField(this, col, "Store Name", s.name);
    m_addr = DlgField(this, col, "Store Address", s.address);

    col->AddStretchSpacer();
    wxStdDialogButtonSizer* btns = CreateStdDialogButtonSizer(wxOK | wxCANCEL);
    if (auto* ok = wxDynamicCast(FindWindowById(wxID_OK, this), wxButton)) {
        ok->SetBackgroundColour(Pal::BLUE_PILL);
        ok->SetForegroundColour(Pal::TEXT_DK);
    }
    col->Add(btns, 0, wxEXPAND | wxALL, 10);
    SetSizerAndFit(col); Centre();
}
Store StoreDialog::Get() const {
    Store s;
    s.name = m_name ? m_name->GetValue().ToStdString() : "";
    s.address = m_addr ? m_addr->GetValue().ToStdString() : "";
    return s;
}

// ─── ItemDialog ──────────────────────────────────────────────────────────────
ItemDialog::ItemDialog(wxWindow* p, const std::vector<Store>& stores,
    const Item& item, const wxString& title)
    : wxDialog(p, wxID_ANY, title, wxDefaultPosition, wxSize(320, 360),
        wxDEFAULT_DIALOG_STYLE), m_stores(stores)
{
    SetBackgroundColour(wxColour(0x44, 0x44, 0x44));
    wxBoxSizer* col = new wxBoxSizer(wxVERTICAL);

    wxStaticText* hdr = new wxStaticText(this, wxID_ANY, title);
    hdr->SetFont(Pal::FB(11)); hdr->SetForegroundColour(Pal::BLUE_PILL);
    col->Add(hdr, 0, wxALL, 12);
    col->Add(new wxStaticLine(this), 0, wxEXPAND | wxLEFT | wxRIGHT, 8);

    // Store choice
    {
        wxStaticText* l = new wxStaticText(this, wxID_ANY, "Store");
        l->SetFont(Pal::F(9)); l->SetForegroundColour(Pal::TEXT_DIM);
        col->Add(l, 0, wxLEFT | wxTOP, 6);
        m_store = new wxChoice(this, wxID_ANY);
        m_store->SetBackgroundColour(Pal::BLUE_PILL);
        m_store->SetForegroundColour(Pal::TEXT_DK);
        m_store->SetFont(Pal::FB(9));
        int sel = 0;
        for (int i = 0; i < (int)stores.size(); ++i) {
            m_store->Append(stores[i].name);
            if (stores[i].name == item.store.name) sel = i;
        }
        if (!stores.empty()) m_store->SetSelection(sel);
        col->Add(m_store, 0, wxEXPAND | wxLEFT | wxRIGHT | wxBOTTOM, 6);
    }

    m_name = DlgField(this, col, "Item Name",
        item.name == "Input Item Name" ? "" : item.name);

    // Count + Price row
    {
        wxBoxSizer* row = new wxBoxSizer(wxHORIZONTAL);
        wxBoxSizer* cc = new wxBoxSizer(wxVERTICAL);
        wxStaticText* lc = new wxStaticText(this, wxID_ANY, "Count");
        lc->SetFont(Pal::F(9)); lc->SetForegroundColour(Pal::TEXT_DIM);
        m_count = new wxSpinCtrl(this, wxID_ANY, "",
            wxDefaultPosition, wxSize(80, -1), wxSP_ARROW_KEYS, 1, 999, item.count);
        m_count->SetBackgroundColour(wxColour(0x3A, 0x3A, 0x3A));
        m_count->SetForegroundColour(Pal::WHITE);
        cc->Add(lc, 0, wxLEFT | wxTOP, 6); cc->Add(m_count, 0, wxALL, 6);

        wxBoxSizer* pc = new wxBoxSizer(wxVERTICAL);
        wxStaticText* lp = new wxStaticText(this, wxID_ANY, "Unit Price ($)");
        lp->SetFont(Pal::F(9)); lp->SetForegroundColour(Pal::TEXT_DIM);
        double up = item.count > 0 ? item.price / item.count : item.price;
        wxString ps = up > 0 ? wxString::Format("%.2f", up) : "";
        m_price = new wxTextCtrl(this, wxID_ANY, ps);
        m_price->SetBackgroundColour(wxColour(0x3A, 0x3A, 0x3A));
        m_price->SetForegroundColour(Pal::WHITE);
        m_price->SetFont(Pal::F(10));
        pc->Add(lp, 0, wxLEFT | wxTOP, 6); pc->Add(m_price, 1, wxEXPAND | wxALL, 6);

        row->Add(cc, 0); row->Add(pc, 1, wxEXPAND);
        col->Add(row, 0, wxEXPAND);
    }

    // Temp
    {
        wxStaticText* l = new wxStaticText(this, wxID_ANY, "Temperature");
        l->SetFont(Pal::F(9)); l->SetForegroundColour(Pal::TEXT_DIM);
        col->Add(l, 0, wxLEFT | wxTOP, 6);
        m_temp = new wxChoice(this, wxID_ANY);
        m_temp->Append("Room"); m_temp->Append("Cold"); m_temp->Append("Freezer");
        m_temp->SetSelection((int)item.temp);
        m_temp->SetBackgroundColour(wxColour(0x3A, 0x3A, 0x3A));
        m_temp->SetForegroundColour(Pal::WHITE);
        col->Add(m_temp, 0, wxEXPAND | wxLEFT | wxRIGHT | wxBOTTOM, 6);
    }

    col->AddStretchSpacer();
    wxStdDialogButtonSizer* btns = CreateStdDialogButtonSizer(wxOK | wxCANCEL);
    if (auto* ok = wxDynamicCast(FindWindowById(wxID_OK, this), wxButton)) {
        ok->SetBackgroundColour(Pal::BLUE_PILL);
        ok->SetForegroundColour(Pal::TEXT_DK);
    }
    col->Add(btns, 0, wxEXPAND | wxALL, 10);
    SetSizerAndFit(col); Centre();
}

Item ItemDialog::Get() const
{
    Item it;
    int si = m_store ? m_store->GetSelection() : 0;
    if (si >= 0 && si < (int)m_stores.size()) it.store = m_stores[si];
    it.name = m_name ? m_name->GetValue().ToStdString() : "";
    it.count = m_count ? m_count->GetValue() : 1;
    it.temp = m_temp ? (Temp)m_temp->GetSelection() : Room;
    double up = 0;
    if (m_price) m_price->GetValue().ToDouble(&up);
    it.price = up * it.count;
    return it;
}