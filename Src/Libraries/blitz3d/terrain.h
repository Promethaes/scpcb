
#ifndef TERRAIN_H
#define TERRAIN_H

#include "../stdutil/stdutil.h"

#include "model.h"

struct TerrainRep;

class Terrain : public Model{
public:
	Terrain( int size_shift );
	~Terrain();

    virtual Object* clone(){ return new Terrain(*this); }
	Terrain *getTerrain(){ return this; }

	void setDetail( int n,bool morph );
	void setHeight( int x,int z,float h,bool realtime );
	void setShading( bool shading );

	int getSize()const;
	float getHeight( int x,int z )const;

	//model interface
	bool render( const RenderContext &rc );

	//object interface
	bool collide( const Line &line,float radius,Collision *curr_coll,const Transform &tf );
	
private:
	TerrainRep *rep;
};

#endif