var Clay = require('pebble-clay');
var config = require('./config.json');
var breeding = require('./breeding.js');
var power = require('./power.js');
var selection = require('./selection.js');

function customFn () {
    var c = this;
    c.on('AFTER_BUILD', function() {
        c.getItemByMessageKey('breeding').setData(c.meta.userData);
    });
}
customFn.toString = function () {
    return 'function(){var e=this;e.on("AFTER_BUILD",(function(){e.getItemByMessageKey("breeding").setData(e.meta.userData)}))}';
};

var clay = new Clay(config, customFn, {
    userData: breeding.userData,
    autoHandleEvents: false
});

breeding.init(clay);
power.init(clay);
selection.init(clay);

var timeout;

function openUrl() {
    timeout = null;
    Pebble.openURL(clay.generateUrl());
}

Pebble.addEventListener('ready', function () {
    Pebble.addEventListener('appmessage', function (event) {
        breeding.pebble(clay, event.payload);
        if (timeout) {
            clearTimeout(timeout);
            openUrl();
        }
    });
});

Pebble.addEventListener('showConfiguration', function() {
    timeout = setTimeout(openUrl, 1000);
    Pebble.sendAppMessage({});
});

Pebble.addEventListener('webviewclosed', function(e) {
    Pebble.sendAppMessage(clay.getSettings(e && e.response || {}));
});
