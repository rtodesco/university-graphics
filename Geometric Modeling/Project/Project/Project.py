#Ryan Todesco
#V00914926

import numpy as np
import igl
import meshplot as mp

def main():
	mp.offline()
	v, f = igl.read_triangle_mesh("camel_head.off")
	TT,TTi = igl.triangle_triangle_adjacency(f)
	B = igl.barycenter(v,f)
	
	#Compute Extremalities
	v_max, v_min, k_max, k_min = igl.principal_curvature(v,f)
	e = np.zeros_like(k_max)
	G = igl.grad(v,f)
	G_v = G*v_max
	for i in range(e.shape[0]):
		e[i] = np.inner(G_v[i],v_max[i])
		
	#Compute Triangle Types
	s = [1,-1]
	r = np.zeros(f.shape[0])
	for x in range(f.shape[0]):
		for i in range(0,2):
			for j in range(0,2):
				for k in range(0,2):
					p1 = s[i] * v_max[f[x][0]]
					p2 = s[j] * v_max[f[x][1]]
					p3 = s[k] * v_max[f[x][2]]
					
					if(p1.dot(p2) > 0 and p2.dot(p3) > 0 and p3.dot(p1) > 0):
						r[x] = 1
						
	edges = np.zeros((0,2,3))
	faces_marked = np.zeros(f.shape[0])
	
	#Regular Triangles
	for x in range(f.shape[0]):
		if(r[x] == 0): continue
		
		v_max_T = np.zeros([3,3])
		e_T = np.zeros([3,])
		v_T = np.zeros([3,3])
		
		for i in range(0,3):
			v_T[i] = v[f[x][i]]
		
			if(v_max[f[x][i]].dot(v_max[f[x][0]]) < 0):
				v_max_T[i] = -v_max[f[x][i]]
				e_T[i] = -e[f[x][i]]
			else:
				v_max_T[i] = v_max[f[x][i]]
				e_T[i] = e[f[x][i]]
			
		#Check if conditions are satisfied
		G_T = igl.grad(v_T,np.array([[0,1,2]]))
		G_e_T = G_T*e_T
		
		v_max_sum = np.zeros([3,])
		k_max_sum = 0
		k_min_sum = 0
		
		for i in range(0,3):
			v_max_sum += v_max_T[i]
			k_max_sum += k_max[f[x][i]]
			k_min_sum += k_min[f[x][i]]
			
		if(not(G_e_T.dot(v_max_sum) < 0 and abs(k_max_sum) > abs(k_min_sum))): continue
		
		#Determine if zero set is not empty
		zero_set = np.zeros([0,3])
		for i in range(f.shape[1]):
			e1 = abs(e[f[x][i]])
			if(v_max[f[x][i]].dot(v_max[f[x][0]]) < 0): e1 *= -1
			e2 = abs(e[f[x][(i+1)%3]])
			if(v_max[f[x][(i+1)%3]].dot(v_max[f[x][0]]) < 0): e2 *= -1
			e3 = abs(e[f[x][(i+2)%3]])
			if(v_max[f[x][(i+2)%3]].dot(v_max[f[x][0]]) < 0): e3 *= -1
			
			if(e1 == 0 and e2 == 0 and e3 == 0): continue
			
			if((e1 > 0 and e2 > 0) or (e1 < 0 and e2 < 0)): continue
			
			p1 = v[f[x][i]]
			p2 = v[f[x][(i+1)%3]]
			
			if(e1 == 0):
				zero_set = np.append(zero_set,[p1],axis=0)
				faces_marked[x] = 1
			else:
				if(e2 != 0):
					p0 = (e1 * p2 - e2 * p1) / (e1 - e2)
					zero_set = np.append(zero_set,[p0],axis=0)
					faces_marked[x] = 1
		
		if(zero_set.shape[0] == 2):
			edge = np.zeros([2,3])
			edge[0] = zero_set[0]
			edge[1] = zero_set[1]
			edges = np.append(edges,[edge],axis=0)
			
	#Singular Triangles
	for x in range(f.shape[0]):
		if(r[x] == 1): continue
		
		#Determine is zero set is not empty
		zero_set = np.zeros([0,3])
		for i in range(f.shape[1]):
			g = TT[x][i]
			if(g == -1): continue
			if(r[g] == 0): continue
			if(faces_marked[g] == 0): continue
			
			e1 = abs(e[f[x][i]])
			e2 = abs(e[f[x][(i+1)%3]])
			e2 = abs(e[f[x][(i+2)%3]])
			
			if((e1 > 0 and e2 > 0) or (e1 < 0 and e2 < 0)): continue
			
			p1 = v[f[x][i]]
			p2 = v[f[x][(i+1)%3]]
			
			if(e2 == 0):
				zero_set = np.append(zero_set,[p2],axis=0)
			else:
				if(e1 != 0):
					p0 = (e1 * p2 - e2 * p1)/(e1 - e2)
					zero_set = np.append(zero_set,[p0],axis=0)
		
		if(zero_set.shape[0] == 2):
			edge = np.zeros([2,3])
			edge[0] = zero_set[0]
			edge[1] = zero_set[1]
			edges = np.append(edges,[edge],axis=0)	
		elif(zero_set.shape[0] == 3):
			for j in range(0,3):
				edge = np.zeros([2,3])
				edge[0] = B[x]
				edge[1] = zero_set[j]
				edges = np.append(edges,[edge],axis=0)
			
	p = mp.plot(v,f)
	#Draw edges
	for i in range(edges.shape[0]):
		p.add_lines(edges[i][0],edges[i][1],shading={"line_color":"red"})
	p.save("Feature Lines.html")
	
if __name__ == "__main__":
	main()