#pragma once

#include "BaseParameter.h"
#include "cinder/app/App.h"


// Main class to represent parameters
template <typename T>
class Parameter : public BaseParameter
{
public:
	Parameter(T* value_, std::string const& name, std::vector<std::string> const& basePath = {}, std::string const& options = "", std::function<void()> customCallback = nullptr)
		: BaseParameter(name, basePath, defaultOptions() + " " + options, customCallback)
		, value(value_)
		, mIsUpdateSignalConnected(false)
		, mPrevValue(-42)
	{}

	virtual ~Parameter()
	{
		//std::cout << " ~Parameter" << std::endl;
		cleanUpEnum();
	}

	virtual void setup(ci::params::InterfaceGl& params, std::function<void()> extraCallback_ = nullptr) override
	{
		extraCallback = extraCallback_;
		addToGui(params, options);
	}

	virtual void callbackFunction() override
	{
		if (extraCallback != nullptr)
			extraCallback();
		if (customCallback != nullptr)
			customCallback();
	}

	std::string defaultOptions() const
	{
		return "";
	}



protected:
	void addToGui(ci::params::InterfaceGl& params, std::string optionsString)
	{
		params.addParam(mGuiName, value).optionsStr(optionsString).updateFn([this](){ callbackFunction(); });
	}

	virtual void toJson(Json::Value& child) const override
	{
		child << *value;
	}

	virtual bool fromJson(Json::Value const& child) override
	{
		T tempValue;
		if (child >> tempValue)
		{
			child >> *value;
			return true;
		}
		else
			return false;
	}

	T* value;
	std::function<void()> extraCallback;

	// For enumeration types not supporting a callback
	int mPrevValue;
	void updateEnum() {}
	void cleanUpEnum() {}
	ci::signals::Connection mUpdateSignalConnection;
	bool mIsUpdateSignalConnected;
};

template<> inline
std::string Parameter<float>::defaultOptions() const
{
	return "step=0.01 precision=6";
}


template<> inline
void Parameter<int>::updateEnum()
{
	if (*value != mPrevValue)
	{
		callbackFunction();
		mPrevValue = *value;
	}
}

template<> inline
void Parameter<int>::addToGui(ci::params::InterfaceGl& params, std::string optionsString)
{
	if (enumerationLabels.empty())
	{
		params.addParam(mGuiName, value).optionsStr(std::move(optionsString)).updateFn([this](){ callbackFunction(); });
	}
	else
	{
		params.addParam(mGuiName, enumerationLabels, value, std::move(optionsString));
		mUpdateSignalConnection = ci::app::App::get()->getSignalUpdate().connect(std::bind(&Parameter<int>::updateEnum, this));
		mIsUpdateSignalConnected = true;
		mPrevValue = *value;
	}
}

template<> inline
void Parameter<int>::cleanUpEnum()
{
	//std::cout << "Parameter<int>::cleanUpEnum" << std::endl;
	if (mIsUpdateSignalConnected)
	{
		mUpdateSignalConnection.disconnect();
		mIsUpdateSignalConnected = false;
	}
}



template <typename T>
using ParameterPtrT = std::shared_ptr<Parameter<T>>;
