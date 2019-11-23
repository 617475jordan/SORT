/*
    This file is a part of SORT(Simple Open Ray Tracing), an open-source cross
    platform physically based renderer.

    Copyright (c) 2011-2019 by Jiayin Cao - All rights reserved.

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

#include "core/define.h"
#include "math/ray.h"
#include "math/bbox.h"

#ifdef SSE_ENABLED

//! @brief	SIMD version bounding box.
/**
 * This is basically 4 bounding box in a single data structure. For best performance, they are saved in
 * structure of arrays.
 * Since this data structure is only used in limited places, only very few interfaces are implemented for
 * simplicity.
 */
struct BBox4{
public:
	simd_data_sse	m_min_x;
	simd_data_sse	m_min_y;
	simd_data_sse	m_min_z;

	simd_data_sse	m_max_x;
	simd_data_sse	m_max_y;
	simd_data_sse	m_max_z;
};

#ifdef SIMD_SSE_IMPLEMENTATION
	#define Simd_BBox		BBox4
#endif

#endif // SSE_ENABLED

#ifdef AVX_ENABLED

//! @brief	SIMD version bounding box.
/**
 * This is basically 8 bounding box in a single data structure. For best performance, they are saved in
 * structure of arrays.
 * Since this data structure is only used in limited places, only very few interfaces are implemented for
 * simplicity.
 */
struct BBox8{
public:
	simd_data_avx	m_min_x;
	simd_data_avx	m_min_y;
	simd_data_avx	m_min_z;

	simd_data_avx	m_max_x;
	simd_data_avx	m_max_y;
	simd_data_avx	m_max_z;
};

#ifdef SIMD_AVX_IMPLEMENTATION
	#define Simd_BBox		BBox8
#endif

#endif // AVX_ENABLED

#if defined( SSE_ENABLED ) || defined( AVX_ENABLED )

SORT_FORCEINLINE int IntersectBBox4(const Ray& ray, const BBox4& bb, simd_data& f_min ) {
	f_min = simd_set_ps1( ray.m_fMin );
	simd_data f_max = simd_set_ps1( ray.m_fMax );

	simd_data t1	= simd_add_ps( ray.m_ori_dir_x , simd_mul_ps( ray.m_rcp_dir_x , bb.m_max_x ) );
	simd_data t2	= simd_add_ps( ray.m_ori_dir_x , simd_mul_ps( ray.m_rcp_dir_x , bb.m_min_x ) );
	f_min	    = simd_max_ps( f_min , simd_min_ps( t1 , t2 ) );
	f_max		= simd_min_ps( f_max , simd_max_ps( t1 , t2 ) );

    t1		    = simd_add_ps( ray.m_ori_dir_y , simd_mul_ps( ray.m_rcp_dir_y , bb.m_max_y ) );
	t2		    = simd_add_ps( ray.m_ori_dir_y , simd_mul_ps( ray.m_rcp_dir_y , bb.m_min_y ) );
	f_min	    = simd_max_ps( f_min , simd_min_ps( t1 , t2 ) );
	f_max	    = simd_min_ps( f_max , simd_max_ps( t1 , t2 ) );

    t1		    = simd_add_ps( ray.m_ori_dir_z , simd_mul_ps( ray.m_rcp_dir_z , bb.m_max_z ) );
	t2		    = simd_add_ps( ray.m_ori_dir_z , simd_mul_ps( ray.m_rcp_dir_z , bb.m_min_z ) );
	f_min	    = simd_max_ps( f_min , simd_min_ps( t1 , t2 ) );
	f_max	    = simd_min_ps( f_max , simd_max_ps( t1 , t2 ) );

	const simd_data mask = simd_cmple_ps( f_min , f_max );
	f_min = simd_pick_ps( mask , f_min , simd_neg_ones );

	return simd_movemask_ps( mask );
}

#endif // SSE_ENABLED || AVX_ENABLED