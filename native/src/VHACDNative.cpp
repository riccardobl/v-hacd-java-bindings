/*
Copyright (c) 2016, Riccardo Balbo
All rights reserved.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#ifndef NO_DEBUG
    #include <iostream>
#endif
#include "VHACDNative.h"

using namespace VHACD;

class Logger : public IVHACD::IUserLogger {
    public:
        Logger(bool b){
            log=b;
        };
        void Log(const char* const msg){
            #ifndef NO_DEBUG
                if(log)std::cout << msg << std::endl;
            #endif      
        };

    private:
        bool log;
};

class Callback : public IVHACD::IUserCallback {
    public:
        void Update(const double overallProgress,
            const double stageProgress,
            const double operationProgress,
            const char* const stage,
            const char* const operation)
            {};
};

void initParams(VHACDNativeParameters* params){
    params->Init();
}

void release(VHACDNativeResults* results){
    delete results;
}

VHACDNativeResults* compute(
            float *points,
            unsigned int points_n,
            int * triangles,
            unsigned int triangles_n,
            VHACDNativeParameters* params,
            bool debug
){

    IVHACD::Parameters nparams;
    nparams.m_concavity=params->m_concavity;
    nparams.m_alpha=params->m_alpha;
    nparams.m_beta=params->m_beta;
    nparams.m_gamma=params->m_gamma;
    nparams.m_minVolumePerCH=params->m_minVolumePerCH;
    nparams.m_resolution=params->m_resolution;
    nparams.m_maxNumVerticesPerCH=params->m_maxNumVerticesPerCH;
    nparams.m_depth=params->m_depth;
    nparams.m_planeDownsampling=params->m_planeDownsampling;
    nparams.m_convexhullDownsampling=params->m_convexhullDownsampling;
    nparams.m_maxNumVerticesPerCH=params->m_maxNumVerticesPerCH;
    nparams.m_pca=params->m_pca;
    nparams.m_mode=params->m_mode;
    nparams.m_convexhullApproximation=params->m_convexhullApproximation;
    nparams.m_oclAcceleration=params->m_oclAcceleration;
        

    IVHACD *vh = CreateVHACD();
    Logger logger(debug);
    Callback callback;

    
    #ifndef NO_DEBUG  
    if(debug){
        std::cout << "------"<< std::endl;
        std::cout << "Vertices: ";
        for(int i=0;i<points_n;i++){
            std::cout << points[i] << " ";
        }
        std::cout << std::endl;
        std::cout << "Indexes: ";
        for(int i=0;i<triangles_n;i++){
            std::cout << triangles[i] << " ";
        }
        std::cout << std::endl;
        std::cout << "------"<< std::endl;
    } 
    #endif
     
    nparams.m_logger=&logger;
    nparams.m_callback=&callback;
    bool res= vh->Compute(&points[0], 3, points_n / 3, &triangles[0], 3, triangles_n / 3, nparams);
   
    VHACDNativeResults *out=new VHACDNativeResults();
    if (res) {
        out->n_hulls=vh->GetNConvexHulls();
        out->hulls=new VHACDNativeConvexHull[out->n_hulls];
        
        IVHACD::ConvexHull v_hull;
        for (unsigned int p=0;p<out->n_hulls;p++) {
            vh->GetConvexHull(p, v_hull);
            
            out->hulls[p]=VHACDNativeConvexHull();
            
            out->hulls[p].n_positions=v_hull.m_nPoints;
            out->hulls[p].positions=new float[v_hull.m_nPoints*3];
            for(unsigned int i=0;i<v_hull.m_nPoints*3;i++){
                float x=(float)v_hull.m_points[i];
                out->hulls[p].positions[i]=x;  
            }              
            
            out->hulls[p].n_indexes=v_hull.m_nTriangles;
            out->hulls[p].indexes=new int[v_hull.m_nTriangles*3];
            for(unsigned int  i=0;i<v_hull.m_nTriangles*3;i++){
                int x=v_hull.m_triangles[i];
                out->hulls[p].indexes[i]=x;  
            }              
            
        }   
    }else{
        out->n_hulls=0;
    }
    vh->Clean();
    vh->Release();
    return out;  
}   