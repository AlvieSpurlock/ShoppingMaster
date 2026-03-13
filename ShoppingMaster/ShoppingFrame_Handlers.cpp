// ShoppingFrame_Handlers.cpp
// Copyright © Mad Tea Party Development 2025

#include "ShoppingFrame.h"
#include <wx/msgdlg.h>
#include <wx/filedlg.h>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <cctype>

void ShoppingFrame::SortItems()
{
    std::sort(m_items.begin(), m_items.end(), [](const Item& a, const Item& b) {
        auto up = [](std::string s) { for (auto& c : s) c = std::toupper((unsigned char)c); return s; };
        if (up(a.store.name) != up(b.store.name)) return up(a.store.name) < up(b.store.name);
        if (a.temp != b.temp) return a.temp < b.temp;
        return up(a.name) < up(b.name);
        });
}

void ShoppingFrame::OnAddStore(wxCommandEvent&)
{
    if (!m_budgetSet) {
        wxMessageBox(
            "Please enter your budget first, then press Enter to confirm.",
            "Set Budget First", wxOK | wxICON_INFORMATION, this);
        if (m_budgetField) m_budgetField->SetFocus();
        return;
    }
    Store blank;
    StoreDialog dlg(this, blank, "Add Store");
    if (dlg.ShowModal() == wxID_OK) {
        Store s = dlg.Get();
        if (!s.name.empty()) {
            m_stores.push_back(s);
            std::sort(m_stores.begin(), m_stores.end(),
                [](const Store& a, const Store& b) { return a.name < b.name; });
            RefreshStores();
        }
    }
}

void ShoppingFrame::DoEditStore(int i)
{
    if (i < 0 || i >= (int)m_stores.size()) return;
    StoreDialog dlg(this, m_stores[i], "Edit Store");
    if (dlg.ShowModal() == wxID_OK) {
        Store s = dlg.Get();
        if (!s.name.empty()) {
            std::string old = m_stores[i].name;
            m_stores[i] = s;
            for (auto& item : m_items)
                if (item.store.name == old) item.store = s;
            RefreshStores(); RefreshItems();
        }
    }
}

void ShoppingFrame::DoDeleteStore(int i)
{
    if (i < 0 || i >= (int)m_stores.size()) return;
    if (wxMessageBox(wxString::Format("Delete store '%s'?", m_stores[i].name),
        "Confirm", wxYES_NO | wxNO_DEFAULT | wxICON_WARNING, this) != wxYES) return;
    std::string nm = m_stores[i].name;
    m_stores.erase(m_stores.begin() + i);
    m_items.erase(std::remove_if(m_items.begin(), m_items.end(),
        [&](const Item& it) { return it.store.name == nm; }), m_items.end());
    RefreshStores(); RefreshItems();
}

void ShoppingFrame::OnAddItem(wxCommandEvent&)
{
    if (!m_budgetSet) {
        wxMessageBox(
            "Please enter your budget first, then press Enter to confirm.",
            "Set Budget First", wxOK | wxICON_INFORMATION, this);
        if (m_budgetField) m_budgetField->SetFocus();
        return;
    }
    if (m_stores.empty()) {
        wxMessageBox("Add a store first.", "No Stores", wxOK | wxICON_INFORMATION, this);
        return;
    }
    Item blank;
    ItemDialog dlg(this, m_stores, blank, "Add Item");
    if (dlg.ShowModal() == wxID_OK) {
        Item it = dlg.Get();
        if (!it.name.empty()) { m_items.push_back(it); SortItems(); RefreshItems(); }
    }
}

void ShoppingFrame::DoEditItem(int i)
{
    if (i < 0 || i >= (int)m_items.size()) return;
    ItemDialog dlg(this, m_stores, m_items[i], "Edit Item");
    if (dlg.ShowModal() == wxID_OK) {
        Item it = dlg.Get();
        if (!it.name.empty()) { m_items[i] = it; SortItems(); RefreshItems(); }
    }
}

void ShoppingFrame::DoDeleteItem(int i)
{
    if (i < 0 || i >= (int)m_items.size()) return;
    if (wxMessageBox(wxString::Format("Delete '%s'?", m_items[i].name),
        "Confirm", wxYES_NO | wxNO_DEFAULT | wxICON_WARNING, this) != wxYES) return;
    m_items.erase(m_items.begin() + i);
    RefreshItems();
}

void ShoppingFrame::OnSort(wxCommandEvent&) { SortItems(); RefreshItems(); }

void ShoppingFrame::OnClearAll(wxCommandEvent&)
{
    if (wxMessageBox("Clear all items?", "Confirm",
        wxYES_NO | wxNO_DEFAULT | wxICON_WARNING, this) == wxYES) {
        m_items.clear(); RefreshItems();
    }
}

void ShoppingFrame::OnBudget(wxCommandEvent& e)
{
    if (!m_budgetField) return;
    double d = 0;
    m_budgetField->GetValue().ToDouble(&d);
    m_budget = d < 0 ? 0 : d;

    // Mark as set once user types a positive value
    if (m_budget > 0) m_budgetSet = true;

    // On Enter key, move focus away to confirm and update colour immediately
    if (e.GetEventType() == wxEVT_TEXT_ENTER)
        m_budgetField->Navigate();

    UpdateBudget();
}

void ShoppingFrame::OnSave(wxCommandEvent&)
{
    wxFileDialog fd(this, "Save List", "", "ShoppingList.txt",
        "Text files (*.txt)|*.txt", wxFD_SAVE | wxFD_OVERWRITE_PROMPT);
    if (fd.ShowModal() == wxID_CANCEL) return;
    std::ofstream f(fd.GetPath().ToStdString());
    if (!f) { wxMessageBox("Could not write file.", "Error", wxOK | wxICON_ERROR, this); return; }
    for (auto& st : m_stores) {
        f << "===== " << st.name << " - " << st.address << " =====\n";
        for (auto& it : m_items) {
            if (it.store.name != st.name) continue;
            std::string tmp = it.temp == Room ? "Room" : it.temp == Cold ? "Cold" : "Freezer";
            f << "  [" << tmp << "] " << it.name << " x" << it.count
                << "  $" << std::fixed << std::setprecision(2) << it.price << "\n";
        }
        f << "\n";
    }
    double spent = TotalSpent();
    f << "Total: $" << std::fixed << std::setprecision(2) << spent << "\n";
    if (m_budget > 0) {
        double rem = m_budget - spent;
        f << "Budget: $" << std::fixed << std::setprecision(2) << m_budget << "\n";
        if (rem >= 0) f << "Remaining: $" << std::fixed << std::setprecision(2) << rem << "\n";
        else f << "OVER BUDGET by $" << std::fixed << std::setprecision(2) << -rem << "\n";
    }
    wxMessageBox("Saved to:\n" + fd.GetPath(), "Saved", wxOK | wxICON_INFORMATION, this);
}

void ShoppingFrame::OnClose(wxCloseEvent&) { Destroy(); }