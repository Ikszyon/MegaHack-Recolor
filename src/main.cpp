#include <Geode/Geode.hpp>
using namespace geode::prelude;

#include "manager.hpp"

$on_mod(Loaded) {
    MHRecolor::Manager::Init();
    MHRecolor::Manager::LoadSave();
    MHRecolor::Manager::StartGUIThread();
}

$on_mod(DataSaved) {
    MHRecolor::Manager::SaveSave();
}