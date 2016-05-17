#pragma once
#include "json/json.h"
#include "cinder/Quaternion.h"
#include "cinder/Color.h"
#include <atomic>

namespace Json
{
	// To avoid having to specialise the Parameter class
	bool operator>>(Json::Value const& child, double& value);
	bool operator>>(Json::Value const& child, float& value);
	bool operator>>(Json::Value const& child, int& value);
	bool operator>>(Json::Value const& child, ci::Vec2f& value);
	bool operator>>(Json::Value const& child, ci::Vec2i& value);
	bool operator>>(Json::Value const& child, ci::Vec3f& value);
	bool operator>>(Json::Value const& child, std::string& value);
	bool operator>>(Json::Value const& child, bool& value);
	bool operator>>(Json::Value const& child, ci::Quatf& value);
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
	Json::Value& operator<<(Json::Value& lhs, ci::Vec3f const& rhs);
	Json::Value& operator<<(Json::Value& lhs, ci::Vec2f const& rhs);
	Json::Value& operator<<(Json::Value& lhs, ci::Vec2i const& rhs);
	Json::Value& operator<<(Json::Value& lhs, ci::Quatf const& rhs);
	Json::Value& operator<<(Json::Value& lhs, ci::ColorA const& rhs);
}

