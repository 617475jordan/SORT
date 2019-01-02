/*
    This file is a part of SORT(Simple Open Ray Tracing), an open-source cross
    platform physically based renderer.
 
    Copyright (c) 2011-2019 by Cao Jiayin - All rights reserved.
 
    SORT is a free software written for educational purpose. Anyone can distribute
    or modify it under the the terms of the GNU General Public License Version 3 as
    published by the Free Software Foundation. However, there is NO warranty that
    all components are functional in a perfect manner. Without even the implied
    warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
    General Public License for more details.
 
    You should have received a copy of the GNU General Public License along with
    this program. If not, see <http://www.gnu.org/licenses/gpl-3.0.html>.
 */

#pragma once

#include "texture.h"

///////////////////////////////////////////////////////////////
// definition of grid texture
class	GridTexture : public Texture
{
public:
	DEFINE_CREATOR( GridTexture , Texture , "grid" );

	// default constructor
	GridTexture();
	
	// get the texture value
	// para 'x'	:	x coordinate , if out of range , use filter
	// para 'y' :	y coordinate , if out of range , use filter
	virtual Spectrum GetColor( int x , int y ) const;

	// set color of the spectrum
	void	SetGridColor( const Spectrum& c0 , const Spectrum& c1 );

private:
	// two colors
	Spectrum	m_Color0;
	Spectrum	m_Color1;

	// the threshold for the center grid , default value is 0.9f
    float		m_Threshold;
};
