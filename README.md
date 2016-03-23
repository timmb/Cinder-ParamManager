For Cinder 0.8.6.

Quick and dirty Parameter Manager for Cinder. It wraps around AntTweakBar to support groups more easily and read/write parameters to JSON files.

Main features:
	- Parameters can be saved and loaded from Json. Add "unsaved" to the options to prevent saving.
	- Easy to add callback functions.
	- Remembers which groups are open on the AntTweakBar UI when saving parameters.
	- Support for nested groups with a list of strings to represent a path.
	- Automatically ensures all parameters names are unique by appending full path to end of all parameter and group names, and removing spaces from group names.

Basic Usage:

	ParameterManager pars;

	pars.addParam(std::make_shared<Parameter<int>>(&myVar, "my var", { "Vars" }));
	pars.addParam(std::make_shared<Parameter<std::string>>(&myString, "my string", { "Vars" }))->customCallback = [this] { std::cout << myVar; };

	// load from Json.
	pars.load(getAssetPath("params.json").string());
	// Create interface and write to parameter variables. Call after all parameters have been written and load() succeeded.
	pars.setup();

	a = a + 1;

	pars.save();

Tim Murray-Browne
2014-16