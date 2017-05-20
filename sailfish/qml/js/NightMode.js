// Code heavily inspired and partially borrowed from
// harbour-webpirate project (https://github.com/Dax89/harbour-webpirate)

window.ZimpediaNightModeObject = function() {
    this.enabled = false;
    this.style_id = "_zimpedia_nightmode_style";
    this.class_name = "_zimpedia_nightmode_class";
    zimpediaPostMessage("nightmode_" + (this.enabled ? "enabled" : "disabled"));
};

window.ZimpediaNightModeObject.prototype.createStyle = function() {
    var css = "html." + this.class_name + " { -webkit-filter: contrast(68%) brightness(108%) invert(); }\n" +
              "html." + this.class_name + " iframe { -webkit-filter: invert(); }\n" + // Keep iframes normal
              "html." + this.class_name + " object { -webkit-filter: invert(); }\n" + // Keep Flash items normal
              "html." + this.class_name + " embed { -webkit-filter: invert(); }\n"  + // Keep Flash items normal (HTML5)
              "html." + this.class_name + " video { -webkit-filter: invert(); }\n"  + // Keep HTML5 Videos normal
              "html." + this.class_name + " img { -webkit-filter: invert(); }" ;      // Keep images normal
    var style_ele = document.getElementById(this.style_id);

    if (style_ele) {
        style_ele.innerHTML = css;
    } else {
        style_ele = document.createElement("style");
        style_ele.id = this.style_id;
        style_ele.type = "text/css";
        style_ele.appendChild(document.createTextNode(css));
        document.getElementsByTagName('head').item(0).appendChild(style_ele);
    }
};

window.ZimpediaNightModeObject.prototype.switchMode = function(enabled) {
    if (this.enabled === enabled)
        return;

    var html = document.getElementsByTagName("html")[0];

    if (this.enabled) {
        this.enabled = false;
        html.className = html.className.replace(this.class_name, "");
    } else {
        this.enabled = true;
        this.createStyle();
        html.className += " " + this.class_name;
    }
    
    zimpediaPostMessage("nightmode_" + (this.enabled ? "enabled" : "disabled"));
};

window.ZimpediaNightMode = new window.ZimpediaNightModeObject();
