import bpy
from bpy_extras.io_utils import ImportHelper


print("====" * 30)




def menu_func_import(self, context):
    self.layout.operator(import_ipl_file.bl_idname, text = 'IPL Import')


class import_ipl_file(bpy.types.Operator, ImportHelper): 
    bl_idname = "object.custom_draw" 
    bl_label = "Import" 
  
    
    filepath = bpy.props.StringProperty(subtype = "FILE_PATH") 
    
    instancing_check1 : bpy.props.BoolProperty(name = 'Import as instancing', description = 'Description', default = False)
    filter_glob : bpy.props.StringProperty(default='*.ipl', options={'HIDDEN'})

    def execute(self, context): 
        instancing_check = not (self.instancing_check1)
        
        print(self.filepath) 
        
        print("\n\n")

        encoding = "cp1251" 
        ipl_path = self.filepath 
 
        ipl_objects = {} 
        
        #selecting active collection to the root "scene collection" to avoid some problems with the instancing
        bpy.context.view_layer.active_layer_collection = bpy.context.view_layer.layer_collection
 
        with open(ipl_path, "r", encoding=encoding) as f: 
            for line in f: 
                if line.strip() == "end":
                    break
                
                elements = [elem.strip() for elem in line.split(",")] 
                
                if len(elements) == 11: 
                    
                    obj_name = elements[1]
                    model_id = int(elements[0])
                    
                    x, y, z, rotx, roty, rotz, rotw = map(float, elements[3:10]) 
                    
                    if obj_name not in ipl_objects:
                        ipl_objects[obj_name] = {"coords_rot_list": [], "check": 0}
                    
                    ipl_objects[obj_name]["coords_rot_list"].append((x, y, z, rotx, roty, rotz, rotw, model_id))
                    ipl_objects[obj_name]["check"] += 1
        
        for obj_name, data in ipl_objects.items():
            obj = bpy.data.objects.get(obj_name)
        
            if obj is None:
                #print(f" {obj_name} not found in the scene.")
                continue
            
            obj_location = bpy.data.objects[obj_name].location
            
            if obj_location != [0.0, 0.0, 0.0]:
                obj.location = (0.0, 0.0, 0.0)
            
            coords_rot_list = data['coords_rot_list']
            check = data['check']
            
            bpy.data.objects[obj_name]['id'] = coords_rot_list[0][7]     #model id
            
            
            if check > 1 and instancing_check == True:
               
                collection_name = obj_name + "_collection"
             
                if collection_name not in bpy.data.collections:
                    
                    new_collection = bpy.data.collections.new(collection_name)
                    bpy.context.scene.collection.children.link(new_collection)
                    
                for i, coords_rot in enumerate(coords_rot_list): 
                    
                    existing_obj = None             #   if an object with the coords from ipl already 
                    for o in bpy.data.objects:      #   exist in the scene - it won't be copied

                        obj_name_check = o.name
                        obj_coord_check = [float(f"{coord:.1f}") for coord in (o.location)]
                        coords_list_check = [float(f"{coord:.1f}") for coord in coords_rot[0:3]]

                        if "." in o.name:
                            obj_name_check = o.name.split(".")[0]

                        if obj_name_check == obj_name and obj_coord_check == coords_list_check:
                            existing_obj = o
                            #print("match found")
                    
                    if existing_obj is None:
                        
                        new_obj = obj.copy()
                        new_obj.data = obj.data.copy()
                        new_obj.location = coords_rot[0:3] 
                        new_obj.rotation_mode = 'QUATERNION'
                        new_obj.rotation_quaternion = (-(coords_rot[6]), coords_rot[3], coords_rot[4], coords_rot[5])
                        new_obj.name = f"{obj_name}.0{i}"
                        bpy.data.collections[collection_name].objects.link(new_obj)
                    
                        #print(f"{new_obj.name} location: {new_obj.location} rotation: (x = {coords_rot[3]}, y = {coords_rot[4]}, z = {coords_rot[5]}, w = {coords_rot[6]})")
                        #print("\n")
                    
            elif check == 1:
               
                coords_rot = coords_rot_list[0]
                obj.location = coords_rot[0:3] 
                obj.rotation_mode = 'QUATERNION'
                obj.rotation_quaternion = (-(coords_rot[6]), coords_rot[3], coords_rot[4], coords_rot[5])
                
                #print(f"{obj_name} location: {obj.location} rotation: (x = {coords_rot[3]}, y = {coords_rot[4]}, z = {coords_rot[5]}, w = {coords_rot[6]})")
                #print("\n")
            else:
                
                collection_name = obj_name + "_collection_inst"
                
                if collection_name not in bpy.data.collections:
                    
                    new_collection = bpy.data.collections.new(collection_name)
                    bpy.context.scene.collection.children.link(new_collection)
                    
                for i, coords_rot in enumerate(coords_rot_list):
                    
                    existing_obj = None
                    for o in bpy.data.objects:

                        obj_name_check = o.name
                        obj_coord_check = [float(f"{coord:.1f}") for coord in (o.location)]
                        coords_list_check = [float(f"{coord:.1f}") for coord in coords_rot[0:3]]

                        if "." in o.name:
                            obj_name_check = o.name.split(".")[0]

                        if obj_name_check == obj_name and obj_coord_check == coords_list_check:
                            existing_obj = o

                    if existing_obj is None:

                        bpy.ops.object.empty_add(type='PLAIN_AXES', align='WORLD', location=(0.0, 0.0, 0.0), scale=(1, 1, 1))                   
                        obj_inst = bpy.context.object
                        obj_inst.name = f"{obj_name}.0{i}"
                        obj_inst.rotation_mode = 'QUATERNION'
                        obj_inst.rotation_quaternion = (-(coords_rot[6]), coords_rot[3], coords_rot[4], coords_rot[5])
                        obj_inst.location = coords_rot[0:3] 
                        bpy.data.collections[collection_name].objects.link(obj_inst)
                        obj_inst.instance_type = 'COLLECTION'
                        obj_inst.instance_collection = bpy.data.collections[obj_name + ".dff"]
                    
                        #print(f"{obj_inst.name} location: {obj_inst.location} rotation: (x = {coords_rot[3]}, y = {coords_rot[4]}, z = {coords_rot[5]}, w = {coords_rot[6]})")
                        #print("\n")
                    
        print ("Import magic happend!\n")

        return {'FINISHED'} 
