// Fetch stock data for a given stock symbol (NYSE or NASDAQ only) from markitondemand.com
// & send the stock price back to the watch via app message
// API documentation at http://dev.markitondemand.com/#doc

function tv() {
  var response;
  var req = new XMLHttpRequest();

  // build the GET request 
  var url = "https://api.getirkit.com/1/messages"; 
  var data = '?clientkey=42D6FD2E1E3A42DFAA46E9942DBD8BF6&deviceid=40A179A4B05847F680553EC4DA96AA59&message={"format":"raw","freq":38,"data":[17421,8755,1073,1073,1073,1190,1073,1073,1073,1073,1073,1190,1073,1190,1073,3228,1073,1073,1073,3228,1073,3228,1073,3228,1073,3228,1073,3228,1073,3228,1073,1073,1073,3228,1073,1073,1073,3228,1073,1073,1073,1190,1037,3341,1111,1111,1111,1111,1111,1111,1111,3341,1111,1111,1111,3341,1111,3341,1111,1111,1111,3341,1111,3341,1111,3341,1111,65535,0,13693,17421,4400,1073]}';
  
  var target = url + data;
  req.open('POST', target, true);
  req.setRequestHeader('Content-Type', 'application/x-www-form-urlencoded');
  req.send();

  req.onload = function(e)  {
      if (req.readyState == 4) {
	  // 200 - HTTP OK
	  if(req.status == 200) {
	  }
      }
  };
  req.send(null);
}

function up() {
  var response;
  var req = new XMLHttpRequest();

  // build the GET request 
  var url = "https://api.getirkit.com/1/messages"; 
  var data = '?clientkey=42D6FD2E1E3A42DFAA46E9942DBD8BF6&deviceid=40A179A4B05847F680553EC4DA96AA59&message={"format":"raw","freq":38,"data":[17421,8755,1073,1073,1073,1073,1073,1073,1073,1073,1073,1073,1073,1073,1073,3228,1073,1073,1073,3228,1073,3228,1073,3228,1073,3228,1073,3228,1073,3228,1073,1190,1037,3341,1073,3341,1073,3341,1073,1073,1073,3341,1073,3341,1073,1190,1037,1190,1037,1190,1037,1190,1037,1190,1190,3228,1073,1073,1073,1190,1037,3341,1111,3341,1111,3341,1111,65535,0,13693,17421,4400,1073]}';

  var target = url + data;
  req.open('POST', target, true);
  req.setRequestHeader('Content-Type', 'application/x-www-form-urlencoded');
  req.send();

  req.onload = function(e)  {
      if (req.readyState == 4) {
	  // 200 - HTTP OK
	  if(req.status == 200) {
	  }
      }
  };
  req.send(null);
}

function down() {
  var response;
  var req = new XMLHttpRequest();

  // build the GET request 
  var url = "https://api.getirkit.com/1/messages"; 
  var data = '?clientkey=42D6FD2E1E3A42DFAA46E9942DBD8BF6&deviceid=40A179A4B05847F680553EC4DA96AA59&message={"format":"raw","freq":38,"data":[17421,8755,1073,1073,1073,1073,1073,1073,1073,1073,1073,1073,1073,1073,1073,3341,1073,1073,1073,3341,1073,3341,1073,3341,1073,3341,1073,3341,1073,3341,1073,1073,1073,3341,1073,3341,1073,3341,1073,3341,1073,3341,1073,3341,1073,1073,1073,1073,1073,1073,1073,1073,1073,1073,1073,1073,1073,1073,1073,1073,1073,3341,1073,3341,1073,3341,1073,65535,0,13693,17421,4400,1073]}';

  var target = url + data;
  req.open('POST', target, true);
  req.setRequestHeader('Content-Type', 'application/x-www-form-urlencoded');
  req.send();

  req.onload = function(e)  {
      if (req.readyState == 4) {
	  // 200 - HTTP OK
	  if(req.status == 200) {
	  }
      }
  };
  req.send(null);
}


// Set callback for the app ready event
Pebble.addEventListener("ready", function(e) {

	// Fetch saved symbol from local storage (using standard localStorage webAPI)
	// symbol = localStorage.getItem("symbol");
	// if (!symbol) {
	// symbol = "PBL";
	// }
	// var isInitMsg = true;
	// fetchStockQuote(symbol, isInitMsg); 
    });

// Set callback for appmessage events
Pebble.addEventListener("appmessage", function(e) {
	var isInitMsg;
	if (e.payload.init) {
	} else if (e.payload.fetch) {
	    tv();
	    isInitMsg = false;
	} else if (e.payload.symbol) {
	    up();
	} else {
	    down();
	}
    });
