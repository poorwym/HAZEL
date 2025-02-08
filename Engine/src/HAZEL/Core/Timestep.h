#pragma once
/**
* @note: 这里Timestep用秒为单位，本质就是个float的封装
*/

namespace Hazel {
	class Timestep {
	public:
		Timestep(float time = 0.0f)
                : m_Time(time)
        {
        }

        operator float() const { return m_Time; }

        float GetSeconds() const { return m_Time;}
        float GetMilliseconds() const { return m_Time * 1000.0f; }

	private:
        float m_Time;
	};
}