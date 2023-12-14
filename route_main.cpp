
#include <optional>
#include "route_service/http_route_service.h"

int main(int argc, char*argv[]) 
{
    HTTPRouteOtherWebServer app;
    return app.run(argc, argv);
}