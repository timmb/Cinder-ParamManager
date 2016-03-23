#pragma once
#include "jsoncpp/json.h"
#include "cinder/Quaternion.h"
#include "cinder/Color.h"
#include <atomic>

namespace Json
{
	// To avoid having to specialise the Parameter class
	bool operator>>(Json::Value const& child, double& value);
	bool operator>>(Json::Value const& child, float& value);
	bool operator>>(Json::Value const& child, int& value);
	bool operator>>(Json::Value const& child, ci::vec2& value);
	bool operator>>(Json::Value const& child, ci::vec3& value);
	bool operator>>(Json::Value const& child, std::string& value);
	bool operator>>(Json::Value const& child, bool& value);
	bool operator>>(Json::Value const& child, ci::quat& value);
	bool operator>>(Json::Value const& child, ci::ColorA& value);
	template <typename T>
	bool operator>>(Json::Value const& child, std::atomic<T>& value)
	{
		T t;
		if (child >> t)
		{
			value.store(t);
			return true;
		}
		return false;
	}

	template <typename T>
	Json::Value& operator<<(Json::Value& lhs, T const& rhs)
	{
		lhs = rhs;
		return lhs;
	}

	template <typename T>
	Json::Value& operator<<(Json::Value& lhs, std::atomic<T> const& rhs)
	{
		T t = rhs;
		return lhs << t;
	}
	Json::Value& operator<<(Json::Value& lhs, ci::vec3 const& rhs);
	Json::Value& operator<<(Json::Value& lhs, ci::vec2 const& rhs);
	Json::Value& operator<<(Json::Value& lhs, ci::quat const& rhs);
	Json::Value& operator<<(Json::Value& lhs, ci::ColorA const& rhs);
}

