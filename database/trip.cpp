#include "trip.h"
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

    void Trip::init()
    {
        try
        {

            Poco::Data::Session session = database::Database::get().create_session();
            Statement create_route(session);
            create_route << "CREATE TABLE IF NOT EXISTS `Trip` (`id` INT NOT NULL AUTO_INCREMENT PRIMARY KEY,"
                        << "`host_id` INT NOT NULL,"
                        << "`route_id` INT NOT NULL,"
                        << "`name` VARCHAR(256) NOT NULL,"
                        << "`type` VARCHAR(256) NOT NULL,"
                        << "`trip_date` VARCHAR(256) NOT NULL);",
                now;
          std::cout << "Trip table initialized successfully." << std::endl;
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

    Poco::JSON::Object::Ptr Trip::toJSON() const
    {
        Poco::JSON::Object::Ptr root = new Poco::JSON::Object();

        root->set("id", _id);
        root->set("host_id", _host_id);
        root->set("route_id", _route_id);
        root->set("name", _name);
        root->set("type", _type);
        root->set("trip_date", _trip_date);

        return root;
    }

    Trip Trip::fromJSON(const std::string &str)
    {
        Trip trip;
        Poco::JSON::Parser parser;
        Poco::Dynamic::Var result = parser.parse(str);
        Poco::JSON::Object::Ptr object = result.extract<Poco::JSON::Object::Ptr>();

        trip.id() = object->getValue<long>("id");
        trip.host_id() = object->getValue<long>("host_id");
        trip.route_id() = object->getValue<long>("route_id");
        trip.name() = object->getValue<std::string>("name");
        trip.type() = object->getValue<std::string>("type");
        trip.trip_date() = object->getValue<std::string>("trip_date");

        return trip;
    }

     std::optional<Trip> Trip::get_trip(long id)
    {
        try
        {
            Poco::Data::Session session = database::Database::get().create_session();
            Poco::Data::Statement select(session);
            Trip t;
            select << "SELECT id, host_id, route_id, name, type, trip_date FROM Trip where id=?",
                into(t._id),
                into(t._host_id),
                into(t._route_id),
                into(t._name),
                into(t._type),
                into(t._trip_date),
                use(id),
                range(0, 1); //  iterate over result set one row at a time

            select.execute();
            Poco::Data::RecordSet rs(select);
            if (rs.moveFirst()) return t;
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
     std::vector<Trip> Trip::get_all_trips()
    {
 try
        {
            Poco::Data::Session session = database::Database::get().create_session();
            Poco::Data::Statement select(session);
            Trip t;
            std::vector<Trip> result;
            select << "SELECT id, host_id, route_id, name, type, trip_date FROM Trip",
                into(t._id),
                into(t._host_id),
                into(t._route_id),
                into(t._name),
                into(t._type),
                into(t._trip_date),
                range(0, 1); //  iterate over result set one row at a time

             while (!select.done())
            {
                if (select.execute())
                    result.push_back(t);
            }
            return result;
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

    void Trip::save_to_mysql()
    {

        try
        {
            Poco::Data::Session session = database::Database::get().create_session();
            std::cout << "session+" << std::endl;
            Poco::Data::Statement insert(session);
            std::cout << "insert_session+" << std::endl;

            insert << "INSERT INTO Trip (host_id,route_id,name,type,trip_date) VALUES(?, ?, ?, ?, ?)",
                use(_host_id),
                use(_route_id),
                use(_name),
                use(_type),
                use(_trip_date);

            insert.execute();
            std::cout << "insert+" << std::endl;
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

    long Trip::get_id() const
    {
        return _id;
    }

    long Trip::get_host_id() const
    {
        return _host_id;
    }

   long Trip::get_route_id() const
    {
        return _route_id;
    }
    const std::string &Trip::get_name() const
    {
        return _name;
    }
    const std::string &Trip::get_type() const
    {
        return _type;
    }
    const std::string &Trip::get_trip_date() const
    {
        return _trip_date;
    }

  
    long &Trip::id()
    {
        return _id;
    }

    long &Trip::host_id()
    {
        return _host_id;
    }
    
    long &Trip::route_id()
    {
        return _route_id;
    }
    std::string &Trip::name()
    {
        return _name;
    }
    std::string &Trip::type()
    {
        return _type;
    }
    std::string &Trip::trip_date()
    {
        return _trip_date;
    }
}