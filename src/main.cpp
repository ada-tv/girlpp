#include <QCoreApplication>
#include <QThreadPool>
#include <QRunnable>

#include "stanza_stream.hpp"

using namespace girlpp;

int main(int argc, char *argv[]) {
    QCoreApplication::setOrganizationDomain("thingvellir.net");
    QCoreApplication::setApplicationName("girlpp");
    QCoreApplication app(argc, argv);

    auto stream = StanzaStream(Jid {
        .local = "ada",
        .domain = "thingvellir.net",
    }, "localhost", 5222);

    QObject::connect(&stream, &StanzaStream::readyRead, [&]() {
        stream.read();
    });

    QTimer::singleShot(5000, [&]() {
        stream.write(PresenceStanza {});

        auto msg = MessageStanza {};
        msg.from = Jid { .local = "ada", .domain = "thingvellir.net" };
        msg.body = "test message?";

        stream.write(msg);
    });

    return app.exec();
}
