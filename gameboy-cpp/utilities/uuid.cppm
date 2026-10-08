module;
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/uuid/uuid_generators.hpp>

export module utilities:uuid;
import std;

namespace utils
{
    export [[nodiscard]] std::string generate_uuid()
    {
        using namespace boost::uuids;

        const uuid id = random_generator()();
        return to_string(id);
    }
}
