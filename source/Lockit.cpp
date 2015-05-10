#include "Lockit.h"

Lockit::Lockit() {

}

Lockit::~Lockit() {

}

string Lockit::getText(int id){
	pugi::xml_node lockit = _doc.child("lockit");
	OX_ASSERT(lockit);
	pugi::xml_node child = lockit.first_child();

	for (int i = 1; i < id; i++) {
		child = child.next_sibling();
	}

	return child.first_attribute().as_string();
}