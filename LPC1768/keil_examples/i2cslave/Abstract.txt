This I2C slave can be tested with the program in "I2C" directory which 
is an I2C master example. The slave address is set the same as the NXP
I2C serial EEPROM, PCF8594_ADDR, used in the "I2C" directory.

In the I2C directory, it does the following things:
(1) Write a block of data, send NACK to stop.
(2) Read the data back from the same location, send NACK to stop.
(3) Verify the results.

In the I2C slave test program, as long as the I2C master sends the 
NACK, it stops the operation and reads the result from the read buffer. 
So, it ends at the first step in the previous sequence.

If you connect two boards with the I2C SCK and SDA, you can use the 
exact same program from I2C directory to test the I2C slave. at the 
end of the I2C slave program, the result of I2C slave read should 
match that from the I2C master write.



