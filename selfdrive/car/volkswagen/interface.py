from cereal import car
from selfdrive.swaglog import cloudlog
from selfdrive.car.volkswagen.values import CAR, BUTTON_STATES, TransmissionType, GearShifter, NetworkLocation, PQ_CARS
from selfdrive.car import STD_CARGO_KG, scale_rot_inertia, scale_tire_stiffness, gen_empty_fingerprint
from selfdrive.car.interfaces import CarInterfaceBase
from common.params import Params

EventName = car.CarEvent.EventName


class CarInterface(CarInterfaceBase):
  def __init__(self, CP, CarController, CarState):
    super().__init__(CP, CarController, CarState)



    self.displayMetricUnitsPrev = None
    self.buttonStatesPrev = BUTTON_STATES.copy()

    self.displayMetricUnitsPrev = None
    self.cruise_enabled_prev = False
    # Set up an alias to PT/CAM parser for ACC depending on its detected network location
    self.cp_acc = self.cp if CP.networkLocation == NetworkLocation.fwdCamera else self.cp_cam

    # timebomb_counter mod
    self.timebomb_counter = 0
    self.wheel_grabbed = False
    self.timebomb_bypass_counter = 0

    params = Params()
    self.is_timebomb_assist = params.get("IsTimebombAssist", encoding='utf8') == "1"


  @staticmethod
  def compute_gb(accel, speed):
    return float(accel) / 4.0

  @staticmethod
  def get_params(candidate, fingerprint=gen_empty_fingerprint(), has_relay=False, car_fw=None):
    ret = CarInterfaceBase.get_std_params(candidate, fingerprint, has_relay)

    # VW port is a community feature, since we don't own one to test
    ret.communityFeature = False

    if True:  # pylint: disable=using-constant-test
      # Set common MQB parameters that will apply globally
      ret.carName = "volkswagen"
      ret.radarOffCan = True
      ret.safetyModel = car.CarParams.SafetyModel.volkswagen
      ret.steerActuatorDelay = 0.05

      if candidate in PQ_CARS:
        # Configurations shared between all PQ35/PQ46/NMS vehicles
        ret.safetyModel = car.CarParams.SafetyModel.volkswagenPq

        # Determine installed network location and trans type from fingerprint
        ret.networkLocation = NetworkLocation.fwdCamera if 0x368 in fingerprint[0] else NetworkLocation.gateway
        if 0x440 in fingerprint[0]:  # Getriebe_1
          ret.transmissionType = TransmissionType.automatic
        else:  # No trans at all
          ret.transmissionType = TransmissionType.manual
      else:
        ret.safetyModel = car.CarParams.SafetyModel.volkswagen
        if 0xAD in fingerprint[0]:
          # Getriebe_11 detected: traditional automatic or DSG gearbox
          ret.transmissionType = TransmissionType.automatic
        elif 0x187 in fingerprint[0]:
          # EV_Gearshift detected: e-Golf or similar direct-drive electric
          ret.transmissionType = TransmissionType.direct
        else:
          # No trans message at all, must be a true stick-shift manual
          ret.transmissionType = TransmissionType.manual
      cloudlog.info("Detected transmission type: %s", ret.transmissionType)

    # Global tuning defaults, can be overridden per-vehicle

    ret.steerRateCost = 1.0
    ret.steerLimitTimer = 0.4
    ret.steerRatio = 15.6  # Let the params learner figure this out
    tire_stiffness_factor = 1.0  # Let the params learner figure this out
    ret.lateralTuning.pid.kpBP = [0.]
    ret.lateralTuning.pid.kiBP = [0.]
    ret.lateralTuning.pid.kf = 0.00006
    ret.lateralTuning.pid.kpV = [0.6]
    ret.lateralTuning.pid.kiV = [0.2]

    # Per-chassis tuning values, override tuning defaults here if desired

    if candidate == CAR.GOLF_MK7:
      # Averages of all AU Golf variants
      ret.mass = 1397 + STD_CARGO_KG
      ret.wheelbase = 2.62

    elif candidate == CAR.JETTA_MK7:
      # Averages of all BU Jetta variants
      ret.mass = 1328 + STD_CARGO_KG
      ret.wheelbase = 2.71

    elif candidate == CAR.PASSAT_MK8:
      # Averages of all 3C Passat variants
      ret.mass = 1551 + STD_CARGO_KG
      ret.wheelbase = 2.79

    elif candidate == CAR.TIGUAN_MK2:
      # Average of SWB and LWB variants
      ret.mass = 1715 + STD_CARGO_KG
      ret.wheelbase = 2.74

    elif candidate == CAR.AUDI_A3:
      # Temporarily carry forward old tuning values while we test vehicle identification
      ret.mass = 1500 + STD_CARGO_KG
      ret.wheelbase = 2.64

    elif candidate == CAR.SEAT_ATECA_MK1:
      # Averages of all 5F Ateca variants
      ret.mass = 1900 + STD_CARGO_KG
      ret.wheelbase = 2.64

    elif candidate == CAR.SKODA_KODIAQ_MK1:
      # Averages of all 5N Kodiaq variants
      ret.mass = 1569 + STD_CARGO_KG
      ret.wheelbase = 2.79

    elif candidate == CAR.SKODA_SCALA_MK1:
      # Averages of all NW Scala variants
      ret.mass = 1192 + STD_CARGO_KG
      ret.wheelbase = 2.65

    elif candidate == CAR.SKODA_SUPERB_MK3:
      # Averages of all 3V/NP Scala variants
      ret.mass = 1505 + STD_CARGO_KG
      ret.wheelbase = 2.84
    elif candidate == CAR.GENERICPQ:
      ret.mass = 1375 + STD_CARGO_KG  # Average, varies on trim/package
      ret.wheelbase = 2.58
      ret.steerRatio = 15.6
      tire_stiffness_factor = 1.0

    ret.centerToFront = ret.wheelbase * 0.45

    ret.enableCamera = True  # Stock camera detection doesn't apply to VW

    # TODO: get actual value, for now starting with reasonable value for
    # civic and scaling by mass and wheelbase
    ret.rotationalInertia = scale_rot_inertia(ret.mass, ret.wheelbase)

    # TODO: start from empirically derived lateral slip stiffness for the civic and scale by
    # mass and CG position, so all cars will have approximately similar dyn behaviors
    ret.tireStiffnessFront, ret.tireStiffnessRear = scale_tire_stiffness(ret.mass, ret.wheelbase, ret.centerToFront,
                                                                         tire_stiffness_factor=tire_stiffness_factor)

    return ret

  # returns a car.CarState
  def update(self, c, can_strings):
    buttonEvents = []

    # Process the most recent CAN message traffic, and check for validity
    # The camera CAN has no signals we use at this time, but we process it
    # anyway so we can test connectivity with can_valid
    self.cp.update_strings(can_strings)
    self.cp_cam.update_strings(can_strings)

    ret = self.CS.update(self.cp, self.cp_cam, self.cp_acc, self.CP.transmissionType)
    ret.canValid = self.cp.can_valid and self.cp_cam.can_valid
    ret.steeringRateLimited = self.CC.steer_rate_limited if self.CC is not None else False

    # TODO: add a field for this to carState, car interface code shouldn't write params
    # Update the device metric configuration to match the car at first startup,
    # or if there's been a change.
    #if self.CS.displayMetricUnits != self.displayMetricUnitsPrev:
    #  put_nonblocking("IsMetric", "1" if self.CS.displayMetricUnits else "0")

    # Check for and process state-change events (button press or release) from
    # the turn stalk switch or ACC steering wheel/control stalk buttons.
    for button in self.CS.buttonStates:
      if self.CS.buttonStates[button] != self.buttonStatesPrev[button]:
        be = car.CarState.ButtonEvent.new_message()
        be.type = button
        be.pressed = self.CS.buttonStates[button]
        buttonEvents.append(be)

    events = self.create_common_events(ret, extra_gears=[GearShifter.eco, GearShifter.sport])

    # Vehicle health and operation safety checks
    if self.CS.parkingBrakeSet:
      events.add(EventName.parkBrake)
    if self.CS.steeringFault:
      events.add(EventName.steerTempUnavailable)

    # Engagement and longitudinal control using stock ACC. Make sure OP is
    # disengaged if stock ACC is disengaged.
    if not ret.cruiseState.enabled:
      events.add(EventName.pcmDisable)
    # Attempt OP engagement only on rising edge of stock ACC engagement.
    elif not self.cruise_enabled_prev:
      events.add(EventName.pcmEnable)

    # add vw timebomb assist
    if self.is_timebomb_assist:
      if ret.cruiseState.enabled:
        self.timebomb_counter += 1
      else:
        self.timebomb_counter = 0
        self.timebomb_bypass_counter = 0

      ret.stopSteering = False
      if self.timebomb_counter >= 33000: # 330*100 time in seconds until counter threshold for timebombWarn alert
        if not self.wheel_grabbed:
          events.add(EventName.timebombWarn)
        if self.wheel_grabbed or ret.steeringPressed:
          self.wheel_grabbed = True
          ret.stopSteering = True
          self.timebomb_bypass_counter += 1
          if self.timebomb_bypass_counter >= 300: # 3*100 time alloted for bypass
            self.wheel_grabbed = False
            self.timebomb_counter = 0
            self.timebomb_bypass_counter = 0
            events.add(EventName.timebombBypassed)
          else:
            events.add(EventName.timebombBypassing)

    ret.events = events.to_msg()
    ret.buttonEvents = buttonEvents

    # update previous car states
    self.cruise_enabled_prev = ret.cruiseState.enabled

    # update previous car states
    self.displayMetricUnitsPrev = self.CS.displayMetricUnits
    self.buttonStatesPrev = self.CS.buttonStates.copy()

    self.CS.out = ret.as_reader()
    return self.CS.out

  def apply(self, c):
    can_sends = self.CC.update(c.enabled, self.CS, self.frame, c.actuators,
                   c.hudControl.visualAlert,
                   c.hudControl.audibleAlert,
                   c.hudControl.leftLaneVisible,
                   c.hudControl.rightLaneVisible)
    self.frame += 1
    return can_sends
