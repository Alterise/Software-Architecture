#pragma once

#include <iostream>
#include <iostream>
#include <fstream>

#include "Poco/URI.h"
#include "Poco/Net/HTTPServerRequest.h"
#include "Poco/Net/HTTPServerResponse.h"
#include "Poco/Net/HTTPRequestHandler.h"
#include "Poco/Net/HTTPRequestHandlerFactory.h"
#include "Poco/Net/HTMLForm.h"

#include "utils/utils.h"
#include "web_server/checks.h"


class SecuredHandler : public Poco::Net::HTTPRequestHandler
{

public:
    SecuredHandler(const std::string& format, std::shared_ptr<Poco::Crypto::DigestEngine> digestEngine) :
    _format(format), _digestEngine(digestEngine) {}

    void handleRequest(Poco::Net::HTTPServerRequest &request,
                       Poco::Net::HTTPServerResponse &response)
    {
        Poco::Net::HTMLForm form(request, request.stream());
        try
        {
            std::string scheme;
            std::string info;
            long id {-1};
            std::string login;
            request.getCredentials(scheme, info);
            std::cout << "scheme: " << scheme << " identity: " << info << std::endl;
            if(scheme == "Bearer") {
                if(!extract_payload(info,id,login)) {
                    response.setStatus(Poco::Net::HTTPResponse::HTTPStatus::HTTP_FORBIDDEN);
                    response.setChunkedTransferEncoding(true);
                    response.setContentType("application/json");
                    Poco::JSON::Object::Ptr root = new Poco::JSON::Object();
                    root->set("type", "/errors/not_authorized");
                    root->set("title", "Internal exception");
                    root->set("status", "403");
                    root->set("detail", "user not authorized");
                    root->set("instance", "/pizza_order");
                    std::ostream &ostr = response.send();
                    Poco::JSON::Stringifier::stringify(root, ostr);
                    return;                   
                }
            }
            std::cout << "id:" << id << " login:" << login << std::endl;

            if (request.getMethod() == Poco::Net::HTTPRequest::HTTP_PUT) {
                if (form.has("login") && form.has("password")) {
                    database::User user;

                    user.login() = form.get("login");

                    _digestEngine->update(form.get("password"));
                    user.password() = _digestEngine->digestToHex(_digestEngine->digest());

                    if (!user.check_credentials()) {
                        response.setStatus(Poco::Net::HTTPResponse::HTTP_NOT_FOUND);

                        std::ostream& ostr = response.send();
                        ostr << "Invalid login or password";

                        response.send();

                        return;
                    }

                    if (form.has("new_login")) {
                        auto new_login = form.get("new_login");
                        if (!database::User::check_login_uniqueness(new_login)) {
                            response.setStatus(Poco::Net::HTTPResponse::HTTP_CONFLICT);

                            std::ostream& ostr = response.send();
                            ostr << "Login must be unique<br>";

                            response.send();

                            return;
                        }

                        user.update_login(new_login);

                        response.setStatus(Poco::Net::HTTPResponse::HTTP_OK);
                        response.setChunkedTransferEncoding(true);
                        response.setContentType("application/json");

                        std::ostream& ostr = response.send();
                        ostr << user.get_id();

                        return;
                    } else if (form.has("new_password")) {
                        user.update_password(form.get("new_password"));

                        response.setStatus(Poco::Net::HTTPResponse::HTTP_OK);
                        response.setChunkedTransferEncoding(true);
                        response.setContentType("application/json");

                        std::ostream& ostr = response.send();
                        ostr << user.get_id();

                        return;
                    } else if (form.has("new_name")) {
                        auto new_name = form.get("new_name");

                        std::string reason;
                        if (!check_name(new_name, reason)) {
                            response.setStatus(Poco::Net::HTTPResponse::HTTP_NOT_FOUND);

                            std::ostream& ostr = response.send();
                            ostr << reason + "<br>";

                            response.send();

                            return;
                        }

                        user.update_name(new_name);

                        response.setStatus(Poco::Net::HTTPResponse::HTTP_OK);
                        response.setChunkedTransferEncoding(true);
                        response.setContentType("application/json");

                        std::ostream& ostr = response.send();
                        ostr << user.get_id();

                        return;
                    } else if (form.has("new_surname")) {
                        auto new_surname = form.get("new_surname");

                        std::string reason;
                        if (!check_name(new_surname, reason)) {
                            response.setStatus(Poco::Net::HTTPResponse::HTTP_NOT_FOUND);

                            std::ostream& ostr = response.send();
                            ostr << reason + "<br>";

                            response.send();

                            return;
                        }

                        user.update_surname(new_surname);

                        response.setStatus(Poco::Net::HTTPResponse::HTTP_OK);
                        response.setChunkedTransferEncoding(true);
                        response.setContentType("application/json");

                        std::ostream& ostr = response.send();
                        ostr << user.get_id();

                        return;
                    } else if (form.has("new_email")) {
                        auto new_email = form.get("new_email");
                        
                        std::string reason;
                        if (!check_email(new_email, reason)) {
                            response.setStatus(Poco::Net::HTTPResponse::HTTP_NOT_FOUND);

                            std::ostream& ostr = response.send();
                            ostr << reason + "<br>";

                            response.send();

                            return;
                        }

                        user.update_email(new_email);

                        response.setStatus(Poco::Net::HTTPResponse::HTTP_OK);
                        response.setChunkedTransferEncoding(true);
                        response.setContentType("application/json");

                        std::ostream& ostr = response.send();
                        ostr << user.get_id();

                        return;
                    }
                }
            } else if (request.getMethod() == Poco::Net::HTTPRequest::HTTP_DELETE) {
                if (form.has("login") && form.has("password")) {
                    database::User user;

                    user.login() = form.get("login");

                    _digestEngine->update(form.get("password"));
                    user.password() = _digestEngine->digestToHex(_digestEngine->digest());

                    if (user.check_credentials()) {
                        user.remove();

                        response.setStatus(Poco::Net::HTTPResponse::HTTP_OK);
                        response.setChunkedTransferEncoding(true);
                        response.setContentType("application/json");

                        response.send();

                        return;
                    } else {
                        response.setStatus(Poco::Net::HTTPResponse::HTTP_NOT_FOUND);

                        std::ostream& ostr = response.send();
                        ostr << "Invalid login or password";

                        response.send();

                        return;
                    }
                }
            }
            
        }
        catch (std::exception &ex)
        {
            std::cout << "exception:" << ex.what() << std::endl;
        }

        response.setStatus(Poco::Net::HTTPResponse::HTTPStatus::HTTP_NOT_FOUND);
        response.setChunkedTransferEncoding(true);
        response.setContentType("application/json");
        Poco::JSON::Object::Ptr root = new Poco::JSON::Object();
        root->set("type", "/errors/not_found");
        root->set("title", "Internal exception");
        root->set("status", Poco::Net::HTTPResponse::HTTPStatus::HTTP_NOT_FOUND);
        root->set("detail", "request ot found");
        root->set("instance", "/pizza_order");
        std::ostream &ostr = response.send();
        Poco::JSON::Stringifier::stringify(root, ostr);
    }

private:
    std::string _format;
    std::shared_ptr<Poco::Crypto::DigestEngine> _digestEngine;
};
