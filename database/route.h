#ifndef ROUTE_H
#define ROUTE_H

#include <string>
#include <vector>
#include "Poco/JSON/Object.h"
#include <optional>

namespace database
{
    class Route{
        private:
            long _id;
            long _host_id;
            std::string _title;
            std::string _type;
            std::string _creation_date;
            std::string _start_point;
            std::string _finish_point;
            

        public:

            static Route fromJSON(const std::string & str);

            long get_id() const;
            long get_host_id() const;
            const std::string &get_title() const;
            const std::string &get_type() const;
            const std::string &get_creation_date() const;
            const std::string &get_start_point() const;
            const std::string &get_finish_point() const;

            long &id();
            long &host_id();
            std::string &title();
            std::string &type();
            std::string &creation_date();
            std::string &start_point();
            std::string &finish_point();

            static void init();
            static std::vector<Route> get_routes(long host_id);
            void save_to_mysql();

            Poco::JSON::Object::Ptr toJSON() const;

    };
}

#endif