/*
SpaceNavigatorEmulator - 3dconnexion-compatible siappdll.dll replacement
Copyright (C) 2015  Luke Nuttall <lukenuttall@gmail.com>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#include "Timer.h"
#include <boost\bind.hpp>
#include "Log.h"

using namespace std;
using namespace boost;


namespace SpaceNavigatorEmulator
{
	Timer::Timer(int timeMilliseconds, std::function<void()>&& callback) :
		timer(io),
		timeout(timeMilliseconds),
		timeoutCallback(move(callback))
	{
		timer.expires_from_now(timeout);
		timer.async_wait(bind(&Timer::Timeout, this, asio::placeholders::error));
	}

	Timer::~Timer()
	{
	}

	void Timer::start()
	{
		if (timer_thread.joinable())
			stop();

		timer_thread = std::thread([&]
		{
			io.reset();
			io.run();
		});
	}

	void Timer::stop()
	{
		if (!io.stopped())
			io.stop();

		if (timer_thread.joinable())
			timer_thread.join();
	}

	void Timer::Timeout(const boost::system::error_code& error)
	{
		LOG(TRACE) << "Timeout";
		if (error)
		{
			LOG(ERROR) << "Error:" << error.message();
		}

		timeoutCallback();

		timer.expires_from_now(timeout);
		timer.async_wait(boost::bind(&Timer::Timeout, this, boost::asio::placeholders::error));
	}
}