#if !defined (PROGRESSBAR_H)
#define PROGRESSBAR_H
//---------------------------------------
//  ProgressBar.h
//  (c) Reliable Software, 1998, 99, 2000
//---------------------------------------

#include <Ctrl/Controls.h>

class ProgressMeter
{
public:
	virtual ~ProgressMeter () {}
	virtual void SetRange (int mini, int maxi, int step = 1) {}
	virtual void GetRange (int & mini, int & maxi, int & step) const
	{
		mini = 0;
		maxi = 0;
		step = 1;
	}
	virtual void SetCaption (char const * caption) {}
	virtual void SetActivity (char const * activity) {}
	virtual void StepIt () {}
	virtual void StepTo (int step) {}
	virtual void StepAndCheck () throw (Win::Exception) {}
	virtual bool WasCanceled () { return false; }
	virtual void Close () {}
};

namespace Win
{
	class ProgressBar : public SimpleControl
	{
	public:
		ProgressBar () {}
		ProgressBar (Win::Dow::Handle winParent, int id)
			: SimpleControl (winParent, id)
		{}
		ProgressBar (Win::Dow::Handle win)
			: SimpleControl (win)
		{}

		void ReSize (int left, int top, int width, int height)
		{
			Move (left, top, width, height);
		}

		void SetRange (int min, int max, int step);
		void StepIt ();
		void StepAndCheck () throw (Win::Exception) { StepIt (); }
		void StepTo (int min);
	};

	class ProgressBarMaker : public Win::ControlMaker
	{
	public:
		ProgressBarMaker (Win::Dow::Handle hParent, int id = 0);
	};
}

#endif
