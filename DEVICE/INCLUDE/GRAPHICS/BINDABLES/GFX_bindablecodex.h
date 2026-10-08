#pragma once

#include "GFX_bindable.h"

#include <string>
#include <unordered_map>
#include <type_traits>

// BindableCodex stores all of the bindables that have been created as shared_ptr
// and enables sharing bindables between drawables.
// This class works as a singleton.
class BindableCodex
{
public:
	// These static methods are used from other classes to interact with the codex.
	template<class T, typename ...Params>
	static std::shared_ptr<T> Resolve(D3DClass* d3d, Params&&... p)
	{
		static_assert(std::is_base_of<Bindable, T>::value, "Can only resolve classes derived from Bindable");
		return Get().Resolve_<T>(d3d, std::forward<Params>(p)...);
	}

private:
	template<class T, typename ...Params>
	std::shared_ptr<T> Resolve_(D3DClass* d3d, Params&&... p)
	{
		const auto key = T::GenerateUID(std::forward<Params>(p)...);
		const auto i = m_bindsMap.find(key);
		if (i == m_bindsMap.end())
		{
			auto bind = std::make_shared<T>(d3d, std::forward<Params>(p)...);
			m_bindsMap[key] = bind;
			return bind;
		}
		else
		{
			return std::static_pointer_cast<T>(i->second);
		}
	}
	
	// Returns the single static instance of the codex.
	static BindableCodex& Get()
	{
		static BindableCodex codex;
		return codex;
	}

// A hash that maps a string to a shared bindable.
// Every bindable has a unique ID, a string, which is used to 
// determine if we have identical bindables that can be shared.
private:
	std::unordered_map<std::string, std::shared_ptr<Bindable>> m_bindsMap;
};

