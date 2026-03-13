// ShoppingFrame_Layout.cpp
// Copyright © Mad Tea Party Development 2025

#include "ShoppingFrame.h"
#include <wx/msgdlg.h>

// ════════════════════════════════════════════════════════════════════════════
//  Event table
// ════════════════════════════════════════════════════════════════════════════
wxBEGIN_EVENT_TABLE(ShoppingFrame, wxFrame)
EVT_BUTTON(ID_ADD_STORE, ShoppingFrame::OnAddStore)
EVT_BUTTON(ID_ADD_ITEM, ShoppingFrame::OnAddItem)
EVT_BUTTON(ID_SORT_ITEMS, ShoppingFrame::OnSort)
EVT_BUTTON(ID_CLEAR_ALL, ShoppingFrame::OnClearAll)
EVT_BUTTON(ID_SAVE_LIST, ShoppingFrame::OnSave)
EVT_TEXT(ID_BUDGET_FIELD, ShoppingFrame::OnBudget)
EVT_CLOSE(ShoppingFrame::OnClose)
wxEND_EVENT_TABLE()

// ─── Construction ────────────────────────────────────────────────────────────
ShoppingFrame::ShoppingFrame()
    : wxFrame(nullptr, wxID_ANY, "Shopping Master",
        wxDefaultPosition, wxSize(460, 820),
        wxDEFAULT_FRAME_STYLE & ~wxRESIZE_BORDER)
{
    SetBackgroundColour(Pal::APP_BG);
    Build();
    Centre();
}

// ─── Button factory ──────────────────────────────────────────────────────────
wxButton* ShoppingFrame::Btn(wxWindow* p, wxWindowID id, const wxString& lbl,
    const wxColour& bg, const wxColour& fg, const wxSize& sz)
{
    wxButton* b = new wxButton(p, id, lbl, wxDefaultPosition, sz, wxBORDER_NONE);
    b->SetBackgroundColour(bg);
    b->SetForegroundColour(fg);
    b->SetFont(Pal::FB(11));
    return b;
}

// ─── Top bar ─────────────────────────────────────────────────────────────────
wxWindow* ShoppingFrame::MakeTopBar(wxWindow* p)
{
    wxPanel* bar = new wxPanel(p, wxID_ANY);
    bar->SetBackgroundColour(wxColour(0x18, 0x18, 0x18));
    bar->SetMinSize(wxSize(-1, 58));

    wxBoxSizer* s = new wxBoxSizer(wxHORIZONTAL);

    wxStaticText* sub = new wxStaticText(bar, wxID_ANY, "Shopping");
    sub->SetFont(Pal::FI(9));  sub->SetForegroundColour(wxColour(0x88, 0x88, 0x88));

    wxStaticText* ttl = new wxStaticText(bar, wxID_ANY, "Master");
    ttl->SetFont(Pal::FB(15)); ttl->SetForegroundColour(Pal::BLUE_PILL);

    wxBoxSizer* tc = new wxBoxSizer(wxVERTICAL);
    tc->Add(sub, 0); tc->Add(ttl, 0);

    wxButton* btnSave = Btn(bar, ID_SAVE_LIST, "Save List",
        Pal::BTN_GREEN, Pal::TEXT_DK, wxSize(100, 36));

    s->AddSpacer(14);
    s->Add(tc, 1, wxALIGN_CENTRE_VERTICAL | wxTOP | wxBOTTOM, 8);
    s->Add(btnSave, 0, wxALIGN_CENTRE_VERTICAL | wxRIGHT, 12);
    bar->SetSizer(s);
    return bar;
}

// ─── Section header: [+] [LABEL PILL] [extra btn] ────────────────────────────
static wxPanel* SectionHdr(wxWindow* p, const wxString& label,
    wxWindowID addId, wxWindowID extraId = wxID_ANY,
    const wxString& extraLbl = "")
{
    wxPanel* hdr = new wxPanel(p, wxID_ANY);
    hdr->SetBackgroundColour(Pal::APP_BG);
    hdr->SetMinSize(wxSize(-1, 52));

    wxBoxSizer* s = new wxBoxSizer(wxHORIZONTAL);

    // + button — round grey
    wxButton* bAdd = new wxButton(hdr, addId, "+",
        wxDefaultPosition, wxSize(44, 44), wxBORDER_NONE);
    bAdd->SetBackgroundColour(Pal::BTN_GREY);
    bAdd->SetForegroundColour(Pal::WHITE);
    bAdd->SetFont(wxFont(wxFontInfo(16).Family(wxFONTFAMILY_SWISS).Bold()));

    // Black rounded-pill label (drawn as button with no action — wxBORDER_NONE)
    wxButton* pill = new wxButton(hdr, wxID_ANY, label,
        wxDefaultPosition, wxSize(130, 44), wxBORDER_NONE);
    pill->SetBackgroundColour(wxColour(0x18, 0x18, 0x18));
    pill->SetForegroundColour(Pal::WHITE);
    pill->SetFont(wxFont(wxFontInfo(13).Family(wxFONTFAMILY_SWISS).Bold()));
    pill->Enable(false);   // label only

    s->AddSpacer(10);
    s->Add(bAdd, 0, wxALIGN_CENTRE_VERTICAL | wxRIGHT, 8);
    s->Add(pill, 0, wxALIGN_CENTRE_VERTICAL);
    s->AddStretchSpacer();

    if (extraId != wxID_ANY)
    {
        wxButton* bEx = new wxButton(hdr, extraId, extraLbl,
            wxDefaultPosition, wxSize(44, 44), wxBORDER_NONE);
        bEx->SetBackgroundColour(Pal::BTN_GREY);
        bEx->SetForegroundColour(Pal::WHITE);
        bEx->SetFont(wxFont(wxFontInfo(13).Family(wxFONTFAMILY_SWISS).Bold()));
        s->Add(bEx, 0, wxALIGN_CENTRE_VERTICAL | wxRIGHT, 10);
    }

    hdr->SetSizer(s);
    return hdr;
}

// ─── Store section ───────────────────────────────────────────────────────────
wxWindow* ShoppingFrame::MakeStoreSection(wxWindow* p)
{
    wxPanel* sec = new wxPanel(p, wxID_ANY);
    sec->SetBackgroundColour(Pal::APP_BG);
    wxBoxSizer* col = new wxBoxSizer(wxVERTICAL);

    col->Add(SectionHdr(sec, "Stores", ID_ADD_STORE), 0, wxEXPAND);

    m_storeScroll = new wxScrolledWindow(sec, wxID_ANY,
        wxDefaultPosition, wxSize(-1, 150), wxVSCROLL | wxBORDER_NONE);
    m_storeScroll->SetScrollRate(0, 8);
    m_storeScroll->SetBackgroundColour(Pal::APP_BG);
    col->Add(m_storeScroll, 0, wxEXPAND | wxLEFT | wxRIGHT, 6);

    sec->SetSizer(col);
    return sec;
}

// ─── Item section ─────────────────────────────────────────────────────────────
wxWindow* ShoppingFrame::MakeItemSection(wxWindow* p)
{
    wxPanel* sec = new wxPanel(p, wxID_ANY);
    sec->SetBackgroundColour(Pal::APP_BG);
    wxBoxSizer* col = new wxBoxSizer(wxVERTICAL);

    col->Add(SectionHdr(sec, "Items", ID_ADD_ITEM, ID_SORT_ITEMS, "^"),
        0, wxEXPAND);

    m_itemScroll = new wxScrolledWindow(sec, wxID_ANY,
        wxDefaultPosition, wxDefaultSize, wxVSCROLL | wxBORDER_NONE);
    m_itemScroll->SetScrollRate(0, 8);
    m_itemScroll->SetBackgroundColour(Pal::APP_BG);
    col->Add(m_itemScroll, 1, wxEXPAND | wxLEFT | wxRIGHT, 6);

    sec->SetSizer(col);
    return sec;
}

// ─── Bottom bar ──────────────────────────────────────────────────────────────
wxWindow* ShoppingFrame::MakeBottomBar(wxWindow* p)
{
    wxPanel* bar = new wxPanel(p, wxID_ANY);
    bar->SetBackgroundColour(Pal::BUDGET_BG);
    bar->SetMinSize(wxSize(-1, 110));
    wxBoxSizer* col = new wxBoxSizer(wxVERTICAL);

    // ── Row: [X trash]  [Budget label]  [Budget field]  [Save] ──────────
    wxBoxSizer* row = new wxBoxSizer(wxHORIZONTAL);

    // Red trash (clear all)
    wxButton* bClear = Btn(bar, ID_CLEAR_ALL, "X",
        Pal::BTN_RED, Pal::WHITE, wxSize(48, 48));

    // "Budget" black pill — label only, fixed width, no warn text anywhere
    wxButton* bLbl = new wxButton(bar, wxID_ANY, "Budget",
        wxDefaultPosition, wxSize(110, 44), wxBORDER_NONE);
    bLbl->SetBackgroundColour(wxColour(0x18, 0x18, 0x18));
    bLbl->SetForegroundColour(Pal::WHITE);
    bLbl->SetFont(Pal::FB(12));
    bLbl->Enable(false);

    // Create border panel FIRST — m_budgetField must have it as parent,
    // not bar. wxWidgets asserts if a window's parent differs from the
    // window that owns the sizer it is added to.
    wxPanel* budgetBorder = new wxPanel(bar, wxID_ANY,
        wxDefaultPosition, wxDefaultSize, wxBORDER_NONE);
    budgetBorder->SetBackgroundColour(Pal::BLACK);

    // Budget field — parent is budgetBorder
    // wxTE_RICH2 vertically centres single-line text on Windows
    m_budgetField = new wxTextCtrl(budgetBorder, ID_BUDGET_FIELD, "",
        wxDefaultPosition, wxSize(110, 32),
        wxTE_CENTRE | wxTE_PROCESS_ENTER | wxTE_RICH2 | wxBORDER_NONE);
    m_budgetField->SetHint("Budget");
    m_budgetField->SetBackgroundColour(wxColour(0x55, 0x55, 0x55));
    m_budgetField->SetForegroundColour(Pal::WHITE);
    m_budgetField->SetFont(Pal::FB(12));

    {
        wxBoxSizer* bs = new wxBoxSizer(wxHORIZONTAL);
        bs->Add(m_budgetField, 1, wxEXPAND | wxALL, 2);
        budgetBorder->SetSizer(bs);
        budgetBorder->Fit();
    }

    // Green save button
    wxButton* bSave = Btn(bar, ID_SAVE_LIST, "Save",
        Pal::BTN_GREEN, Pal::TEXT_DK, wxSize(48, 48));

    row->AddSpacer(10);
    row->Add(bClear, 0, wxALIGN_CENTRE_VERTICAL | wxRIGHT, 10);
    row->AddStretchSpacer(1);   // push everything to centre
    row->Add(bLbl, 0, wxALIGN_CENTRE_VERTICAL | wxRIGHT, 0);
    row->Add(budgetBorder, 0, wxALIGN_CENTRE_VERTICAL | wxRIGHT, 0);
    row->AddStretchSpacer(1);   // symmetric stretch on right
    row->Add(bSave, 0, wxALIGN_CENTRE_VERTICAL | wxRIGHT, 10);

    // Spent / Budget line — fixed width text, no shifting elements
    m_spentLabel = new wxStaticText(bar, wxID_ANY,
        "Spent: $0.00   /   Budget: $0.00",
        wxDefaultPosition, wxDefaultSize, wxALIGN_CENTRE_HORIZONTAL);
    m_spentLabel->SetFont(Pal::F(8));
    m_spentLabel->SetForegroundColour(Pal::TEXT_DIM);

    // Budget progress bar — also shows overflow in red
    m_budgetBar = new BudgetBar(bar);
    m_budgetBar->SetMinSize(wxSize(-1, 20));

    col->AddSpacer(8);
    col->Add(row, 0, wxEXPAND);
    col->AddSpacer(6);
    col->Add(m_spentLabel, 0, wxEXPAND | wxLEFT | wxRIGHT, 12);
    col->AddSpacer(6);
    col->Add(m_budgetBar, 0, wxEXPAND | wxLEFT | wxRIGHT | wxBOTTOM, 12);
    bar->SetSizer(col);
    return bar;
}

// ─── Build ───────────────────────────────────────────────────────────────────
void ShoppingFrame::Build()
{
    wxBoxSizer* root = new wxBoxSizer(wxVERTICAL);
    root->Add(MakeTopBar(this), 0, wxEXPAND);
    root->Add(MakeStoreSection(this), 0, wxEXPAND);

    wxPanel* div = new wxPanel(this, wxID_ANY);
    div->SetBackgroundColour(wxColour(0x50, 0x50, 0x50));
    div->SetMinSize(wxSize(-1, 2));
    root->Add(div, 0, wxEXPAND | wxLEFT | wxRIGHT, 12);

    root->Add(MakeItemSection(this), 1, wxEXPAND);
    root->Add(MakeBottomBar(this), 0, wxEXPAND);
    SetSizer(root);
    Layout();
}