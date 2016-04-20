


var dealTitle;
var dealCostAsString;

function loadMehDeal()
{
  
      var method = 'GET';
      var url = 'https://api.meh.com/1/current.json?apikey=7Fq9Kc7AE8nmg2RIOWHX8lgw!2fPUaBp';

      // Create the request
      var request = new XMLHttpRequest();

      // Specify the callback for when the request is completed
      request.onload = function() {
      try {
          // Transform in to JSON
          var json = JSON.parse(this.responseText);

          // Read data
          dealTitle = json.deal.title;
        
          var dealCost = json.deal.items[0].price;
          dealCostAsString = '$' + dealCost.toString();
        
        Pebble.sendAppMessage({
          
            "MEH_TITLE_KEY":dealTitle,
            "MEH_COST_KEY":dealCostAsString
          }
                                
        );
        
        console.log(dealTitle);
        console.log(dealCost);
        
          
          
      
  } catch(err) {
    console.log('Error parsing JSON response!');
  }
};

// Send the request
request.open(method, url);
request.send();
  
}


Pebble.addEventListener("ready", function(e) {
  
   loadMehDeal();
  
});

Pebble.addEventListener("appmessage", function(e) {  
  
  loadMehDeal();
  
});

Pebble.addEventListener("webviewclosed", function(e) {
  console.log("webview closed");
  console.log(e.type);
  console.log(e.response);
});




