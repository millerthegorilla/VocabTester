#if !defined(TREENODE_H)
#define TREENODE_H

class TreeNode : public TVITEMEX
{
public:
	TreeNode() : _firstChild(NULL), _nextSibling(NULL) {}
	~TreeNode();
	//members
	TreeNode *_firstChild;
	TreeNode *_nextSibling;
};

class TvTree
{
public:
	TvTree(TreeNode root)  
	{
		_root = new TreeNode;
		*_root = root;
		_current = _root;
	}
	~TvTree();
	void DestroyTree(TreeNode * current);
	bool AddSibling (TreeNode * toadd); //returns false on error
	bool AddChild (TreeNode * toadd);
private:
	TreeNode	*_root;
	TreeNode	*_current;
};

#endif