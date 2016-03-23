//
//  Params.h
//
//  Created by Tim Murray-Browne on 2014-04-06.
//
//

#pragma once

#include <set>
#include <atomic>
//#include <boost/thread/recursive_mutex.hpp>
//#include <boost/thread/lock_guard.hpp>
#include "BaseParameter.h"
#include "Parameter.h"
#include "Button.h"
#include "JsonOperators.h"




class ParamManager
{
public:
	ParamManager();
	virtual ~ParamManager();

	/// Effectively destroys this instance allowing load and setup to be called again
	void clear();

	bool load(std::string const& jsonFile);
	void setup();
	void save();
	void save(std::string const& filename);
	void snapshot();
	void update(float dt, float elapsedTime);
	void draw();

	void addParam(std::shared_ptr<BaseParameter> parameter);
	/// Takes ownership of \p parameter
	std::shared_ptr<BaseParameter> addParam(BaseParameter* parameter)
	{
		auto p = std::shared_ptr<BaseParameter>(parameter);
		addParam(p);
		return p;
	}

	void removeParam(std::shared_ptr<BaseParameter> parameter);

	template <typename T>
	std::shared_ptr<Parameter<T>> addParam(T* value, std::string const& name, std::vector<std::string> const& path = {}, std::string const& options = "");

	std::shared_ptr<BaseParameter> addButton(std::function<void()> function, std::string const& name, std::vector<std::string> const& path = {}, std::string const& options = "");

	// callback for all saved parameters, triggers a save on this parameter
	void callbackParameterChanged()
	{
		save();
	}


private:
	std::string getStringOption(std::string paramGuiName, std::string const& optionName) const;
	int getIntOption(std::string paramGuiName, std::string const& optionName) const;

	typedef std::lock_guard<std::recursive_mutex> Lock;

	ci::params::InterfaceGl mParams;
	std::string mJsonFile;
	Json::Value mRoot;

	std::vector<std::shared_ptr<BaseParameter> > mParameters;
	std::set<std::string> mGuiGroupNames;
	bool mHasSetupBeenCalled;

	std::recursive_mutex mMutex;

};


template <typename T>
std::shared_ptr<Parameter<T>> ParamManager::addParam(T* value, std::string const& name, std::vector<std::string> const& path, std::string const& options)
{
	std::shared_ptr<Parameter<T>> p(new Parameter<T>(value, name, path, options));
	addParam(p);
	return p;

}




