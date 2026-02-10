#include "stanza_stream.hpp"

using namespace girlpp;

constexpr int KEEPALIVE_INTERVAL_MS = 15 * 1000;

StanzaStream::StanzaStream(Jid srcJid, const QString &host, quint16 port) : QObject() {
    assert(srcJid.valid() && srcJid.local.has_value());

    _srcJid = srcJid;
    _srcJidString = *_srcJid.toString();

    _host = host;

    _socket = new QTcpSocket(this);
    _socket->connectToHost(_host, port);

    _writer = new QXmlStreamWriter(_socket);

    // not attached to the socket, we feed data in when readyRead fires
    _reader = new QXmlStreamReader();

    _pingTimer = new QTimer(this);
    _pingTimer->setInterval(KEEPALIVE_INTERVAL_MS);

    connect(_socket, &QTcpSocket::connected, [this]() {
        _valid = true;
        emit connected();
        open();
    });

    connect(_socket, &QTcpSocket::disconnected, [this]() {
        _valid = false;
        emit disconnected();
    });

    connect(_socket, &QTcpSocket::errorOccurred, [this](QAbstractSocket::SocketError socketError) {
        _valid = false;
        emit socketErrorOccured(socketError);
    });

    connect(_socket, &QTcpSocket::readyRead, [this]() {
        _reader->addData(_socket->readAll());
        emit readyRead();
    });
}

StanzaStream::~StanzaStream() {
    close();
    delete _reader;
    delete _writer;
    delete _socket;
}

bool StanzaStream::valid() const {
    return (_socket && _socket->isValid()) && _valid;
}

void StanzaStream::open() {
    if (!valid()) { return; }

    _writer->writeStartDocument();
    _writer->writeStartElement("stream:stream");
    _writer->writeAttribute("from", _srcJidString);
    _writer->writeAttribute("to", _host);
    _writer->writeAttribute("version", "1.0");
    _writer->writeAttribute("xml:lang", "en");
    _writer->writeAttribute("xmlns", "jabber:client");
    _writer->writeAttribute("xmlns:stream", "http://etherx.jabber.org/streams");

    _writer->writeStartElement("stream:features");
        _writer->writeStartElement("mechanisms");
        _writer->writeAttribute("xmlns", "urn:ietf:params:xml:ns:xmpp-sasl");
        _writer->writeTextElement("mechanism", "ANONYMOUS");
        _writer->writeEndElement();
    _writer->writeEndElement();

    // TODO: only do whitespace keepalive after the stream negotiation
    connect(_pingTimer, &QTimer::timeout, [this]() {
        if (!valid()) { return; }
        _socket->write(" ");
        qDebug("<!-- keepalive -->");
    });
    _pingTimer->start(KEEPALIVE_INTERVAL_MS);
}

void StanzaStream::close() {
    if (valid()) {
        _writer->writeEndDocument();
        _socket->close();
    }
    _valid = false;
}

StanzaStream::MaybeStanza StanzaStream::read() {
    if (!_valid) { return std::nullopt; }

    auto startDepth = _streamDepth;

    while (!_reader->atEnd()) {
        auto indent = QString(_streamDepth * 4, ' ');
        _reader->readNext();

        if (_reader->isStartElement()) {
            _streamDepth += 1;

            if (_reader->attributes().size() > 0) {
                qInfo("%s<%s", qPrintable(indent), qPrintable(QString(_reader->qualifiedName())));

                for (auto attr : _reader->attributes()) {
                    qInfo("%s  %s=\"%s\"", qPrintable(indent), qPrintable(QString(attr.qualifiedName())), qPrintable(QString(attr.value())));
                }
                qInfo("%s>", qPrintable(indent));
            } else {
                qInfo("%s<%s>", qPrintable(indent), qPrintable(QString(_reader->qualifiedName())));
            }
        }

        if (_reader->isCharacters()) {
            qInfo("%s", qPrintable(QString(_reader->text())));
        }

        if (_reader->isEndElement()) {
            _streamDepth -= 1;
            indent = QString(_streamDepth * 4, ' ');
            qInfo("%s</%s>", qPrintable(indent), qPrintable(QString(_reader->qualifiedName())));

            if (_streamDepth == startDepth) { break; }
        }
    }

    if (_reader->hasError()) {
        return std::nullopt;
    }

    return std::nullopt;
}

void StanzaStream::write(const Stanza &stanza) {
    stanza.write(_writer);
}

void MessageStanza::write(QXmlStreamWriter *w) const {
    w->writeStartElement("message");

    switch (type) {
        case MessageStanza::Type::Error:
            w->writeAttribute("type", "error");
            break;
        case MessageStanza::Type::Chat:
            w->writeAttribute("type", "chat");
            break;
        case MessageStanza::Type::GroupChat:
            w->writeAttribute("type", "groupchat");
            break;
        case MessageStanza::Type::Headline:
            w->writeAttribute("type", "headline");
            break;
        case MessageStanza::Type::Normal:
            // normal is the default
            break;
    }

    if (id.has_value()) {
        w->writeAttribute("id", *id);
    }

    w->writeAttribute("from", *from.toString());

    if (to.has_value()) {
        w->writeAttribute("to", *to->toString());
    }

    if (subject.has_value()) {
        w->writeTextElement("subject", *subject);
    }

    w->writeTextElement("body", body);

    if (thread.has_value()) {
        w->writeStartElement("thread");

        if (threadParent.has_value()) {
            w->writeAttribute("parent", *threadParent);
        }

        w->writeEndElement();
    }

    w->writeEndElement();
}

void PresenceStanza::write(QXmlStreamWriter *w) const {
    w->writeStartElement("presence");

    if (type.has_value()) {
        switch (*type) {
            case PresenceStanza::Type::Error:
              w->writeAttribute("type", "error");
              break;
            case PresenceStanza::Type::Probe:
              w->writeAttribute("type", "probe");
              break;
            case PresenceStanza::Type::Subscribe:
              w->writeAttribute("type", "subscribe");
              break;
            case PresenceStanza::Type::Subscribed:
              w->writeAttribute("type", "subscribed");
              break;
            case PresenceStanza::Type::Unavailable:
              w->writeAttribute("type", "unavailable");
              break;
            case PresenceStanza::Type::Unsubscribe:
              w->writeAttribute("type", "unsubscribe");
              break;
            case PresenceStanza::Type::Unsubscribed:
              w->writeAttribute("type", "unsubscribed");
              break;
        }
    }

    if (from.has_value()) {
        w->writeAttribute("from", *from->toString());
    }

    if (to.has_value()) {
        w->writeAttribute("to", *to->toString());
    }

    if (show.has_value()) {
        switch (*show) {
            case PresenceStanza::Show::DoNotDisturb:
                w->writeTextElement("show", "dnd");
                break;
            case PresenceStanza::Show::ExtendedAway:
                w->writeTextElement("show", "xa");
                break;
            case PresenceStanza::Show::Away:
                w->writeTextElement("show", "away");
                break;
            case PresenceStanza::Show::Chat:
                w->writeTextElement("show", "chat");
                break;
        }
    }

    if (status.has_value()) {
        for (const auto &chunk : *status) {
            w->writeTextElement("status", chunk);
        }
    }

    if (priority != 0) {
        w->writeTextElement("priority", QString::number(priority));
    }

    w->writeEndElement();
}

void InfoQueryStanza::write(QXmlStreamWriter *w) const {
    w->writeStartElement("iq");

    switch (type) {
        case InfoQueryStanza::Type::Result:
          w->writeAttribute("type", "result");
          break;
        case InfoQueryStanza::Type::Error:
          w->writeAttribute("type", "error");
          break;
        case InfoQueryStanza::Type::Get:
          w->writeAttribute("type", "get");
          break;
        case InfoQueryStanza::Type::Set:
          w->writeAttribute("type", "set");
          break;
    }

    if (from.has_value()) {
        w->writeAttribute("from", *from->toString());
    }

    if (to.has_value()) {
        w->writeAttribute("to", *to->toString());
    }

    if (rosterQuery.has_value()) {
        w->writeStartElement("query");
        w->writeAttribute("xmlns", "jabber:iq:roster");

        for (const auto &item : *rosterQuery) {
            const auto &jid = std::get<0>(item);
            const auto &group = std::get<1>(item);

            w->writeStartElement("item");
            w->writeAttribute("jid", *jid.toString());
            if (group.has_value()) {
                w->writeTextElement("group", *group);
            }
            w->writeEndElement();
        }

        w->writeEndElement();
    }

    w->writeEndElement();
}
