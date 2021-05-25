from common.params import Params


params = Params()

def get_camera_offset():
  params.put("CameraOffset","6")
  camera_offset = params.get("CameraOffset", encoding='utf-8')
  if camera_offset is not None:
    camera_offset = int(camera_offset) * 0.01
  else:
    camera_offset = 0.06
  return camera_offset


