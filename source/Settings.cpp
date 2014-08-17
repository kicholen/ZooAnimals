#include "Settings.h"
#include <vector>
#include "s3e.h"
#include "FlashUtils.h"

#ifdef MARMALADE
#include "IwAutoFile.h"
#endif

namespace oxygine
{
	Settings Settings::instance;

	struct xml_string_writer: pugi::xml_writer {
		std::string result;

		virtual void write(const void* data, size_t size) {
			result += std::string(static_cast<const char*>(data), size);
		}
	};

	Settings::Settings():_path("settings.xml") {

	}

	Settings::~Settings() {

	}

	void Settings::setPath(const string &path) {
		_path = path;
	}

	void Settings::init(const string &version) {
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

	void Settings::load() {
		_doc.reset();
		file::buffer fb;
		file::read(_path.c_str(), fb);
		if (fb.getSize())
			_doc.load_buffer(fb.getData(), fb.getSize());
	}
	
	pugi::xml_attribute Settings::addValue(const string &name) {
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

	pugi::xml_attribute Settings::getValue(const string &name) {
		s3eDebugOutputString(FlashUtils::CMath::stringFormat("settings::getValue : %s", name.c_str()).c_str());

		pugi::xml_node root = _doc.child("options");
		OX_ASSERT(root);
		pugi::xml_node child = root.child(name.c_str());
		OX_ASSERT(child);
		pugi::xml_attribute attr = child.attribute("value");
		OX_ASSERT(attr);

		return attr;
	}

	void Settings::save() {
#ifdef MARMALADE
		s3eFileDelete(_path.c_str());
		CIwAutoFile f(_path.c_str(), "wb");
		s3eFile *fh = f.s3eFileGet();

		xml_string_writer sw;
		_doc.save(sw);

		int size = sw.result.size();
		const char *buf = sw.result.c_str();
		s3eFileWrite(buf, 1, size, fh);
#else
		assert(0);
#endif
	}
}