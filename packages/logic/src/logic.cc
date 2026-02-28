#include "logic.hh"

Logic::Logic()
{
}

void Logic::load(emscripten::val args)
{
    auto data = args["data"];
    auto pool = data["pool"];
    loadPoolData("oot", pool);
    loadPoolData("mm", pool);
}

void Logic::run()
{

}

void Logic::loadPoolData(const char* game, emscripten::val pool)
{
    auto poolData = pool[game];
    auto length = poolData["length"].as<unsigned>();
    for (unsigned i = 0; i < length; ++i) {
        auto entry = poolData[i];
        auto location = entry["location"].as<std::string>();
        regLocations.reg(location.c_str());
    }
}
