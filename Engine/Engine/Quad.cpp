#include "Quad.h"

Quad::Quad(void)
{
	triCount=0;
	vertCount=0;
	box = new BoundingBox();
	hasChildren=false;
}

Quad::~Quad(void)
{
}

void Quad::Initialize(D3DXVECTOR3* vecs,unsigned int numverts,BoundingBox* bbox)
{
	unsigned int tr = (unsigned int)numverts / 3;
	triCount=tr;
	vertCount=numverts;
	unsigned int i=0;
	box = bbox;
	if(tr > QUAD_MAX_TRICOUNT)
	{
		Split(vecs);
	}
	else
	{
		positions=new D3DXVECTOR3[numverts];
		do
		{
			positions[i]=vecs[i];
			i++;
		}while(i<numverts);
	}
}

void Quad::Initialize()
{
	if(triCount>QUAD_MAX_TRICOUNT)
	{
		unsigned int i=0;
		do
		{
			if(children[i]->triCount > QUAD_MAX_TRICOUNT)
			{
				children[i]->Split();
			}
			i++;
		}while(i<8);
	}
}

bool Quad::Intersects(Quad* q,Ray* r,float* d)
{
	if(q->triCount>0)
	if(Ray::InterceptsBox(r,&q->box->origMin,&q->box->origMax))
	{
		bool inter=false;
		if(q->hasChildren)
		{
			if(q->children[0]->triCount>0)
			if(Quad::Intersects(q->children[0],r,d))
				inter =true;
			if(q->children[1]->triCount>0)
			if(Quad::Intersects(q->children[1],r,d))
				inter =true;
			if(q->children[2]->triCount>0)
			if(Quad::Intersects(q->children[2],r,d))
				inter =true;
			if(q->children[3]->triCount>0)
			if(Quad::Intersects(q->children[3],r,d))
				inter =true;
			if(q->children[4]->triCount>0)
			if(Quad::Intersects(q->children[4],r,d))
				inter =true;
			if(q->children[5]->triCount>0)
			if(Quad::Intersects(q->children[5],r,d))
				inter =true;
			if(q->children[6]->triCount>0)
			if(Quad::Intersects(q->children[6],r,d))
				inter =true;
			if(q->children[7]->triCount>0)
			if(Quad::Intersects(q->children[7],r,d))
				inter =true;
			return inter;
		}
		else
		{
		unsigned int i=0;
		do
		{
			float f;
			if(Ray::InterceptsTriangle(r,&q->positions[i],&q->positions[i+1],&q->positions[i+2],&f))
			{
				if(f<*d)
				*d=f;
				inter = true;
			}
			i+=3;
		}while(i<q->vertCount);
		}
		return inter;
	}
	return false;
}

bool Quad::Intersects(Quad* q,Ray* r,float dist,float* d)
{
	if(q->triCount>0)
	{
		float fl;
		if(Ray::InterceptsBox(r,&q->box->origMin,&q->box->origMax,&fl))
		{
			bool inter=false;
			if(fl<dist)
			{
				if(q->hasChildren)
				{
					if(q->children[0]->triCount>0)
					if(Quad::Intersects(q->children[0],r,dist,d))
						inter =true;
					if(q->children[1]->triCount>0)
					if(Quad::Intersects(q->children[1],r,dist,d))
						inter =true;
					if(q->children[2]->triCount>0)
					if(Quad::Intersects(q->children[2],r,dist,d))
						inter =true;
					if(q->children[3]->triCount>0)
					if(Quad::Intersects(q->children[3],r,dist,d))
						inter =true;
					if(q->children[4]->triCount>0)
					if(Quad::Intersects(q->children[4],r,dist,d))
						inter =true;
					if(q->children[5]->triCount>0)
					if(Quad::Intersects(q->children[5],r,dist,d))
						inter =true;
					if(q->children[6]->triCount>0)
					if(Quad::Intersects(q->children[6],r,dist,d))
						inter =true;
					if(q->children[7]->triCount>0)
					if(Quad::Intersects(q->children[7],r,dist,d))
						inter =true;
					return inter;
				}
				else
				{
					unsigned int i=0;
					do
					{
						float f;
						if(Ray::InterceptsTriangle(r,&q->positions[i],&q->positions[i+1],&q->positions[i+2],&f))
						{
							if(f<*d)
							*d=f;
							inter = true;
						}
						i+=3;
					}while(i<q->vertCount);
				}
			}
			return false;
		}
	}
	return false;
}

void Quad::Split(D3DXVECTOR3* vecs)
{
	hasChildren = true;
	D3DXVECTOR3 v = BoundingBox::GetOrigDimensions(box);
	float dx = v.x/2;
	float dy = v.y/2;
	float dz = v.z/2;
	//quad 1
	//[ ][x]
	//[ ][ ]
	children[0]=new Quad();
	children[0]->box->origMax.x = box->origMax.x;
	children[0]->box->origMax.y = box->origMax.y;
	children[0]->box->origMax.z = box->origMax.z;

	children[0]->box->origMin.x = box->origMax.x-dx;
	children[0]->box->origMin.y = box->origMax.y-dy;
	children[0]->box->origMin.z = box->origMin.z+dz;
	children[0]->box->Transform(&box->lastTransform);
	//quad 2
	//[ ][ ]
	//[ ][x]
	children[1] = new Quad();
	children[1]->box->origMax.x = box->origMax.x;
	children[1]->box->origMax.y = box->origMax.y -dy;
	children[1]->box->origMax.z = box->origMax.z;

	children[1]->box->origMin.x = box->origMax.x -dx;
	children[1]->box->origMin.y = box->origMin.y;
	children[1]->box->origMin.z = box->origMin.z+dz;
	children[1]->box->Transform(&box->lastTransform);
	//quad 3
	//[x][ ]
	//[ ][ ]
	children[2]=new Quad();
	children[2]->box->origMax.x = box->origMax.x -dx;
	children[2]->box->origMax.y = box->origMax.y;
	children[2]->box->origMax.z = box->origMax.z;

	children[2]->box->origMin.x = box->origMin.x;
	children[2]->box->origMin.y = box->origMax.y-dy;
	children[2]->box->origMin.z = box->origMin.z+dz;
	children[2]->box->Transform(&box->lastTransform);
	//quad 4
	//[ ][ ]
	//[x][ ]
	children[3]=new Quad();
	children[3]->box->origMax.x = box->origMax.x -dx;
	children[3]->box->origMax.y = box->origMax.y -dy;
	children[3]->box->origMax.z = box->origMax.z;
	
	children[3]->box->origMin.x = box->origMin.x;
	children[3]->box->origMin.y = box->origMin.y;
	children[3]->box->origMin.z = box->origMin.z+dz;
	children[3]->box->Transform(&box->lastTransform);
	//quad 5
	//[ ][x]
	//[ ][ ]
	children[4]=new Quad();
	children[4]->box->origMax.x = box->origMax.x;
	children[4]->box->origMax.y = box->origMax.y;
	children[4]->box->origMax.z = box->origMax.z-dz;

	children[4]->box->origMin.x = box->origMax.x-dx;
	children[4]->box->origMin.y = box->origMax.y-dy;
	children[4]->box->origMin.z = box->origMin.z;
	children[4]->box->Transform(&box->lastTransform);
	//quad 6
	//[ ][ ]
	//[ ][x]
	children[5] = new Quad();
	children[5]->box->origMax.x = box->origMax.x;
	children[5]->box->origMax.y = box->origMax.y -dy;
	children[5]->box->origMax.z = box->origMax.z-dz;

	children[5]->box->origMin.x = box->origMax.x -dx;
	children[5]->box->origMin.y = box->origMin.y;
	children[5]->box->origMin.z = box->origMin.z;
	children[5]->box->Transform(&box->lastTransform);
	//quad 7
	//[x][ ]
	//[ ][ ]
	children[6]=new Quad();
	children[6]->box->origMax.x = box->origMax.x -dx;
	children[6]->box->origMax.y = box->origMax.y;
	children[6]->box->origMax.z = box->origMax.z-dz;

	children[6]->box->origMin.x = box->origMin.x;
	children[6]->box->origMin.y = box->origMax.y-dy;
	children[6]->box->origMin.z = box->origMin.z;
	children[6]->box->Transform(&box->lastTransform);
	//quad 8
	//[ ][ ]
	//[x][ ]
	children[7]=new Quad();
	children[7]->box->origMin.x = box->origMin.x;
	children[7]->box->origMin.y = box->origMin.y;
	children[7]->box->origMin.z = box->origMin.z;

	children[7]->box->origMax.x = box->origMax.x -dx;
	children[7]->box->origMax.y = box->origMax.y -dy;
	children[7]->box->origMax.z = box->origMax.z -dz;
	children[7]->box->Transform(&box->lastTransform);

	D3DXVECTOR3* vs = new D3DXVECTOR3[3];
	map<unsigned int,D3DXVECTOR3> pos1;
	map<unsigned int,D3DXVECTOR3> pos2;
	map<unsigned int,D3DXVECTOR3> pos3;
	map<unsigned int,D3DXVECTOR3> pos4;
	map<unsigned int,D3DXVECTOR3> pos5;
	map<unsigned int,D3DXVECTOR3> pos6;
	map<unsigned int,D3DXVECTOR3> pos7;
	map<unsigned int,D3DXVECTOR3> pos8;
	//we need to find the min and max of the triangle so that we can check 
	//which quads the triangle intercepts
	unsigned int i =0;
	do
	{
		vs[0]=vecs[i];
		vs[1]=vecs[i+1];
		vs[2]=vecs[i+2];
		bool inter=false;
		int ti=0;
		if(BoundingBox::IntersectsOrig(children[0]->box,&vs[0]) || BoundingBox::IntersectsOrig(children[0]->box,&vs[1]) || BoundingBox::IntersectsOrig(children[0]->box,&vs[2]))
		{
			pos1.insert(pair<unsigned int,D3DXVECTOR3>(children[0]->vertCount,vecs[i]));
			children[0]->vertCount++;
			pos1.insert(pair<unsigned int,D3DXVECTOR3>(children[0]->vertCount,vecs[i+1]));
			children[0]->vertCount++;
			pos1.insert(pair<unsigned int,D3DXVECTOR3>(children[0]->vertCount,vecs[i+2]));
			children[0]->vertCount++;
			children[0]->triCount++;
			inter=true;
			ti++;
		}
		if(BoundingBox::IntersectsOrig(children[1]->box,&vs[0]) || BoundingBox::IntersectsOrig(children[1]->box,&vs[1]) || BoundingBox::IntersectsOrig(children[1]->box,&vs[2]))
		{
			pos2.insert(pair<unsigned int,D3DXVECTOR3>(children[1]->vertCount,vecs[i]));
			children[1]->vertCount++;
			pos2.insert(pair<unsigned int,D3DXVECTOR3>(children[1]->vertCount,vecs[i+1]));
			children[1]->vertCount++;
			pos2.insert(pair<unsigned int,D3DXVECTOR3>(children[1]->vertCount,vecs[i+2]));
			children[1]->vertCount++;
			children[1]->triCount++;
			inter=true;
			ti++;
		}
		if(BoundingBox::IntersectsOrig(children[2]->box,&vs[0]) || BoundingBox::IntersectsOrig(children[2]->box,&vs[1]) || BoundingBox::IntersectsOrig(children[2]->box,&vs[2]))
		{
			pos3.insert(pair<unsigned int,D3DXVECTOR3>(children[2]->vertCount,vecs[i]));
			children[2]->vertCount++;
			pos3.insert(pair<unsigned int,D3DXVECTOR3>(children[2]->vertCount,vecs[i+1]));
			children[2]->vertCount++;
			pos3.insert(pair<unsigned int,D3DXVECTOR3>(children[2]->vertCount,vecs[i+2]));
			children[2]->vertCount++;
			children[2]->triCount++;
			inter=true;
			ti++;
		}
		if(BoundingBox::IntersectsOrig(children[3]->box,&vs[0]) || BoundingBox::IntersectsOrig(children[3]->box,&vs[1]) || BoundingBox::IntersectsOrig(children[3]->box,&vs[2]))
		{
			pos4.insert(pair<unsigned int,D3DXVECTOR3>(children[3]->vertCount,vecs[i]));
			children[3]->vertCount++;
			pos4.insert(pair<unsigned int,D3DXVECTOR3>(children[3]->vertCount,vecs[i+1]));
			children[3]->vertCount++;
			pos4.insert(pair<unsigned int,D3DXVECTOR3>(children[3]->vertCount,vecs[i+2]));
			children[3]->vertCount++;
			children[3]->triCount++;
			inter=true;
			ti++;
		}
		if(BoundingBox::IntersectsOrig(children[4]->box,&vs[0]) || BoundingBox::IntersectsOrig(children[4]->box,&vs[1]) || BoundingBox::IntersectsOrig(children[4]->box,&vs[2]))
		{
			pos5.insert(pair<unsigned int,D3DXVECTOR3>(children[4]->vertCount,vecs[i]));
			children[4]->vertCount++;
			pos5.insert(pair<unsigned int,D3DXVECTOR3>(children[4]->vertCount,vecs[i+1]));
			children[4]->vertCount++;
			pos5.insert(pair<unsigned int,D3DXVECTOR3>(children[4]->vertCount,vecs[i+2]));
			children[4]->vertCount++;
			children[4]->triCount++;
			inter=true;
			ti++;
		}
		if(BoundingBox::IntersectsOrig(children[5]->box,&vs[0]) || BoundingBox::IntersectsOrig(children[5]->box,&vs[1]) || BoundingBox::IntersectsOrig(children[5]->box,&vs[2]))
		{
			pos6.insert(pair<unsigned int,D3DXVECTOR3>(children[5]->vertCount,vecs[i]));
			children[5]->vertCount++;
			pos6.insert(pair<unsigned int,D3DXVECTOR3>(children[5]->vertCount,vecs[i+1]));
			children[5]->vertCount++;
			pos6.insert(pair<unsigned int,D3DXVECTOR3>(children[5]->vertCount,vecs[i+2]));
			children[5]->vertCount++;
			children[5]->triCount++;
			inter=true;
			ti++;
		}
		if(BoundingBox::IntersectsOrig(children[6]->box,&vs[0]) || BoundingBox::IntersectsOrig(children[6]->box,&vs[1]) || BoundingBox::IntersectsOrig(children[6]->box,&vs[2]))
		{
			pos7.insert(pair<unsigned int,D3DXVECTOR3>(children[6]->vertCount,vecs[i]));
			children[6]->vertCount++;
			pos7.insert(pair<unsigned int,D3DXVECTOR3>(children[6]->vertCount,vecs[i+1]));
			children[6]->vertCount++;
			pos7.insert(pair<unsigned int,D3DXVECTOR3>(children[6]->vertCount,vecs[i+2]));
			children[6]->vertCount++;
			children[6]->triCount++;
			inter=true;
			ti++;
		}
		if(BoundingBox::IntersectsOrig(children[7]->box,&vs[0]) || BoundingBox::IntersectsOrig(children[7]->box,&vs[1]) || BoundingBox::IntersectsOrig(children[7]->box,&vs[2]))
		{
			pos8.insert(pair<unsigned int,D3DXVECTOR3>(children[7]->vertCount,vecs[i]));
			children[7]->vertCount++;
			pos8.insert(pair<unsigned int,D3DXVECTOR3>(children[7]->vertCount,vecs[i+1]));
			children[7]->vertCount++;
			pos8.insert(pair<unsigned int,D3DXVECTOR3>(children[7]->vertCount,vecs[i+2]));
			children[7]->vertCount++;
			children[7]->triCount++;
			inter=true;
			ti++;
		}
		if(inter == false)
			int test=0;
		i+=3;
	}while(i<vertCount);
	this;
	children[0]->positions=new D3DXVECTOR3[children[0]->vertCount];
	i=0;
	do
	{
		children[0]->positions[i]=pos1[i];
		i++;
	}while(i<children[0]->vertCount);
	children[1]->positions=new D3DXVECTOR3[children[1]->vertCount];
	i=0;
	do
	{
		children[1]->positions[i]=pos2[i];
		i++;
	}while(i<children[1]->vertCount);
	children[2]->positions=new D3DXVECTOR3[children[2]->vertCount];
	i=0;
	do
	{
		children[2]->positions[i]=pos3[i];
		i++;
	}while(i<children[2]->vertCount);
	children[3]->positions=new D3DXVECTOR3[children[3]->vertCount];
	i=0;
	do
	{
		children[3]->positions[i]=pos4[i];
		i++;
	}while(i<children[3]->vertCount);
	i=0;
	children[4]->positions=new D3DXVECTOR3[children[4]->vertCount];
	do
	{
		children[4]->positions[i]=pos5[i];
		i++;
	}while(i<children[4]->vertCount);
	children[5]->positions=new D3DXVECTOR3[children[5]->vertCount];
	i=0;
	do
	{
		children[5]->positions[i]=pos6[i];
		i++;
	}while(i<children[5]->vertCount);
	children[6]->positions=new D3DXVECTOR3[children[6]->vertCount];
	i=0;
	do
	{
		children[6]->positions[i]=pos7[i];
		i++;
	}while(i<children[6]->vertCount);
	children[7]->positions=new D3DXVECTOR3[children[7]->vertCount];
	i=0;
	do
	{
		children[7]->positions[i]=pos8[i];
		i++;
	}while(i<children[7]->vertCount);
	vs=0;
	delete vs;
}

void Quad::Split()
{
	hasChildren = true;
	D3DXVECTOR3 v = BoundingBox::GetOrigDimensions(box);
	float dx = v.x/2;
	float dy = v.y/2;
	float dz = v.z/2;
	//quad 1
	//[ ][x]
	//[ ][ ]
	children[0]=new Quad();
	children[0]->box->origMax.x = box->origMax.x;
	children[0]->box->origMax.y = box->origMax.y;
	children[0]->box->origMax.z = box->origMax.z;

	children[0]->box->origMin.x = box->origMax.x-dx;
	children[0]->box->origMin.y = box->origMax.y-dy;
	children[0]->box->origMin.z = box->origMin.z+dz;
	children[0]->box->Transform(&box->lastTransform);
	//quad 2
	//[ ][ ]
	//[ ][x]
	children[1] = new Quad();
	children[1]->box->origMax.x = box->origMax.x;
	children[1]->box->origMax.y = box->origMax.y -dy;
	children[1]->box->origMax.z = box->origMax.z;

	children[1]->box->origMin.x = box->origMax.x -dx;
	children[1]->box->origMin.y = box->origMin.y;
	children[1]->box->origMin.z = box->origMin.z+dz;
	children[1]->box->Transform(&box->lastTransform);
	//quad 3
	//[x][ ]
	//[ ][ ]
	children[2]=new Quad();
	children[2]->box->origMax.x = box->origMax.x -dx;
	children[2]->box->origMax.y = box->origMax.y;
	children[2]->box->origMax.z = box->origMax.z;

	children[2]->box->origMin.x = box->origMin.x;
	children[2]->box->origMin.y = box->origMax.y-dy;
	children[2]->box->origMin.z = box->origMin.z+dz;
	children[2]->box->Transform(&box->lastTransform);
	//quad 4
	//[ ][ ]
	//[x][ ]
	children[3]=new Quad();
	children[3]->box->origMax.x = box->origMax.x -dx;
	children[3]->box->origMax.y = box->origMax.y -dy;
	children[3]->box->origMax.z = box->origMax.z;
	
	children[3]->box->origMin.x = box->origMin.x;
	children[3]->box->origMin.y = box->origMin.y;
	children[3]->box->origMin.z = box->origMin.z+dz;
	children[3]->box->Transform(&box->lastTransform);
	//quad 5
	//[ ][x]
	//[ ][ ]
	children[4]=new Quad();
	children[4]->box->origMax.x = box->origMax.x;
	children[4]->box->origMax.y = box->origMax.y;
	children[4]->box->origMax.z = box->origMax.z-dz;

	children[4]->box->origMin.x = box->origMax.x-dx;
	children[4]->box->origMin.y = box->origMax.y-dy;
	children[4]->box->origMin.z = box->origMin.z;
	children[4]->box->Transform(&box->lastTransform);
	//quad 6
	//[ ][ ]
	//[ ][x]
	children[5] = new Quad();
	children[5]->box->origMax.x = box->origMax.x;
	children[5]->box->origMax.y = box->origMax.y -dy;
	children[5]->box->origMax.z = box->origMax.z-dz;

	children[5]->box->origMin.x = box->origMax.x -dx;
	children[5]->box->origMin.y = box->origMin.y;
	children[5]->box->origMin.z = box->origMin.z;
	children[5]->box->Transform(&box->lastTransform);
	//quad 7
	//[x][ ]
	//[ ][ ]
	children[6]=new Quad();
	children[6]->box->origMax.x = box->origMax.x -dx;
	children[6]->box->origMax.y = box->origMax.y;
	children[6]->box->origMax.z = box->origMax.z-dz;

	children[6]->box->origMin.x = box->origMin.x;
	children[6]->box->origMin.y = box->origMax.y-dy;
	children[6]->box->origMin.z = box->origMin.z;
	children[6]->box->Transform(&box->lastTransform);
	//quad 8
	//[ ][ ]
	//[x][ ]
	children[7]=new Quad();
	children[7]->box->origMin.x = box->origMin.x;
	children[7]->box->origMin.y = box->origMin.y;
	children[7]->box->origMin.z = box->origMin.z;

	children[7]->box->origMax.x = box->origMax.x -dx;
	children[7]->box->origMax.y = box->origMax.y -dy;
	children[7]->box->origMax.z = box->origMax.z -dz;
	children[7]->box->Transform(&box->lastTransform);

	D3DXVECTOR3* vs = new D3DXVECTOR3[3];
	map<unsigned int,D3DXVECTOR3> pos1;
	map<unsigned int,D3DXVECTOR3> pos2;
	map<unsigned int,D3DXVECTOR3> pos3;
	map<unsigned int,D3DXVECTOR3> pos4;
	map<unsigned int,D3DXVECTOR3> pos5;
	map<unsigned int,D3DXVECTOR3> pos6;
	map<unsigned int,D3DXVECTOR3> pos7;
	map<unsigned int,D3DXVECTOR3> pos8;
	//we need to find the min and max of the triangle so that we can check 
	//which quads the triangle intercepts
	unsigned int i =0;
	do
	{
		vs[0]=positions[i];
		vs[1]=positions[i+1];
		vs[2]=positions[i+2];
		bool inter=false;
		int ti=0;
		if(BoundingBox::IntersectsOrig(children[0]->box,&vs[0]) || BoundingBox::IntersectsOrig(children[0]->box,&vs[1]) || BoundingBox::IntersectsOrig(children[0]->box,&vs[2]))
		{
			pos1.insert(pair<unsigned int,D3DXVECTOR3>(children[0]->vertCount,positions[i]));
			children[0]->vertCount++;
			pos1.insert(pair<unsigned int,D3DXVECTOR3>(children[0]->vertCount,positions[i+1]));
			children[0]->vertCount++;
			pos1.insert(pair<unsigned int,D3DXVECTOR3>(children[0]->vertCount,positions[i+2]));
			children[0]->vertCount++;
			children[0]->triCount++;
			inter=true;
			ti++;
		}
		if(BoundingBox::IntersectsOrig(children[1]->box,&vs[0]) || BoundingBox::IntersectsOrig(children[1]->box,&vs[1]) || BoundingBox::IntersectsOrig(children[1]->box,&vs[2]))
		{
			pos2.insert(pair<unsigned int,D3DXVECTOR3>(children[1]->vertCount,positions[i]));
			children[1]->vertCount++;
			pos2.insert(pair<unsigned int,D3DXVECTOR3>(children[1]->vertCount,positions[i+1]));
			children[1]->vertCount++;
			pos2.insert(pair<unsigned int,D3DXVECTOR3>(children[1]->vertCount,positions[i+2]));
			children[1]->vertCount++;
			children[1]->triCount++;
			inter=true;
			ti++;
		}
		if(BoundingBox::IntersectsOrig(children[2]->box,&vs[0]) || BoundingBox::IntersectsOrig(children[2]->box,&vs[1]) || BoundingBox::IntersectsOrig(children[2]->box,&vs[2]))
		{
			pos3.insert(pair<unsigned int,D3DXVECTOR3>(children[2]->vertCount,positions[i]));
			children[2]->vertCount++;
			pos3.insert(pair<unsigned int,D3DXVECTOR3>(children[2]->vertCount,positions[i+1]));
			children[2]->vertCount++;
			pos3.insert(pair<unsigned int,D3DXVECTOR3>(children[2]->vertCount,positions[i+2]));
			children[2]->vertCount++;
			children[2]->triCount++;
			inter=true;
			ti++;
		}
		if(BoundingBox::IntersectsOrig(children[3]->box,&vs[0]) || BoundingBox::IntersectsOrig(children[3]->box,&vs[1]) || BoundingBox::IntersectsOrig(children[3]->box,&vs[2]))
		{
			pos4.insert(pair<unsigned int,D3DXVECTOR3>(children[3]->vertCount,positions[i]));
			children[3]->vertCount++;
			pos4.insert(pair<unsigned int,D3DXVECTOR3>(children[3]->vertCount,positions[i+1]));
			children[3]->vertCount++;
			pos4.insert(pair<unsigned int,D3DXVECTOR3>(children[3]->vertCount,positions[i+2]));
			children[3]->vertCount++;
			children[3]->triCount++;
			inter=true;
			ti++;
		}
		if(BoundingBox::IntersectsOrig(children[4]->box,&vs[0]) || BoundingBox::IntersectsOrig(children[4]->box,&vs[1]) || BoundingBox::IntersectsOrig(children[4]->box,&vs[2]))
		{
			pos5.insert(pair<unsigned int,D3DXVECTOR3>(children[4]->vertCount,positions[i]));
			children[4]->vertCount++;
			pos5.insert(pair<unsigned int,D3DXVECTOR3>(children[4]->vertCount,positions[i+1]));
			children[4]->vertCount++;
			pos5.insert(pair<unsigned int,D3DXVECTOR3>(children[4]->vertCount,positions[i+2]));
			children[4]->vertCount++;
			children[4]->triCount++;
			inter=true;
			ti++;
		}
		if(BoundingBox::IntersectsOrig(children[5]->box,&vs[0]) || BoundingBox::IntersectsOrig(children[5]->box,&vs[1]) || BoundingBox::IntersectsOrig(children[5]->box,&vs[2]))
		{
			pos6.insert(pair<unsigned int,D3DXVECTOR3>(children[5]->vertCount,positions[i]));
			children[5]->vertCount++;
			pos6.insert(pair<unsigned int,D3DXVECTOR3>(children[5]->vertCount,positions[i+1]));
			children[5]->vertCount++;
			pos6.insert(pair<unsigned int,D3DXVECTOR3>(children[5]->vertCount,positions[i+2]));
			children[5]->vertCount++;
			children[5]->triCount++;
			inter=true;
			ti++;
		}
		if(BoundingBox::IntersectsOrig(children[6]->box,&vs[0]) || BoundingBox::IntersectsOrig(children[6]->box,&vs[1]) || BoundingBox::IntersectsOrig(children[6]->box,&vs[2]))
		{
			pos7.insert(pair<unsigned int,D3DXVECTOR3>(children[6]->vertCount,positions[i]));
			children[6]->vertCount++;
			pos7.insert(pair<unsigned int,D3DXVECTOR3>(children[6]->vertCount,positions[i+1]));
			children[6]->vertCount++;
			pos7.insert(pair<unsigned int,D3DXVECTOR3>(children[6]->vertCount,positions[i+2]));
			children[6]->vertCount++;
			children[6]->triCount++;
			inter=true;
			ti++;
		}
		if(BoundingBox::IntersectsOrig(children[7]->box,&vs[0]) || BoundingBox::IntersectsOrig(children[7]->box,&vs[1]) || BoundingBox::IntersectsOrig(children[7]->box,&vs[2]))
		{
			pos8.insert(pair<unsigned int,D3DXVECTOR3>(children[7]->vertCount,positions[i]));
			children[7]->vertCount++;
			pos8.insert(pair<unsigned int,D3DXVECTOR3>(children[7]->vertCount,positions[i+1]));
			children[7]->vertCount++;
			pos8.insert(pair<unsigned int,D3DXVECTOR3>(children[7]->vertCount,positions[i+2]));
			children[7]->vertCount++;
			children[7]->triCount++;
			inter=true;
			ti++;
		}
		if(inter == false)
			int test=0;
		i+=3;
	}while(i<vertCount);
	this;
	children[0]->positions=new D3DXVECTOR3[children[0]->vertCount];
	i=0;
	do
	{
		children[0]->positions[i]=pos1[i];
		i++;
	}while(i<children[0]->vertCount);
	children[1]->positions=new D3DXVECTOR3[children[1]->vertCount];
	i=0;
	do
	{
		children[1]->positions[i]=pos2[i];
		i++;
	}while(i<children[1]->vertCount);
	children[2]->positions=new D3DXVECTOR3[children[2]->vertCount];
	i=0;
	do
	{
		children[2]->positions[i]=pos3[i];
		i++;
	}while(i<children[2]->vertCount);
	children[3]->positions=new D3DXVECTOR3[children[3]->vertCount];
	i=0;
	do
	{
		children[3]->positions[i]=pos4[i];
		i++;
	}while(i<children[3]->vertCount);
	i=0;
	children[4]->positions=new D3DXVECTOR3[children[4]->vertCount];
	do
	{
		children[4]->positions[i]=pos5[i];
		i++;
	}while(i<children[4]->vertCount);
	children[5]->positions=new D3DXVECTOR3[children[5]->vertCount];
	i=0;
	do
	{
		children[5]->positions[i]=pos6[i];
		i++;
	}while(i<children[5]->vertCount);
	children[6]->positions=new D3DXVECTOR3[children[6]->vertCount];
	i=0;
	do
	{
		children[6]->positions[i]=pos7[i];
		i++;
	}while(i<children[6]->vertCount);
	children[7]->positions=new D3DXVECTOR3[children[7]->vertCount];
	i=0;
	do
	{
		children[7]->positions[i]=pos8[i];
		i++;
	}while(i<children[7]->vertCount);
	vs=0;
	delete vs;
	positions=0;
	delete positions;
}