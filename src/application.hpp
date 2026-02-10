#ifndef girlpp_application_hpp
#define girlpp_application_hpp

#include <QApplication>
#include <QMainWindow>

namespace girlpp {

class MainWindow : public QMainWindow {

};

class Application : public QApplication {
private:
    QMainWindow m_MainWindow;
    static constexpr const char* m_Version = GIRLPP_VERSION;
    static constexpr int m_VersionMajor = GIRLPP_VERSION_MAJOR;
    static constexpr int m_VersionMinor = GIRLPP_VERSION_MINOR;
    static constexpr int m_VersionPatch = GIRLPP_VERSION_PATCH;
public:
    Application(int& argc, char** argv) : QApplication(argc, argv) {
        QCoreApplication::setOrganizationDomain("thingvellir.net");
        QCoreApplication::setApplicationName("girlpp");
        QCoreApplication::setApplicationVersion( m_Version );
        SetMainWindowTitle("girlpp");
        SetMainWindowSize(640, 480);
        SetMainWindowVisible();
    }

    ~Application() = default;

    void SetMainWindowTitle(const QString& title) {
        m_MainWindow.setWindowTitle(title);
    }

    void SetMainWindowSize(int w, int h) {
        m_MainWindow.resize(w, h);
    }

    void SetMainWindowVisible() {
        m_MainWindow.show();
    }
};

}
#endif // girlpp_application_hpp
