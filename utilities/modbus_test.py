# ------------------------------------------------------------------------------
#  © 2025 Microchip Technology Inc.
#
#  Project Name:   dsPIC33A Curiosity Modbus Demo
#  File Name:      timer1.py
#
#  ------------------------------------------------------------------------------
#
#  Description:
#  The Python script in the project directory uses the pymodbus library to create 
#  a basic client to write and then read some registers and coils.
#
#  ------------------------------------------------------------------------------
#  MICROCHIP SOFTWARE NOTICE AND DISCLAIMER:
#  You may use this software, and any derivatives created by any person or entity
#  by or on your behalf, exclusively with Microchip's products in accordance with
#  applicable software license terms and conditions. A copy of these terms is
#  provided for your reference in accompanying documentation.
#
#  Microchip and its licensors retain all ownership and intellectual property
#  rights in the accompanying software and in all derivatives thereof.
#
#  This software and any accompanying information is provided for suggestion only.
#  It does not modify Microchip’s standard warranty for its products. You agree
#  that you are solely responsible for testing the software and determining its
#  suitability. Microchip has no obligation to modify, test, certify, or support
#  the software.
#
#  THIS SOFTWARE IS PROVIDED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER EXPRESS,
#  IMPLIED OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
#  NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A PARTICULAR PURPOSE APPLY
#  TO THIS SOFTWARE, ITS INTERACTION WITH MICROCHIP PRODUCTS, COMBINATION WITH
#  ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION.
#
#  IN NO EVENT SHALL MICROCHIP BE LIABLE, WHETHER IN CONTRACT, WARRANTY, TORT
#  (INCLUDING NEGLIGENCE OR BREACH OF STATUTORY DUTY), STRICT LIABILITY, INDEMNITY,
#  CONTRIBUTION, OR OTHERWISE, FOR ANY INDIRECT, SPECIAL, PUNITIVE, EXEMPLARY,
#  INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST, OR EXPENSE OF ANY KIND
#  WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS BEEN
#  ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE.
#
#  TO THE FULLEST EXTENT ALLOWABLE BY LAW, MICROCHIP’S TOTAL LIABILITY ON ALL
#  CLAIMS RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
#  YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
#
#  MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE TERMS.
#
#  ------------------------------------------------------------------------------
#  Written by:
#
# ------------------------------------------------------------------------------


from pymodbus.client import ModbusSerialClient

from pymodbus import (
    FramerType,
    ModbusException,
    pymodbus_apply_logging_config,
)

# Configure the Modbus RTU client
client = ModbusSerialClient(
    port='COM5',  # Serial port (e.g., COM3 on Windows or /dev/ttyUSB0 on Linux)
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
        result = client.read_holding_registers(address=1, count=4)
        print(f"Holding Registers: {result.registers}")

    except ModbusException as exc:
        print(f"Received ModbusException({exc}) from library")

    try:
        result = client.read_coils(address=1, count=32)
        print(f"Coils: {result.bits}")

    except ModbusException as exc:
        print(f"Received ModbusException({exc}) from library")

    # Read Discrete Inputs (FC 02) - read-only digital inputs
    try:
        result = client.read_discrete_inputs(address=0, count=32)
        print(f"Discrete Inputs: {result.bits}")

    except ModbusException as exc:
        print(f"Received ModbusException({exc}) from library")

    # Read Input Registers (FC 04) - read-only analog values
    try:
        result = client.read_input_registers(address=0, count=4)
        print(f"Input Registers: {result.registers}")

    except ModbusException as exc:
        print(f"Received ModbusException({exc}) from library")



        

    # Close the connection
    client.close()
else:
    print("Failed to connect to Modbus RTU device.")
