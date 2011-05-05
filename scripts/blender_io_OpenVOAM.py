########################################
# OpenVOAM Blender Exporter
# 2011, Mark Pitman
########################################
    
bl_info = {
    "name": "OpenVOAM Export",
    "author": "Mark Pitman",
    'version': (0, 1),
    "blender": (2, 5, 7),
    "api": 36079,
    'description': 'Export basic test geometry from Blender to OpenVOAM',
    "location": "File > Export > OpenVOAM",
    "warning": "",
    "wiki_url": "",
    "tracker_url": "",
    "category": "Import-Export"}

import bpy
from random import random
from math import copysign
import os

import pdb

class addRandomPoints(bpy.types.Operator):
    bl_label = "Add Random Vertices"
    bl_idname = "mesh.addrandomverts"
    bl_description = "Add Random Vertices"
    bl_options = {'REGISTER', 'UNDO'}

    Nv = bpy.props.IntProperty(name="Nv", description="Number of verts", min=2, default=100)
    Dx = bpy.props.FloatProperty(name="Dx", description="Horizontal distribution coefficient for vertices (0=even distribution)", default=1)
    Dy = bpy.props.FloatProperty(name="Dy", description="Vertical distribution coefficient for vertices (0=even distribution)", default=1)
    dimX = bpy.props.FloatProperty(name="dimX", description="Hozizontal dimension of distribution", default=4)
    dimY = bpy.props.FloatProperty(name="dimY", description="Vertical dimension of distribution", default=1)
    #-------------
    Ne = bpy.props.IntProperty(name="Nv", description="Number of edges on wavy section", min=2, default=10)

    def draw(self, context):
        layout = self.layout
        layout.row().prop(self, 'Nv')
        col = layout.row().split()
        col.prop(self, 'dimX')
        col.prop(self, 'dimY')
        col = layout.row().split()
        col.prop(self, 'Dx')
        col.prop(self, 'Dy')

    def execute(self, context):
        def dist(dim, c):
            #pdb.set_trace()
            vo = (random() - 0.5)*2.0 # Distribution between -1 and 1
            vn = abs(vo**c)
            vn = copysign(vn,vo)
            vn = (vn + 1.0)*dim/2.0
            return vn
        # Add vertex distribution first
        verts = [( dist(self.dimX,self.Dx) , dist(self.dimY,self.Dy) , 0.0 ) for i in range(self.Nv)]
        mesh = bpy.data.meshes.new('randVerts')
        mesh.from_pydata(verts, [], [])
        mesh.update()
        
        # Deselect all objects.
        bpy.ops.object.select_all(action='DESELECT')

        ob_new = bpy.data.objects.new(mesh.name, mesh)
        bpy.context.scene.objects.link(ob_new)
        ob_new.select = True
        return {'FINISHED'}

    def invoke(self, context, event):
        self.execute(context)
        return {'FINISHED'}

bpy.utils.register_class(addRandomPoints)


def menu_export(self, context):
    default_path = os.path.splitext(bpy.data.filepath)[0]
    self.layout.operator(openvoamExport.bl_idname, text="OpenVOAM Case Directory").filepath = default_path

class openvoamExport(bpy.types.Operator):
    '''Export OpenVOAM Case'''
    bl_idname = "export_mesh.ovm"
    bl_label = "Export OpenVOAM Case"
    
    filepath = bpy.props.StringProperty(name="File Path", description="File path used for exporting the STL file", maxlen=1024, default="")

    def prependFile(self, infile, text, outfile):
        f = open(infile)
        orgtext = f.read()
        f.close()
        f = open(outfile, 'w')
        f.write(text)
        f.write(orgtext)
        f.close()
        return

    def execute(self, context):
        if not os.path.exists(self.filepath):
            os.mkdir(self.filepath)

        pointsFile = self.filepath + "/points"
        eleFile = self.filepath + "/elements"
        points = open(pointsFile,'w')
        elements = open(eleFile,'w')
        nPoints = 0;
        nElements = 0;

        for o in context.selected_objects:
            if len(o.data.edges) == 0:  # Object is a set of free vertices only
                for v in o.data.vertices:
                    points.write("("+str(v.co[0])+" "+str(v.co[1])+" "+str(v.co[2])+")\n")
                    elements.write("1 ("+ str(nPoints) +")\n")
                    nPoints += 1
                    nElements += 1
            else:                       # Object defines an edge (in 2d at least)
                outIndex = [-1]*len(o.data.vertices)
                for e in o.data.edges:
                    for vi in e.key:
                        if not outIndex[vi] > 0:
                            v = o.data.vertices[vi]
                            points.write("("+str(v.co[0])+" "+str(v.co[1])+" "+str(v.co[2])+")\n")
                            outIndex[vi] = nPoints;
                            nPoints += 1;
                    elements.write("2 ("+ str(outIndex[e.key[0]]) +" "+ str(outIndex[e.key[1]]) +")\n")
                    nElements += 1

        points.write(")\n"); points.close();
        elements.write(")\n"); elements.close();

        self.prependFile(pointsFile, str(nPoints)+"\n(\n", pointsFile)
        self.prependFile(eleFile, str(nElements)+"\n(\n", eleFile)

        return('FINISHED')

    def invoke(self, context, event):
        wm = context.window_manager
        wm.fileselect_add(self)
        return {'RUNNING_MODAL'}

bpy.utils.register_class(openvoamExport)

def register():
    bpy.types.INFO_MT_file_export.append(menu_export)
    return

def unregister():
    bpy.types.INFO_MT_file_export.remove(menu_export)
    return
    
if __name__ == "__main__":
    register()
