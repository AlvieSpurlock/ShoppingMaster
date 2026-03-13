// Copyright © Mad Tea Party Development 2025
// Shopping Master — wxWidgets GUI

#include "ShoppingApp.h"
#include "ShoppingFrame.h"

wxIMPLEMENT_APP(ShoppingApp);

bool ShoppingApp::OnInit()
{
    SetAppName("Shopping Master");
    SetAppDisplayName("Shopping Master");
    ShoppingFrame* frame = new ShoppingFrame();
    frame->Show(true);
    return true;
}
