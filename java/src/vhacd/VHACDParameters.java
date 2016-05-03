package vhacd;
import vhacd.vhacd_native.VhacdLibrary;
import vhacd.vhacd_native.VHACDNativeParameters;
/*
Copyright (c) 2016, Riccardo Balbo
All rights reserved.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

public class VHACDParameters extends VHACDNativeParameters{
	private boolean DEBUG;

    public VHACDParameters(){
        super();
        VhacdLibrary.INSTANCE.initParams(this);
    }
	

	public void setDebugEnabled(boolean d) {
		DEBUG=d;
	}

	public boolean getDebugEnabled() {
		return DEBUG;
	}

	/**
	 * 
	 * @description Set maximum number of voxels generated during the voxelization stage
	 * @param v  default = 100000, min = 10000, max = 64000000
	 */
	public void setVoxelResolution(int v) {
		m_resolution=v;
	}

	public int getVoxelResolution() {
		return m_resolution;
	}

	/**
	 * 
	 * @description Set maximum number of clipping stages. During each split stage, all the model parts (with a concavity higher than the user defined threshold) are clipped according the "best" clipping plane
	 * @param  default = 20, min = 1, max = 32
	 */
	public void setClippingDepth(int v) {
		m_depth=v;
	}

	public int getClippingDepth() {
		return m_depth;
	}

	/**
	 * 
	 * @description Set maximum concavity
	 * @param default = 0.001, min = 0.0, max = 1.0
	 */
	public void setMaxConcavity(double v) {
		m_concavity=v;
	}

	public double getMaxConcavity() {
		return m_concavity;
	}

	/**
	 * 
	 * @description Set granularity of the search for the "best" clipping plane
	 * @param v default = 4, min = 1, max = 16
	 */
	public void setPlaneDownSampling(int v) {
		m_planeDownsampling=v;
	}

	public int getPlaneDownSampling() {
		return m_planeDownsampling;
	}

	/**
	 * 
	 * @description Set precision of the convex-hull generation process during the clipping plane selection stage
	 * @param v  default = 4, min = 1, max = 16
	 */
	public void setConvexHullDownSampling(int v) {
		m_convexhullDownsampling=v;
	}

	public int getConvexHullDownSampling() {
		return m_convexhullDownsampling;
	}

	/**
	 * 
	 * @description Set bias toward clipping along symmetry planes
	 * @param v default = 0.05, min = 0.0, max = 1.0,
	 */
	public void setAlpha(double v) {
		m_alpha=v;
	}

	public double getAlpha() {
		return m_alpha;
	}

	/**
	 * 	
	 * @description Set bias toward clipping along revolution axes
	 * @param v default = 0.05, min = 0.0, max = 1.0
	 */
	public void setBeta(double v) {
		m_beta=v;
	}

	public double getBeta() {
		return m_beta;
	}

	/**
	 * 
	 * @description Set maximum allowed concavity during the merge stage
	 * @param v  default = 0.0005, min = 0.0, max = 1.0
	 */
	public void setGamma(double v) {
		m_gamma=v;
	}

	public double getGamma() {
		return m_gamma;
	}

	/**
	 * 
	 * @description Enable/disable normalizing the mesh before applying the convex decomposition
	 * @param v  default = False
	 */
	public void setPCA(boolean v) {
		m_pca=v?1:0;
	}

	public boolean getPCA() {
		return m_pca==1;
	}

	/**
	 * 
	 * @description Set approximate convex decomposition mode
	 * @param v  default = VOXEL
	 */
	public void setACDMode(ACDMode mode) {
		m_mode=mode.ordinal();
	}

	public ACDMode getACDMode() {
		return ACDMode.values()[m_mode];
	}

	/**
	 * 
	 * @description Set minimum volume to add vertices to convex-hulls
	 * @param v  default = 0.0001, min = 0.0, max = 0.01
	 */
	public void setMinVolumePerHull(double v) {
		m_minVolumePerCH=v;
	}

	public double getMinVolumePerHull() {
		return m_minVolumePerCH;
	}

	/**
	 * 
	 * @description Set maximum number of vertices per convex-hull
	 * @param v  default = 64, min = 4, max = 1024)
	 */
	public void setMaxVerticesPerHull(int v) {
		m_maxNumVerticesPerCH=v;
	}

	public int getMaxVerticesPerHull() {
		return m_maxNumVerticesPerCH;
	}

}
