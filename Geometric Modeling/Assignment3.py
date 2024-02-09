#Ryan Todesco
#V00914926

import igl
import numpy as np
import meshplot as mp
from scipy.sparse.linalg import spsolve
import scipy.sparse as sp

def main():
	mp.offline()
	
	#Vertex Normal
	v, f = igl.read_triangle_mesh("data/bunny.off")

	#Standard face normal
	n_s = igl.per_vertex_normals(v,f,igl.PER_VERTEX_NORMALS_WEIGHTING_TYPE_UNIFORM)
	mp.plot(v, f, n=n_s, shading={"flat": False}).save("Standard Normal.html")
	
	#Area-weighted face normal
	n_a = igl.per_vertex_normals(v,f,igl.PER_VERTEX_NORMALS_WEIGHTING_TYPE_AREA)
	mp.plot(v, f, n=n_a, shading={"flat": False}).save("Area-weighted Normal.html")
	
	#Mean-curvature normal
	l = igl.cotmatrix(v,f)
	H = np.abs(l)/2
	n_m = -l/(2*H)
	mp.plot(v, f, n=n_m, shading={"flat": False}).save("Mean-curvature Normal.html")
	
	#PCA normal
	n_p = np.zeros((0,3))
	k = 10
	for vi in v:
		Y = np.zeros((3,0))
		x = v[np.argpartition(np.sum(np.abs(vi-v),axis=1),k)[:k]]
		m = (np.sum(x,axis=0))/k
		for xi in x:
			yi = xi - m
			Y = np.insert(Y,0,yi,axis=1)
		S = np.matmul(Y,np.transpose(Y))
		w,e = np.linalg.eig(S)
		vn = e[:,np.argmin(w)]
		n_p = np.append(n_p,[vn],axis=0)
	mp.plot(v,f,n=n_p,shading={"flat": False}).save("PCA Normal.html")
	
	#Quadratic fitting normal


	#Curvature
	v, f = igl.read_triangle_mesh("data/bunny.off")
	
	#discrete mean
	l = igl.cotmatrix(v,f)
	m = igl.massmatrix(v,f,igl.MASSMATRIX_TYPE_VORONOI)
	minv = sp.diags(1/m.diagonal())
	
	hn = -minv.dot(l.dot(v))
	h = np.linalg.norm(hn,axis=1)
	mp.plot(v,f,h).save("discrete mean curvature.html")
	
	#gaussian curvature
	k_g = igl.gaussian_curvature(v,f)
	mp.plot(v,f,k_g).save("gaussian curvature.html")

	#principal curvature
	v1, v2, k_max, k_min = igl.principal_curvature(v, f)
	p_max = mp.plot(v, f, k_max, return_plot=True)
	p_min = mp.plot(v,f,k_min,return_plot=True)
	
	avg = igl.avg_edge_length(v, f) / 2.0
	p_max.add_lines(v + v1 * avg, v - v1 * avg, shading={"line_color": "red"})
	p_max.add_lines(v + v2 * avg, v - v2 * avg, shading={"line_color": "green"})
	p_min.add_lines(v + v1 * avg, v - v1 * avg, shading={"line_color": "red"})
	p_min.add_lines(v + v2 * avg, v - v2 * avg, shading={"line_color": "green"})
	
	p_max.save("principal max curvature.html")
	p_min.save("principal min curvature.html")
	
	
	#Smoothing with the Laplacian
	
	#Explicit laplacian
	v, f = igl.read_triangle_mesh("data/cow.off")
	
	for x in range(1000):
		l = igl.cotmatrix(v,f)
		m = igl.massmatrix(v,f, igl.MASSMATRIX_TYPE_BARYCENTRIC)
		mInv = sp.diags(1/m.diagonal())
		i = np.identity(l.shape[0])
		v = (i + 0.000001*mInv*l)*v
		
	mp.plot(v,f,shading={"wireframe": True}).save("Explicit Smoothing.html")
	

	#Implicit laplacian
	v, f = igl.read_triangle_mesh("data/cow.off")
	
	l = igl.cotmatrix(v, f)
	m = igl.massmatrix(v, f, igl.MASSMATRIX_TYPE_BARYCENTRIC)
	s = (m - 0.002 * l)
	v = spsolve(s, m.dot(v))
	
	mp.plot(v,f,shading={"wireframe": True}).save("Implicit Smoothing.html")
	

if __name__ == "__main__":
	main()