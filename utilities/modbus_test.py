from pymodbus.client import ModbusSerialClient

from pymodbus import (
    FramerType,
    ModbusException,
    pymodbus_apply_logging_config,
)

# Configure the Modbus RTU client
client = ModbusSerialClient(
    port='COM8',  # Serial port (e.g., COM3 on Windows or /dev/ttyUSB0 on Linux)
    baudrate=115200,      # Baud rate
    timeout=1           # Timeout in seconds
)

# Connect to the Modbus device
if client.connect():
    print("Connected to Modbus RTU device.")

    # Set the first 4 coils
    coils = [True, True, True, True]
    # Write the coil values
    client.write_coils(1, coils)

    # Write the first 4 registers with values 1 to 4
    for i in range(1,5):
        client.write_register(i,i)
    


    try:
        result = client.read_holding_registers(address=1, count=4)  # Address is typically 1
        print(f"Registers: {result.registers}")

    except ModbusException as exc:
        print(f"Received ModbusException({exc}) from library")

    try:
        result = client.read_coils(address=1, count=32)  # Address is typically 1
        print(f"Coils: {result.bits}")

    except ModbusException as exc:
        print(f"Received ModbusException({exc}) from library")



        

    # Close the connection
    client.close()
else:
    print("Failed to connect to Modbus RTU device.")
