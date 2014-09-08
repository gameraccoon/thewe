#include "LuaInstance.h"

#include <luabind/luabind.hpp>

#include "Log.h"
#include "Cell.h"

template<>
void LuaInstance::BindClass<Log>()
{
	luabind::module(_luaState) [
	luabind::class_<Log>("LogClass")
			.def("Log", &Log::writeLog)
			.def("Warning", &Log::writeWarning)
			.def("Error", &Log::writeError)
	];

	luabind::globals(_luaState)["Log"] = &(Log::Instance());
}
