

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

curSettings_url = "http://" + window.location.hostname + ":" + window.location.port + "/curSettings"

function getCurSettings() {
	showModal("Retrieving Settings...")
	var xmlHttp = new XMLHttpRequest();
	xmlHttp.onreadystatechange = function () {
		if (xmlHttp.readyState == 4 && xmlHttp.status == 200) {
			onRxCurSettings(xmlHttp.responseText);
		}
	}
	xmlHttp.open("GET", curSettings_url, true); // true for asynchronous 
	xmlHttp.send(null);
}

newSettings_url = "http://" + window.location.hostname + ":" + window.location.port + "/newSettings"

function sendNewSettings(newSettings) {
	showModal("Updating Settings...")
	var xmlHttp = new XMLHttpRequest();

	xmlHttp.onreadystatechange = function () {
		if (xmlHttp.readyState == 4) {
			if (xmlHttp.status == 200) {
				showModal("Settings successfully updated!");
				setTimeout(window.location.reload.bind(window.location), 1500);
			} else {
				showModal("ERROR, failed to update settings!");
			}
		}
	}

	xmlHttp.open("POST", newSettings_url, true); // true for asynchronous 
	xmlHttp.send(newSettings);

}

function onRxCurSettings(settings) {
	rxData = JSON.parse(settings);

	document.getElementById("deviceName").value = rxData.deviceName;
	document.getElementById("ucIPAddress").value = rxData.ucIPAddress;
	document.getElementById("ucNetMask").value = rxData.ucNetMask;
	document.getElementById("ucGatewayAddress").value = rxData.ucGatewayAddress;
	//document.getElementById("ucDNSServerAddress").value = rxData.ucDNSServerAddress;
	document.getElementById("nt4ServerAddress").value = rxData.nt4ServerAddress;

	window.document.title = rxData.deviceName;
	document.getElementById("deviceTitleText").innerHTML = rxData.deviceName;

	closeModal();
}



function validateDeviceName(str_in) {
	var devNameRegex = new RegExp("[a-zA-Z_]*");
	return devNameRegex.test(str_in);
}

function validateIpAddr(str_in) {
	var ipRegex = new RegExp("^(?:(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\.){3}(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)$");
	return ipRegex.test(str_in);
}