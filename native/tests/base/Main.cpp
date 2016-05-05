/*
Copyright (c) 2016, Riccardo Balbo
All rights reserved.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include "VHACDNative.h"
#include <iostream>
#include <vector>
#include <stdio.h>
#include <string.h>
#include <string>
#include <algorithm>
using namespace std;

/* 
 * public domain strtok_r() by Charlie Gordon
 *
 *   from comp.lang.c  9/14/2007
 *
 *      http://groups.google.com/group/comp.lang.c/msg/2ab1ecbb86646684
 *
 *     (Declaration that it's public domain):
 *      http://groups.google.com/group/comp.lang.c/msg/7c7b39328fefab9c
 */

char* strtok_r2(
    char *str, 
    const char *delim, 
    char **nextp)
{
    char *ret;

    if (str == NULL)
    {
        str = *nextp;
    }

    str += strspn(str, delim);

    if (*str == '\0')
    {
        return NULL;
    }

    ret = str;

    str += strcspn(str, delim);

    if (*str)
    {
        *str++ = '\0';
    }

    *nextp = str;

    return ret;
}

bool LoadOBJ(const string& fileName, vector<float>& points, vector<int>& triangles)
{
    const unsigned int BufferSize = 1024;
    FILE* fid = fopen(fileName.c_str(), "r");

    if (fid) {
        char buffer[BufferSize];
        int ip[4];
        float x[3];
        char* pch;
        char* str;
        while (!feof(fid)) {
            if (!fgets(buffer, BufferSize, fid)) {
                break;
            }
            else if (buffer[0] == 'v') {
                if (buffer[1] == ' ') {
                    str = buffer + 2;
                    for (int k = 0; k < 3; ++k) {
                        char *s;
                        pch = strtok_r2(str, " ",&s);
                        if (pch)
                            x[k] = (float)atof(pch);
                        else {
                            return false;
                        }

                        str = NULL;
                    }

                    points.push_back(x[0]);
                    points.push_back(x[1]);
                    points.push_back(x[2]);
                }
            }
            else if (buffer[0] == 'f') {

                pch = str = buffer + 2;
                int k = 0;
                while (pch) {
                    pch = strtok(str, " ");                  
                    if (pch) {
                        char *x;
                        char *spch=strtok_r2(pch, "/", &x);
                        if(spch){
                            int r=atoi(spch)-1;
                            ip[k++] = r ;

                        }
                    }
                    else {
                        break;
                    }
                    str = NULL;                   
                }

                if (k == 3) {
                    triangles.push_back(ip[0]);
                    triangles.push_back(ip[1]);
                    triangles.push_back(ip[2]);
                }
                else if (k == 4) {
                    triangles.push_back(ip[0]);
                    triangles.push_back(ip[1]);
                    triangles.push_back(ip[2]);

                    triangles.push_back(ip[0]);
                    triangles.push_back(ip[2]);
                    triangles.push_back(ip[3]);
                }
            }
        }
        fclose(fid);
    }
    else {
        std::cout << "File not found\n" << std::endl;
        return false;
    }
    return true;
}

int main(){
    VHACDNativeParameters params; 

    std::vector<float> positions;
    std::vector<int> indexes;    
    LoadOBJ("resources/wasp.obj",positions,indexes);
        

    VHACDNativeResults* results=compute(&positions[0],
        (unsigned int)positions.size(),
        &indexes[0],
        (unsigned int)indexes.size(),      
        &params,
        true
    );
    
    cout << "Result:" << endl;
    cout << "  Hulls: "<<results->n_hulls << endl;
    for(unsigned int i=0;i<results->n_hulls;i++){
        cout << "    ["<<i<<"] "<<"Vertices: "<<results->hulls[i].n_positions << endl;
    }
    
    release(results);
  
    return 0;
}
