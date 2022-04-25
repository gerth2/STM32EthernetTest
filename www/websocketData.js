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
        var dataTable = document.getElementById("dataTable");
        for(key in rxData){
            value = rxData[key];

            //get row
            var row = document.getElementById(key);

            //Add row if it does not exist
            if(!row){
                row = dataTable.insertRow(-1);
                row.id = key;
                row.insertCell(0);
                row.insertCell(1);
            }

            row.cells[0].innerHTML = key;
            row.cells[1].innerHTML = rxData[key];

            if(key == "pitch"){
                setPitchDeg(value);
            } else if (key == "roll"){
                setRollDeg(value);
            } else if (key == "yaw"){
                setYawDeg(value);
            }

        }
	} else if (rxData.msgType == "curSettings") {

	} else {
		console.log("Unknown message type " + rxData.msgType);
	}
}