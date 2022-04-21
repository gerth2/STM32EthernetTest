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


/////////////////////////////////////////////////////////////////////////////////////
// Settings Handling


function onSettingsReset(evt) {
	getCurSettings();

}

function onSettingsSubmit(evt) {

	txData = {}

	txData.deviceName = document.getElementById("deviceName").value;
	txData.ucIPAddress = document.getElementById("ucIPAddress").value;
	txData.ucNetMask = document.getElementById("ucNetMask").value;
	txData.ucGatewayAddress = document.getElementById("ucGatewayAddress").value;
	//txData.ucDNSServerAddress = document.getElementById("ucDNSServerAddress");
	txData.nt4ServerAddress = document.getElementById("nt4ServerAddress").value;

	newSettings = JSON.stringify(txData);

	sendNewSettings(newSettings);

}

curSettings_url = "http://" + window.location.hostname +":" + window.location.port + "/curSettings"

function getCurSettings() {
	var xmlHttp = new XMLHttpRequest();
    xmlHttp.onreadystatechange = function() { 
        if (xmlHttp.readyState == 4 && xmlHttp.status == 200)
		onRxCurSettings(xmlHttp.responseText);
    }
    xmlHttp.open("GET", curSettings_url, true); // true for asynchronous 
    xmlHttp.send(null);
}

newSettings_url = "http://" + window.location.hostname +":" + window.location.port + "/newSettings"

function sendNewSettings(newSettings) {
	var xmlHttp = new XMLHttpRequest();
    xmlHttp.open("POST", newSettings_url, true); // true for asynchronous 
    xmlHttp.send(newSettings);
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

function validateDeviceName(str_in){
	var devNameRegex = new RegExp("[a-zA-Z_]*");
	return devNameRegex.test(str_in);
}

function validateIpAddr(str_in){
	var ipRegex = new RegExp("^(?:(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\.){3}(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)$");
	return ipRegex.test(str_in);
}