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

// This file wraps most of OSL related interface

#pragma once

#include <OSL/oslexec.h>
#include <string>
#include "core/define.h"
#include "math/vector3.h"

#if defined(SORT_IN_WINDOWS)
    #define STDOSL_PATH     "..\\src\\stdosl.h"
#else
    #define STDOSL_PATH     "../src/stdosl.h"
#endif

constexpr int LAMBERT_ID                            = 1;
constexpr int OREN_NAYAR_ID                         = 2;
constexpr int DISNEY_ID                             = 3;
constexpr int MICROFACET_REFLECTION_ID              = 4;
constexpr int MICROFACET_REFRACTION_ID              = 5;
constexpr int ASHIKHMANSHIRLEY_ID                   = 6;
constexpr int PHONG_ID                              = 7;
constexpr int LAMBERT_TRANSMITTANCE_ID              = 8;
constexpr int MIRROR_ID                             = 9;
constexpr int DIELETRIC_ID                          = 10;
constexpr int MICROFACET_REFLECTION_DIELETRIC_ID    = 11;
constexpr int HAIR_ID                               = 12;
constexpr int FOURIER_BDRF_ID                       = 13;
constexpr int MERL_BRDF_ID                          = 14;
constexpr int COAT_ID                               = 15;
constexpr int DOUBLESIDED_ID                        = 16;

struct ShadingContextWrapper {
public:
    OSL::ShadingContext* GetShadingContext(OSL::ShadingSystem* shadingsys);
    void DestroyContext(OSL::ShadingSystem* shadingsys);

private:
    OSL::PerThreadInfo              *thread_info = nullptr;
    OSL::ShadingSystem              *shadingsys = nullptr;
    OSL::ShadingContext             *ctx = nullptr;
};

// begin building shader
OSL::ShaderGroupRef beginShaderGroup( const std::string& group_name );
bool endShaderGroup();

// Optimize shader
void optimizeShader(OSL::ShaderGroup* group);

// Compile OSL source code
bool compile_buffer ( const std::string &sourcecode, const std::string &shadername );

// Build a shader from source code
bool build_shader( const std::string& shader_source, const std::string& shader_name, const std::string& shader_layer , const std::string& shader_group_name = "" );

// Connect parameters between shaders
bool connect_shader( const std::string& source_shader , const std::string& source_param , const std::string& target_shader , const std::string& target_param );

// Execute a shader and populate the bsdf
void execute_shader( class Bsdf* bsdf , const class Intersection* intersection , OSL::ShaderGroup* shader );

// Register closures
void register_closures(OSL::ShadingSystem* shadingsys);

// Create thread contexts
void create_thread_contexts();

// Destroy thread contexts
void detroy_thread_contexts();