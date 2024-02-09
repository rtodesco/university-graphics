#Ryan Todesco
#V00914926
#Part 1 Working
#Part 2 Close, but not quite working. I think the weight function is causing the problem, but I don't know
#Part 3 Didn't attempt
#Part 4 Started, but unsure were to go next
#Part 5 Working, with the not quite working parts of Part 2

import numpy as np
import igl
import meshplot as mp

polyDegree = 2

def tet_grid(n, mmin, mmax):
    nx = n[0]
    ny = n[1]
    nz = n[2]
    
    delta = mmax-mmin
    
    deltax = delta[0]/(nx-1)
    deltay = delta[1]/(ny-1)
    deltaz = delta[2]/(nz-1)
    
    T = np.zeros(((nx-1)*(ny-1)*(nz-1)*6, 4), dtype=np.int64)
    V = np.zeros((nx*ny*nz, 3))

    mapping = -np.ones((nx, ny, nz), dtype=np.int64)


    index = 0
    for i in range(nx):
        for j in range(ny):
            for k in range(nz):
                mapping[i, j, k] = index
                V[index, :] = [i*deltax, j*deltay, k*deltaz]
                index += 1
    assert(index == V.shape[0])
    
    tets = np.array([
        [0,1,3,4],
        [5,2,6,7],
        [4,1,5,3],
        [4,3,7,5],
        [3,1,5,2],
        [2,3,7,5]
    ])
    
    index = 0
    for i in range(nx-1):
        for j in range(ny-1):
            for k in range(nz-1):
                indices = [
                    (i,   j,   k),
                    (i+1, j,   k),
                    (i+1, j+1, k),
                    (i,   j+1, k),

                    (i,   j,   k+1),
                    (i+1, j,   k+1),
                    (i+1, j+1, k+1),
                    (i,   j+1, k+1),
                ]
                
                for t in range(tets.shape[0]):
                    tmp = [mapping[indices[ii]] for ii in tets[t, :]]
                    T[index, :]=tmp
                    index += 1
                    
    assert(index == T.shape[0])
    
    V += mmin
    return V, T
	
def find_closed_point(point,points):
	min = points.copy()
	min = np.absolute(min - point)
	n = np.zeros(min.shape[0])
	for i in range(n.shape[0]):
		n[i] = min[i][0] + min[i][1] + min[i][2]
	m = np.argmin(n)
	return m
	
def closest_points(point,points,h):
	p = points.copy()
	p = np.absolute(p - point)
	n = np.array([])
	for i in range(points.shape[0]):
		if(p[i][0] + p[i][1] + p[i][2] < h):
			n = np.append(n,i)
	return n
	
def b_vector(x,y,z):
	if(polyDegree == 0):
		return np.array([1])
	elif(polyDegree == 1):
		return np.array([1,x,y,z])
	else:
		return np.array([1,x,y,z,x*y,x*z,y*z,x**2,y**2,z**2])

def main():
	mp.offline()
	
	pi, v = igl.read_triangle_mesh("data/cat.off")
	pi /= 10
	ni = igl.per_vertex_normals(pi, v)
	#mp.plot(pi, shading={"point_size": 8})
	
	di = np.zeros(pi.shape[0])
	pi_new = pi.copy()
	
	for x in range(pi.shape[0]):
		p = pi[x]
		
		eps = 0.01 * igl.bounding_box_diagonal(pi)
		m = -1
		while(m != x):
			pi_plus = p + eps*ni[x]
			m = find_closed_point(pi_plus,pi)
			eps = eps/2
		
		di = np.append(di,eps*2)
		pi_new = np.append(pi_new,[pi_plus],axis=0)
		
		eps = -0.01 * igl.bounding_box_diagonal(pi)
		m = -1
		while(m != x):
			pi_minus = p + eps*ni[x]
			m = find_closed_point(pi_minus,pi)
			eps = eps/2
		
		di = np.append(di,eps*2)
		pi_new = np.append(pi_new,[pi_minus],axis=0)
		
		
	ind = np.zeros_like(di)
	ind[di > 0] = 1
	ind[di == 0] = 0
	ind[di < 0] = -1
	(mp.plot(pi_new, c=ind, shading={"point_size": 8})).save("point_cloud_extended.html")
		
	bbox_min = np.array([pi_new.min(), pi_new.min(), pi_new.min()])
	bbox_max = np.array([pi_new.max(), pi_new.max(), pi_new.max()])
	bbox_diag = np.linalg.norm(bbox_max - bbox_min)

	n = 20

	G, T = tet_grid((n, n, n), bbox_min - 0.05 * bbox_diag, bbox_max + 0.05 * bbox_diag)
	
	fx = np.zeros(G.shape[0])
	
	h = 20
			
	for i in range(G.shape[0]):
		cgi = closest_points(G[i],pi_new,h)
		if(cgi.shape[0] == 0):
			fx[i] = 100
		elif((polyDegree == 0 and cgi.shape[0] < 1) or (polyDegree == 1 and cgi.shape[0] < 4) or (polyDegree == 2 and cgi.shape[0] < 10)):
			fx[i] = 100
		else:
			d = np.array([])
			w = np.empty((0,cgi.shape[0]))
			if(polyDegree == 0):
				B = np.empty((0,1))
			elif(polyDegree == 1):
				B = np.empty((0,4))
			else:
				B = np.empty((0,10))
			for j in range(cgi.shape[0]):
				l = int(cgi[j])
				b = b_vector(pi_new[l][0],pi_new[l][1],pi_new[l][2])
				B = np.append(B,[b],axis=0)
				d = np.append(d,di[l])
				r = np.sum(np.absolute(pi_new[l] - G[i]))
				wi = np.zeros(cgi.shape[0])
				if(j<b.shape[0]):
					wi[j] = ((1-(r/h))**4) * (4*(r/h)+1)
				w = np.append(w,[wi],axis=0)
			Bt = np.transpose(B)
			right = np.matmul(np.matmul(Bt,w),B)
			left = np.matmul(np.matmul(Bt,w),d)
			a = np.linalg.solve(right,left)
			fx[i] = np.dot(np.transpose(a),b_vector(G[i][0],G[i][1],G[i][2]))
				
	ind = np.zeros_like(fx)
	ind[fx >= 0] = 1
	ind[fx < 0] = -1
	(mp.plot(G, c=ind, shading={"point_size": 10,"width": 800, "height": 800})).save("grid_point.html")

	sv, sf, _, _ = igl.marching_tets(G, T, fx, 0)
	(mp.plot(sv, sf, shading={"wireframe": True})).save("surface.html")
	
	
	pi, v = igl.read_triangle_mesh("data/luigi.off")
	pi /= 10
	ni = igl.per_vertex_normals(pi, v)
	#mp.plot(pi, shading={"point_size": 1})
	
	mx = 0
	my = 0
	mz = 0
	for i in range(pi.shape[0]):
		mx = mx + pi[i][0]
		my = my + pi[i][1]
		mz = mz + pi[i][2]
	m = np.array([(1/pi.shape[0])*mx,(1/pi.shape[0])*my,(1/pi.shape[0])*mz])
	
	y = np.empty((0,3))
	for i in range(pi.shape[0]):
		y = np.append(y,[pi[i] - m],axis=0)
	s = np.matmul(y,np.transpose(y))
	
	w,v = np.linalg.eig(s)
	

if __name__ == "__main__":
	main()