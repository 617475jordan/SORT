/*
    This file is a part of SORT(Simple Open Ray Tracing), an open-source cross
    platform physically based renderer.
 
    Copyright (c) 2011-2018 by Cao Jiayin - All rights reserved.
 
    SORT is a free software written for educational purpose. Anyone can distribute
    or modify it under the the terms of the GNU General Public License Version 3 as
    published by the Free Software Foundation. However, there is NO warranty that
    all components are functional in a perfect manner. Without even the implied
    warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
    General Public License for more details.
 
    You should have received a copy of the GNU General Public License along with
    this program. If not, see <http://www.gnu.org/licenses/gpl-3.0.html>.
 */

#include "thirdparty/gtest/gtest.h"
#include "bsdf/bsdf.h"
#include "sampler/sample.h"
#include "spectrum/spectrum.h"
#include "bsdf/lambert.h"
#include "bsdf/orennayar.h"
#include "bsdf/phong.h"
#include "bsdf/ashikhmanshirley.h"
#include "bsdf/disney.h"
#include "bsdf/microfacet.h"
#include "bsdf/dielectric.h"
#include <thread>

static const int N = 8192;

// A physically based BRDF should obey the rule of reciprocity
void checkReciprocity(const Bxdf* bxdf) {
    for (int i = 0; i < N; ++i) {
        Vector wi(sort_canonical() * 2.0f - 1.0f, sort_canonical() * 2.0f - 1.0f, sort_canonical() * 2.0f - 1.0f);
        wi.Normalize();
        Vector wo(sort_canonical() * 2.0f - 1.0f, sort_canonical() * 2.0f - 1.0f, sort_canonical() * 2.0f - 1.0f);
        wo.Normalize();

        const auto f0 = bxdf->F(wo, wi) * AbsCosTheta(wo);
        const auto f1 = bxdf->F(wi, wo) * AbsCosTheta(wi);
        EXPECT_NEAR(f0.GetR(), f1.GetR(), 0.001f);
        EXPECT_NEAR(f0.GetG(), f1.GetG(), 0.001f);
        EXPECT_NEAR(f0.GetB(), f1.GetB(), 0.001f);
    }
}

// A physically based BRDF/BTDF should not reflect more energy than it receives
void checkEnergyConservation(const Bxdf* bxdf) {
    static const Vector wo(DIR_UP);

    Spectrum rho;
    for (int i = 0; i < N; ++i) {
        Vector wi;
        float pdf = 0.0f;
        Spectrum r = bxdf->Sample_F(wo, wi, BsdfSample(true), &pdf);
        rho += pdf > 0.0f ? r / pdf : 0.0f;
    }
    rho *= 1.0f / (float)N;
    EXPECT_LE(rho.GetR(), 1.0f);
    EXPECT_LE(rho.GetG(), 1.0f);
    EXPECT_LE(rho.GetB(), 1.0f);
}

// Check whether the pdf evaluated from sample_f matches the one from Pdf
// The exact algorithm is mentioned in my blog, except that the following algorithm also evaluates BTDF
// https://agraphicsguy.wordpress.com/2018/03/09/how-does-pbrt-verify-bxdf/
void checkPdf( const Bxdf* bxdf ){
    Vector wo(sort_canonical() * 2.0f - 1.0f, sort_canonical() * 2.0f - 1.0f, sort_canonical() * 2.0f - 1.0f);
    wo.Normalize();

    if( CosTheta( wo ) < 0.0f )
        wo = -wo;

    for( int i = 0 ; i < 1024 ; ++i ){
        float pdf = 0.0f;
        Vector wi;
        auto f0 = bxdf->Sample_F( wo , wi , BsdfSample(true) , &pdf );

        float calculated_pdf = bxdf->Pdf( wo , wi );
        EXPECT_NEAR(pdf, calculated_pdf, 0.001f);

        EXPECT_TRUE( !isnan(pdf) );
        EXPECT_GE( pdf , 0.0f );
        
        auto f1 = bxdf->F( wo , wi );
        EXPECT_NEAR(f0.GetR(), f1.GetR(), 0.001f);
        EXPECT_NEAR(f0.GetG(), f1.GetG(), 0.001f);
        EXPECT_NEAR(f0.GetB(), f1.GetB(), 0.001f);
    }

    const int TN = 8;   // thread number
    float total[TN] = { 0.0f };
    std::thread threads[TN];
    for( int i = 0 ; i < TN ; ++i ){
        threads[i] = std::thread([&]( int tid ) {
            const long long N = 1024 * 1024 * 8;
            double local = 0.0f;
            for( long long i = 0 ; i < N ; ++i ){
                Vector wi;
                float pdf;
                bxdf->Sample_F( wo , wi , BsdfSample(true) , &pdf );
                local += pdf != 0.0f ? 1.0f / pdf : 0.0f;
            }
            total[tid] += local / ( N * TN );
        } , i );
    }
    for( int i = 0 ; i < TN ; ++i )
        threads[i].join();

    double final_total = 0.0f;
    for( int i = 0 ; i < TN ; ++i )
        final_total += total[i];
    // 0.5% error is tolorated
    EXPECT_NEAR( final_total , TWO_PI , 0.03f);
}

void checkAll( const Bxdf* bxdf ){
    checkPdf( bxdf );
    checkEnergyConservation( bxdf );
    checkReciprocity( bxdf );
}

TEST (BXDF, Labmert) { 
    static const Spectrum R(1.0f);
    Lambert lambert( R , R , DIR_UP );
    checkAll( &lambert );
}

TEST(BXDF, LabmertTransmittion) {
    static const Spectrum R(1.0f);
    LambertTransmission lambert(R, R, DIR_UP);
    checkAll(&lambert);
}

TEST(BXDF, OrenNayar) {
    static const Spectrum R(1.0f);
    OrenNayar orenNayar(R, sort_canonical(), R, DIR_UP);
    checkAll(&orenNayar);
}

TEST(BXDF, Phong) {
    static const Spectrum R(1.0f);
    const float ratio = sort_canonical();
    Phong phong( R * ratio , R * ( 1.0f - ratio ) , sort_canonical(), R, DIR_UP);
    checkAll(&phong);
}

TEST(BXDF, AshikhmanShirley) {
    static const Spectrum R(1.0f);
    AshikhmanShirley as( R , sort_canonical() , sort_canonical() , sort_canonical() , R , DIR_UP );
    checkAll(&as);
}

TEST(BXDF, Disney) {
    static const Spectrum R(1.0f);
    DisneyBRDF disney( R , sort_canonical() , sort_canonical() , sort_canonical() , sort_canonical() , sort_canonical() , sort_canonical() , sort_canonical() , sort_canonical() , sort_canonical() , sort_canonical() , R , DIR_UP );
    checkAll(&disney);
}

TEST(BXDF, MicroFacetReflection) {
    static const Spectrum R(1.0f);
    const FresnelConductor fresnel( 1.0f , 1.5f );
    const Beckmann ggx( sort_canonical() , sort_canonical() );
    MicroFacetReflection mf( R , &fresnel , &ggx , R , DIR_UP );
    checkAll(&mf);
}

TEST(BXDF, MicroFacetRefraction) {
    static const Spectrum R(1.0f);
    const FresnelConductor fresnel( 1.0f , 1.5f );
    const GGX ggx( sort_canonical() , sort_canonical() );
    MicroFacetRefraction mr( R , &ggx , sort_canonical() , sort_canonical() , R , DIR_UP );
    checkEnergyConservation( &mr );
    //checkPdf( &mr );
}

TEST(BXDF, Dielectric) {
    static const Spectrum R(1.0f);
    const FresnelConductor fresnel( 1.0f , 1.5f );
    const GGX ggx( sort_canonical() , sort_canonical() );
    Dielectric dielectric( R , R , &ggx , sort_canonical() , sort_canonical() , R , DIR_UP );
    checkEnergyConservation( &dielectric );
    //checkPdf( &dielectric );
}