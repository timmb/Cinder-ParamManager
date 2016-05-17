#include "JsonOperators.h"



/////////

namespace Json
{
	// To avoid having to specialise the Parameter class
	bool operator>>(Json::Value const& child, double& value)
	{
		if (child.isNull() || !child.isConvertibleTo(Json::realValue))
			return false;
		value = child.asDouble();
		return true;
	}


	bool operator>>(Json::Value const& child, float& value)
	{
		if (child.isNull() || !child.isConvertibleTo(Json::realValue))
			return false;
		value = child.asFloat();
		return true;
	}

	bool operator>>(Json::Value const& child, int& value)
	{
		if (child.isNull() || !child.isConvertibleTo(Json::intValue))
			return false;
		value = child.asInt();
		return true;
	}

	bool operator>>(Json::Value const& child, ci::Vec2f& value)
	{
		if (!child.isNull() && child["x"].isConvertibleTo(Json::realValue)
			&& child["y"].isConvertibleTo(Json::realValue))
		{
			value.x = child["x"].asFloat();
			value.y = child["y"].asFloat();
			return true;
		}
		return false;
	}

	bool operator>>(Json::Value const& child, ci::Vec2i& value)
	{
		if (!child.isNull() && child["x"].isConvertibleTo(Json::intValue)
			&& child["y"].isConvertibleTo(Json::intValue))
		{
			value.x = child["x"].asInt();
			value.y = child["y"].asInt();
			return true;
		}
		return false;
	}


	bool operator>>(Json::Value const& child, ci::Vec3f& value)
	{
		if (!child.isNull() && child["x"].isConvertibleTo(Json::realValue)
			&& child["y"].isConvertibleTo(Json::realValue)
			&& child["z"].isConvertibleTo(Json::realValue))
		{
			value.x = child["x"].asFloat();
			value.y = child["y"].asFloat();
			value.z = child["z"].asFloat();
			return true;
		}
		return false;
	}

	bool operator>>(Json::Value const& child, std::string& value)
	{
		if (child.isNull() || !child.isConvertibleTo(Json::stringValue))
			return false;
		value = child.asString();
		return true;
	}

	bool operator>>(Json::Value const& child, bool& value)
	{
		if (child.isNull() || !child.isConvertibleTo(Json::booleanValue))
			return false;
		value = child.asBool();
		return true;
	}


	bool operator>>(Value const& child, ci::Quatf& value)
	{
		if (!child.isNull()
			&& child["w"].isConvertibleTo(Json::realValue)
			&& child["x"].isConvertibleTo(Json::realValue)
			&& child["y"].isConvertibleTo(Json::realValue)
			&& child["z"].isConvertibleTo(Json::realValue)
			)
		{
			value.w = child["w"].asFloat();
			value.v.x = child["x"].asFloat();
			value.v.y = child["y"].asFloat();
			value.v.z = child["z"].asFloat();
			return true;
		}
		return false;
	}


	bool operator>>(Value const& child, ci::ColorA& value)
	{
		if (!child.isNull()
			&& child["r"].isConvertibleTo(Json::realValue)
			&& child["g"].isConvertibleTo(Json::realValue)
			&& child["b"].isConvertibleTo(Json::realValue)
			&& child["a"].isConvertibleTo(Json::realValue))
		{
			value.r = child["r"].asFloat();
			value.g = child["g"].asFloat();
			value.b = child["b"].asFloat();
			value.a = child["a"].asFloat();
			return true;
		}
		return false;
	}

	Json::Value& operator<<(Json::Value& lhs, ci::Vec2f const& rhs)
	{
		lhs["x"] = rhs.x;
		lhs["y"] = rhs.y;
		return lhs;
	}

	Json::Value& operator<<(Json::Value& lhs, ci::Vec2i const& rhs)
	{
		lhs["x"] = rhs.x;
		lhs["y"] = rhs.y;
		return lhs;
	}

	Json::Value& operator<<(Json::Value& lhs, ci::Vec3f const& rhs)
	{
		lhs["x"] = rhs.x;
		lhs["y"] = rhs.y;
		lhs["z"] = rhs.z;
		return lhs;
	}

	Json::Value& operator<<(Value& lhs, ci::Quatf const& rhs)
	{
		lhs << rhs.v;
		lhs["w"] = rhs.w;
		return lhs;
	}


	Json::Value& operator<<(Value& lhs, ci::ColorA const& rhs)
	{
		lhs["r"] = rhs.r;
		lhs["g"] = rhs.g;
		lhs["b"] = rhs.b;
		lhs["a"] = rhs.a;
		return lhs;
	}

}





