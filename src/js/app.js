
function fetchMeh() {
  var req = new XMLHttpRequest();
  req.open('GET', "https://api.meh.com/1/current.json?apikey=7Fq9Kc7AE8nmg2RIOWHX8lgw!2fPUaBp");
  req.onload = function(e) {
    if (req.readyState == 4) {
      if(req.status == 200) {
        console.log(req.responseText);

        var response = JSON.parse(req.responseText);
        var dealTitle = response.deal.title;
        var dealCost = response.deal.items[0].price;
        
        var costAsString = '$' + dealCost.toString();
        
        Pebble.sendAppMessage({
          
          "MEH_TITLE_KEY":dealTitle,
          "MEH_COST_KEY":costAsString}
        );

      } else {
        console.log("Error");
      }
    }
  };
  req.send(null);
}




Pebble.addEventListener("ready", function(e) {
  console.log("connect!" + e.ready);
  fetchMeh();
});

Pebble.addEventListener("appmessage", function(e) {  
  console.log("message!");
  fetchMeh();
});

Pebble.addEventListener("webviewclosed", function(e) {
  console.log("webview closed");
  console.log(e.type);
  console.log(e.response);
});




