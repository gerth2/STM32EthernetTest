function myFunction() {
	document.getElementById("demo").style.color = "red";
}

url = "ws://" + window.location.hostname +":" + window.location.port + "/websocket"
webSocket = new WebSocket(url);

webSocket.onopen = function (event) {

};

webSocket.onmessage = function (event) {
	rxData = JSON.parse(event.data);
	document.getElementById("time").innerHTML = rxData.time;
	document.getElementById("xaccel").innerHTML = rxData.accelX;
	document.getElementById("yaccel").innerHTML = rxData.accelY;
	document.getElementById("zaccel").innerHTML = rxData.accelZ;
	document.getElementById("xgyro").innerHTML = rxData.gyroX;
	document.getElementById("ygyro").innerHTML = rxData.gyroY;
	document.getElementById("zgyro").innerHTML = rxData.gyroZ;
	document.getElementById("yaw").innerHTML = rxData.yaw;

}