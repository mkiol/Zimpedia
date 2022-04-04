function zimpediaPostMessage(message, data) {
  navigator.qt.postMessage(JSON.stringify({ "type": message, "data": data }));
}

/*window.XMLHttpRequest.prototype.open = function() {
	console.log("BLOCKED: " + arguments[1]);
	return null;
};*/
