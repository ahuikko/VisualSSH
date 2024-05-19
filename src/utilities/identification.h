#ifndef IDENTIFICATION_H
#define IDENTIFICATION_H

#include <string>
#include <ostream>
#include <chrono>




static std::string generateHandle(std::string identifier){
	// Generate a unique handle using localtime_s
	std::ostringstream ss;
	auto now = std::chrono::system_clock::now();
	auto in_time_t = std::chrono::system_clock::to_time_t(now);

	std::tm buf; // Buffer to hold the local time
	localtime_s(&buf, &in_time_t); // Secure version of localtime

	// Format the current time and append instance count to ensure uniqueness, bit of a overkill maybe
	ss << identifier << std::put_time(&buf, "%Y%m%d%H%M%S");
	return ss.str();
}

#endif 