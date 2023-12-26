#ifndef ROUTEHANDLER_H
#define ROUTEHANDLER_H

#include "../../database/route.h"
#include "../../helper.h"

#include <iostream>
#include <ctime>
#include <iomanip>

#include "Poco/Net/HTTPServer.h"
#include "Poco/Net/HTTPRequestHandler.h"
#include "Poco/Net/HTTPRequestHandlerFactory.h"
#include "Poco/Net/HTTPServerParams.h"
#include "Poco/Net/HTTPServerRequest.h"
#include "Poco/Net/HTTPServerResponse.h"
#include "Poco/Net/HTTPServerParams.h"
#include "Poco/Net/HTMLForm.h"
#include "Poco/Net/ServerSocket.h"
#include "Poco/Timestamp.h"
#include "Poco/DateTimeFormatter.h"
#include "Poco/DateTimeFormat.h"
#include "Poco/Exception.h"
#include "Poco/ThreadPool.h"
#include "Poco/Util/ServerApplication.h"
#include "Poco/Util/Option.h"
#include "Poco/Util/OptionSet.h"
#include "Poco/Util/HelpFormatter.h"
#include <iostream>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>


using Poco::DateTimeFormat;
using Poco::DateTimeFormatter;
using Poco::ThreadPool;
using Poco::Timestamp;
using Poco::Net::HTMLForm;
using Poco::Net::HTTPRequestHandler;
using Poco::Net::HTTPRequestHandlerFactory;
using Poco::Net::HTTPServer;
using Poco::Net::HTTPServerParams;
using Poco::Net::HTTPServerRequest;
using Poco::Net::HTTPServerResponse;
using Poco::Net::NameValueCollection;
using Poco::Net::ServerSocket;
using Poco::Util::Application;
using Poco::Util::HelpFormatter;
using Poco::Util::Option;
using Poco::Util::OptionCallback;
using Poco::Util::OptionSet;
using Poco::Util::ServerApplication;

static bool hasSubstrRoute(const std::string &str, const std::string &substr)
{
    if (str.size() < substr.size())
        return false;
    for (size_t i = 0; i <= str.size() - substr.size(); ++i)
    {
        bool ok{true};
        for (size_t j = 0; ok && (j < substr.size()); ++j)
            ok = (str[i + j] == substr[j]);
        if (ok)
            return true;
    }
    return false;
}


class RouteHandler : public HTTPRequestHandler
{
private:
    bool check_host_id(const long &host_id, std::string &reason)
    {
       

        if (host_id < 0)
        {
            reason = "Wrong id";
            return false;
        }       

        return true;
    };

    bool check_userroute(std::vector<std::string> &results, std::string &reason)
    {
       reason = results[0];
       return true;
    }

    bool check_creation_date(const std::string &creation_date, std::string &reason)
    {       
        
        if (creation_date.find(' ') != std::string::npos)
        {

            std::string m = "Creation date can't contain spaces ";            
            reason =  m + creation_date;
            return false;
        }
        

        return true;
    };

public:
    RouteHandler(const std::string &format) : _format(format)
    {
    }

    void handleRequest(HTTPServerRequest &request,
                       HTTPServerResponse &response)
    {
        HTMLForm form(request, request.stream());
        try
        {
            if (form.has("host_id") && (request.getMethod() == Poco::Net::HTTPRequest::HTTP_GET))
            {
                long host_id = atol(form.get("host_id").c_str());

                auto results = database::Route::get_routes(host_id);
                Poco::JSON::Array arr;
                for (auto s : results)
                    arr.add(s.toJSON());
                response.setStatus(Poco::Net::HTTPResponse::HTTP_OK);
                response.setChunkedTransferEncoding(true);
                response.setContentType("application/json");
                std::ostream &ostr = response.send();
                Poco::JSON::Stringifier::stringify(arr, ostr);
               
            }
            if (request.getMethod() == Poco::Net::HTTPRequest::HTTP_POST) 
            {
                if (form.has("host_id") && form.has("title") && form.has("type") && form.has("creation_date") && form.has("start_point") && form.has("finish_point"))
                {
                    database::Route route;
                    std::cout << "route+" << std::endl;
                    route.host_id() =  atol(form.get("host_id").c_str()); 
                    std::cout << route.host_id() << "+" << std::endl;     
                          
                   
                    route.title() = form.get("title");
                    std::cout << route.title() << "+" << std::endl; 
                    route.type() = form.get("type");
                    std::cout << route.type() << "+" << std::endl; 
                    route.creation_date() = form.get("creation_date");
                    std::cout << route.creation_date() << "+-" << std::endl;
                    route.start_point() = form.get("start_point");
                    std::cout << route.start_point() << "+" << std::endl; 
                    route.finish_point() = form.get("finish_point");
                    std::cout << route.start_point() << "+" << std::endl; 

                    
                   

                
                
                        route.save_to_mysql();
                        std::cout << "save+" << std::endl;
                        response.setStatus(Poco::Net::HTTPResponse::HTTP_OK);
                        response.setChunkedTransferEncoding(true);
                        response.setContentType("application/json");
                        std::ostream &ostr = response.send();
                        ostr << route.get_id();
                        return;
                }
            }
        }
        catch (...)
        {
                std::string message;
                std::string reason;
                
                if (1 == 1)
                    {                        
                        message += "запрос не работает";
                        message += "<br>";
                        response.setStatus(Poco::Net::HTTPResponse::HTTP_NOT_FOUND);
                        std::ostream &ostr = response.send();
                        ostr << message;
                        response.send();
                        return;
                    }
        }

        response.setStatus(Poco::Net::HTTPResponse::HTTPStatus::HTTP_NOT_FOUND);
        response.setChunkedTransferEncoding(true);
        response.setContentType("application/json");
        Poco::JSON::Object::Ptr root = new Poco::JSON::Object();
        root->set("type", "/errors/not_found");
        root->set("title", "Internal exception");
        root->set("status", Poco::Net::HTTPResponse::HTTPStatus::HTTP_NOT_FOUND);
        root->set("detail", "request not found");
        root->set("instance", "/route");
        std::ostream &ostr = response.send();
        Poco::JSON::Stringifier::stringify(root, ostr);
    }

private:
    std::string _format;
};
#endif