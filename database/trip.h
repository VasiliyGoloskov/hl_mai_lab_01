#ifndef TRIP_H
#define TRIP_H

#include <string>
#include <vector>
#include "Poco/JSON/Object.h"
#include <optional>

namespace database
{
    class Trip{
        private:
            long _id;
            long _host_id;
            long _route_id;
            std::string _name;
            std::string _type;
            std::string _trip_date;
            

        public:

            static Trip fromJSON(const std::string & str);

            long get_id() const;
            long get_host_id() const;
            long get_route_id() const;
            const std::string &get_name() const;
            const std::string &get_type() const;
            const std::string &get_trip_date() const;

            long &id();
            long &host_id();
            long &route_id();
            std::string &name();
            std::string &type();
            std::string &trip_date();

            static void init();
            static std::optional<Trip> get_trip(long id);
            static std::vector<Trip> get_all_trips();
            void save_to_mysql();

            Poco::JSON::Object::Ptr toJSON() const;

    };
}

#endif