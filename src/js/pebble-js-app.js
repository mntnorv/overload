var url = "http://mntnorv.github.io/overload-config/?";

var messageTypes = {
  getSettings: 0,
  setSettings: 1
};

function objectToGetParams(object) {
  var params = "";

  for (var key in object) {
    if (object.hasOwnProperty(key)) {
      if (params.length > 0) {
        params += '&';
      }

      params += encodeURIComponent(key) + '=' + encodeURIComponent(object[key]);
    }
  }

  return params;
}

Pebble.addEventListener("ready", function() {});

Pebble.addEventListener("showConfiguration", function() {
  Pebble.sendAppMessage({
    type: messageTypes.getSettings
  });
});

Pebble.addEventListener("webviewclosed", function (e) {
  console.log(JSON.stringify(e));
  var message = JSON.parse(decodeURIComponent(e.response));
  message.type = messageTypes.setSettings;
  console.log(JSON.stringify(message));
  Pebble.sendAppMessage(message);
});

Pebble.addEventListener("appmessage", function (e) {
  Pebble.openURL(url + objectToGetParams(e.payload));
});
