#pragma once

#include "BaseParameter.h"

// Class to represent buttons
class Button : public BaseParameter
{
public:
	Button(std::function<void()> function_, std::string const& name, std::vector<std::string> const& basePath = {}, std::string const& options = "");

	virtual void setup(ci::params::InterfaceGl& params, std::function<void()> ignored = nullptr) override;

protected:
	virtual void toJson(Json::Value& child) const override;

	virtual bool fromJson(Json::Value const& child) override;

	std::function<void()> function;
};




