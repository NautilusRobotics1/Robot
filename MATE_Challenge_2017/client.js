/**
 * client.js
 * @author Caleb Yang
 * @version 1.0 
 * 
 */

var five = require("johnny-five");
var board = new five.Board();
var adafruitSensor, barometer;//, magnetometer; // mag not supported

//connects to arduino and create the esc/sensor objects on corrisponding pins
board.on("ready", function() {
    adafruitSensor = new five.IMU([21,20]);
    barometer = new five.Barometer([21,20]);
});

adafruitSensor.on("data", function() {
    var accel = this.accelerometer;
    var gyro = this.gyro;
    var temp = this.temperature.celsius;
    console.log("Accelerometer: %d, %d, %d", accel.x, accel.z, accel.z);
    console.log("Gyro: %d, %d, %d", gyro.x, gyro.z, gyro.z);
    console.log("Temperature: %d", temp.celsius);
});

barometer.on("data", function() {
    console.log("  pressure : ", this.pressure);
});

