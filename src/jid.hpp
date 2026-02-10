#ifndef girlpp_JID_hpp
#define girlpp_JID_hpp

#include <optional>
#include <QString>

namespace girlpp {

struct Jid {
    // local@domain/resource
    std::optional<QString> local;
    QString domain;
    std::optional<QString> resource;

    bool valid() const;
    std::optional<QString> toString() const;

    static std::optional<Jid> tryParse(const QStringView &src);
};

}
#endif // girlpp_JID_hpp
