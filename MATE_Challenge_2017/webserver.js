/**
 * webserver.js
 * @auther David Chau
 * @version 1.0 
 * 
 */
 
const express = require('express');
const app = express();
var port = 8000;
var io = require('socket.io').listen(app.listen(port));

var five = require("johnny-five");
var board = new five.Board();
var motorEsc;

//create socket server to retreve data from controller
io.on('connection', function(socket) {
	console.log('client connected');
	socket.on('notping', function(data) {
		//console.log(data[1] + ' | ' + data[2]);
		//motorEsc[0].speed(throttle(data[1]));
		navagate(data);
	});	
});

//webserver for our website
app.use(express.static('public'))
app.get('/', function(req, res) {
    res.sendFile(path.join(__dirname + '/public/index.html'));
});

//connects to arduino and create the esc objects on corrisponding pins
board.on("ready", function() {
  motorEsc = new five.ESCs([11, 10, 9, 6, 5, 3]);
});

//Makes the throttle response of the motors. For bi-directional esc 50 is midpoint(off).
function halfThrottle(percent){
	var speed = 50 + (25 * percent);
	return speed;
}

function fullThrottle(percent){
	var speed = 50 + (50 * percent);
	return speed;
}

function navagate(data){
	if(data[2] > 0.1 || data[2] < 0){
		motorEsc[0].speed(halfThrottle(data[1]));	//
		motorEsc[1].speed(halfThrottle(data[1]));	//
		motorEsc[2].speed(halfThrottle(data[2]));	//
		motorEsc[3].speed(halfThrottle(data[2]));	//
		motorEsc[4].speed(halfThrottle(data[4]));	//
		motorEsc[5].speed(halfThrottle(data[4]));	//
	} else {
		motorEsc[0].speed(halfThrottle(data[1]));	//
		motorEsc[1].speed(halfThrottle(data[1]));	//
		motorEsc[2].speed(halfThrottle(data[1]));	//
		motorEsc[3].speed(halfThrottle(data[1]));	//
		motorEsc[4].speed(halfThrottle(data[4]));	//	
		motorEsc[5].speed(halfThrottle(data[4]));	//
	}
}



