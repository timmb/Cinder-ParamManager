#pragma once
#include "json/json.h"
#include "cinder/params/Params.h"
//#include "cinder/Cinder.h"
//#include <boost/tokenizer.hpp>
//#include <atomic>
//#include "cinder/app/App.h"
//#include <boost/thread/recursive_mutex.hpp>
//#include <boost/thread/lock_guard.hpp>
#include <set>
//#include "cinder/Quaternion.h"
//#include "BaseParameter.h"
#include <functional>
#include <vector>
#include <string>

class BaseParameter
{
public:
	// basePath is list of parent nodes. root node first.
	// name should not be included in basePath
	BaseParameter(std::string const& name,
		std::vector<std::string> const& basePath = {},
		std::string const& options_ = "",
		std::function<void()> customCallback_ = nullptr)
		: path(basePath)
		, options(options_)
		, customCallback(customCallback_)
	{
		path.push_back(name);
		size_t unsavedPos = options.find("unsaved");
		isSaved = unsavedPos == std::string::npos;
		if (unsavedPos != std::string::npos)
		{
			options = options.substr(0, unsavedPos) + options.substr(unsavedPos + std::string("unsaved").size());
		}
		calculateGuiNamesAndGroups();
	}

	virtual ~BaseParameter() {}

	virtual void setup(ci::params::InterfaceGl& params, std::function<void()> callbackFunction = nullptr) = 0;
	virtual void writeJson(Json::Value& root) const;
	virtual bool readJson(Json::Value const& root);

	// called when parameter value has been changed
	virtual void callbackFunction() {}

	void removeFrom(ci::params::InterfaceGl& params)
	{
		params.removeParam(mGuiName);
	}

	std::vector<std::string> path;
	std::string options;
	bool isSaved;
	std::function<void()> customCallback;
	/// Only used for int parameters. Leave blank for no labels
	std::vector<std::string> enumerationLabels;

	std::vector<std::string> getGuiGroups() const { return mGuiGroups; }

protected:
	/// Sets mGuiName and mGuiGroups from `path`. This should be called in setup()
	void calculateGuiNamesAndGroups();

	/// Name of the parameter in anttweakbar. This includes the path appended to ensure it's unique
	std::string mGuiName;
	/// Name of the groups in anttweakbar (equivalent to the path). Each group includes the path appended and all spaces removed for compatibility.
	std::vector<std::string> mGuiGroups;
	template <typename JsonOrConstJson>
	JsonOrConstJson& getChild(JsonOrConstJson& root, std::vector<std::string> path) const;
	virtual void toJson(Json::Value& child) const = 0;
	virtual bool fromJson(Json::Value const& child) = 0;
};


template <typename T>
T& BaseParameter::getChild(T& root, std::vector<std::string> path) const
{
	if (path.empty())
	{
		return root;
	}
	return getChild(root[path.front()], std::vector<std::string>(path.begin() + 1, path.end()));
}


typedef std::shared_ptr<BaseParameter> ParameterPtr;