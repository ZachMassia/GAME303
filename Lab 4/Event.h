#pragma once

#include <type_traits>
#include <algorithm>
#include <functional>
#include <vector>

template <typename T>
class Event
{
private:
	std::vector<std::function<void (T)>> functions;

public:
	Event<T>& operator+=(std::function<void (T)>& f)
	{
		functions.push_back(f);
		return *this;
	}

	void RaiseEvent(T arg)
	{
		std::for_each(functions.begin(), functions.end(), [&] (std::function<void (T)>& f) {
			try {
				f(arg);
			} catch (...) {}
		});

		// Cleanup the argument if it was a pointer.
		if (std::is_pointer<T>::value) {
			delete arg;
		}
	}
};

