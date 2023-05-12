import smbus2
import time
import struct

# Define a function to convert a byte array to a float number
def bytes_to_float(b):
    return struct.unpack('f', b)[0]

# Create an instance of the I2C bus
bus = smbus2.SMBus(1)

# Define the slave address
address = 0x04

class GPS_coords():
    lat = 0.0
    lon = 0.0

    def get_pos(self):
        # Request 8 bytes (2 floats) from the slave
        data = bus.read_i2c_block_data(address, 0, 8)
        # Convert the bytes to two float numbers
        self.lat = bytes_to_float(bytes(data[0:4]))
        self.lon = bytes_to_float(bytes(data[4:8]))
        # Print the numbers
        # print("Received from Arduino: {} and {}".format(self.lat, self.lon))