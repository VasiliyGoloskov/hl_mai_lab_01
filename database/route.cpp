#include "route.h"
#include "database.h"
#include "../config/config.h"

#include <Poco/Data/MySQL/Connector.h>
#include <Poco/Data/MySQL/MySQLException.h>
#include <Poco/Data/SessionFactory.h>
#include <Poco/Data/RecordSet.h>
#include <Poco/JSON/Parser.h>
#include <Poco/Dynamic/Var.h>

#include <sstream>
#include <exception>

using namespace Poco::Data::Keywords;
using Poco::Data::Session;
using Poco::Data::Statement;

namespace database
{

    void Route::init()
    {
        try
        {

            Poco::Data::Session session = database::Database::get().create_session();
            Statement create_route(session);
            create_route << "CREATE TABLE IF NOT EXISTS `Route` (`id` INT NOT NULL AUTO_INCREMENT,"
                        << "`host_id` VARCHAR(256) NOT NULL,"
                        << "`title` VARCHAR(256) NOT NULL,"
                        << "`type` VARCHAR(256) NOT NULL,"
                        << "`creation_date` DATETIME NOT NULL,"
                        << "`start_point` VARCHAR(256) NOT NULL,"
                        << "`finish_point` VARCHAR(256) NOT NULL,"
                        << "PRIMARY KEY (`id`)",
                now;
        }

        catch (Poco::Data::MySQL::ConnectionException &e)
        {
            std::cout << "connection:" << e.what() << std::endl;
            throw;
        }
        catch (Poco::Data::MySQL::StatementException &e)
        {

            std::cout << "statement:" << e.what() << std::endl;
            throw;
        }
    }

    Poco::JSON::Object::Ptr Route::toJSON() const
    {
        Poco::JSON::Object::Ptr root = new Poco::JSON::Object();

        root->set("id", _id);
        root->set("host_id", _host_id);
        root->set("tytle", _tytle);
        root->set("type", _type);
        root->set("creation_date", _creation_date);
        root->set("start_point", _start_point);
        root->set("finish_point", _finish_point);

        return root;
    }

    Route Route::fromJSON(const std::string &str)
    {
        Route route;
        Poco::JSON::Parser parser;
        Poco::Dynamic::Var result = parser.parse(str);
        Poco::JSON::Object::Ptr object = result.extract<Poco::JSON::Object::Ptr>();

        route.id() = object->getValue<long>("id");
        route.host_id() = object->getValue<long>("host_id");
        route.tytle() = object->getValue<std::string>("tytle");
        route.type() = object->getValue<std::string>("type");
        route.creation_date() = object->getValue<std::string>("creation_date");
        route.start_point() = object->getValue<std::string>("start_point");
        route.finish_point() = object->getValue<std::string>("finish_point");

        return route;
    }

    std::optional<Route> Route::get_routes(long host_id)
    {
        try
        {
            Poco::Data::Session session = database::Database::get().create_session();
            Poco::Data::Statement select(session);
            Route a;
            select << "SELECT id, host_id, tytle, type, creation_date, start_point, finish_point FROM User where host_id=?",
                into(a._id),
                into(a._host_id),
                into(a._tytle),
                into(a._type),
                into(a._creation_date),
                into(a._start_point),
                into(a._finish_point),
                use(host_id),
                range(0, 1); //  iterate over result set one row at a time

            select.execute();
            Poco::Data::RecordSet rs(select);
            if (rs.moveFirst()) return a;
        }

        catch (Poco::Data::MySQL::ConnectionException &e)
        {
            std::cout << "connection:" << e.what() << std::endl;
        }
        catch (Poco::Data::MySQL::StatementException &e)
        {

            std::cout << "statement:" << e.what() << std::endl;
            
        }
        return {};
    }

    void Route::save_to_mysql()
    {

        try
        {
            Poco::Data::Session session = database::Database::get().create_session();
            Poco::Data::Statement insert(session);

            insert << "INSERT INTO Route (host_id,tytle,type,creation_date,start_point,finish_point) VALUES(?, ?, ?, ?, ?, ?)",
                use(_host_id),
                use(_tytle),
                use(_type);
                use(_creation_date),
                use(_start_point),
                use(_finish_point);

            insert.execute();

            Poco::Data::Statement select(session);
            select << "SELECT LAST_INSERT_ID()",
                into(_id),
                range(0, 1); //  iterate over result set one row at a time

            if (!select.done())
            {
                select.execute();
            }
            std::cout << "inserted:" << _id << std::endl;
        }
        catch (Poco::Data::MySQL::ConnectionException &e)
        {
            std::cout << "connection:" << e.what() << std::endl;
            throw;
        }
        catch (Poco::Data::MySQL::StatementException &e)
        {

            std::cout << "statement:" << e.what() << std::endl;
            throw;
        }
    }

    long Route::get_id() const
    {
        return _id;
    }

    long Route::get_host_id() const
    {
        return _host_id;
    }

    const std::string &Route::get_tytle() const
    {
        return _tytle;
    }
    const std::string &Route::get_type() const
    {
        return _type;
    }
    const std::string &Route::get_creation_date() const
    {
        return _creation_date;
    }
    const std::string &Route::get_start_point() const
    {
        return _start_point;
    }

    const std::string &Route::get_finish_point() const
    {
        return _finish_point;
    }


    long &Route::id()
    {
        return _id;
    }

    long &Route::host_id()
    {
        return _host_id;
    }

    std::string &Route::tytle()
    {
        return _tytle;
    }
    std::string &Route::type() 
    {
        return _type;
    }
    std::string &Route::creation_date()
    {
        return _creation_date;
    }
    std::string &Route::start_point()
    {
        return _start_point;
    }

    std::string &Route::finish_point()
    {
        return _finish_point;
    }
}