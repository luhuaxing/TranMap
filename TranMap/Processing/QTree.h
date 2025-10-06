#pragma once
using namespace std;

#define MAX_LEAF_NODES 100

class QTreeExtent
{
public:
	double left;
	double right;
	double top;//max
	double bottom;//min
public:
	QTreeExtent(){
		this->left = this->right = this->top = this->bottom = 0.0;
	};
	QTreeExtent(double left,double right,double top ,double bottom)
	{
		this->left = left;
		this->right = right;
		this->top  = top;
		this->bottom = bottom;
	}
	bool Contain(const QTreeExtent& o)
	{
		return (left <= o.left
			&& right>=o.right
			&& top >= o.top
			&& bottom <= o.bottom);

	}

	bool ContainIn(const QTreeExtent& o)
	{
		return (left >= o.left
			&& right<=o.right
			&& top <= o.top
			&& bottom >= o.bottom);

	}

	bool IntersectIn(QTreeExtent& o)
	{
		return !(o.right < left
			||o.left > right
			||o.top < bottom
			|| o.bottom > top
			);
	}
	QTreeExtent& operator= (const QTreeExtent& o )
	{
		if(this==&o)
		{
			return *this;
		}
		
		this->left = o.left;
		this->right = o.right;
		this->top = o.top;
		this->bottom = o.bottom;
		return *this;
	}
};

struct QTreeNode
{
	QTreeExtent Extent;
	int index;//index for storaged element
};

class QTree
{
private:
	QTree* LT,*RT,*LB,*RB;	//Four corners
	vector<QTreeNode*> nodes;	//nodes
	
	QTreeExtent extent;
	void Regenerate();
	bool isFull;
	bool regenerating;
public:
	QTree(){LT=RT=LB=RB=NULL; isFull = false; regenerating = false;};
	QTree(QTreeExtent);
	~QTree(void);

	void AddNode(const QTreeNode&);
	bool RemoveNode(int index);//return if success
	vector<int> GetNodes(QTreeExtent QueryExtent);//Query Nodes

	
};
