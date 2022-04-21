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


ws_url = "ws://" + window.location.hostname +":" + window.location.port + "/websocket"
webSocket = new WebSocket(ws_url);


webSocket.onopen = function (event) {
	getCurSettings();
};

webSocket.onmessage = function (event) {
	rxData = JSON.parse(event.data);
	if(rxData.msgType == "newData"){
		document.getElementById("time").innerHTML = rxData.time;
		document.getElementById("xaccel").innerHTML = rxData.accelX;
		document.getElementById("yaccel").innerHTML = rxData.accelY;
		document.getElementById("zaccel").innerHTML = rxData.accelZ;
		document.getElementById("xgyro").innerHTML = rxData.gyroX;
		document.getElementById("ygyro").innerHTML = rxData.gyroY;
		document.getElementById("zgyro").innerHTML = rxData.gyroZ;
		document.getElementById("yaw").innerHTML = rxData.yaw;
		document.getElementById("heapFree").innerHTML = rxData.heapFree;
	} else if(rxData.msgType == "curSettings"){

	} else {
		console.log("Unknown message type " + rxData.msgType);
	}
}

function onSettingsReset(evt) {
	getCurSettings();

}

function onSettingsSubmit(evt) {


}

//settings

settings_url = "http://" + window.location.hostname +":" + window.location.port + "/curSettings"

function getCurSettings(evt) {
	var xmlHttp = new XMLHttpRequest();
    xmlHttp.onreadystatechange = function() { 
        if (xmlHttp.readyState == 4 && xmlHttp.status == 200)
		onRxCurSettings(xmlHttp.responseText);
    }
    xmlHttp.open("GET", settings_url, true); // true for asynchronous 
    xmlHttp.send(null);
}

function onRxCurSettings(settings){
	rxData = JSON.parse(settings);

	document.getElementById("deviceName").value = rxData.deviceName;
	document.getElementById("ucIPAddress").value = rxData.ucIPAddress;
	document.getElementById("ucNetMask").value = rxData.ucNetMask;
	document.getElementById("ucGatewayAddress").value = rxData.ucGatewayAddress;
	//document.getElementById("ucDNSServerAddress").value = rxData.ucDNSServerAddress;
	document.getElementById("nt4ServerAddress").value = rxData.nt4ServerAddress;

}