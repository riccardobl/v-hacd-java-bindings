/*
Copyright (c) 2016, Riccardo Balbo
All rights reserved.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#ifndef _VHACD_NATIVE_
#define _VHACD_NATIVE_

#include "VHACD.h"

using namespace VHACD;

extern "C" {
    
    struct VHACDNativeConvexHull {
        float *positions;
        int *indexes;
        unsigned int n_positions;
        unsigned int n_indexes;
        ~VHACDNativeConvexHull(){
          delete[] positions;
          delete[] indexes;
        };
    };

    struct VHACDNativeResults {
        unsigned int n_hulls;
        VHACDNativeConvexHull *hulls;
        ~VHACDNativeResults(){
            delete[] hulls;
        };
    };

    struct VHACDNativeParameters {
            double m_concavity;
            double m_alpha;
            double m_beta;
            double m_gamma;
            double m_minVolumePerCH;
            unsigned int m_resolution;
            unsigned int m_maxNumVerticesPerCH;
            int m_depth;
            int m_planeDownsampling;
            int m_convexhullDownsampling;
            int m_pca;
            int m_mode;
            int m_convexhullApproximation;
            int m_oclAcceleration;
            VHACDNativeParameters(){
                Init();   
            };
            void Init(){
                m_resolution = 100000;
                m_depth = 20;
                m_concavity = 0.0025;
                m_planeDownsampling = 4;
                m_convexhullDownsampling = 4;
                m_alpha = 0.05;
                m_beta = 0.05;
                m_gamma = 0.00125;
                m_pca = 0;
                m_mode = 0; // 0: voxel-based (recommended), 1: tetrahedron-based
                m_maxNumVerticesPerCH = 32;
                m_minVolumePerCH = 0.0001;
                m_convexhullApproximation = true;
                m_oclAcceleration = false;
            };
    };
    
    VHACDNativeResults* compute(
                float *points,
                unsigned int points_n,
                int * triangles,
                unsigned int triangles_n,
                VHACDNativeParameters* params,
                bool debug);                
    void release(VHACDNativeResults* results);
    void initParams(VHACDNativeParameters* params);
}
#endif