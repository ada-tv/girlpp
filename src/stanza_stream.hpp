#ifndef girlpp_XmlStream_hpp
#define girlpp_XmlStream_hpp

#include <QXmlStreamReader>
#include <QXmlStreamWriter>
#include <QTcpSocket>
#include <QTimer>
#include <variant>

#include "jid.hpp"

namespace girlpp {

struct Stanza {
    virtual void write(QXmlStreamWriter *writer) const = 0;
};

struct MessageStanza : public Stanza {
    enum Type {
        Error,
        Chat,
        GroupChat, // invalid to actually use?
        Headline,
        Normal,
    };

    std::optional<QString> id;
    Type type { Normal };
    Jid from;
    std::optional<Jid> to { std::nullopt };
    QString body;
    std::optional<QString> subject { std::nullopt };
    std::optional<QString> thread { std::nullopt };
    std::optional<QString> threadParent { std::nullopt };

    void write(QXmlStreamWriter *writer) const override;
};

struct PresenceStanza : public Stanza {
    enum Type {
        Error,
        Probe,
        Subscribe,
        Subscribed,
        Unavailable,
        Unsubscribe,
        Unsubscribed,
    };

    enum Show {
        DoNotDisturb,
        ExtendedAway,
        Away,
        Chat,
    };

    std::optional<Type> type { std::nullopt };
    std::optional<QString> id { std::nullopt };
    std::optional<Jid> from { std::nullopt };
    std::optional<Jid> to { std::nullopt };
    int priority { 0 };
    std::optional<Show> show { std::nullopt };
    std::optional<QStringList> status { std::nullopt };

    void write(QXmlStreamWriter *writer) const override;
};

struct InfoQueryStanza : public Stanza {
    enum Type {
        Result,
        Error,
        Get,
        Set,
    };

    QString id;
    Type type;
    std::optional<Jid> from { std::nullopt };
    std::optional<Jid> to { std::nullopt };
    std::optional<std::vector<std::tuple<Jid, std::optional<QString>>>> rosterQuery { std::nullopt };

    void write(QXmlStreamWriter *writer) const override;
};

class StanzaStream : public QObject {
    Q_OBJECT

public:
    using MaybeStanza = std::optional<std::variant<MessageStanza, PresenceStanza, InfoQueryStanza>>;

    explicit StanzaStream(Jid srcJid, const QString &host, quint16 port);
    ~StanzaStream();

    void close();
    bool valid() const;

    MaybeStanza read();
    void write(const Stanza &stanza);

signals:
    void connected();
    void disconnected();
    void socketErrorOccured(QAbstractSocket::SocketError socketError);
    void streamErrorOccurred(QString error);
    void readyRead();

private:
    void open();

    QString _host;
    QString _srcJidString;
    Jid _srcJid {};
    bool _valid { false };
    bool _xmlValid { false };
    QTcpSocket *_socket { nullptr };
    QXmlStreamWriter *_writer { nullptr };
    QXmlStreamReader *_reader { nullptr };
    QTimer *_pingTimer { nullptr };

    int _streamDepth { 0 };
};

}
#endif // girlpp_XmlStream_hpp
