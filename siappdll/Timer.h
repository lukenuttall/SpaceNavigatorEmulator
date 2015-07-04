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

#pragma once
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <boost/asio.hpp>
#include <iostream>
#include <thread>

namespace SpaceNavigatorEmulator
{
	class Timer
	{


	public:
		Timer(int timeMilliseconds, std::function<void()>&& callback);
		~Timer();

		void start();

		void stop();

		Timer() = delete;
		Timer(const Timer&) = delete;
		Timer& operator=(const Timer&) = delete;

	private:

		void Timeout(const boost::system::error_code& error);

		std::thread timer_thread;
		boost::asio::io_service io;
		boost::asio::deadline_timer timer;

		boost::posix_time::milliseconds timeout;

		std::function<void()> timeoutCallback;

	};

}