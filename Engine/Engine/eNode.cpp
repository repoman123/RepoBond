#include "eNode.h"


eNode::eNode(aiNode* node)
{
	node->
}


eNode::~eNode(void)
{
}

void eNode::SetMatrix(aiMatrix4x4* m)
{
	m->Transpose();
	D3DXMATRIX* ma = new D3DXMATRIX(
	m->a1,m->a2,m->a3,m->a4,
	m->b1,m->b2,m->b3,m->b4,
	m->c1,m->c2,m->c3,m->c4,
	m->d1,m->d2,m->d3,m->d4
		);
	Matrix = ma;
}

void eNode::SetName(aiString* st)
{
	name = new char[st->length];
	int i =0;
	do
	{
		name[i]=st->data[i];
		i++;
	}while(i<st->length+1);
	length=st->length;
}