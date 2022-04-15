function selectTab(evt, tabName) {
	var i, tabcontent, tablinks;
	tabcontent = document.getElementsByClassName("tabcontent");
	for (i = 0; i < tabcontent.length; i++) {
	  tabcontent[i].style.display = "none";
	}
	tablinks = document.getElementsByClassName("tablinks");
	for (i = 0; i < tablinks.length; i++) {
	  tablinks[i].className = tablinks[i].className.replace(" active", "");
	}
	document.getElementById(tabName).style.display = "block";
	evt.currentTarget.className += " active";
  }

// Get the element with id="defaultOpen" and click on it
document.getElementById("defaultOpen").click();

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