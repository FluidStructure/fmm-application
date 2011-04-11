#
# This script tests the multipole translations and forumulations
# presented in Beatson & Greengard "A short course on fast multipole
# methods"
#
# The script creates a sample distribution of particles in a unit square
# centered at some point.  The performs a series of multipole translations
# while continuously re-evaluating the potential at a point.

import random
import matplotlib.pyplot as plt
from math import pi, factorial
from cmath import log

def bincoeff(n,k):
    return factorial(n)/(factorial(k)*factorial(n-k))

class source():
    def __init__(self,X=0,Y=0):
        self.x = random.random() + X - 0.5
        self.y = random.random() + Y - 0.5
        self.s = random.random()

    def potential(self,x,y):
        R = ((x-self.x)**2.0 + (y-self.y)**2.0)**0.5
        return (self.s)*log(R)

    def velocity(self,x,y):
        X = x-self.x
        Y = y-self.y
        Rsq = X**2.0 + Y**2.0
        return ( (self.s*Y)/Rsq , (self.s*X)/Rsq )

    def expandMultipole(self, box, p):
        box.ak[0] += self.s + 0j
        for k in range(1,p):
            zi = (self.x - box.x) + (self.y - box.y)*1j
            box.ak[k] += -1.0*self.s*(zi**k)/k

    def plotPoint(self):
        plt.plot(self.x,self.y,'bo')


class fmmBox():
    def __init__(self, X=0, Y=0, p=8):
        self.x = X
        self.y = Y
        self.size = 1
        self.p = p
        self.sources = []
        self.ak = [0.0]*p
        self.bl = [0.0]*p
    
    def addRandomSources(self,N):
        for i in range(N):
            self.sources.append( source (self.x, self.y) )

    def directPotential(self,x,y):
        """Calculate VELOCITY POTENTIAL directly.  Nothing fancy here"""
        p = 0.0
        for s in self.sources:
            p += s.potential(x,y)
        return p

    def directVelocity(self,x,y):
        """Calculate VELOCITY directly.  Nothing fancy here either"""
        u = 0.0;v=0.0
        for s in self.sources:
            (uu,vv) = s.velocity(x,y)
            u += uu
            v += vv
        return (u,v)

    def expandMultipoles(self):
        """Lemma 4.2 (Multipole expansion)"""
        for s in self.sources:
            s.expandMultipole(self, self.p)

    def multipolePotential(self,x,y):
        """Get the VELOCITY POTENTIAL at evaluation point (x,y) 
        directly from the multipole expansion about the centre of 
        this fmm-box (using coefficients ak)"""
        z = (x-self.x) + (y-self.y)*1j
        pot = self.ak[0]*log(z)
        for k in range(1,self.p):
            pot += self.ak[k]/(z**k)
        return pot

    def multipoleVelocity(self,x,y):
        """Get the VELOCITY at evaluation point (x,y) 
        directly from the multipole expansion about the centre of 
        this fmm-box (using coefficients ak) 
        Note: this is simply the derivative of the multipolePotential function"""
        z = (x-self.x) + (y-self.y)*1j
        #pdb.set_trace()
        vel = self.ak[0]*(1/z)
        for k in range(1,self.p):
            vel += -1.0*k*self.ak[k]/(z**(k+1))
        return (-1.0*vel.imag, vel.real)

    def translateMultipole(self,box):
        """Lemma 4.6 (Translation of a multipole expansion)"""
        box.ak[0] += self.ak[0]
        zo = (self.x-box.x) + (self.y-box.y)*1j
        for l in range(1,self.p):
            box.ak[l] += -1.0*self.ak[0]*(zo**l)/l
            for k in range(1,l+1):
                box.ak[l] += self.ak[k]*(zo**(l-k))*bincoeff(l-1,k-1)
        return None

    def makeLocalExpansion(self, box):
        """Lemma 4.7 (Conversion of a multipole expansion into a local expansion)"""
        zo = (box.x-self.x) + (box.y-self.y)*1j
        # Equation 4.18 - calculating b0        
        self.bl[0] += box.ak[0]*log(-1.0*zo)
        for k in range(1,self.p):
            self.bl[0] += box.ak[k]*((-1.0)**k)/(zo**k)
        # Equation 4.19 - calculation bl
        for l in range(1,self.p):
            self.bl[l] += -1.0*box.ak[0]/(l*(zo**l))
            tmp = 0.0
            for k in range(1,self.p):
                tmp += ((box.ak[k])/(zo**k))*((-1.0)**k)*bincoeff(l+k-1,k-1)
            self.bl[l] += (1/(zo**l))*tmp
        return None

    def translateLocalExp(self, box):
        """Lemma 4.8 (Translation of a local expansion)"""
        zo = (box.x-self.x) + (box.y-self.y)*1j
        # First make a copy of the vector
        for l in range(self.p):
            self.bl[l] = box.bl[l]
        # Do a Horner scheme translating the polynomial (Equation 4.21->4.22)
        for l in range(0,self.p):        
            for k in range(self.p-1-l,self.p-1):
                self.bl[k] = self.bl[k] - zo*self.bl[k+1]
        return None

    def localExpPotential(self,x,y):
        """Get the VELOCITY POTENTIAL at evaluation point (x,y) 
        directly from the local expansion about the centre of 
        this fmm-box (using coefficients bl)"""
        z = (x-self.x) + (y-self.y)*1j
        pot = 0.0
        for l in range(0,self.p):
            pot += self.bl[l]*(z**l)
        return pot

    def localExpVelocity(self,x,y):
        """Get the VELOCITY at evaluation point (x,y) 
        directly from the local expansion about the centre of 
        this fmm-box (using coefficients bl) 
        Note: this is simply the derivative of the localExpPotential function"""
        z = (x-self.x) + (y-self.y)*1j
        vel = 0.0 + 0j
        for l in range(0,self.p):
            vel += l*self.bl[l]*(z**(l-1))
        return (-1.0*vel.imag, vel.real)

    def plotBox(self):
        # Plot all points inside box (if any)    
        for s in self.sources:
            s.plotPoint()
        # Plot the center-point of the box
        plt.plot(self.x, self.y, 'ro')
        # Plot limits of box bounds
        minX = self.x - self.size/2.0
        maxX = self.x + self.size/2.0
        minY = self.y - self.size/2.0
        maxY = self.y + self.size/2.0
        plt.plot([minX, maxX, maxX, minX, minX],[minY, minY, maxY, maxY, minY],'g-')
        

if __name__ == "__main__":
    X0 = (2,3)      # Point about which sources are located
    e = (0.2,-0.8)  # Evaluation point (position inside of leaf-box)
    N = 20;         # Number of points
    p = 6;          # Truncation of multipole expansions

    fig1 = plt.figure()
    plt.hold(True)

    # Create the relevant FMM boxes and plot them
    leafBox1 = fmmBox(X0[0],X0[1],p)
    leafBox1.addRandomSources(N)
    leafBox1.plotBox()
    
    parentBox1 = fmmBox(
        X0[0]-leafBox1.size/2.0,
        X0[1]+leafBox1.size/2.0, p)
    parentBox1.size = 2*leafBox1.size
    parentBox1.plotBox()
    
    parentBox2 = fmmBox(
        parentBox1.x + parentBox1.size*2,
        parentBox1.y + parentBox1.size*2, p)
    parentBox2.size = parentBox1.size
    parentBox2.plotBox()

    leafBox2 = fmmBox(
        parentBox2.x + parentBox2.size/4.0,
        parentBox2.y - parentBox2.size/4.0, p)
    leafBox2.size = parentBox2.size/2.0
    leafBox2.plotBox()

    # Re-evaluate the coordinate of the evaluation point
    e = ((e[0]*leafBox2.size/2.0 + leafBox2.x), (e[1]*leafBox2.size/2.0 + leafBox2.y))

    # Plot the location of the evaluation point
    plt.plot(e[0],e[1],'mo')

    #############################
    #############################
    print("")
    print("Direct potential == " + str(leafBox1.directPotential(e[0],e[1]).real) )
    print("Direct velocity == " + str(leafBox1.directVelocity(e[0],e[1])) )
    
    print("")
    print("Step 1: multipole expansions")
    leafBox1.expandMultipoles()
    #print("leafBox1:Coefficients(ak) :: " + str(leafBox1.ak))
    pot = leafBox1.multipolePotential(e[0],e[1])
    print("Multipole potential == " + str(pot.real))
    vel = leafBox1.multipoleVelocity(e[0],e[1])
    print("Multipole velocity == " + str(vel) )

    print("")
    print("Step 2: multipole translation to parent")
    leafBox1.translateMultipole(parentBox1)
    #print("parentBox1:Coefficients(ak) :: " + str(parentBox1.ak))
    pot = parentBox1.multipolePotential(e[0],e[1])
    print("Multipole potential == " + str(pot.real))
    vel = parentBox1.multipoleVelocity(e[0],e[1])
    print("Multipole velocity == " + str(vel) )

    print("")
    print("Step 3: creation of local expansion at parentBox2 ")
    parentBox2.makeLocalExpansion(parentBox1)
    #print("parentBox2:Coefficients(bl) :: " + str(parentBox2.bl))
    pot = parentBox2.localExpPotential(e[0],e[1])
    print("Local Exp. potential == " + str(pot.real))
    vel = parentBox2.localExpVelocity(e[0],e[1])
    print("Local Exp. velocity == " + str(vel) )


    print("")
    print("Step 4: translation of local expansion to leafBox2 ")
    leafBox2.translateLocalExp(parentBox2)
    #print("leafBox2:Coefficients(bl) :: " + str(leafBox2.bl))
    pot = leafBox2.localExpPotential(e[0],e[1])
    print("Local Exp. potential == " + str(pot.real))
    vel = leafBox2.localExpVelocity(e[0],e[1])
    print("Local Exp. velocity == " + str(vel) )

    # Show the final plot
    plt.show()
