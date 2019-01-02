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

#include "memmanager.h"
#include "core/define.h"
#include "core/log.h"

// default constructor
MemManager::MemManager()
{
	// 16mb memory for default
	PreMalloc( 1024 * 1024 * 1024 );
}

// pre-allocate memory
void MemManager::PreMalloc( unsigned size , unsigned id )
{
	// if size is equal to zero , just return
	if( size == 0 )
		return;

	Memory* mem = _getMemory( id );

	if( mem != 0 )
	{
		if( size != mem->m_size )
			DeAlloc( id );
		else
		{
			ClearMem( id );
			return;
		}
	}

	// create new memory
	m_MemPool[id] = std::make_unique<Memory>();
    m_MemPool[id]->m_memory = std::make_unique<char[]>(size);
	// reset offset
	m_MemPool[id]->m_offset = 0;
	// set size
	m_MemPool[id]->m_size = size;
}

// clear the allocated memory
void MemManager::ClearMem( unsigned id )
{
	Memory* mem = _getMemory( id );
	if( mem == 0 )
        slog( WARNING , GENERAL , "Can't clear memory, because there is no memory with id %d." , id );

	//reset the offset
	mem->m_offset = 0;
}

// de-allocate memory
void MemManager::DeAlloc( unsigned id )
{
	Memory* mem = _getMemory( id );
	if( mem == 0 )
        slog( WARNING , GENERAL , "Can't delete memory, because there is no memory with id %d." , id );

	// reset offset and size
	delete mem;

	m_MemPool.erase( id );
}
