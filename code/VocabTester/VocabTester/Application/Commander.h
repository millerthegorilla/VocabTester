#if !defined (SOLUTIONTOOLCOMMANDER_H)
#define SOLUTIONTOOLCOMMANDER_H
//----------------------------
// (c) Reliable Software, 2004
//----------------------------
#include <Ctrl/Command.h>
class TopCtrl;

class Commander
{
public:
	Commander (TopCtrl & ctrl)
		: _ctrl (ctrl)
	{}
	void Program_Exit ();
	Cmd::Status can_Program_Exit () const { return Cmd::Disabled; }
	void Program_About ();
private:
	TopCtrl    & _ctrl;
};

#endif
