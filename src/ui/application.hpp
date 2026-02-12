#ifndef girlpp_ui_application_hpp
#define girlpp_ui_application_hpp

#include <QApplication>
#include <QMainWindow>
#include <QMenuBar>
#include <QHBoxLayout>
#include <QTreeView>

#include "mainwindow.hpp"

namespace girlpp::ui {

class Application : public QApplication {
private:
    MainWindow m_MainWindow;
    static constexpr const char* m_Version = GIRLPP_VERSION;
    static constexpr int m_VersionMajor = GIRLPP_VERSION_MAJOR;
    static constexpr int m_VersionMinor = GIRLPP_VERSION_MINOR;
    static constexpr int m_VersionPatch = GIRLPP_VERSION_PATCH;
public:
    Application(int& argc, char** argv);
    ~Application() = default;
    void SetMainWindowTitle(const QString& title);
    void SetMainWindowSize(int w, int h);
    void SetMainWindowVisible();
};

}
#endif // girlpp_ui_application_hpp
