#ifndef girlpp_ui_mainwindow_hpp
#define girlpp_ui_mainwindow_hpp

#include <QApplication>
#include <QMainWindow>
#include <QMenuBar>
#include <QHBoxLayout>
#include <QTreeView>

namespace girlpp::ui {

class MainWindow : public QMainWindow {
private:
    QTreeView* m_TreeView;
public:
    MainWindow();
    ~MainWindow() = default;
};

}
#endif // girlpp_ui_mainwindow_hpp
