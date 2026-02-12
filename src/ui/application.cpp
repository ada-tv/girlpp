
#include "application.hpp"

using namespace girlpp;
using namespace girlpp::ui;

Application::Application(int& argc, char** argv) : QApplication(argc, argv) {
    QCoreApplication::setOrganizationDomain("thingvellir.net");
    QCoreApplication::setApplicationName("girlpp");
    QCoreApplication::setApplicationVersion( m_Version );
    SetMainWindowTitle("girlpp");
    SetMainWindowSize(640, 480);
    SetMainWindowVisible();
}

void Application::SetMainWindowTitle(const QString& title) {
    m_MainWindow.setWindowTitle(title);
}

void Application::SetMainWindowSize(int w, int h) {
    m_MainWindow.resize(w, h);
}

void Application::SetMainWindowVisible() {
    m_MainWindow.show();
}
