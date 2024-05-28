#pragma once

#include <iostream>

#include <Poco/Net/HTTPRequestHandler.h>
#include <Poco/Net/HTTPRequestHandlerFactory.h>

#include "handlers/delivery_handler.h"


class HTTPRequestFactory: public Poco::Net::HTTPRequestHandlerFactory {
public:
    HTTPRequestFactory(const std::string& format): _format(format) {}

    Poco::Net::HTTPRequestHandler* createRequestHandler(const Poco::Net::HTTPServerRequest& request) {
        std::cout << "request:" << request.getURI() << std::endl;
        auto uri = Poco::URI(request.getURI());
        
        std::vector<std::string> path_segments;
        uri.getPathSegments(path_segments);

        if (!path_segments.empty() && path_segments[0] == "delivery") {
            return new DeliveryHandler(_format);
        } 

        return 0;
    }

private:
    std::string _format;
};
