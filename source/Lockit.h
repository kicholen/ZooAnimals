#ifndef _LOCKIT_
#define _LOCKIT_

#include "FileSaver.h"

class Lockit : public FileSaver
{
public:
	Lockit();
	~Lockit();

	std::string getText(int id);
};

#endif