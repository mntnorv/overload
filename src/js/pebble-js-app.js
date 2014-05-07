var url = "http://mntnorv.github.io/overload-config";

Pebble.addEventListener("ready", function() {});

Pebble.addEventListener("showConfiguration", function() {
  Pebble.openURL(url);
});

Pebble.addEventListener("webviewclosed", function (e) {
  var configuration = JSON.parse(decodeURIComponent(e.response));
  Pebble.sendAppMessage(configuration);
});
