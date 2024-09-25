#include <Geode/Geode.hpp>
using namespace geode::prelude;

#include "mhinfo.hpp"
#include "mhmenu.hpp"
#include "gui.hpp"
#include "colorfade.hpp"
#include "animationhandler.hpp"
#include "savedata.hpp"
#include "manager.hpp"

$on_mod(Loaded) {
    MHRecolor::Manager::Init();
    MHRecolor::Manager::LoadSave();
    MHRecolor::Manager::StartGUIThread();
}

$on_mod(DataSaved) {
    MHRecolor::Manager::SaveSave();
}