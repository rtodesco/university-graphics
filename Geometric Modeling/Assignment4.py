#Ryan Todesco
#V00914926

import math
import numpy as np
import numpy.matlib
import scipy.sparse as sp

import igl
import meshplot as mp

from math import sqrt

def align_field(V, F, TT, hard_id, hard_value, llambda):
	assert(hard_id[0] > 0)
	assert(hard_id.shape[0] == hard_value.shape[0])


	# Edges
	e1 = V[F[:, 1], :] - V[F[:, 0], :]
	e2 = V[F[:, 2], :] - V[F[:, 0], :]

	# Compute the local reference systems for each face, T1, T2
	T1 = e1 / np.linalg.norm(e1, axis=1)[:,None]
		
	T2 =  np.cross(T1, np.cross(T1, e2))
	T2 /= np.linalg.norm(T2, axis=1)[:,None]

	# Arrays for the entries of the matrix
	data = []
	ii = []
	jj = []

	index = 0
	for f in range(F.shape[0]):
		for ei in range(3): # Loop over the edges
			
			# Look up the opposite face
			g = TT[f, ei]
			
			# If it is a boundary edge, it does not contribute to the energy
			# or avoid to count every edge twice
			if g == -1 or f > g:
				continue
				
			# Compute the complex representation of the common edge
			e  = V[F[f, (ei+1)%3], :] - V[F[f, ei], :]
			
			vef = np.array([np.dot(e, T1[f, :]), np.dot(e, T2[f, :])])
			vef /= np.linalg.norm(vef)
			ef = (vef[0] + vef[1]*1j).conjugate()
			
			veg = np.array([np.dot(e, T1[g, :]), np.dot(e, T2[g, :])])
			veg /= np.linalg.norm(veg)
			eg = (veg[0] + veg[1]*1j).conjugate()
			
			
			# Add the term conj(f)^n*ui - conj(g)^n*uj to the energy matrix
			data.append(ef);  ii.append(index); jj.append(f)
			data.append(-eg); ii.append(index); jj.append(g)

			index += 1
			

	sqrtl = sqrt(llambda)

	#Create and modify matrices with hard contraints
	A = sp.coo_matrix((data, (ii, jj)), shape=(index, F.shape[0])).asformat("csr")
	b = np.zeros(index, dtype=complex)
	
	for ci in range(hard_id.shape[0]):
		f = hard_id[ci]
		v = hard_value[ci,:]
		c = np.dot(v, T1[f, :]) + np.dot(v, T2[f, :])*1j
		x = np.zeros(A.shape[1],dtype=complex)
		x[f] = 1
		A[f] = x		
		b[f] = c		

	# Solve the linear system
	u = sp.linalg.spsolve(A.H @ A, A.H @ b)

	R = T1 * u.real[:,None] + T2 * u.imag[:,None]

	return R
	
def plot_mesh_field(V, F, R, constrain_faces):
    # Highlight in red the constrained faces
    col = np.ones_like(F)
    col[constrain_faces, 1:] = 0
    
    # Scaling of the representative vectors
    avg = igl.avg_edge_length(V, F)/2

    #Plot from face barycenters
    B = igl.barycenter(V, F)

    p = mp.plot(V, F, c=col)
    p.add_lines(B, B + R * avg)
    
    return p

def main():
	mp.offline()
	
	v, f = igl.read_triangle_mesh("data/irr4-cyl2.off")
	tt, _ = igl.triangle_triangle_adjacency(f)

	c = np.loadtxt("data/irr4-cyl2.constraints")
	cf = c[:, 0].astype(np.int64)
	c = c[:, 1:]
	
	R = align_field(v, f, tt, cf, c, 1e6)
	plot_mesh_field(v, f, R, cf).save("Vector Field.html")
	
	
	#Reconstructing a scalar field from a vector field
	v, f = igl.read_triangle_mesh("data/irr4-cyl2.off")
	G = igl.grad(v,f)
	G_t = G.transpose()
	a = igl.doublearea(v,f)/2
	A = np.matlib.repmat(a,1,3).transpose().flatten()
	u = R.flatten()
	
	K = G_t @ G
	b = G_t @ u
	s = sp.linalg.spsolve(K,b)
	
	mp.plot(v,f,s).save("Scalar Gradient.html")

	
	#Harmonic Parameterizations
	v,f = igl.read_triangle_mesh("data/camel_head.off")
	
	bnd = igl.boundary_loop(f)
	bnd_uv = igl.map_vertices_to_circle(v,bnd)
	
	uv = igl.harmonic_weights(v,f,bnd,bnd_uv,1)
	v_p = np.hstack([uv,np.zeros((uv.shape[0],1))])
	
	p = mp.subplot(v,f,uv=uv,s=[1,2,0])
	mp.subplot(v_p,f,shading={"wireframe":True},data=p,s=[1,2,1])
	p.save("Harmonic Parameterizations.html")
	
	#LSCM Parameterizations
	v,f = igl.read_triangle_mesh("data/camel_head.off")
	
	b = np.array([2,1])
	
	bnd = igl.boundary_loop(f)
	b[0] = bnd[0]
	b[1] = bnd[int(bnd.size/2)]
	
	bc = np.array([[0.0,0.0],[1.0,0.0]])
	
	_,uv = igl.lscm(v,f,b,bc)

	p = mp.subplot(v,f,uv=uv,s=[1,2,0])
	mp.subplot(uv,f,shading={"wireframe":True},data=p,s=[1,2,1])
	p.save("LSCM Parameterizations.html")
	
	#Editing a parameterization with vector fields
	v,f = igl.read_triangle_mesh("data/irr4-cyl2.off")
	bnd = igl.boundary_loop(f)
	bnd_uv = igl.map_vertices_to_circle(v,bnd)
	uv = igl.harmonic_weights(v,f,bnd,bnd_uv,1)
	v_p = np.hstack([uv,np.zeros((uv.shape[0],1))])
	u = uv[:,0]
	e_s = np.column_stack((u,s))
	
	mp.plot(v,f,uv=e_s).save("Editing Parameterization")
	

if __name__ == "__main__":
	main()