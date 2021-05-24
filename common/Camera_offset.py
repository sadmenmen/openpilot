from common.params import Params


params = Params()

def get_camera_offset():
  camera_offset = params.get("CameraOffset", encoding='utf8')
  if camera_offset is not None:
    camera_offset = int(camera_offset) * 0.01
  else:
    camera_offset = 0.06
  return camera_offset


