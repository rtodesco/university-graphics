import numpy
from numpy.linalg import inv
from numpy.linalg import norm
import sys
import math

spheres = []
lights = []

bg_r = 0
bg_g = 0
bg_b = 0

amb_r = 0
amb_g = 0
amb_b = 0


def shadowRay(light,p,hit_sphere,s):
	l = numpy.array([light[1]-p[0],light[2]-p[1],light[3]-p[2]])
	l = l/norm(l)
	
	hit = False
	for sphere in spheres:
		p_inv = numpy.matmul(sphere[15],numpy.array([p[0],p[1],p[2],1]))
		l_inv = numpy.matmul(sphere[15],numpy.array([l[0],l[1],l[2],0]))
		p_inv = numpy.array([p_inv[0],p_inv[1],p_inv[2]])
		l_inv = numpy.array([l_inv[0],l_inv[1],l_inv[2]])
		
		quad_a = pow(norm(l_inv),2)
		quad_b = numpy.dot(p_inv,l_inv)
		quad_c = pow(norm(p_inv),2) - 1 

		temp = pow(quad_b,2) - quad_a*quad_c
		if temp < 0:
			continue
		elif temp == 0:
			t = -quad_b/quad_a
			if t > 0.0001:
				hit = True
		else:
			t1 = -quad_b/quad_a + math.sqrt(temp)/quad_a
			t2 = -quad_b/quad_a - math.sqrt(temp)/quad_a
			t = min(t1,t2)
			if t > 0.0001:
				hit = True
				
	if not hit:
		n = numpy.array([2*(p[0]-hit_sphere[1]),2*(p[1]-hit_sphere[2]),2*(p[2]-hit_sphere[3])])
		n = n/norm(n)
		n = numpy.matmul(hit_sphere[16],n)
		n = n/norm(n)
		
		nl = max(numpy.dot(n,l),0)
		
		v = numpy.array([s[0]-p[0],s[1]-p[1],s[2]-p[2]])
		v = v/norm(v)
		re = 2*nl*n-l
		re = re/norm(re)
		
		rv = max(numpy.dot(re,v),0)
		
		r = hit_sphere[11]*light[4]*nl*hit_sphere[7] + hit_sphere[12]*light[4]*(pow(rv,hit_sphere[14]))
		g = hit_sphere[11]*light[5]*nl*hit_sphere[8] + hit_sphere[12]*light[5]*(pow(rv,hit_sphere[14]))
		b = hit_sphere[11]*light[6]*nl*hit_sphere[9] + hit_sphere[12]*light[6]*(pow(rv,hit_sphere[14]))
		
		return [r,g,b]
	else:		
		return [0,0,0]
	
def raytrace(s,c,d,r):
	if(d > 3):
		return [0,0,0]
		
	hit = False
	t = 1000
	p = [0,0,0]
	
	for sphere in spheres:
		
		s_inv = numpy.matmul(sphere[15],numpy.array([s[0],s[1],s[2],1]))
		c_inv = numpy.matmul(sphere[15],numpy.array([c[0],c[1],c[2],0]))
		s_inv = numpy.array([s_inv[0],s_inv[1],s_inv[2]])
		c_inv = numpy.array([c_inv[0],c_inv[1],c_inv[2]])
		
		quad_a = pow(norm(c_inv),2)
		quad_b = numpy.dot(s_inv,c_inv)
		quad_c = pow(norm(s_inv),2) - 1 

		temp = pow(quad_b,2) - quad_a*quad_c
		if temp < 0:
			continue
		elif temp == 0:
			t_temp = -quad_b/quad_a
			if t_temp > 0.0001:
				if r:
					hit = True
					if t_temp < t:
						t = t_temp
						p = s+t*c
						hit_sphere = sphere
				else:
					p_temp = s+t_temp*c
					if(p_temp[2] < -1):
						hit = True
						if t_temp < t:
							t = t_temp
							p = p_temp
							hit_sphere = sphere
		else:
			t1 = -quad_b/quad_a + math.sqrt(temp)/quad_a
			t2 = -quad_b/quad_a - math.sqrt(temp)/quad_a
			if r:
				t_temp = min(t1,t2)
				if t_temp > 0.0001:
					hit = True
					if t_temp < t:
						t = t_temp
						p = s+t*c
						hit_sphere = sphere
			else:
				p_temp1 = s+t1*c
				p_temp2 = s+t2*c
				if(t1 < t2):
					if(p_temp1[2] < -1):
						hit = True
						if t1 < t:
							t = t1
							p = p_temp1
							hit_sphere = sphere
					elif(p_temp2[2] < -1):
						hit = True
						if t2 < t:
							t = t2
							p = p_temp2
							hit_sphere = sphere
				else:
					if(p_temp2[2] < -1):
						hit = True
						if t2 < t:
							t = t2
							p = p_temp2
							hit_sphere = sphere
					elif(p_temp1[2] < -1):
						hit = True
						if t1 < t:
							t = t1
							p = p_temp1
							hit_sphere = sphere
			
	if hit:
		color_local = [hit_sphere[7]*hit_sphere[10]*amb_r,hit_sphere[8]*hit_sphere[10]*amb_g,hit_sphere[9]*hit_sphere[10]*amb_b]
		
		for light in lights:
			color_local = numpy.add(color_local,shadowRay(light,p,hit_sphere,s))
			
		n = numpy.array([2*(p[0]-hit_sphere[1]),2*(p[1]-hit_sphere[2]),2*(p[2]-hit_sphere[3])])
		n = n/norm(n)
		n = numpy.matmul(hit_sphere[16],n)
		n = n/norm(n)
		
		v = -2*(numpy.dot(n,c))*n + c
		v = v/norm(v)
		color_reflect = raytrace(p,v,d+1,True)
		color_reflect = [x * hit_sphere[13] for x in color_reflect]
		
		return (numpy.add(color_local,color_reflect))
		
	else:		
		if r:
			return [0,0,0]
		else:
			return [bg_r,bg_g,bg_b]

def main():
	input = open(str(sys.argv[1]),"r")
	lines = []
	image = []
	
	for line in input:
		ls = line.split()
		if not ls:
			continue
		if(ls[0] == "NEAR"):
			near = float(ls[1])
		elif(ls[0] == "LEFT"):
			left = float(ls[1])
		elif(ls[0] == "RIGHT"):
			right = float(ls[1])
		elif(ls[0] == "BOTTOM"):
			bottom = float(ls[1])
		elif(ls[0] == "TOP"):
			top = float(ls[1])
		elif(ls[0] == "RES"):
			res_x = int(ls[1])
			res_y = int(ls[2])
		elif(ls[0] == "BACK"):
			global bg_r	
			bg_r = float(ls[1])
			global bg_g
			bg_g = float(ls[2])
			global bg_b
			bg_b = float(ls[3])
		elif(ls[0] == "AMBIENT"):
			global amb_r	
			amb_r = float(ls[1])
			global amb_g
			amb_g = float(ls[2])
			global amb_b
			amb_b = float(ls[3])
		elif(ls[0] == "OUTPUT"):
			output = ls[1]
		elif(ls[0] == "SPHERE"):
			spheres.append(ls)
		elif(ls[0] == "LIGHT"):
			lights.append(ls)
		
	# Convert sphere values to correct type
	for s in spheres:
		s.remove("SPHERE")
		for j in range(1,14):
			s[j] = float(s[j])
		s[14] = int(s[14])
		s.append(inv(numpy.array([[s[4],0,0,s[1]],[0,s[5],0,s[2]],[0,0,s[6],s[3]],[0,0,0,1]])))
		s.append(numpy.transpose(inv(numpy.array([[s[4],0,0],[0,s[5],0],[0,0,s[6]]]))))
		
	# Convert light values to correct type
	for l in lights:
		l.remove("LIGHT")
		for j in range(1,7):
			l[j] = float(l[j])
	
	w = (right-left)/2
	h = (bottom-top)/2
		
	for y in range(0,res_y):
		for x in range(0,res_x):
			uc = -w + w*((2*x)/res_x)
			vr = -h + h*((2*y)/res_y)
			s = numpy.array([0,0,0])
			c = numpy.array([uc,vr,-near])
			c = c/norm(c)
			
			color = raytrace(s,c,1,False)
			
			if(color[0] > 1):
				color[0] = 1
			if(color[1] > 1):
				color[1] = 1
			if(color[2] > 1):
				color[2] = 1
			image.append(color[0]*255)
			image.append(color[1]*255)
			image.append(color[2]*255)
	
	with open(output, "wb") as f:
		f.write(bytearray("P3\n","ascii"))
		f.write(bytearray(str(res_x) + " " + str(res_y) + "\n","ascii"))
		f.write(bytearray("255\n","ascii"))
		for p in image:
			f.write(bytearray(str(p) + " ","ascii"))

if __name__ == "__main__":
	main()
	exit(0)