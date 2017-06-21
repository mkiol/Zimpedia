// Code borrowed from harbour-webpirate project (https://github.com/Dax89/harbour-webpirate)

console.log = function(msg) {
  zimpediaPostMessage("console_log", { log: ((typeof msg === "object") ? msg.toString() : msg) });
};

console.error = function(msg) {
  zimpediaPostMessage("console_error", { log: ((typeof msg === "object") ? msg.toString() : msg) });
};
