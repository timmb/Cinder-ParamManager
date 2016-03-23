#include "BaseParameter.h"
#include "JsonOperators.h"
#include <sstream>
#include <algorithm>
using namespace std;
using namespace ci;

////////


void BaseParameter::writeJson(Json::Value& root) const
{
	Json::Value& child = getChild(root, path);
	toJson(child);
}


bool BaseParameter::readJson(Json::Value const& root)
{
	Json::Value const& child = getChild(root, path);
	return fromJson(child);
}




namespace
{
	/// e.g. a/b/c
	std::string createPath(std::vector<std::string> path)
	{
		std::stringstream out;
		if (!path.empty())
		{
			out << std::move(path[0]);
		}
		for (int i = 1; i < path.size(); i++)
		{
			out << '/' << std::move(path[i]);
		}
		return out.str();
	}

	/// e.g. c (a/b/c)
	std::string formatPathWithPrefix(std::vector<std::string> path)
	{
		if (path.empty())
		{
			return "";
		}
		std::stringstream out;
		out << path.back();
		if (path.size() > 1)
		{
			out << " (" << path.front();
			for (int i = 1; i < path.size() - 1; i++)
			{
				out << '.' << std::move(path[i]);
			}
			out << ')';
		}
		return out.str();
	}
}

void BaseParameter::calculateGuiNamesAndGroups()
{
	assert(!path.empty());
	mGuiName = formatPathWithPrefix(path);
	for (int i = 0; i < path.size() - 1; i++)
	{
		int groupPathLength = i + 1;
		vector<string> groupPath = vector<string>(begin(path), begin(path) + groupPathLength);
		std::string guiGroup = groupPathLength == 1 ? groupPath.front() : formatPathWithPrefix(groupPath);
		std::replace(begin(guiGroup), end(guiGroup), ' ', '_');
		mGuiGroups.push_back(guiGroup);
	}
	if (!mGuiGroups.empty())
	{
		options += " group=" + mGuiGroups.back();
	}
}

