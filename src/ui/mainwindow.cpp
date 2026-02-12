
#include "mainwindow.hpp"

using namespace girlpp;
using namespace girlpp::ui;

MainWindow::MainWindow() : QMainWindow() {
    menuBar()->addMenu("File");

    auto* widget = new QWidget(this);
    auto* hbox = new QHBoxLayout(widget);

    m_TreeView = new QTreeView(widget);

    hbox->addWidget(m_TreeView);

    setCentralWidget(widget);
    widget->show();
}
