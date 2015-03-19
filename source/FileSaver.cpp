#include "FileSaver.h"
#include <vector>

#ifdef MARMALADE
#include "IwAutoFile.h"
#endif

namespace oxygine
{
	struct xml_string_writer: pugi::xml_writer {
		std::string result;

		virtual void write(const void* data, size_t size) {
			result += std::string(static_cast<const char*>(data), size);
		}
	};

	FileSaver::FileSaver() : _path("dupa_path.xml") {
	
	}

	FileSaver::~FileSaver() {
	
	}

	void FileSaver::setPath(const string &path) {
		_path = path;
	}
	
	void FileSaver::init(const string &version) {
		_version = version;
		reset();
	}

	void FileSaver::reset() {
		_doc.reset();
		pugi::xml_node root = _doc.append_child("box2d");
		root.append_child("bodies");
	}

	void FileSaver::load() {
		file::buffer fb;
		file::read(_path.c_str(), fb, ep_ignore_error);

		if (fb.getSize()) {
			_doc.reset();
			_doc.load_buffer(fb.getData(), fb.getSize());
		}
	}
	
	pugi::xml_node FileSaver::getFirstNodeByName(const string &nodeName) {
		pugi::xml_node descendant = _doc.first_child();
		pugi::xml_node descendantDesc;
		while (descendant) {
			if (descendant.name() == nodeName) {
				return descendant;
			}
			else {
				descendantDesc = descendant.first_child();
				while (descendantDesc) {
					if (descendantDesc.name() == nodeName) {
						return descendantDesc;
					}

					descendantDesc = descendantDesc.next_sibling();
				}
			}

			descendant = descendant.next_sibling();
		}
		return descendant;
	}

	pugi::xml_node FileSaver::addChild(pugi::xml_node parent, const string &childName) {
		return parent.append_child(childName.c_str());
	}

	pugi::xml_attribute FileSaver::addAttribute(pugi::xml_node child, const string &attributeName) {
		return child.append_attribute(attributeName.c_str());
	}

	pugi::xml_attribute FileSaver::addValue(const string &name) {
		pugi::xml_node root = _doc.child("box2d");
		if (!root)
			root = _doc.append_child("box2d");
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

	void FileSaver::save() {
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