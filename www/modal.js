// Get the modal
var modal = document.getElementById("myModal");
var span = document.getElementsByClassName("close")[0];



//////////////////////////////////////////////////////
//Modal Support

function showModal(message) {
	document.getElementById("modalText").innerHTML = message;
	modal.style.display = "block";
}

function closeModal() {
	modal.style.display = "none";
	document.getElementById("modalText").innerHTML = "";
}


// When the user clicks on <span> (x), close the modal
span.onclick = function () {
	closeModal();
}

// When the user clicks anywhere outside of the modal, close it
window.onclick = function (event) {
	if (event.target == modal) {
		closeModal();
	}
}