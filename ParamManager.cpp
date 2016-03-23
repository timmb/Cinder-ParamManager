//
//  Params.cpp
//
//  Created by Tim Murray-Browne on 2014-04-06.
//
//

#include "ParamManager.h"
#include "cinder/app/AppBasic.h"
#include "json/json.h"
#include <fstream>
#include <iostream>
#include <ctime>
#include <boost/tokenizer.hpp>

#include "../src/AntTweakBar/AntTweakBar.h"

using namespace std;
using namespace ci;


ParamManager::ParamManager()
	: mHasSetupBeenCalled(false)
{
}

ParamManager::~ParamManager()
{
	clear();
}

void ParamManager::clear()
{
	Lock lock(mMutex);
	mParams = params::InterfaceGl();
	mParameters.clear();
	mRoot = Json::Value();
	mHasSetupBeenCalled = false;
}

bool ParamManager::load(string const& filename)
{
	Lock lock(mMutex);

	mJsonFile = filename;


	ifstream in(filename.c_str(), ifstream::in);
	bool success = true;
	try {
		in >> mRoot;
	}
	catch (std::runtime_error& e)
	{
		cerr << ("Error parsing json file " + filename, "Settings") << endl;
		success = false;
	}
	if (success)
	{
		cout << "Successfully loaded " + filename << endl;
	}
	if (!success)
	{
		cerr << "Problem with settings json file." << endl;
	}
	return success;
}

void ParamManager::save()
{
	save(mJsonFile);
}


void ParamManager::snapshot()
{
	// get date string
	time_t t = time(0);   // get time now
	struct tm * now = localtime(&t);
	char buf[128];
	strftime(buf, sizeof(buf), "-%Y-%m-%d-%H-%M-%S", now);

	int extensionPos = mJsonFile.find(".json");
	string filestem = mJsonFile.substr(0, extensionPos);

	save(filestem + buf + ".json");
}

void ParamManager::save(string const& filename)
{
	Lock lock(mMutex);

	Json::Value root;
	for (auto it = mParameters.begin(); it != mParameters.end(); ++it)
	{
		if ((**it).isSaved)
		{
			(**it).writeJson(root["params"]);
		}
	}
	for (std::string const& group : mGuiGroupNames)
	{
		root["groups"][group]["opened"] = getIntOption(group, "opened");
	}

	ofstream out(filename.c_str(), ofstream::out);
	try
	{
		out << root;
	}
	catch (...)
	{
		cerr << "Error saving JSON settings file " + filename << endl;
		if (out.bad()) {
			cerr << "Problem with writing file" << endl;
		}
	}
}


void ParamManager::setup()
{
	Lock lock(mMutex);

	mHasSetupBeenCalled = true;

	Vec2i size(350, 420);
	mParams = params::InterfaceGl("Params", Vec2i(size));
	mParams.setPosition(Vec2i(app::getWindowWidth() - size.x, 0));
	mParams.addButton("Save", std::bind((void (ParamManager::*)())&ParamManager::save, this));
	mParams.addButton("Save snapshot", std::bind(&ParamManager::snapshot, this));
	map<string, string> groupParents;
	Json::Value& paramsRoot = mRoot["params"];
	//std::string temp = paramsRoot.toStyledString();
	for (int i = 0; i < mParameters.size(); ++i)
	{
		if (mParameters[i]->readJson(paramsRoot))
		{
			mParameters[i]->callbackFunction();
		}
	}
	for (auto it = mParameters.begin(); it != mParameters.end(); ++it)
	{
		vector<string> guiGroups = (**it).getGuiGroups();
		for (int i = 1; i < guiGroups.size(); i++)
		{
			string const& group = guiGroups[i];
			string const& parent = guiGroups[i - 1];
			if (groupParents.count(group) > 0)
			{
				if (groupParents.at(group) != parent)
				{
					assert(groupParents.at(group) == parent);
					cerr << "Params error. Group '" << group << "' has parent '" << parent << "' here but previously parent was '" << groupParents.at(group) << "'." << endl;;
				}
			}
			else
			{
				groupParents[group] = parent;
			}
		}
		for (string group : guiGroups)
		{
			mGuiGroupNames.insert(move(group));
		}
		if ((**it).isSaved)
		{
			(**it).setup(mParams, [&]() { callbackParameterChanged(); });
		}
		else
		{
			(**it).setup(mParams);
		}
	}
	for (auto kv : groupParents)
	{
		mParams.setOptions(kv.first, "group="+kv.second);
	}
	for (string const& group : mGuiGroupNames)
	{
		bool const opened = mRoot["groups"][group]["opened"].isIntegral() ? (mRoot["groups"][group]["opened"].asInt() > 0) : false;
			mParams.setOptions(group, opened?"opened=true" : "opened=false");
	}
}

void ParamManager::update(float dt, float elapsed)
{

}


void ParamManager::addParam(std::shared_ptr<BaseParameter> parameter)
{
	Lock lock(mMutex);

	mParameters.push_back(parameter);
	if (mHasSetupBeenCalled)
	{
		if (parameter->isSaved)
		{
			parameter->setup(mParams, [&]() { callbackParameterChanged(); });
			if (parameter->readJson(mRoot))
			{
				parameter->callbackFunction();
			}
		}
		else
		{
			parameter->setup(mParams);
		}
	}
}

std::shared_ptr<BaseParameter> ParamManager::addButton(std::function<void()> function, std::string const& name, std::vector<std::string> const& path, std::string const& options)
{
	Lock lock(mMutex);

	mParameters.push_back(std::shared_ptr<BaseParameter>(new Button(function, name, path, options)));
	return mParameters.back();
}


namespace
{
	class InterfaceGlWithGetOption : public ci::params::InterfaceGl
	{
	public:
		std::string getStringOption(std::string paramName, std::string const& optionName) const
		{
			TwSetCurrentWindow(mTwWindowId);

			std::string target = "`" + std::string(TwGetBarName(mBar.get())) + "`";
			if (!paramName.empty())
				target += "/`" + std::move(paramName) + "`";

			std::vector<char> outBuffer(1024, '\0');
			int stringLength = TwGetParam(mBar.get(), target.c_str(), optionName.c_str(), TW_PARAM_CSTRING, outBuffer.size(), outBuffer.data());
			assert(stringLength <= outBuffer.size());
			return std::string(begin(outBuffer), begin(outBuffer) + stringLength);
		}

		int getIntOption(std::string paramName, std::string const& optionName, bool* isOk=nullptr) const
		{
			TwSetCurrentWindow(mTwWindowId);

			std::string target;// = "`" + std::string(TwGetBarName(mBar.get())) + "`/";
			if (!paramName.empty())
				target += std::move(paramName);

			int value(0);
			int numValuesReturned = TwGetParam(mBar.get(), target.c_str(), optionName.c_str(), TW_PARAM_INT32, 1, &value);
			assert(numValuesReturned == 0 || numValuesReturned == 1);
			if (isOk != nullptr)
			{
				*isOk = numValuesReturned == 1;
			}
			return value;

		}
	};
}

std::string ParamManager::getStringOption(std::string paramName, std::string const& optionName) const
{
	InterfaceGlWithGetOption const& p = reinterpret_cast<InterfaceGlWithGetOption const&>(mParams);
	return p.getStringOption(std::move(paramName), optionName);
}

int ParamManager::getIntOption(std::string paramName, std::string const& optionName) const 
{
	InterfaceGlWithGetOption const& p = reinterpret_cast<InterfaceGlWithGetOption const&>(mParams);
	return p.getIntOption(std::move(paramName), optionName);
}

void ParamManager::removeParam(std::shared_ptr<BaseParameter> parameter)
{
	Lock lock(mMutex);

	auto it = find(begin(mParameters), end(mParameters), parameter);
	while (it != end(mParameters))
	{
		mParameters.erase(it);
		it = find(begin(mParameters), end(mParameters), parameter);
	}
	parameter->removeFrom(mParams);
}


void ParamManager::draw()
{
	Lock lock(mMutex);

	mParams.draw();
}


