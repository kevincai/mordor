#ifndef __MORDOR_HTTP_OAUTH_H__
#define __MORDOR_HTTP_OAUTH_H__
// Copyright (c) 2009 - Mozy, Inc.

#include <boost/function.hpp>

#include "broker.h"
#include "http.h"

namespace Mordor {
namespace HTTP {
namespace OAuth {

std::pair<std::string, std::string>
getTemporaryCredentials(RequestBroker::ptr requestBroker, const URI &uri,
    Method method, const std::string &signatureMethod,
    const std::pair<std::string, std::string> &clientCredentials,
    const URI &callbackUri = URI());

std::pair<std::string, std::string>
getTokenCredentials(RequestBroker::ptr requestBroker, const URI &uri,
    Method method, const std::string signatureMethod,
    const std::pair<std::string, std::string> &clientCredentials,
    const std::pair<std::string, std::string> &temporaryCredentials,
    const std::string &verifier);

void authorize(Request &nextRequest,
    const std::string &signatureMethod,
    const std::pair<std::string, std::string> &clientCredentials,
    const std::pair<std::string, std::string> &tokenCredentials,
    const std::string &realm = std::string());

// Helpers for setting up an OAuth request
template <class T>
void nonceAndTimestamp(T &oauthParameters);

template <class T>
void sign(const URI &uri, Method method,
    const std::string &signatureMethod, const std::string &clientSecret,
    const std::string &tokenSecret, T &oauthParameters,
    const URI::QueryString &postParameters = URI::QueryString());

}}}

#endif
