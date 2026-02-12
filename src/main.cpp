#include <QCoreApplication>
#include <QThreadPool>
#include <QRunnable>

#include "stanza_stream.hpp"
#include "ui/application.hpp"

using namespace girlpp;
using namespace girlpp::ui;

int main(int argc, char** argv) {
    Application app(argc, argv);

#if 0
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
#endif

    return app.exec();
}
