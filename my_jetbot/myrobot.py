import time
import traitlets
from traitlets.config.configurable import SingletonConfigurable
from .mymotors import MyMotors


class MyRobot(MyMotors):
    i2c_address = traitlets.Integer(default_value=0x04)
    steering_gain = traitlets.Float(default_value=-0.65)
    steering_offset = traitlets.Float(default_value=0)
    steering_channel = traitlets.Integer(default_value=0)
    throttle_gain = traitlets.Float(default_value=0.8)
    throttle_channel = traitlets.Integer(default_value=1)

    def __init__(self, *args, **kwargs):
        super(MyRobot, self).__init__(*args, **kwargs)

    @traitlets.observe('steering')
    def _on_steering(self, change):
        self.steering_motor = change['new'] * self.steering_gain + self.steering_offset

    @traitlets.observe('throttle')
    def _on_throttle(self, change):
        self.throttle_motor = change['new'] * self.throttle_gain
