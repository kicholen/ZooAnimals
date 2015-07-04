#include "Settings.h"
#include "s3e.h"
#include "FlashUtils.h"

namespace oxygine
{
	Settings Settings::instance;

	Settings::Settings() {
		_path = "settings.xml";
	}

	Settings::~Settings() {

	}

	void Settings::init(const std::string &version) {
		_version = version;
		load();
		s3eDebugOutputString("settings::init");

		if (_doc.child("options").attribute("version").value() != _version) {
			reset();
		}
	}

	void Settings::reset() {
		_doc.reset();
		s3eDebugOutputString("settings::reset");
		pugi::xml_node root = _doc.append_child("options");
		root.append_attribute("version").set_value(_version.c_str());
	}

	pugi::xml_attribute Settings::addValue(const std::string &name) {
		s3eDebugOutputString(FlashUtils::CMath::stringFormat("settings::addValue : %s", name.c_str()).c_str());

		pugi::xml_node root = _doc.child("options");
		if (!root)
			root = _doc.append_child("options");
		pugi::xml_node child = root.child(name.c_str());
		if (!child)
			child = root.append_child(name.c_str());

		pugi::xml_attribute attr = child.attribute("value");
		if (!attr)
			attr = child.append_attribute("value");
		else
			attr = pugi::xml_attribute();

		return attr;
	}

	pugi::xml_attribute Settings::getValue(const std::string &name) {
		s3eDebugOutputString(FlashUtils::CMath::stringFormat("settings::getValue : %s", name.c_str()).c_str());

		pugi::xml_node root = _doc.child("options");
		OX_ASSERT(root);
		pugi::xml_node child = root.child(name.c_str());
		OX_ASSERT(child);
		pugi::xml_attribute attr = child.attribute("value");
		OX_ASSERT(attr);

		return attr;
	}
}