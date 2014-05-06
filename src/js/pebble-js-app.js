var url = "http://mntnorv.github.io/overload";

Pebble.addEventListener("showConfiguration", function() {
  Pebble.openURL(url);
});

Pebble.addEventListener("webviewclosed", function(e) {
  var config = JSON.parse(decodeURIComponent(e.response));
  console.log("Configuration window returned: ", JSON.stringify(config));
});
