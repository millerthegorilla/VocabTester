#include "precompiled.h"
#include "TreeNode.h"

TreeNode::~TreeNode ()
{

}

TvTree::~TvTree()
{
	DestroyTree(_root);
}

bool TvTree::AddSibling (TreeNode *toadd)
{
	TreeNode * newsibling = new TreeNode;
	if(newsibling == NULL)
		return false;
	*newsibling = *toadd;
	_current->_nextSibling = newsibling;
	_current = newsibling;
	return true;
}

bool TvTree::AddChild (TreeNode *toadd)
{
	TreeNode * newchild = new TreeNode;
	if(newchild == NULL)
		return false;
	*newchild = *toadd;
	_current->cChildren = 1;
	_current->_firstChild = newchild;
	_current = newchild;
	return true;
}
//does this delete root? I think not!
void TvTree::DestroyTree (TreeNode *current)
{
	if(current->_firstChild != NULL)
	{
		DestroyTree(current->_firstChild );
	}
	if(current->_nextSibling != NULL)
	{
		DestroyTree(current->_nextSibling );
	}
	if(current->_firstChild != NULL)
	{
		delete current->_firstChild;
		current->_firstChild = NULL;
	}
	if(current->_nextSibling != NULL)
	{
		delete current->_nextSibling;
		current->_nextSibling = NULL;
	}
	return;
}