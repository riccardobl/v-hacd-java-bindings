import java.io.BufferedReader;
import java.io.File;
import java.io.FileInputStream;
import java.io.InputStreamReader;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collection;
import java.util.LinkedList;

import vhacd.VHACD;
import vhacd.VHACDParameters;
import vhacd.VHACDResults;
/*
Copyright (c) 2016, Riccardo Balbo
All rights reserved.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

public class Main{

	public static void loadOBJ(String path, Collection<Float> positions, Collection<Integer> indexes) throws Exception {
		File f=new File(path);
		BufferedReader reader=new BufferedReader(new InputStreamReader(new FileInputStream(f)));
		String line;
		while((line=reader.readLine())!=null){
			line=line.trim();
			String vals[]=line.split(" ");
			if(vals[0].equals("v")){
				vals=Arrays.copyOfRange(vals,1,vals.length);
				for(String v:vals){
					positions.add(Float.parseFloat(v));
				}
			}else if(vals[0].equals("f")){
				vals=Arrays.copyOfRange(vals,1,vals.length);
				ArrayList<Integer> in=new ArrayList<Integer>();
				for(String v:vals){
					String x=v.split("/")[0];
					int r=Integer.parseInt(x)-1;
					in.add(r);

				}
				if(in.size()==3){
					indexes.add(in.get(0));
					indexes.add(in.get(1));
					indexes.add(in.get(2));
				}else if(in.size()==4){
					indexes.add(in.get(0));
					indexes.add(in.get(1));
					indexes.add(in.get(2));

					indexes.add(in.get(0));
					indexes.add(in.get(2));
					indexes.add(in.get(3));
				}else{
					reader.close();
					throw new Exception("Error in parsing .obj");
				}
			}
		}
		reader.close();
	}

	public static void main(String[] args) throws Exception {
		Collection<Float> positions=new LinkedList<Float>();
		Collection<Integer> indexes=new LinkedList<Integer>();
		loadOBJ("resources/wasp.obj",positions,indexes);

		float fpositions[]=new float[positions.size()];
		int iindexes[]=new int[indexes.size()];
		int i=0;
		for(Float p:positions){
            //System.out.println(p);
			fpositions[i++]=(float)p;
		}
		i=0;
		for(Integer p:indexes){
			iindexes[i++]=(int)p;
		}
		VHACDParameters p=new VHACDParameters();
		p.setDebugEnabled(true);

		VHACDResults res=VHACD.compute(fpositions,iindexes,p);

		System.out.println("Results:");
		System.out.println("  Hulls: "+res.size());
		for(i=0;i<res.size();i++){
			System.out.println("    ["+i+"] Vertices: "+(res.get(i).positions.length/3));
		}
	}
}
