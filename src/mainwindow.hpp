#ifndef girlpp_mainwindow_hpp
#define girlpp_mainwindow_hpp

#include <QApplication>
#include <QMainWindow>
#include <QMenuBar>
#include <QHBoxLayout>
#include <QTreeView>

namespace girlpp {

class MainWindow : public QMainWindow {
private:
    QTreeView* m_TreeView;
public:
    MainWindow() : QMainWindow() {

        menuBar()->addMenu("File");

        auto* widget = new QWidget(this);
        auto* hbox = new QHBoxLayout(widget);

        m_TreeView = new QTreeView(widget);

        hbox->addWidget(m_TreeView);

        setCentralWidget(widget);
        widget->show();
    }

    ~MainWindow() = default;
};

}
#endif // girlpp_mainwindow_hpp
