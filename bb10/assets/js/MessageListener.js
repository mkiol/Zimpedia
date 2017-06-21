// Code heavily inspired and partially borrowed from
// harbour-webpirate project (https://github.com/Dax89/harbour-webpirate)

window.ZimpediaMessageListenerObject = function() {
    navigator.cascades.onmessage = this.onMessage.bind(this);
};

window.ZimpediaMessageListenerObject.prototype.onMessage = function(message) {
    //console.log("onMessage: " + message);
    try {
        var obj = JSON.parse(message);
        var data = obj.data;

        if(obj.type === "theme_set")
          ZimpediaTheme.set(data.theme);
        else if(obj.type === "theme_update_scale")
          ZimpediaTheme.updateScale();
        else if(obj.type === "theme_apply")
          ZimpediaTheme.apply();
        else if(obj.type === "nightmode_disable")
          ZimpediaNightMode.switchMode(false);
        else if(obj.type === "nightmode_enable")
          ZimpediaNightMode.switchMode(true);
    } catch (err) {
        console.error("Exception in ZimpediaMessageListener.onMessage: " + err);;
    }
};

window.ZimpediaMessageListener = new window.ZimpediaMessageListenerObject();
