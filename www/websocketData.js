ws_url = "ws://" + window.location.hostname + ":" + window.location.port + "/websocket"

webSocket = null;

function websocketConnect(){
    webSocket = new WebSocket(ws_url);
    webSocket.onopen = wsOnOpenHandler;
    webSocket.onmessage = wsOnMessageHandler;
}

function wsOnOpenHandler(event) {

};

function wsOnMessageHandler(event) {
	rxData = JSON.parse(event.data);
	if (rxData.msgType == "newData") {
		document.getElementById("time").innerHTML = rxData.time;
		document.getElementById("xaccel").innerHTML = rxData.accelX;
		document.getElementById("yaccel").innerHTML = rxData.accelY;
		document.getElementById("zaccel").innerHTML = rxData.accelZ;
		document.getElementById("xgyro").innerHTML = rxData.gyroX;
		document.getElementById("ygyro").innerHTML = rxData.gyroY;
		document.getElementById("zgyro").innerHTML = rxData.gyroZ;
		document.getElementById("yaw").innerHTML = rxData.yaw;
		document.getElementById("heapFree").innerHTML = rxData.heapFree;
	} else if (rxData.msgType == "curSettings") {

	} else {
		console.log("Unknown message type " + rxData.msgType);
	}
}