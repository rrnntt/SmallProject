import sys
import os
import os.path
import shutil

Jmax = 10
dKmax = 3

def ener(v1,v2,v3,J,Ka,Kc):
	nu1 = 1000.0
	nu2 = 600.0
	nu3 = 1300.0
	A = 10.0 + 0.1 * v1 + 0.2 * v2 + 0.3 * v3
	B = 5.0 - 0.01 * v1 - 0.02 * v2 - 0.03 * v3
	C = 3.0 - 0.02 * v1 - 0.05 * v2 - 0.04 * v3
	E = nu1 * v1 + nu2 * v2 + nu3 * v3 + A * J + B * Ka + C * Kc
	return E

def term(f,v1,v2,v3,J,Ka,Kc):
	s  = '"[11] ('+str(v1)+str(v2)+str(v3)+') '
	s += str(J)+' '+str(Ka)+' '+str(Kc)+'" '
	s += str(ener(v1,v2,v3,J,Ka,Kc))+'\n'
	f.write(s)

def jstate(f, v1,v2,v3,J):
	for k in range(0,J+1):
		#print J,k,J-k
		term(f,v1,v2,v3,J,k,J-k)
		if k < J:
			term(f,v1,v2,v3,J,k+1,J-k)
			#print J,k+1,J-k

def vstate(f,v1,v2,v3):
	for J in range(0,Jmax+1):
		jstate(f,v1,v2,v3,J)

def add_jstate(v1,v2,v3,J,lst):
	lst
	for k in range(0,J+1):
		lst.append([v1,v2,v3,J,k,J-k,ener(v1,v2,v3,J,k,J-k)])
		if k < J:
			lst.append([v1,v2,v3,J,k+1,J-k,ener(v1,v2,v3,J,k+1,J-k)])

def make_vstate(v1,v2,v3):
	st = []
	for J in range(0,Jmax+1):
		add_jstate(v1,v2,v3,J,st)
	return st
	
def intens(vu1,vu2,vu3,J,Ka,Kc,vg1,vg2,vg3,J0,Ka0,Kc0):
	h = 3.0 / (3.0 + vu1+vu2+vu3+vg1+vg2+vg3)
	h /= J + 1
	h /= abs(Ka-Ka0) + 1
	return h

def add_band(vu1,vu2,vu3,vg1,vg2,vg3,lst):
	sym = (vu3 + vg3 + 1) % 2 # sym == 1 for same parity of vu3 and vg3
	upper = make_vstate(vu1,vu2,vu3)
	lower = make_vstate(vg1,vg2,vg3)
	nup = len(upper)
	nlo = len(lower)
	for i in range(0,nup):
		J = upper[i][3]
		Ka = upper[i][4]
		Kc = upper[i][5]
		E = upper[i][6]
		for k in range(0,nlo):
			J0 = lower[k][3]
			if (abs(J - J0) > 1):
				continue
			Ka0 = lower[k][4]
			dK = abs(Ka - Ka0)
			if dK % 2 != sym or dK > dKmax:
				continue
			Kc0 = lower[k][5]
			if abs(Kc - Kc0) % 2 != 1:
				continue
			E0 = lower[k][6]
			lst.append([upper[i],lower[k],E-E0,intens(vu1,vu2,vu3,J,Ka,Kc,vg1,vg2,vg3,J0,Ka0,Kc0)])
			#s  = '"[11] ('+str(vu1)+str(vu2)+str(vu3)+') '
			#s += str(J)+' '+str(Ka)+' '+str(Kc)+'" '
			#s += '"[11] ('+str(vg1)+str(vg2)+str(vg3)+') '
			#s += str(J0)+' '+str(Ka0)+' '+str(Kc0)+'" '
			#s += str(E - E0) + ' ' + str(intens(vu1,vu2,vu3,J,Ka,Kc,vg1,vg2,vg3,J0,Ka0,Kc0)) + '\n'
			#f.write(s)
			
#def spectrum(band):
	

f=open('ground.txt','w')
f.write('#VJKG:q[""]\n')
f.write('#double:Energy\n')
vstate(f,0,0,0)
f.close()

f=open('llist.txt','w')
f.write('#VJKG:q[""]\n')
f.write('#VJKG:q0[""]\n')
f.write('#double:Line\n')
f.write('#double:Intensity\n')
sp = []
add_band(1,0,0,0,0,0,sp)
add_band(0,1,0,0,0,0,sp)
add_band(0,0,1,0,0,0,sp)
add_band(0,2,0,0,0,0,sp)
add_band(1,1,0,0,0,0,sp)
add_band(0,1,1,0,0,0,sp)
sp.sort(lambda a,b: cmp(a[2] , b[2]))
for lin in sp:
	up = lin[0]
	lo = lin[1]
	freq = lin[2]
	h = lin[3]
	s  = '"[11] ('+str(up[0])+str(up[1])+str(up[2])+') '
	s += str(up[3])+' '+str(up[4])+' '+str(up[5])+'" '
	s += '"[11] ('+str(lo[0])+str(lo[1])+str(lo[2])+') '
	s += str(lo[3])+' '+str(lo[4])+' '+str(lo[5])+'" '
	s += str(freq) + ' ' + str(h) + '\n'
	f.write(s)
f.close()

