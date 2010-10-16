/*
** Taiga, a lightweight client for MyAnimeList
** Copyright (C) 2010, Eren Okka
** 
** This program is free software: you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation, either version 3 of the License, or
** (at your option) any later version.
** 
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
** 
** You should have received a copy of the GNU General Public License
** along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef DLG_FILTER_H
#define DLG_FILTER_H

#include "../std.h"
#include "../ui/ui_control.h"
#include "../ui/ui_dialog.h"

// =============================================================================

/* Filter dialog */

class CFilterWindow : public CDialog {
public:
  CFilterWindow();
  ~CFilterWindow() {}

  void RefreshFilters();
  
  BOOL OnCommand(WPARAM wParam, LPARAM lParam);
  BOOL OnInitDialog();
  BOOL PreTranslateMessage(MSG* pMsg);
};

extern CFilterWindow FilterWindow;

#endif // DLG_FILTER_H