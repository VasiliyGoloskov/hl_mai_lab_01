#ifndef TRIPHANDLER_H
#define TRIPHANDLER_H

#include "../../database/trip.h"
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

static bool hasSubstrTrip(const std::string &str, const std::string &substr)
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


class TripHandler : public HTTPRequestHandler
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
     bool check_route_id(const long &route_id, std::string &reason)
    {
       

        if (route_id < 0)
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

    bool check_trip_date(const std::string &creation_date, std::string &reason)
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
    TripHandler(const std::string &format) : _format(format)
    {
    }

 /*   Poco::JSON::Object::Ptr remove_password(Poco::JSON::Object::Ptr src)
    {
        if (src->has("password"))
            src->set("password", "*******");
        return src;
    }
*/
 void handleRequest(HTTPServerRequest &request,
                       HTTPServerResponse &response)
    {
        HTMLForm form(request, request.stream());
        try
        {
            if (form.has("id") && (request.getMethod() == Poco::Net::HTTPRequest::HTTP_GET))
            {
                long host_id = atol(form.get("host_id").c_str());

                std::optional<database::Trip> result = database::Trip::get_trip(host_id);
                if (result)
                {
                    response.setStatus(Poco::Net::HTTPResponse::HTTP_OK);
                    response.setChunkedTransferEncoding(true);
                    response.setContentType("application/json");
                    //std::ostream &ostr = response.send();
                    //Poco::JSON::Stringifier::stringify(remove_password(result->toJSON()), ostr);
                    return;
                }
                else
                {
                    response.setStatus(Poco::Net::HTTPResponse::HTTPStatus::HTTP_NOT_FOUND);
                    response.setChunkedTransferEncoding(true);
                    response.setContentType("application/json");
                    Poco::JSON::Object::Ptr root = new Poco::JSON::Object();
                    root->set("type", "/errors/not_found");
                    root->set("title", "Internal exception");
                    root->set("status", "404");
                    root->set("detail", "user not found");
                    root->set("instance", "/user");
                    std::ostream &ostr = response.send();
                    Poco::JSON::Stringifier::stringify(root, ostr);
                    return;
                }
            }
            if (request.getMethod() == Poco::Net::HTTPRequest::HTTP_POST) 
            {
                if (form.has("host_id") && form.has("route_id") && form.has("name") && form.has("trip_date"))
                {
                    database::Trip trip;
                 
                    trip.host_id() =  atol(form.get("host_id").c_str());      
                    time_t now = time(0);
    
                    // В таком формате мы получаем дату. Sat Nov 18 21:34:02 2023
                    std::string temp_value = ctime(&now);

                    // Создаем объект структуры tm для хранения времени
                    std::tm date = {};
                    /*
                    // Парсим дату из строки в структуру tm
                    std::istringstream ss(temp_value);
                    ss >> std::get_time(&date, "%a %b %d %H:%M:%S %Y");
                    
                    if (ss.fail()) {
                        std::cout << "Ошибка при парсинге даты" << std::endl;                        
                    }
                    */
                    // Преобразуем дату в нужный формат
                    std::ostringstream oss;
                    oss << std::put_time(&date, "%Y-%m-%d %H:%M:%S");
                    
                    std::string output_date = oss.str();
                                        
                    trip.trip_date() = output_date;

                    bool check_result = true;
                    std::string message;
                    std::string reason;
                    
                    trip.route_id() = atol(form.get("route_id").c_str());
                    trip.name() = form.get("name");
                    trip.type() = form.get("type");
        

                   /*

                    if (!check_user_id(visit.get_user_id(), reason))
                    {
                        check_result = false;
                        message += reason;
                        message += "<br>";
                    }

                    if (!check_datetime(visit.datetime(), reason))
                    {
                        check_result = false;
                        message += reason;
                        message += "<br>";
                    }
                    */
                    /*

                    if (!check_email(user.get_email(), reason))
                    {
                        check_result = false;
                        message += reason;
                        message += "<br>";
                    }
                    */

                    if (check_result)
                    {
                        trip.save_to_mysql();
                        response.setStatus(Poco::Net::HTTPResponse::HTTP_OK);
                        response.setChunkedTransferEncoding(true);
                        response.setContentType("application/json");
                        std::ostream &ostr = response.send();
                        ostr << trip.get_id();
                        return;
                        
                    }
                    else
                    {
                        response.setStatus(Poco::Net::HTTPResponse::HTTP_NOT_FOUND);
                        std::ostream &ostr = response.send();
                        ostr << message;
                        response.send();
                        return;
                    }
                }
            }
        }
        catch (...)
        {
                std::string message;
                std::string reason;
                
                if (1 == 1)
                    {                        
                        message += "не стаботало!";
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
        root->set("detail", "request ot found");
        root->set("instance", "/trip");
        std::ostream &ostr = response.send();
        Poco::JSON::Stringifier::stringify(root, ostr);
    }
    private:
    std::string _format;
};
#endif