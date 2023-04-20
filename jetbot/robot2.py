import traitlets
import smbus2
from motors import Motors

bus = smbus2.SMBus(1)

# This is the address we setup in the Arduino Program
address = 0x04

def writeData(value):
    byteValue = StringToBytes(value)
    bus.write_i2c_block_data(address,0x00,byteValue) #first byte is 0=command byte.. just is.
    return -1

def StringToBytes(val):
        retVal = []
        for c in val:
                retVal.append(ord(c))
        return retVal

def _map(x, in_min, in_max, out_min, out_max):
    return int((x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min)

class Robot2(Motors):
    # i2c_address = traitlets.Integer(default_value=0x04)
    steering_gain = traitlets.Float(default_value=-0.65)
    steering_offset = traitlets.Float(default_value=0.0)
    throttle_gain = traitlets.Float(default_value=0.8)
    steering_f = traitlets.Float()
    throttle_f = traitlets.Float()

    def __init__(self, *args, **kwargs):
        super(Robot2, self).__init__(*args, **kwargs)

    @traitlets.observe('steering')
    def _on_steering(self, change):
        steering_f = change['new'] * self.steering_gain + self.steering_offset
        message = "ss"
        val = _map(steering_f, -1.0, 1.0, 50, 130)
        var = message + str(val)
        writeData(var)


    @traitlets.observe('throttle')
    def _on_throttle(self, change):
        throttle_f = change['new'] * self.throttle_gain
        message = "ms"
        val = _map(throttle_f, -1.0, 1.0, -255, 255)
        var = message + str(val)
        writeData(var)


    def stop(self):
        self.throttle = 0
        self.steering = 0
