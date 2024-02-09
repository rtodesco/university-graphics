#Ryan Todesco
#V00914926

#All images are saved as html files
#If written output, it is saved as text files
#Ignore default named html files
#Per-vertex shader not working
#Subdivion not finished, only up to connecting new vertices to face corners

import numpy as np
import igl
import meshplot

def main():
	bunny_v, bunny_f = igl.read_triangle_mesh("data/bunny.off")
	cube_v, cube_f = igl.read_triangle_mesh("data/cube.obj")
	sphere_v, sphere_f = igl.read_triangle_mesh("data/sphere.obj")
	
	meshplot.offline()
	
	#Value-To-Face Relations
	o = open("Neighborhood Computations.txt","w")
	v,f = igl.vertex_triangle_adjacency(bunny_f,bunny_v.size)
	o.write("Value-To-Face:\n")
	o.write("[ " + str(v[0]) + " " + str(v[1]) + " " + str(v[2]) + " ... " + str(v[-3]) + " " + str(v[-2]) + " " + str(v[-1]) + " ]\n")
	o.write("\n")
	
	#Verte-To-Vertex Relations
	o.write("Verte-To-Vertex Relations\n")
	v = igl.adjacency_list(bunny_f)
	for x in v:
		for i in x:
			o.write(str(i) + " ")
		o.write("\n")
	o.write("\n")
	
	# #Visualizing the Neighborhood Relations
	o.write("Bunny_f\n")
	for x in bunny_f:
		for i in x:
			o.write(str(i) + " ")
		o.write("\n")
	
	o.write("Bunny_v\n")
	for x in bunny_v:
		for i in x:
			o.write(str(i) + " ")
		o.write("\n")
	o.close()
	
	
	#Shading Cube
	f = cube_f.copy()
	v = cube_v.copy()

	f_new = np.zeros(f.shape,dtype=np.int64)
	v_new = np.zeros((f.shape[0]*3,3))
	old_v_to_new = np.zeros(f.shape,dtype=np.int64)

	v_index = 0
	for i in range(f.shape[0]):
		new_face = [v_index,v_index+1,v_index+2]
		f_new[i] = new_face
		v_new[v_index] = v[f[i,0]]
		v_new[v_index+1] = v[f[i,1]]
		v_new[v_index+2] = v[f[i,2]]
		
		old_v_to_new[i,0] = v_index
		old_v_to_new[i,1] = v_index+1
		old_v_to_new[i,2] = v_index+2
		
		v_index += 3
		
	#Flat Shading
	n = igl.per_face_normals(v,f,np.array([0,1.0,0]))
	n_flat = np.zeros(v_new.shape)

	for i in range(f.shape[0]):
		f_normal = n[i]
		n_flat[f_new[i,0]] = f_normal
		n_flat[f_new[i,1]] = f_normal
		n_flat[f_new[i,2]] = f_normal
    
	(meshplot.plot(v_new,f_new,n=n_flat,shading={"wireframe":True})).save("cube_flat_shading.html")
	
	#Per-Vertex Shading
	n = igl.per_vertex_normals(v,f,0)
	n_vertex = np.zeros(v_new.shape)

	for i in range(v.shape[0]):
		v_normal = n[i]
		n_vertex[old_v_to_new[i,0]] = v_normal
		n_vertex[old_v_to_new[i,1]] = v_normal
		n_vertex[old_v_to_new[i,2]] = v_normal
		
	(meshplot.plot(v_new,f_new,n=n,shading={"wireframe":True})).save("cube_vertex_shading.html")
	
	#Per-Corner Shading
	n = igl.per_corner_normals(v,f,90)
	(meshplot.plot(v_new,f_new,n=n,shading={"wireframe":True,"flat":False})).save("cube_corner_shading.html")
	
	
	#Shading Sphere
	f = sphere_f.copy()
	v = sphere_v.copy()

	f_new = np.zeros(f.shape,dtype=np.int64)
	v_new = np.zeros((f.shape[0]*3,3))
	old_v_to_new = np.zeros(f.shape,dtype=np.int64)

	v_index = 0
	for i in range(f.shape[0]):
		new_face = [v_index,v_index+1,v_index+2]
		f_new[i] = new_face
		v_new[v_index] = v[f[i,0]]
		v_new[v_index+1] = v[f[i,1]]
		v_new[v_index+2] = v[f[i,2]]
		
		old_v_to_new[i,0] = v_index
		old_v_to_new[i,1] = v_index+1
		old_v_to_new[i,2] = v_index+2
		
		v_index += 3
		
	#Flat Shading
	n = igl.per_face_normals(v,f,np.array([0,1.0,0]))
	n_flat = np.zeros(v_new.shape)

	for i in range(f.shape[0]):
		f_normal = n[i]
		n_flat[f_new[i,0]] = f_normal
		n_flat[f_new[i,1]] = f_normal
		n_flat[f_new[i,2]] = f_normal
    
	(meshplot.plot(v_new,f_new,n=n_flat,shading={"wireframe":True})).save("sphere_flat_shading.html")
	
	#Per-Vertex Shading
	n = igl.per_vertex_normals(v,f,0)
	n_vertex = np.zeros(v_new.shape)

	for i in range(v.shape[0]):
		v_normal = n[i]
		n_vertex[old_v_to_new[i,0]] = v_normal
		n_vertex[old_v_to_new[i,1]] = v_normal
		n_vertex[old_v_to_new[i,2]] = v_normal
		
	(meshplot.plot(v_new,f_new,n=n,shading={"wireframe":True})).save("sphere_vertex_shading.html")
	
	#Per-Corner Shading
	n = igl.per_corner_normals(v,f,90)
	(meshplot.plot(v_new,f_new,n=n,shading={"wireframe":True,"flat":False})).save("sphere_corner_shading.html")
	
	
	#Connected Components
	o = open("Connected Components.txt","w")
	car_v, car_f = igl.read_triangle_mesh("data/car.off")
	car_c = igl.face_components(car_f)
	(meshplot.plot(car_v,car_f,c=car_c)).save("car_components.html")
	o.write("Car: \nNumber of components: " + str(np.unique(car_c).size))
	for i in range(np.unique(car_c).size):
		o.write("\nComponent " + str(i) + ": " + str(np.count_nonzero(car_c == i)))
	
	cup_v, cup_f = igl.read_triangle_mesh("data/coffeecup.off")
	cup_c = igl.face_components(cup_f)
	(meshplot.plot(cup_v,cup_f,c=cup_c)).save("coffeecup_components.html")
	o.write("\n\nCoffee Cup: \nNumber of components: " + str(np.unique(cup_c).size))
	for i in range(np.unique(cup_c).size):
		o.write("\nComponent " + str(i) + ": " + str(np.count_nonzero(cup_c == i)))
	o.close()
	
	
	#Subdivision Scheme
	v,f = igl.read_triangle_mesh("data/sphere.obj")
	
	center = igl.barycenter(v,f)
	v2 = np.concatenate((v,center))
	
	f2 = f.copy()
	counter = v.shape[0]
	
	for i in range(f.shape[0]):
		f2 = np.append(f2,[[f[i][0],f[i][1],counter]],axis=0)
		f2 = np.append(f2,[[f[i][0],counter,f[i][2]]],axis=0)
		f2 = np.append(f2,[[counter,f[i][1],f[i][2]]],axis=0)
		counter+=1
		
	(meshplot.plot(v2,f2,shading={"wireframe":True}).save("sphere_sub.html"))
	

if __name__ == "__main__":
	main()