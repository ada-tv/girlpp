#include "jid.hpp"

using namespace girlpp;

bool Jid::valid() const {
    // TODO
    return true;
}

std::optional<QString> Jid::toString() const {
    if (!valid()) { return std::nullopt; }

    // +1 for '@'
    auto local_len = local.has_value() ? local->size() + 1 : 0;

    // +1 for '/'
    auto resource_len = resource.has_value() ? resource->size() + 1 : 0;

    QString s;

    // +1 for '\x00'
    s.reserve(local_len + domain.size() + resource_len + 1);

    if (local.has_value()) {
        s += *local;
        s += '@';
    }

    s += domain;

    if (resource.has_value()) {
        s += '@';
        s += *resource;
    }

    return s;
}
