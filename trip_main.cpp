
#include <optional>
#include "trip_service/http_trip_service.h"

int main(int argc, char*argv[]) 
{
    HTTPTripOtherWebServer app;
    return app.run(argc, argv);
}