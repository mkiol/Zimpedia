function zimpediaPostMessage(message, data) {
  navigator.cascades.postMessage(JSON.stringify({ "type": message, "data": data }));
}

window.XMLHttpRequest.prototype.open = function() {
	console.log("BLOCKED: " + arguments[1]);
	return null;
};
