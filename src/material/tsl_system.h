/*
    This file is a part of SORT(Simple Open Ray Tracing), an open-source cross
    platform physically based renderer.

    Copyright (c) 2011-2020 by Jiayin Cao - All rights reserved.

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

#include <tslversion.h>
#include <shading_system.h>
#include <shading_context.h>
#include <string>
#include "core/define.h"
#include "math/vector3.h"
#include "spectrum/spectrum.h"
#include "closures.h"
#include "scatteringevent/scatteringevent.h"

class ScatteringEvent;
struct SurfaceInteraction;
class MediumStack;
struct MediumInteraction;
class Mesh;

DECLARE_TSLGLOBAL_BEGIN()
DECLARE_TSLGLOBAL_VAR(float3, normal)
DECLARE_TSLGLOBAL_END()

//! @brief  Build a shader from source code
Tsl_Namespace::ShaderUnitTemplate* BuildShader(const std::string& shader_name, const std::string& shader_source);

//! @brief  Begin building the shader group
ShaderGroupTemplate* BeginShaderGroup(const std::string& group_name);

//! @brief  End bulding the shader group
bool EndShaderGroup(ShaderGroupTemplate* sg);

//! @brief  Resolve shader instance.
bool ResolveShaderInstance(ShaderInstance* si);

//! @brief  Execute Jited shader code.
void ExecuteSurfaceShader(Tsl_Namespace::ShaderInstance* shader, ScatteringEvent& se);

/*
struct ShadingContextWrapper {
public:
    OSL::ShadingContext* GetShadingContext(OSL::ShadingSystem* shadingsys);
    void DestroyContext(OSL::ShadingSystem* shadingsys);

private:
    OSL::PerThreadInfo              *thread_info = nullptr;
    OSL::ShadingContext             *ctx = nullptr;
};

//! @brief  Optimize shader
void OptimizeShader(OSL::ShaderGroup* group);

//! @brief  Build a shader from source code
bool BuildShader( const std::string& shader_source, const std::string& shader_name, const std::string& shader_layer , const std::string& shader_group_name = "" );

//! @brief  Execute a shader and populate the scattering event
//!
//! @param  shader      The osl shader to be evaluated.
//! @param  se          The resulting scattering event.
void ExecuteSurfaceShader( OSL::ShaderGroup* shader , ScatteringEvent& se );

//! @brief  Execute a shader and populate the medium stack
//!
//! @param  shader      The osl shader to be evaluated.
//! @param  mi          The interaction inside the medium.
//! @param  ms          The medium stack to hold the results.
//! @param  flag        A flag indicates whether to add or remove the medium.
//! @param  material    The material that spawns the medium.
void ExecuteVolumeShader( OSL::ShaderGroup* shader, const MediumInteraction& mi , MediumStack& se, const SE_Interaction flag, const MaterialBase* material);

//! @brief  Evaluate the properties at a specific position in the volume.
//!
//! @param  shader      The osl shader to be executed.
//! @param  mi          The interaction inside the volume.
//! @param  ms          The medium sample to be returned.
void EvaluateVolumeSample( OSL::ShaderGroup* shader, const MediumInteraction& mi, MediumSample& ms);

//! @brief  Evaluate the transparency of the intersection.
//!
//! @param  shader          The osl shader to be evaluated.
//! @param  intersection    The intersection of interest.
Spectrum EvaluateTransparency( OSL::ShaderGroup* shader , const SurfaceInteraction& intersection );
*/
//! @brief  Create thread contexts
void CreateTSLThreadContexts();

//! @brief  Destroy thread contexts
void DestroyTSLThreadContexts();
