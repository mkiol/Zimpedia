// Code heavily inspired and partially borrowed from
// harbour-webpirate project (https://github.com/Dax89/harbour-webpirate)

window.ZimpediaThemeObject = function() {
    this.zoom = 1.0;
    this.updateScale();
};

window.ZimpediaThemeObject.prototype.set = function(theme) {
    for(var prop in theme)
        this[prop] = theme[prop];
};

// Hack to fix wrong device Pixel Ratio reported by Webview (thanks to llelectronics)
window.ZimpediaThemeObject.prototype.getPixelRatio = function() {
    if(window.screen.width <= 540) // Jolla devicePixelRatio: 1.5
        return 1.5
    if(window.screen.width > 540 && screen.width <= 768) // Nexus 4 devicePixelRatio: 2.0
        return 2.0
    if (window.screen.width > 768) // Nexus 5 devicePixelRatio: 3.0
        return 3.0
};

window.ZimpediaThemeObject.prototype.updateScale = function() {
    var pr = this.getPixelRatio();
    var _scale = this.zoom ? this.zoom * pr : pr;
    var scale = Math.round((_scale <= 0.5 ? 0.5 : _scale ) * 10 ) / 10;
    var viewport_ele = document.querySelector("meta[name='viewport']");
    
    var content = "width=device-width/" + scale + ", initial-scale=" + scale;
    
    if (viewport_ele) {
        viewport_ele.content = content;
    } else {
        var meta_ele = document.createElement("meta");
        var name_att = document.createAttribute("name");
        name_att.value = "viewport";
        var content_att = document.createAttribute("content");
        content_att.value = content;
        meta_ele.setAttributeNode(name_att);
        meta_ele.setAttributeNode(content_att);
        document.getElementsByTagName('head').item(0).appendChild(meta_ele);
    }
};

window.ZimpediaTheme = new window.ZimpediaThemeObject();
