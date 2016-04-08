#! /usr/bin/env python
import OpenGL
OpenGL.USE_ACCELERATE = False
OpenGL.ERROR_ON_COPY = True
OpenGL.ERROR_CHECKING = False
from OpenGL.GL import *
from OpenGL.GLUT import *
import sys, time
from math import sin,cos,sqrt,pi
from OpenGL.constants import GLfloat
import numpy as np
import thread, pdb
vec4 = GLfloat_4
dataname = ""
class View:
    def __init__(self):     
        self.view_rotx, self.view_roty, self.view_rotz = (20.0, 30.0, 0.0)
        self.view_scale = 1.0
        self.mouse_clic_coords = None
        self.mouse_action = None
        glutDisplayFunc(self.draw)
        glutReshapeFunc(self.reshape)
        glutKeyboardFunc(self.key)
        try:
            glutMouseWheelFunc(self.mousewheel)
        except OpenGL.error.NullFunctionError: 
            print "could not set mouse wheel callback"
        glutMouseFunc(self.mouse)
        glutMotionFunc(self.mousemotion)
        glutSpecialFunc(self.special)
        pos = vec4(5.0, 5.0, 10.0, 0.0)
        glLightfv(GL_LIGHT0, GL_POSITION, pos)
        black = vec4(0, 0, 0, 1)
        glLightfv(GL_LIGHT0, GL_AMBIENT, black)
        glEnable(GL_CULL_FACE)
        glEnable(GL_LIGHTING)
        glEnable(GL_LIGHT0)
        glEnable(GL_DEPTH_TEST)
        glEnable(GL_NORMALIZE)
        
    def draw(self):
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT)
        glPushMatrix()
        glRotatef(self.view_rotx, 1.0, 0.0, 0.0)
        glRotatef(self.view_roty, 0.0, 1.0, 0.0)
        glRotatef(self.view_rotz, 0.0, 0.0, 1.0)
        glScalef(self.view_scale, self.view_scale, self.view_scale)
        self.draw_object()
        glPopMatrix()
        glutSwapBuffers()

    def key(self, k, x, y):
        if k == 'z':
            self.view_rotz += 5.0
        elif k == 'Z':
            self.view_rotz -= 5.0
        elif k == '+': 
            self.view_scale *= 1.2
        elif k == '-':
            self.view_scale /= 1.2
        elif ord(k) == 27: # Escape
            sys.exit(0)
        else:
            return
        glutPostRedisplay()

    def special(self, k, x, y):
        if k == GLUT_KEY_UP:
            self.view_rotx += 5.0
        elif k == GLUT_KEY_DOWN:
            self.view_rotx -= 5.0
        elif k == GLUT_KEY_LEFT:
            self.view_roty += 5.0
        elif k == GLUT_KEY_RIGHT:
            self.view_roty -= 5.0
        else:
            return
        glutPostRedisplay()

    def mouse(self, button, state, x, y): 
        # print button, state
        if  button == GLUT_LEFT_BUTTON:
            if state == GLUT_DOWN: 
                self.mouse_clic_coords = (x, y, self.view_rotx, self.view_roty)
                self.mouse_action = "rotate"
            else: 
                self.mouse_action = None
        elif button == 4:
            if state == GLUT_DOWN: 
                self.view_scale /= 1.2
                glutPostRedisplay()
        elif button == 3:
            if state == GLUT_DOWN:
                self.view_scale *= 1.2
                glutPostRedisplay()

    def mousewheel(self, wheel, direction, x, y): 
        if direction > 0: 
            self.view_scale /= 1.2
        else: 
            self.view_scale *= 1.2
        glutPostRedisplay()

    def mousemotion(self, x, y): 
        if self.mouse_action == "rotate": 
            (x0, y0, view_rotx0, view_roty0) = self.mouse_clic_coords
            self.view_roty = view_roty0 + (x - x0) * 0.2
            self.view_rotx = view_rotx0 + (y - y0) * 0.2
            glutPostRedisplay()

    def reshape(self, width, height):
        h = float(height) / float(width);
        glViewport(0, 0, width, height)
        glMatrixMode(GL_PROJECTION)
        glLoadIdentity()
        glFrustum(-1.0, 1.0, -h, h, 5.0, 60.0)
        glMatrixMode(GL_MODELVIEW)
        glLoadIdentity()
        glTranslatef(0.0, 0.0, -40.0)


class ViewSurface(View): 

    def __init__(self):
        View.__init__(self)
        self.make_surface()

    def make_surface(self): 
        f = open(dataname+".off", "r")
        assert f.readline() == "OFF\n"
        nv, nf, zero = f.readline().split()
        nv = int(nv)
        nf = int(nf)
        assert zero == "0"
        V = np.fromfile(f, dtype = 'float32', count = 3 * nv, sep = " ")
        self.points = V.reshape(-1, 3)
        F = np.fromfile(f, dtype = 'int', count = 4 * nf, sep = " ")
        assert F.size == 4 * nf
        self.indices = F.reshape(-1, 4)[:, 1:]
        print "%d vertices, %d triangles" % (self.points.shape[0], self.indices.shape[0])
 
    def draw_object(self): 
        pts = self.points
        glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, vec4(0.7, 0.9, 0.5, 1))
        glBegin(GL_TRIANGLES)
        for i0, i1, i2 in self.indices: 
            normal = np.cross(pts[i0] - pts[i1], pts[i0] - pts[i2])
            normal /= np.linalg.norm(normal)
            glNormal3fv(normal)
            glVertex3fv(pts[i0])
            glVertex3fv(pts[i1])
            glVertex3fv(pts[i2])
        glEnd()

if __name__ == '__main__':
    glutInit(sys.argv)
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH)
    glutCreateWindow("viewer")
    if len(sys.argv) < 2 :
        dataname = 'sphere'
    else :
        dataname = sys.argv[1]
    vg = ViewSurface()
    if "-info" in sys.argv:
        print("GL_RENDERER   = ", glGetString(GL_RENDERER))
        print("GL_VERSION    = ", glGetString(GL_VERSION))
        print("GL_VENDOR     = ", glGetString(GL_VENDOR))
        print("GL_EXTENSIONS = ", glGetString(GL_EXTENSIONS))
    glutMainLoop()
