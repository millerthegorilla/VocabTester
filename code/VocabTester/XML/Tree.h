#if !defined (XMLTREE_H)
#define XMLTREE_H
// ----------------------------------
// (c) Reliable Software, 2003 - 2005
// ----------------------------------

#include "Sink.h"

#include <auto_vector.h>
#include <stack>

namespace XML
{
	class Attribute
	{
	public:
		Attribute (std::string const & name, std::string const & value)
		: _name (name), 
		  _value (value)
		{}
		Attribute (std::string const & name)
			: _name (name)
		{}

		std::string const & GetName  () const { return _name;  }
		std::string const & GetValue () const { return _value; }

		void SetTransformValue (std::string const & val);
		std::string GetTransformValue () const;
	private:
		std::string _name;
		std::string _value;
	};
	
	class Node
	{
	public:
		explicit Node (std::string const & name, bool closed = false, Node * parent = NULL)
			: _name (name), _closed (closed), _parent (parent)
		{}
		Node * AddChild (std::auto_ptr<Node> child);
		Node * AddChild (std::string const & name);
		Node * AddEmptyChild (std::string const & name);
		void AddAttribute (std::string const & name, std::string const & value);
		void AddAttribute (std::string const & name, int value);
		void AddTransformAttribute (std::string const & name, std::string const & value);
		void AddText (std::string const & text);
		void AddTransformText (std::string const & text);
		void WriteOpeningTag (std::ostream & out, unsigned indent) const;
		void WriteClosingTag (std::ostream & out, unsigned indent) const;
		void Write (std::ostream & out, unsigned indent = 0) const;

		std::string const & GetName () const { return _name; }
		std::string GetClosingTag () const;
		Node const * FindFirstChildNamed (std::string const & name) const throw ();
		Node const * GetFirstChildNamed (std::string const & name) const;
		Attribute const * FindAttribute (std::string const & name) const throw ();
		std::string const & GetAttribValue (std::string const & name) const;
		std::string GetTransformAttribValue (std::string const & name) const;
		
		typedef auto_vector<Node>::const_iterator ConstChildIter;
		ConstChildIter FirstChild () const { return _children.begin (); }
		ConstChildIter LastChild  () const { return _children.end  ();  }   
		typedef auto_vector<Attribute>::const_iterator ConstAttribIter;
		ConstAttribIter FirstAttrib () const { return _attributes.begin (); }
		ConstAttribIter LastAttrib  () const { return _attributes.end  ();  }
		Node * GetParent() const { return _parent; }
		void SetParent (Node * parent) { _parent = parent; }
	private:
		std::string				_name;
		auto_vector<Attribute>	_attributes;
		auto_vector<Node>		_children;
		bool _closed; // of the form <name attrib="value"/>
		Node					*_parent;
	};

	class Tree
	{
		friend class TreeMaker;
	public:
		Tree ()
			: _top ("Top")
		{}
		Node * SetRoot (std::auto_ptr<Node> root)
		{
			return _top.AddChild (root);
		}
		Node * SetRoot (std::string const & name);
		Node const * GetRoot () const {	return *_top.FirstChild (); }
		static void WriteHeader (std::ostream & out);
		void Write (std::ostream & out) const;

	private:
		Node _top;
	};

	class TreeMaker : public Sink
	{
	public:
		TreeMaker (Tree & tree)
			: _curr (&tree._top)
		{}
		// Sink interface
		void OnStartTag (std::string const & tagName);
		void OnEndTag ();
		void OnEndTagEmpty ();
		void OnAttribute (std::string const & attrib, std::string const & value);
		void OnText (std::string const & text);
		void OnError (XML::Error const & error);
private:
		Node *			      _curr;
		std::stack<Node*>	  _parents;
	};
}

#endif
