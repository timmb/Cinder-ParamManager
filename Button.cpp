#include "Button.h"

Button::Button(std::function<void()> function_, std::string const& name, std::vector<std::string> const& basePath, std::string const& options)
	: BaseParameter(name, basePath, options)
	, function(function_)
{
	isSaved = false;
}

void Button::setup(ci::params::InterfaceGl& params, std::function<void()> ignored /*= nullptr*/)
{
	params.addButton(mGuiName, function, options);
}

void Button::toJson(Json::Value& child) const
{

}

bool Button::fromJson(Json::Value const& child)
{
	return true;
}
