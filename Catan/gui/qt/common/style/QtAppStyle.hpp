#pragma once

#include <QApplication>

namespace catan::gui::qt {

// Central place to configure global Qt application styling (Fusion + palette + QSS).
// Call this once right after creating QApplication.
void applyDefaultQtStyle(QApplication& app);

}

