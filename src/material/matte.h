/*
 * filename :	matte.h
 *
 * programmer :	Cao Jiayin
 */

#ifndef	SORT_MATTE
#define	SORT_MATTE

// include header file
#include "material.h"
#include "texture/texture.h"
#include "utility/strhelper.h"

//////////////////////////////////////////////////////////////////////
// definition of matte material
class Matte : public Material
{
// public method
public:
	// default constructor
	Matte();
	// destructor
	~Matte();

	// get bsdf
	virtual Bsdf* GetBsdf( const Intersection* intersect ) const;

	// create instance of the brdf
	CREATE_INSTANCE( Matte );

// private field
private:
	// the diffuse color for the material
	Texture* m_d;

	// a scaled color , default is ( 1 , 1 , 1 )
	Spectrum m_scale;

	// register property
	void _registerAllProperty();

	// initialize default value and register property
	void _init();

// property handler
	class ColorProperty : public PropertyHandler<Material>
	{
	public:
		// constructor
		ColorProperty(Material* matte):PropertyHandler(matte){}

		// set value
		void SetValue( Texture* tex )
		{
			Matte* matte = dynamic_cast<Matte*>(m_target);
			SAFE_DELETE( matte->m_d );
			matte->m_d = tex;
		}
	};
	
	class ScaleProperty : public PropertyHandler<Material>
	{
	public:
		// constructor
		ScaleProperty( Material* matte ) : PropertyHandler(matte){}

		// set value
		void SetValue( const string& str )
		{
			Matte* matte = dynamic_cast<Matte*>(m_target);
			matte->m_scale = SpectrumFromStr( str );
		}
	};
};

#endif
