#include "ModelLoader.h"


ModelLoader::ModelLoader(void)
{
	bTrCount=0;
	loc=0;
	atofcs=new char[20];
}

ModelLoader::~ModelLoader(void)
{
}

void ModelLoader::Initialize(Shaders* sha,ID3D10Device* de,HWND* h)
{
	hwnd = h;
	sh=sha;
	dev=de;
}

bool ModelLoader::Load(wchar_t* path,Model* model)
{
	model->filename.assign(path);
	model->filename.erase(0,model->filename.find_last_of('/')+1);
	unsigned int ind=model->filename.find_first_of('.');
	model->filename.erase(ind,model->filename.length()-ind);
	model->filePath.assign(path);
	ifstream ifs;
	ifs.open(path);
	if(ifs.fail())
	{
		return false;
	};
	long begin_byte, end_byte;
	begin_byte = (long)ifs.tellg();
	ifs.seekg (0, ios::end);
	end_byte = (long)ifs.tellg();
	int total_bytes = end_byte - begin_byte;
	ifs.seekg(0, ios::beg);
	buf = new char[total_bytes + 1];
	ifs.read(buf,total_bytes);
	ifs.close();
	loc=0;
	Skip(20,'{');
	Skip(1000,'}');
	Skip(20,'{');
	Skip(1);this;
	mod = model;
	//first in the file is file info we skip it
	GetMaterials();
	//then come the objects so we load them
	GetObjects();
	//then we get object connections
	Connections();
	//we set the root of the model
	mod->root = mod->objects[0];
	//lastly we connect the bone data with their dummy trasforms
	unsigned int i=0;
	unsigned int boi=0;
	do
	{
		if(mod->Meshes[i]->hasBones)
		{
			unsigned int bi=0;
			do
			{
				mod->Meshes[i]->Bones[bi]->base=mod->objects[this->boneTrInds[boi]];
				bi++;
				boi++;
			}while(bi<mod->Meshes[i]->boneCount);
		}
		i++;
	}while(i<mod->meshCount);
	i=0;
	do
	{
		if(mod->objects[i]->type == Base::target)
		{
			unsigned int ci=0;
			do
			{
				if(mod->objects[i]->name->find(mod->Cameras[ci]->name->data())!=-1)
				{
					mod->Cameras[ci]->Target=mod->objects[i];
					mod->Cameras[ci]->hasTarget=true;
				}
				ci++;
			}while(ci<mod->camCount);
			
		}
		i++;
	}while(i<mod->objCount);
	i=0;
	do
	{
		mod->Meshes[i]->UpdateMesh();
		i++;
	}while(i<mod->meshCount);
	mod->root->hasMatrix=false;
	buf=0;
	delete buf;
	mod->Update();
	
	return true;
}

bool ModelLoader::Load(wchar_t* path,Gun* model)
{
	model->filename.assign(path);
	model->filename.erase(0,model->filename.find_last_of('/')+1);
	unsigned int ind=model->filename.find_first_of('.');
	model->filename.erase(ind,model->filename.length()-ind);
	model->filePath.assign(path);
	ifstream ifs;
	ifs.open(path);
	if(ifs.fail())
	{
		return false;
	};
	long begin_byte, end_byte;
	begin_byte = (long)ifs.tellg();
	ifs.seekg (0, ios::end);
	end_byte = (long)ifs.tellg();
	int total_bytes = end_byte - begin_byte;
	ifs.seekg(0, ios::beg);
	buf = new char[total_bytes + 1];
	ifs.read(buf,total_bytes);
	ifs.close();
	loc=0;
	Skip(20,'{');
	Skip(1);
	do
	{
		char* cs = GetParam();
		if(strcmp(cs,"Name")==0)
		{
			Skip(1);
			model->name = *Get();
		}
		else
		if(strcmp(cs,"MagSize")==0)
		{
			Skip(1);
			model->magSize = GetInt();
		}
		else
		if(strcmp(cs,"ScopeMinZoom")==0)
		{
			Skip(1);
			model->scopeMinZoom=GetFloat();
			model->hasScope=true;
		}
		else
		if(strcmp(cs,"ScopeMaxZoom")==0)
		{
			Skip(1);
			model->scopeMaxZoom=GetFloat();
			model->hasScope=true;
		}
		else
		if(strcmp(cs,"Damage")==0)
		{
			Skip(1);
			model->damage=GetFloat();
		}
		else
		if(strcmp(cs,"AnimShootStart")==0)
		{
			Skip(1);
			model->animShootStart=(unsigned int)GetInt();
		}
		else
		if(strcmp(cs,"AnimShootEnd")==0)
		{
			Skip(1);
			model->animShootEnd=(unsigned int)GetInt();
		}
		else
		if(strcmp(cs,"AnimPrepStart")==0)
		{
			Skip(1);
			model->animPrepStart=(unsigned int)GetInt();
		}
		else
		if(strcmp(cs,"AnimPrepEnd")==0)
		{
			Skip(1);
			model->animPrepEnd=(unsigned int)GetInt();
		}
	}while(GoNextParam());
	Skip(20,'{');
	mod = model;
	//first in the file is file info we skip it
	GetMaterials();
	//then come the objects so we load them
	GetObjects();
	//then we get object connections
	Connections();
	//we set the root of the model
	mod->root = mod->objects[0];
	//lastly we connect the bone data with their dummy trasforms
	unsigned int i=0;
	unsigned int boi=0;
	do
	{
		if(mod->Meshes[i]->hasBones)
		{
			unsigned int bi=0;
			do
			{
				mod->Meshes[i]->Bones[bi]->base=mod->objects[this->boneTrInds[boi]];
				bi++;
				boi++;
			}while(bi<mod->Meshes[i]->boneCount);
		}
		i++;
	}while(i<mod->meshCount);
	i=0;
	do
	{
		if(mod->objects[i]->type == Base::target)
		{
			unsigned int ci=0;
			do
			{
				if(mod->objects[i]->name->find(mod->Cameras[ci]->name->data())!=-1)
				{
					mod->Cameras[ci]->Target=mod->objects[i];
					mod->Cameras[ci]->hasTarget=true;
				}
				ci++;
			}while(ci<mod->camCount);
			
		}
		if(strcmp(mod->objects[i]->name->data(),"Handle1")==0)
			model->handle1=model->objects[i];
		else
		if(strcmp(mod->objects[i]->name->data(),"Handle2")==0)
			model->handle2=model->objects[i];
		else
		if(strcmp(mod->objects[i]->name->data(),"DirPoint1")==0)
		{
			model->dir.origDir = Base::MatrixPosition(&mod->objects[i]->Matrix);
			model->barrel=i;
		}
		else
		if(strcmp(mod->objects[i]->name->data(),"DirPoint2")==0)
		{
			model->dir.origDir = Base::MatrixPosition(&mod->objects[i]->Matrix);
			D3DXVec3Normalize(&model->dir.origDir,&model->dir.origDir);
			model->barreldirection=i;
		}
		i++;
	}while(i<mod->objCount);
	i=0;
	do
	{
		mod->Meshes[i]->UpdateMesh();
		i++;
	}while(i<mod->meshCount);
	mod->root->hasMatrix=false;
	buf=0;
	delete buf;
	mod->Update();
	return true;
}

D3DXVECTOR3* ModelLoader::InVec()
{
	D3DXVECTOR3* v=new D3DXVECTOR3();
	v->x = (float)GetDouble();
	Skip(1);
	v->y = (float)GetDouble();
	Skip(1);
	v->z = (float)GetDouble();
	return v;
}

D3DXVECTOR4* ModelLoader::InVec4()
{
	D3DXVECTOR4* v = new D3DXVECTOR4();
	v->x = GetFloat();
	Skip(1);
	v->y = GetFloat();
	Skip(1);
	v->z = GetFloat();
	Skip(1);
	v->w = GetFloat();
	return v;
}

D3DXQUATERNION* ModelLoader::InQuat()
{
	D3DXQUATERNION* v = new D3DXQUATERNION();
	v->x = GetFloat();
	Skip(1);
	v->y = GetFloat();
	Skip(1);
	v->z = GetFloat();
	Skip(1);
	v->w = GetFloat();
	return v;
}

D3DXMATRIX ModelLoader::InMatrix()
{
	D3DXMATRIX mat;
	mat._11=GetFloat();
	Skip(1);
	mat._12=GetFloat();
	Skip(1);
	mat._13=GetFloat();
	Skip(1);
	mat._14=GetFloat();
	Skip(1);

	mat._21=GetFloat();
	Skip(1);
	mat._22=GetFloat();
	Skip(1);
	mat._23=GetFloat();
	Skip(1);
	mat._24=GetFloat();
	Skip(1);

	mat._31=GetFloat();
	Skip(1);
	mat._32=GetFloat();
	Skip(1);
	mat._33=GetFloat();
	Skip(1);
	mat._34=GetFloat();
	Skip(1);

	mat._41=GetFloat();
	Skip(1);
	mat._42=GetFloat();
	Skip(1);
	mat._43=GetFloat();
	Skip(1);
	mat._44=GetFloat();
	return mat;
}

Camera* ModelLoader::InCamera()
{
	Camera* cam = new Camera();
	cam->hasMatrix=true;
	Skip(10,'=');
	cam->name->assign(GetParam());
	Skip(20,'=');
	buf[loc];
	cam->Matrix = InMatrix();
	Skip(20,'=');
	cam->aspect=GetFloat();
	Skip(20,'=');
	cam->clipNear=GetFloat();
	Skip(20,'=');
	cam->clipFar=GetFloat();
	Skip(20,'=');
	cam->fov=GetFloat();
	Skip(20,'=');
	cam->lookAt = *InVec();
	Skip(20,'=');
	cam->position = *InVec();
	Skip(20,'=');
	cam->up = *InVec();
	do
	{
		if(!GoNextParam())
			return cam;
		string s=*Get();
		if(strcmp(s.data(),"Animation")==0)
		{
			cam->AddAnimation(InAnimation());
		}
	}while(true);
}

Mesh* ModelLoader::InMesh()
{
	Mesh* m = new Mesh();
	m->hasMatrix=true;
	m->hasAnimation=true;
	const char* cs=0;
	D3DXVECTOR3* verts=0;
	D3DXVECTOR3* norms=0;
	D3DXVECTOR2* texs=0;
	long* indices=0;
	unsigned int indexCount=0;
	unsigned int count=0;
	Skip(20,'=');
	//we get the name
	m->name->assign(GetParam());

	Skip(20,'=');
	//we get the matrix
	m->Matrix = InMatrix();
	m->hasAnimation=false;
	Skip(20,'=');
	//we get the matIndex
	unsigned int index =0;
	index=GetInt();
	m->mat = mod->Materials[index];

	Skip(20,'=');
	//we get vertexCount
	count=GetInt();
	m->mVertexCount=count;
	verts = new D3DXVECTOR3[count];
	norms = new D3DXVECTOR3[count];
	

	Skip(20,'=');
	//we get indexCount
	indexCount=GetInt();
	m->mIndexCount=indexCount;
	indices=new long[indexCount];

	//we get indices
	Skip(20,'{');
	Skip(1);
	unsigned int i=0;
	do
	{
		indices[i]=(long)GetDouble();
		Skip(1);
		i++;
	}while(i<indexCount);
	Skip(20,'}');
	Skip(20,'{');
	Skip(1);
	i=0;
	do
	{
		verts[i] = *InVec();
		Skip(1);
		i++;
	}while(i<count);
	Skip(20,'}');
	Skip(20,'{');
	Skip(1);
	i=0;
	buf[loc];
	do
	{
		norms[i] = *InVec();
		Skip(1);
		i++;
	}while(i<count);
	texs = new D3DXVECTOR2[count];
	//we get texcoords
	Skip(20,'}');
	//we check for params that may exist by calling
	///the gonextparam which will return the beginning of next param f it exits
	do
	{
		if(!GoNextParam())
		{	
			if(m->boneCount==0)
			{
				if(texs==0)
				{
					m->InitializeBuffers(dev,verts,norms,indices);
					indices =0;
					delete indices;
					verts=0;
					delete verts;
					norms =0;
					delete norms;
				}
				else
				{
					m->InitializeBuffers(dev,verts,norms,texs,indices);
					indices =0;
					delete indices;
					verts=0;
					delete verts;
					norms =0;
					delete norms;
					texs=0;
					delete texs;
				}
			}
			else
			{
				m->InitializeBoneBuffers(dev,verts,norms,texs,indices);
				indices =0;
				delete indices;
				verts=0;
				delete verts;
				norms =0;
				delete norms;
				texs=0;
				delete texs;
			}
			//we generate the quads the function will see if it it necessary
			//m->hMap = Mesh::ray;
			m->GenQuads();
			return m;
		}
		string s=*Get();
		if(strcmp(s.data(),"TexCoords")==0)
		{
			Skip(20,'{');
			Skip(1);
			i=0;
			do
			{
				texs[i].x=GetFloat();
				Skip(1);
				texs[i].y=GetFloat();
				Skip(1);
				i++;
			}while(i<count);
			texs[count-1];
			Skip(20,'}');
		}
		else
		if(strcmp(s.data(),"Animation")==0)
		{
			m->AddAnimation(InAnimation());
		}
		else
		if(strcmp(s.data(),"Bone")==0)
		{
			m->AddBone(InBone());
		}
		else
		if(strcmp(s.data(),"Properties")==0)
		{
			this->InMeshProps(m);
		}
	}while(true);
}

Bone* ModelLoader::InBone()
{
	Bone* b = new Bone();
	Skip(14);
	unsigned int tr=0;
	tr=GetInt();
	this->boneTrInds.insert(pair<unsigned int,unsigned int>(bTrCount,tr));
	bTrCount++;
	Skip(9);
	b->AddOffset(InMatrix());
	//weights
	Skip(8);
	b->numWeights=GetInt();
	b->indices=new unsigned int[b->numWeights];
	b->weights=new float[b->numWeights];
	unsigned int i=0;
	buf[loc];
	Skip(12);
	do
	{
		b->weights[i]=GetFloat();
		Skip(1);
		i++;
	}while(i<b->numWeights);
	i=0;
	Skip(14);
	do
	{
		b->indices[i]=GetInt();
		Skip(1);
		i++;
	}while(i<b->numWeights);
	Skip(5);
	return b;
}

Base* ModelLoader::InDummy()
{
	Base* b = new Base();
	b->hasMatrix=true;
	b->hasAnimation=true;
	Skip(9);
	b->name->assign(GetParam());
	
	if(b->name->find(".Target")!=-1)
	{
		b->type=Base::target;
	}
	Skip(9);
	b->Matrix = InMatrix();
	do
	{
		if(!GoNextParam())
			return b;
		string s=*Get();
		if(strcmp(s.data(),"Animation")==0)
		{
			b->AddAnimation(InAnimation());
		}
	}while(true);
}

Material* ModelLoader::InMaterial()
{
	Material* mat = new Material();
	Skip(17);
	buf[loc];
	mat->name.assign(GetParam());
	Skip(20,'=');
	mat->color.w=GetFloat();
	Skip(20,'=');
	D3DXVECTOR3 col = *InVec();
	mat->color.x =col.x;
	mat->color.y =col.y;
	mat->color.z =col.z;
	Skip(20,'=');
	mat->ambientColor = *InVec();
	Skip(20,'=');
	mat->specularColor = *InVec();
	bool hascolor=true;
	do
	{
		if(GoNextParam())
		{
			Skip(20,'=');
			string* s = Get();
			TextureClass* tex = new TextureClass();
			tex->Initialize(dev,s->data());
			if(!tex)
			{
				wstring st;
				st.assign(L"Could not find texture file:");
				st.append(tex->name);
				st.append(L" at ");
				st.append(tex->path);
				st.append(L" place the texture file to C:/RepoBond/Models/");
				st.append(mod->filename.data());
				st.append(L" to correct the problem");
				MessageBoxW(*hwnd,st.data(),NULL,NULL);
				
			}
			sh->textures.insert(pair<unsigned int,TextureClass*>(sh->texCount,tex));
			sh->texCount++;
			mat->diffuseTex=sh->texCount-1;
			hascolor=false;
		}
		else
		{
			mat->has_color=hascolor;
			return mat;
		}
	}while(true);
	return mat;
}

Animation* ModelLoader::InAnimation()
{
	Animation* anim = new Animation();
	Skip(20,'=');
	anim->length=GetDouble();
	Skip(20,'=');
	//we don't need the tickPerMS so we skip them
	Skip(20,'=');
	anim->name.assign(GetParam());
	
	Skip(20,'{');
	Skip(20,'=');
	anim->numKeyPos=GetInt();
	Skip(1);
	anim->posKeys=new AnimKey[anim->numKeyPos];
	unsigned int i=0;
	do
	{
		anim->posKeys[i].value = InVec();
		Skip(1);
		anim->posKeys[i].time=GetDouble();
		Skip(1);
		i++;
	}while(i<anim->numKeyPos);
	Skip(20,'}');
	Skip(20,'{');
	Skip(20,'=');
	anim->numKeyRot=GetInt();
	Skip(1);
	anim->rotKeys=new AnimKey[anim->numKeyRot];
	i=0;
	
	do
	{
		anim->rotKeys[i].rotvalue = InQuat();
		Skip(1);
		anim->rotKeys[i].time=GetDouble();
		Skip(1);
		i++;
	}while(i<anim->numKeyRot);
	Skip(20,'}');

	Skip(20,'{');
	Skip(20,'=');
	anim->numKeySc=GetInt();
	Skip(1);
	anim->scKeys=new AnimKey[anim->numKeySc];
	i=0;
	do
	{
		anim->scKeys[i].value = InVec();
		Skip(1);
		anim->scKeys[i].time=GetDouble();
		Skip(1);
		i++;
	}while(i<anim->numKeySc);
	Skip(20,'}');
	Skip(20,'}');
	anim->SetStart(0);
	anim->SetEndTime(anim->length);
	mod->Animations.insert(pair<unsigned int,Animation*>(mod->AnimCount,anim));
	mod->AnimCount++;
	return anim;
}

void ModelLoader::InMeshProps(Mesh* m)
{
	do
	{
		buf[loc];
		if(!GoNextParam())
		{
			return;
		}
		char* cs = GetParam();
		if(strcmp(cs,"HMap")==0)
		{
			Skip(1);
			string* s = Get();
			if(strcmp(s->data(),"hmap")==0)
			m->hMap=Mesh::hmap;
			else
			if(strcmp(s->data(),"none")==0)
			m->hMap=Mesh::none;
			else
			if(strcmp(s->data(),"ray")==0)
			m->hMap=Mesh::ray;
			s=0;
			delete s;
		}
	}while(true);
}

void ModelLoader::GetObjects()
{
	do
	{
		buf[loc];
		if(!GoNextParam())
		{
			return;
		}
		string s = *Get();
		if(strcmp(s.data(),"Mesh")==0)
		{
			Mesh* m = InMesh();
			mod->objects.insert(pair<unsigned int,Base*>(mod->objCount,m));
			mod->objCount++;
			mod->Meshes.insert(pair<unsigned int,Mesh*>(mod->meshCount,m));
			mod->meshCount++;
		}
		else
		if(strcmp(s.data(),"Dummy")==0)
		{
			mod->objects.insert(pair<unsigned int,Base*>(mod->objCount,InDummy()));
			mod->objCount++;
		}
		else
		if(strcmp(s.data(),"Camera")==0)
		{
			Camera* cam = InCamera();
			mod->objects.insert(pair<unsigned int,Base*>(mod->objCount,cam));
			mod->objCount++;
			mod->Cameras.insert(pair<unsigned int,Camera*>(mod->camCount,cam));
			mod->camCount++;
		}
	}while(true);
}

void ModelLoader::GetMaterials()
{
	int i =0;
	bool end=false;
	do
	{
		buf[loc];
		if(!GoNextParam())
			return;
		mod->Materials.insert(pair<unsigned int,Material*>(mod->MatCount,InMaterial()));
		mod->MatCount++;
	}while(true);
}

void ModelLoader::Connections()
{
	Skip(10,'{');
	unsigned int i=1;
	Skip(5);
	buf[loc];
	do
	{
		unsigned int index=0;
		index=GetInt();
		Skip(1);
		mod->objects[index]->AddChild(mod->objects[i]);
		i++;
	}while(i<mod->objCount);
	return;
}

bool ModelLoader::GoNextParam()
{
	do
	{
		if(buf[loc]==':')
		{
			loc++;
			return true;
		}
		if(buf[loc]=='}')
		{
			loc++;
			return false;
		}
		loc++;
	}while(true);
}

char* ModelLoader::GetParam()
{
	char c;
	string s;
	do
	{
		c=buf[loc];
		if(c=='=' || c==NEWLINE)
		{
			char* cs = new char[s.length()+1];
			s.copy(cs,s.length());
			cs[s.length()]=0;
			return cs;
		}
		loc++;
		s+=c;
	}while(true);
}

void ModelLoader::Skip(unsigned int c)
{
	loc+=c;
};

void ModelLoader::SkipTo(unsigned int c,char ch)
{
	do
	{
	if(buf[loc]!=ch && c>0)
	{
	loc++;
	c--;
	}
	else
		return;
	}while(true);
};

void ModelLoader::Skip(unsigned int c,char ch)
{
	do
	{
	if(buf[loc]!=ch && c>0)
	{
	loc++;
	c--;
	}
	else
	{
		loc++;
		return;
	}
	}while(true);
};

float ModelLoader::GetFloat()
{
	return (float)GetDouble();
}

double ModelLoader::GetDouble()
{
	//we have to sense the end of the number by sseing
	//whether or not the char is between - and 9
	// or - or E
	unsigned int i=0;
	do
	{
		if((buf[loc] >= '-' && buf[loc] <= '9') || buf[loc]=='e')
		{
			atofcs[i]=buf[loc];
			i++;
		}
		else
		{
		atofcs[i]=0;
		return atof(atofcs);
		}
		loc++;
	}while(true);
};

int ModelLoader::GetInt()
{
	return (int)GetDouble();
};

char ModelLoader::Next()
{
	loc++;
	return buf[loc];
}

char ModelLoader::Peek()
{
	return buf[loc];
}

char* ModelLoader::Get(unsigned int c)
{
	unsigned int i=0;
	char* cs = new char[c+1];
	do
	{
		buf[loc];
		loc++;
		i++;
	}while(i>c);
	return cs;
}

string* ModelLoader::Get()
{
	string* st=new string();
	do
	{
		*st+=buf[loc];
		loc++;
	}while(buf[loc]!=NEWLINE);
	return st;
}