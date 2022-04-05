function myFunction() {
	document.getElementById("demo").style.color = "red";
}

url = "ws://" + window.location.hostname + "/websocket"
webSocket = new WebSocket(url);

webSocket.onopen = function (event) {
	console.log("Opened!")
	webSocket.send("Here's some text that the server is urgently awaiting!");
};

webSocket.onmessage = function (event) {
	console.log(event.data);
}