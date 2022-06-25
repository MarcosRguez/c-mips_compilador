/**
 * @file timer.hh
 * @author MarcosRguez (alu0101470368@ull.edu.es)
 * @brief cronómetro
 * @version 0.1
 * @date 18-06-2022
 *
 * @copyright Copyright (c) 2022
 *
 */

#pragma once

#include <chrono> /// for std::chrono functions

class Timer {
	public:
	void reset() {
		m_beg = clock_type::now();
	}
	double elapsed() const {
		return std::chrono::duration_cast<second_type>(clock_type::now() - m_beg).count();
	}
	private:
	/// Type aliases to make accessing nested type easier
	using clock_type = std::chrono::steady_clock;
	using second_type = std::chrono::duration<double, std::ratio<1>>;
	std::chrono::time_point<clock_type> m_beg{clock_type::now()};
};